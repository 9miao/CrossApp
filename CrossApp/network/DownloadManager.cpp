

#include "DownloadManager.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include "support/sqlite3/sqlite3.h"
#include "support/zip_support/unzip.h"
#include "platform/CAFileUtils.h"
#include "basics/CAScheduler.h"



NS_CC_BEGIN;

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#define CADownloadResponse_START_DOWNLOAD                0
#define CADownloadResponse_PROGRESS                      1
#define CADownloadResponse_ERROR                         2
#define CADownloadResponse_DOWNLOAD_FINISH               3

struct ErrorMessage
{
    CADownloadManager::ErrorCode code;
    CADownloadResponse* request;
};

struct ProgressMessage
{
    int percent;
    ssize_t nowDownloaded;
    ssize_t totalToDownload;
    CADownloadResponse* request;
};

enum
{
	DownloadStatus_Running,
	DownloadStatus_Waiting,
	DownloadStatus_Abort,
};

enum
{
	DownloadCmd_Null,
	DownloadCmd_Pause,
	DownloadCmd_resume,
	DownloadCmd_Delete,
};

static ssize_t _getLocalFileSize(const std::string& fileName)
{
    ssize_t lCurFileSize = 0;
    
    FILE* fp = fopen(fileName.c_str(), "rb");
    if (fp != NULL)
    {
        fseek(fp, 0L, SEEK_END);
        lCurFileSize = ftell(fp);
        fclose(fp);
    }
    fp = fopen(std::string(fileName + ".tmp").c_str(), "rb");
    if (fp != NULL)
    {
        fseek(fp, 0L, SEEK_END);
        lCurFileSize = ftell(fp);
        fclose(fp);
    }
    return lCurFileSize;
}

#pragma CADownloadManager

static CADownloadManager* _manager = NULL;

CADownloadManager* CADownloadManager::getInstance()
{
    if (_manager == NULL)
    {
        _manager = new CADownloadManager();
    }
    return _manager;
}

void CADownloadManager::destroyInstance()
{
    if (_manager)
    {
        delete _manager;
		_manager = NULL;
    }
}


CADownloadManager::CADownloadManager()
:m_pDelegate(nullptr)
,m_nDownloadMaxCount(1)
{
	std::string fullPath = FileUtils::getInstance()->getWritablePath() + "downloadMgr.db";

	int nRet = sqlite3_open(fullPath.c_str(), (sqlite3**)&m_mpSqliteDB);
	CCAssert(nRet == SQLITE_OK, "");
	CCAssert(m_mpSqliteDB != NULL, "");
	sqlite3_busy_timeout((sqlite3*)m_mpSqliteDB, 60000);

	checkSqliteDB();
    loadDownloadTasks();
}

CADownloadManager::~CADownloadManager()
{
	sqlite3_close((sqlite3*)m_mpSqliteDB);
}

void CADownloadManager::checkSqliteDB()
{
	char* szError = 0;
	char** paszResults = 0;
	int nRows = 0, nCols = 0;
	bool tableExist = false;

	std::string cszSql = "SELECT * FROM sqlite_master WHERE type = 'table' AND name = 'T_DownloadMgr'";
	int nRet = sqlite3_get_table((sqlite3*)m_mpSqliteDB, cszSql.c_str(), &paszResults, &nRows, &nCols, &szError);
	if (nRet == SQLITE_OK)
	{
		tableExist = nRows > 0;
	}
	if (paszResults)
	{
		sqlite3_free_table(paszResults);
	}
	paszResults = NULL;

	if (!tableExist)
	{
		cszSql = "CREATE TABLE [T_DownloadMgr] ([id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,[url] NVARCHAR(1024) NOT NULL,[header] NVARCHAR(4096) NOT NULL,[filePath] NVARCHAR(512) NOT NULL,[fileSize] REAL NOT NULL,[startTime] NVARCHAR(64) NOT NULL,[isFinished] INT DEFAULT (0) NOT NULL,[textTag] NVARCHAR(1024) DEFAULT NULL NOT NULL)";
		nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, cszSql.c_str(), 0, 0, &szError);
		CCAssert(nRet == SQLITE_OK, "");

		cszSql = "CREATE UNIQUE INDEX idx_T_DownloadMgr ON [T_DownloadMgr] (url);";
		nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, cszSql.c_str(), 0, 0, &szError);
		CCAssert(nRet == SQLITE_OK, "");
	}
}


double CADownloadManager::getDownloadFileSize(const std::string& downloadUrl, const std::string& downloadHeader)
{
    double downloadFileSize = 0;
    
    CURL *handle = curl_easy_init();
    
    curl_slist* headers = NULL;
	if (!downloadHeader.empty())
	{
		headers = curl_slist_append(headers, downloadHeader.c_str());
		curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
	}
   
    curl_easy_setopt(handle, CURLOPT_URL, downloadUrl.c_str());
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
	curl_easy_setopt(handle, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);
	
    if (curl_easy_perform(handle) == CURLE_OK)
    {
        curl_easy_getinfo(handle, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileSize);
    }
    if (headers)
    {
        curl_slist_free_all(headers);
    }
    curl_easy_cleanup(handle);
    
    return downloadFileSize;
}


ssize_t CADownloadManager::insertDownload(const std::string& downloadUrl, const std::string& downloadHeader, const std::string& fileName, const std::string& textTag)
{
	time_t long_time;
	time(&long_time);

	// Convert to local time.
	struct tm *today = localtime(&long_time);

	char startTime[512] = { 0 };
	sprintf(startTime, " %d-%02d-%02d %02d:%02d:%02d"
		, today->tm_year + 1900
		, today->tm_mon + 1
		, today->tm_mday
		, today->tm_hour
		, today->tm_min
		, today->tm_sec
		);
    
	double fileSize = this->getDownloadFileSize(downloadUrl, downloadHeader);
    char* szError = 0;
	char cszSql[4096] = { 0 };
	sprintf(cszSql, "INSERT INTO [T_DownloadMgr] (url, header, filePath, fileSize, startTime, textTag) values ('%s', '%s', '%s', %.0f, '%s', '%s');",
		downloadUrl.c_str(), downloadHeader.c_str(), fileName.c_str(), fileSize, startTime, textTag.c_str());
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, cszSql, 0, 0, &szError);

    if (nRet != SQLITE_OK)
    {
        return 0;
    }
    
	ssize_t download_id = sqlite3_last_insert_rowid((sqlite3*)m_mpSqliteDB);
	if (download_id == 0)
	{
		return 0;
	}

    DownloadRecord v;
    v.download_id = download_id;
    v.download_Url = downloadUrl;
	v.download_header = downloadHeader;
    v.filePath = fileName;
    v.fileSize = fileSize;
    v.startTime = startTime;
    v.isFinished = 0;
    v.textTag = textTag;
    m_mDownloadRecords.insert(std::map<ssize_t, DownloadRecord>::value_type(v.download_id, v));
    
    return download_id;
}

void CADownloadManager::loadDownloadTasks()
{
	char* szError = 0;
	char** paszResults = 0;
	int nRows = 0, nCols = 0;

	std::string cszSql = "SELECT * FROM [T_DownloadMgr]";
	int nRet = sqlite3_get_table((sqlite3*)m_mpSqliteDB, cszSql.c_str(), &paszResults, &nRows, &nCols, &szError);
	if (nRet != SQLITE_OK)
	{
		return;
	}
    m_mDownloadRecords.clear();

	for (int nCurrentRow = 0; nCurrentRow < nRows; nCurrentRow++)
	{
		DownloadRecord v;
		v.download_id = atol(paszResults[(nCurrentRow*nCols) + nCols + 0]);
		v.download_Url = paszResults[(nCurrentRow*nCols) + nCols + 1];
		v.download_header = paszResults[(nCurrentRow*nCols) + nCols + 2];
		v.filePath = paszResults[(nCurrentRow*nCols) + nCols + 3];
		v.fileSize = atof(paszResults[(nCurrentRow*nCols) + nCols + 4]);
		v.startTime = paszResults[(nCurrentRow*nCols) + nCols + 5];
		v.isFinished = atoi(paszResults[(nCurrentRow*nCols) + nCols + 6]);
        v.textTag = paszResults[(nCurrentRow*nCols) + nCols + 7];
        m_mDownloadRecords.insert(std::map<ssize_t, DownloadRecord>::value_type(v.download_id, v));
	}
	sqlite3_free_table(paszResults);
}

void CADownloadManager::deleteTaskFromDb(ssize_t download_id)
{
	char szSql[256] = { 0 };
	sprintf(szSql, "DELETE FROM [T_DownloadMgr] WHERE id=%lu", download_id);

	char* szError = 0;
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, szSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK || nRet == SQLITE_DONE, "");
    
}

void CADownloadManager::deleteTaskFromDb(const std::string& cszUrl)
{
	char szSql[256] = { 0 };
	sprintf(szSql, "DELETE FROM [T_DownloadMgr] WHERE url=\'%s\'", cszUrl.c_str());

	char* szError = 0;
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, szSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK || nRet == SQLITE_DONE, "");

	for (size_t i = 0; i < m_vDownloadingRequests.size(); i++)
	{
		CADownloadResponse* pDownReq = m_vDownloadingRequests.at(i);
		if (pDownReq->getDownloadUrl() == cszUrl)
		{
			eraseDownload(pDownReq->getDownloadID());
			return;
		}
	}
}

void CADownloadManager::setTaskFinished(ssize_t download_id)
{
	char szSql[256] = { 0 };
	sprintf(szSql, "UPDATE [T_DownloadMgr] SET isFinished=1 WHERE id=%lu", download_id);

	char* szError = 0;
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, szSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK, "");
}

std::vector<ssize_t> CADownloadManager::selectIdFromTextTag(const std::string& textTag)
{
    std::vector<ssize_t> download_ids;
    
    char* szError = 0;
    char** paszResults = 0;
    int nRows = 0, nCols = 0;
    char cszSql[256] = { 0 };
    sprintf(cszSql, "SELECT id FROM [T_DownloadMgr] WHERE textTag=%s", textTag.c_str());
    int nRet = sqlite3_get_table((sqlite3*)m_mpSqliteDB, cszSql, &paszResults, &nRows, &nCols, &szError);
    if (nRet != SQLITE_OK)
    {
        return download_ids;
    }
    
    for (int nCurrentRow = 0; nCurrentRow < nRows; nCurrentRow++)
    {
        ssize_t download_id = atol(paszResults[(nCurrentRow*nCols)]);
        download_ids.push_back(download_id);
    }
    sqlite3_free_table(paszResults);
    return download_ids;
}

ssize_t CADownloadManager::enqueueDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& headers, const std::string& textTag)
{
	ssize_t download_id = insertDownload(downloadUrl, headers, fileName, textTag);
	if (download_id > 0)
	{
		CADownloadResponse* quest = CADownloadResponse::create(downloadUrl, fileName, download_id, headers);
		this->enqueueDownload(quest);
	}
    return download_id;
}

ssize_t CADownloadManager::enqueueDownloadEx(const std::string& downloadUrl, const std::string& fileName, const std::string& headers, const std::string& textTag)
{
	deleteTaskFromDb(downloadUrl);
	return enqueueDownload(downloadUrl, fileName, headers, textTag);
}

void CADownloadManager::enqueueDownload(CADownloadResponse* request)
{
    CC_RETURN_IF(request == NULL);
    m_mCADownloadResponses.insert(request->getDownloadID(), request);
    CC_RETURN_IF(m_vDownloadingRequests.contains(request));

	if (m_vDownloadingRequests.size() < (size_t)m_nDownloadMaxCount)
	{
		if (request->startDownload())
		{
			m_vDownloadingRequests.pushBack(request);
		}
	}
	else
	{
		m_dWaitCADownloadResponses.pushBack(request);
	}
}

void CADownloadManager::resumeDownload(ssize_t download_id)
{
	CADownloadResponse* pDownloadReq = m_mCADownloadResponses.at(download_id);
	if (pDownloadReq && m_vPauseCADownloadResponses.contains(pDownloadReq))
	{
        if (m_vDownloadingRequests.size() < (size_t)m_nDownloadMaxCount)
        {
            if (!pDownloadReq->isDownloaded())
            {
                pDownloadReq->startDownload();
            }
            pDownloadReq->setDownloadCmd(DownloadCmd_resume);
            m_vDownloadingRequests.pushBack(pDownloadReq);
        }
        else
        {
            m_dWaitCADownloadResponses.pushBack(pDownloadReq);
        }
        m_vPauseCADownloadResponses.eraseObject(pDownloadReq);
        if (m_pDelegate)
        {
            m_pDelegate->onResumeDownload(download_id);
        }
	}
    else if (m_mDownloadRecords.count(download_id))
    {
        const DownloadRecord& record = m_mDownloadRecords.at(download_id);
		this->enqueueDownload(CADownloadResponse::create(record.download_Url, record.filePath, download_id, record.download_header));
        if (m_pDelegate)
        {
            m_pDelegate->onResumeDownload(download_id);
        }
    }
}

void CADownloadManager::pauseDownload(ssize_t download_id)
{
    CADownloadResponse* pDownloadReq = m_mCADownloadResponses.at(download_id);
    if (pDownloadReq)
    {
        m_vDownloadingRequests.eraseObject(pDownloadReq);
        m_dWaitCADownloadResponses.eraseObject(pDownloadReq);
        pDownloadReq->setDownloadCmd(DownloadCmd_Pause);
        m_vPauseCADownloadResponses.pushBack(pDownloadReq);
        
        if (m_pDelegate)
        {
            m_pDelegate->onPauseDownload(download_id);
        }
    }
}

void CADownloadManager::eraseDownload(ssize_t download_id)
{
    CADownloadResponse* pDownloadReq = m_mCADownloadResponses.at(download_id);
    if (pDownloadReq)
    {
		pDownloadReq->setDownloadCmd(DownloadCmd_Delete);
    }

	deleteTaskFromDb(download_id);

	std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
	if (itr != m_mDownloadRecords.end())
	{
		const std::string& filePath = m_mDownloadRecords.at(download_id).filePath;

		if (remove(filePath.c_str()) != 0)
		{
			remove(std::string(filePath + ".tmp").c_str());
		}

		m_mDownloadRecords.erase(download_id);
	}
}

const char* CADownloadManager::getDownloadUrl(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.download_Url.c_str();
}

const char* CADownloadManager::getDownloadHeader(ssize_t download_id)
{
	std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
	return itr == m_mDownloadRecords.end() ? NULL : itr->second.download_header.c_str();
}

const char* CADownloadManager::getFilePath(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.filePath.c_str();
}

ssize_t CADownloadManager::getTotalFileSize(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? 0 : (ssize_t)(itr->second.fileSize);
}
ssize_t CADownloadManager::getLocalFileSize(ssize_t download_id)
{
    if (m_mCADownloadResponses.contains(download_id))
    {
        return (ssize_t)m_mCADownloadResponses.at(download_id)->getLocalFileSize();
    }
    else
    {
        return (ssize_t)_getLocalFileSize(this->getFilePath(download_id));
    }
}

const char* CADownloadManager::getStartTime(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.startTime.c_str();
}

bool CADownloadManager::isFinished(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? false : itr->second.isFinished;
}

bool CADownloadManager::isDownloading(ssize_t download_id)
{
    CADownloadResponse* quest = m_mCADownloadResponses.at(download_id);
    if (quest)
    {
        return (m_vDownloadingRequests.contains(quest) || m_dWaitCADownloadResponses.contains(quest));
    }
    return false;
}

void CADownloadManager::clearOnSuccessDownloadAllRecord()
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.begin();
    while (itr!=m_mDownloadRecords.end())
    {
        if (itr->second.isFinished)
        {
			deleteTaskFromDb(itr->second.download_id);
			m_mDownloadRecords.erase(itr);
        }
        else
        {
            itr++;
        }
    }
}

void CADownloadManager::clearOnSuccessDownloadRecord(ssize_t download_id)
{
    std::map<ssize_t, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    if (itr != m_mDownloadRecords.end() && itr->second.isFinished)
    {
		deleteTaskFromDb(itr->second.download_id);
        m_mDownloadRecords.erase(itr);
    }
}

std::vector<ssize_t> CADownloadManager::getDownloadIdsFromTextTag(const std::string& textTag)
{
    std::vector<ssize_t> download_ids;
    std::map<ssize_t, DownloadRecord>::iterator itr;
    for (itr=m_mDownloadRecords.begin(); itr!=m_mDownloadRecords.end(); itr++)
    {
        if (itr->second.textTag.compare(textTag) == 0)
        {
            download_ids.push_back(itr->second.download_id);
        }
    }
    return download_ids;
}

std::vector<std::string> CADownloadManager::getDownloadAllTextTags()
{
    std::set<std::string> download_textTagSet;
    std::vector<std::string> download_textTags;
    std::map<ssize_t, DownloadRecord>::iterator itr;
    for (itr=m_mDownloadRecords.begin(); itr!=m_mDownloadRecords.end(); itr++)
    {
        CC_CONTINUE_IF(download_textTagSet.count(itr->second.textTag) > 0);
        download_textTagSet.insert(itr->second.textTag);
        download_textTags.push_back(itr->second.textTag);
    }
    download_textTagSet.clear();
    return download_textTags;
}

void CADownloadManager::onError(CADownloadResponse* request, CADownloadManager::ErrorCode errorCode)
{
    if (m_pDelegate)
    {
        m_pDelegate->onError(request->getDownloadID(), errorCode);
    }
    m_mCADownloadResponses.erase(request->getDownloadID());
    m_vDownloadingRequests.eraseObject(request);
    setTaskFinished(request->getDownloadID());
    
    if (!m_dWaitCADownloadResponses.empty())
    {
        CADownloadResponse* request = m_dWaitCADownloadResponses.front();
        if (request->isDownloaded())
        {
            request->setDownloadCmd(DownloadCmd_resume);
            m_vDownloadingRequests.pushBack(request);
            m_dWaitCADownloadResponses.popFront();
        }
        else if (request->startDownload())
        {
            m_vDownloadingRequests.pushBack(request);
            m_dWaitCADownloadResponses.popFront();
        }
    }
}

void CADownloadManager::onProgress(CADownloadResponse* request, int percent, ssize_t nowDownloaded, ssize_t totalToDownload)
{
    if (m_pDelegate)
    {
        m_pDelegate->onProgress(request->getDownloadID(), percent, nowDownloaded, totalToDownload);
    }
}

void CADownloadManager::onSuccess(CADownloadResponse* request)
{
	if (request == NULL)
		return;

	ssize_t download_id = request->getDownloadID();
	if (request->isDownloadAbort())
	{
		CADownloadResponse* pDownloadReq = m_mCADownloadResponses.at(download_id);
		if (pDownloadReq)
		{
			pDownloadReq->setDownloadCmd(DownloadCmd_Pause);
			pDownloadReq->retain()->autorelease();
			if (m_vDownloadingRequests.contains(pDownloadReq))
			{
				m_vDownloadingRequests.eraseObject(pDownloadReq);
			}
			else if (m_dWaitCADownloadResponses.contains(pDownloadReq))
			{
				m_dWaitCADownloadResponses.eraseObject(pDownloadReq);
			}
			else if (m_vPauseCADownloadResponses.contains(pDownloadReq))
			{
				m_vPauseCADownloadResponses.eraseObject(pDownloadReq);
			}
		}
		m_mCADownloadResponses.erase(download_id);
	}
	else
	{
		if (m_pDelegate)
		{
			m_pDelegate->onSuccess(request->getDownloadID());
		}
		setTaskFinished(request->getDownloadID());


		if (m_mDownloadRecords.count(request->getDownloadID()) > 0)
		{
			m_mDownloadRecords.at(request->getDownloadID()).isFinished = true;
		}
		if (m_mCADownloadResponses.erase(request->getDownloadID()))
		{

		}

		m_vDownloadingRequests.eraseObject(request);

		if (!m_dWaitCADownloadResponses.empty())
		{
			CADownloadResponse* request = m_dWaitCADownloadResponses.front();
			if (request->isDownloaded())
			{
				request->setDownloadCmd(DownloadCmd_resume);
				m_vDownloadingRequests.pushBack(request);
				m_dWaitCADownloadResponses.popFront();
			}
			else if (request->startDownload())
			{
				request->setDownloadCmd(DownloadCmd_resume);
				m_vDownloadingRequests.pushBack(request);
				m_dWaitCADownloadResponses.popFront();
			}
		}
	}
}

#pragma CADownloadResponse

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
	FILE *fp = (FILE*)userdata;
	size_t written = fwrite(ptr, size, nmemb, fp);
	fflush(fp);
	return written;
}

int CADownloadResponseProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded);

class DownloadResponseHelper : public CrossApp::CAObject
{
	typedef struct _Message
	{
		_Message() : what(0), obj(nullptr){}
		_Message(unsigned int w, void* o) : what(w), obj(o) {}

		unsigned int what;

		void* obj;

	}Message;
public:

	DownloadResponseHelper()
		: _curl(nullptr)
		, m_headers(nullptr)
	{
		_messageQueue = new std::list<Message*>();
		pthread_mutex_init(&_messageQueueMutex, NULL);
		CAScheduler::getScheduler()->schedule(schedule_selector(DownloadResponseHelper::update), this, 0);
	}

	~DownloadResponseHelper()
	{
		delete _messageQueue;
	}

	void sendMessage(unsigned int what, void* obj)
	{
		pthread_mutex_lock(&_messageQueueMutex);
		_messageQueue->push_back(new Message(what, obj));
		pthread_mutex_unlock(&_messageQueueMutex);
	}

	virtual void update(float dt)
	{
		std::list<Message*> MsgListTemp;

		pthread_mutex_lock(&_messageQueueMutex);
		MsgListTemp = *_messageQueue;
		_messageQueue->clear();
		pthread_mutex_unlock(&_messageQueueMutex);

		for (std::list<Message*>::iterator it = MsgListTemp.begin(); it != MsgListTemp.end(); it++)
		{
			Message *msg = *it;

			switch (msg->what)
			{
			case CADownloadResponse_DOWNLOAD_FINISH:
			{
				this->handleUpdateSucceed(msg);
				CADownloadResponse* request = static_cast<CADownloadResponse*>(msg->obj);
				_manager->onSuccess(request);
			}
			break;

			case CADownloadResponse_PROGRESS:
			{
				ProgressMessage* message = static_cast<ProgressMessage*>(msg->obj);
				std::list<Message*>::iterator it2 = it;
				if (++it2 == MsgListTemp.end())
				{
					_manager->onProgress(message->request, message->percent, message->nowDownloaded, message->totalToDownload);
				}

				delete message;
			}
			break;

			case CADownloadResponse_ERROR:
			{
				ErrorMessage* message = static_cast<ErrorMessage*>(msg->obj);
				_manager->onError(message->request, message->code);
				delete message;
			}
			break;

			default: break;
			}

			delete msg;
		}
		MsgListTemp.clear();
	}

	bool downLoad(CADownloadResponse* pDownloadRes, const std::string& downHeaders, const std::string& downloadUrl, double initialFileSize, FILE *fp)
	{
		_curl = curl_easy_init();
		if (_curl == NULL)
		{
			return false;
		}

		if (!downHeaders.empty())
		{
			m_headers = curl_slist_append(m_headers, downHeaders.c_str());
			curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, m_headers);
		}

		// Download package
		CURLcode res;
		curl_easy_setopt(_curl, CURLOPT_URL, downloadUrl.c_str());

		char cRange[32] = { 0 };
		sprintf(cRange, "%.0f-", initialFileSize);
		curl_easy_setopt(_curl, CURLOPT_RANGE, cRange);
		curl_easy_setopt(_curl, CURLOPT_RESUME_FROM, (long)initialFileSize);
		curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
		curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
		curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, CADownloadResponseProgressFunc);
		curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, pDownloadRes);
		curl_easy_setopt(_curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V6);

		res = curl_easy_perform(_curl);
		curl_easy_cleanup(_curl);
		if (m_headers)
		{
			curl_slist_free_all(m_headers);
			m_headers = NULL;
		}
		_curl = NULL;
		return (res == CURLE_OK);
	}
	void Pause()
	{
		if (_curl)
		{
			curl_easy_pause(_curl, CURLPAUSE_ALL);
		}
	}
	void Resume()
	{
		if (_curl)
		{
			curl_easy_pause(_curl, CURLPAUSE_CONT);
		}
	}
	

private:

	void handleUpdateSucceed(Message *msg)
	{
		CADownloadResponse* manager = (CADownloadResponse*)msg->obj;
		manager->setSearchPath();
	}

	CURL *_curl;

	curl_slist *m_headers;

	std::list<Message*> *_messageQueue;

	pthread_mutex_t _messageQueueMutex;
};


CADownloadResponse::CADownloadResponse(const std::string& downloadUrl, const std::string& fileName, ssize_t downloadId, const std::string& downHeaders)
: _fileName(fileName)
, _downloadUrl(downloadUrl)
, _downHeaders(downHeaders)
, _tid(nullptr)
, _connectionTimeout(0)
, _initialFileSize(0)
, _localFileSize(0)
, _totalFileSize(0)
, _schedule(nullptr)
, _downloadCmd(DownloadCmd_Null)
, _downloadStatus(DownloadStatus_Running)
, _download_id(downloadId)
, m_fDelay(0)
{
    checkStoragePath();
	_schedule = new DownloadResponseHelper();
    CCLog("CADownloadResponse id = %lu", _download_id);
}

CADownloadResponse::~CADownloadResponse()
{
	if (_tid)
	{
		delete _tid;
		_tid = NULL;
	}
    CC_SAFE_RELEASE_NULL(_schedule);
    CCLog("~CADownloadResponse id = %lu", _download_id);
}

CADownloadResponse* CADownloadResponse::create(const std::string& downloadUrl, const std::string& fileName, ssize_t downloadId, const std::string& headers)
{
	CADownloadResponse* request = new CADownloadResponse(downloadUrl, fileName, downloadId, headers);
    if (request)
    {
        request->autorelease();
    }
    return request;
}

void CADownloadResponse::checkStoragePath()
{

}

void* CADownloadResponseDownloadAndUncompress(void *data)
{
    CADownloadResponse* self = static_cast<CADownloadResponse*>(data);
    do
    {
		bool bDSucc = self->downLoad();
		bool bAbort = self->isDownloadAbort();

//		if (!bAbort && !self->uncompress())
//        {
//            self->sendErrorMessage(CADownloadManager::kUncompress);
//        }

		if (bDSucc || bAbort)
		{
			self->_schedule->sendMessage(CADownloadResponse_DOWNLOAD_FINISH, self);
		}
    }
    while (0);

    return NULL;
}

bool CADownloadResponse::startDownload()
{
    if (_tid)
    {
        return false;
    }
    
    if (_downloadUrl.empty())
    {
        return false;
    }
    
    _tid = new pthread_t();
    pthread_create(&(*_tid), NULL, CADownloadResponseDownloadAndUncompress, this);
	return true;
}

bool CADownloadResponse::isDownloaded()
{
    return (_tid != NULL);
}

void CADownloadResponse::setDownloadCmd(int cmd)
{
	_downloadCmd = cmd;
}

bool CADownloadResponse::isDownloadAbort()
{
	return _downloadStatus == DownloadStatus_Abort;
}

bool CADownloadResponse::checkDownloadStatus()
{
	if (_downloadCmd == DownloadCmd_Delete)
	{
		_downloadStatus = DownloadStatus_Abort;
	}

	if (_downloadCmd == DownloadCmd_Pause && _downloadStatus == DownloadStatus_Running)
	{
		_schedule->Pause();
		_downloadCmd = DownloadCmd_Null;
		_downloadStatus = DownloadStatus_Waiting;
	}
	
	if (_downloadCmd == DownloadCmd_resume && _downloadStatus == DownloadStatus_Waiting)
	{
		_schedule->Resume();
		_downloadCmd = DownloadCmd_Null;
		_downloadStatus = DownloadStatus_Running;
	}

	return _downloadStatus == DownloadStatus_Running;
}

bool CADownloadResponse::uncompress()
{
    std::string outFileName = _fileName;
    unzFile zipfile = unzOpen(outFileName.c_str());
    if (! zipfile)
    {
        return false;
    }
    
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        unzClose(zipfile);
        return false;
    }
    
    char readBuffer[BUFFER_SIZE];
    
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        int unz = unzGetCurrentFileInfo(zipfile, &fileInfo, fileName, MAX_FILENAME, NULL, 0, NULL, 0);
        if (unz != UNZ_OK)
        {
            unzClose(zipfile);
            return false;
        }
        
		std::string fullPath = _fileName;
		fullPath = fullPath.substr(0, fullPath.find_last_of('.'));
		fullPath += "/";
        createDirectory(fullPath.c_str());
		fullPath += fileName;
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {

            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);

        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    return true;
}

bool CADownloadResponse::createDirectory(const char *path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
#else
    BOOL ret = CreateDirectoryA(path, NULL);
	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
	{
		return false;
	}
    return true;
#endif
}

void CADownloadResponse::setSearchPath()
{
    std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
    std::vector<std::string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, _fileName);
    FileUtils::getInstance()->setSearchPaths(searchPaths);
}

int CADownloadResponseProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    CADownloadResponse* request = (CADownloadResponse*)ptr;
    if (request == NULL)
    {
        return 0;
    }

	if (request->isDownloadAbort())
	{
		return 1;
	}

	if (!request->checkDownloadStatus())
	{
		return 0;
	}
    
    nowDownloaded += request->getInitialFileSize();
	totalToDownload += request->getInitialFileSize();
    
    request->setLocalFileSize(nowDownloaded);
    request->setTotalFileSize(totalToDownload);
    
    ProgressMessage *progressData = new ProgressMessage();
    progressData->nowDownloaded = nowDownloaded;
    progressData->totalToDownload = totalToDownload;
	if (totalToDownload > 0)
	{
		progressData->percent = (int)(nowDownloaded / totalToDownload * 100);
	}
	else
	{
		progressData->percent = 0;
	}
    
    progressData->request = request;

	request->_schedule->sendMessage(CADownloadResponse_PROGRESS, progressData);
    
	CCLog("downloading... %d%%  download_id = %lu", progressData->percent, request->_download_id);
    
    return 0;
}

bool CADownloadResponse::downLoad()
{
    // Create a file to save package.
    std::string outFileName = _fileName + ".tmp";
	_initialFileSize = _getLocalFileSize(outFileName.c_str());
    FILE *fp = fopen(outFileName.c_str(), "ab+");
    if (! fp)
    {
        sendErrorMessage(CADownloadManager::kCreateFile);
        return false;
    }
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    chmod(outFileName.c_str(), 0666);
#endif
    
	if (!_schedule->downLoad(this, _downHeaders, _downloadUrl, _initialFileSize, fp))
	{
		sendErrorMessage(CADownloadManager::kNetwork);
		fclose(fp);
		return false;
	}

	if (isDownloadAbort())
    {
        fclose(fp);
        return false;
    }
    
    fclose(fp);
	rename(outFileName.c_str(), _fileName.c_str());

    return true;
}

ssize_t CADownloadResponse::getDownloadID() const
{
    return _download_id;
}

const std::string& CADownloadResponse::getDownloadUrl() const
{
    return _downloadUrl;
}

const std::string& CADownloadResponse::getFileName() const
{
    return _fileName;
}

unsigned int CADownloadResponse::getConnectionTimeout()
{
    return _connectionTimeout;
}

void CADownloadResponse::sendErrorMessage(CADownloadManager::ErrorCode code)
{
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->request = this;
	_schedule->sendMessage(CADownloadResponse_ERROR, errorMessage);
}

NS_CC_END;

