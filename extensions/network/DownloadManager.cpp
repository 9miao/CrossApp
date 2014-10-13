

#include "DownloadManager.h"
#include "CrossApp.h"
#include "../sqlite3/include/sqlite3.h"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <list>
#include <vector>

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif

#include "support/zip_support/unzip.h"

using namespace CrossApp;
using namespace std;

NS_CC_EXT_BEGIN;

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

#define DownloadRequest_START_DOWNLOAD                0
#define DownloadRequest_PROGRESS                      1
#define DownloadRequest_ERROR                         2
#define DownloadRequest_DOWNLOAD_FINISH               3

struct ErrorMessage
{
    DownloadManager::ErrorCode code;
    DownloadRequest* request;
};

struct ProgressMessage
{
    int percent;
    unsigned long nowDownloaded;
    unsigned long totalToDownload;
    DownloadRequest* request;
};

enum
{
	DownloadStatus_Running,
	DownloadStatus_Waiting,
};

enum
{
	DownloadCmd_Null,
	DownloadCmd_Pause,
	DownloadCmd_resume,
};


class DownloadRequest : public CAObject
{
public:

	friend class DownloadManager;
    
    static DownloadRequest* create(const std::string& downloadUrl,
                                   const std::string& fileName,
                                   unsigned long downloadId);

    DownloadRequest(const std::string& downloadUrl, const std::string& fileName, unsigned long downloadId);
    
    virtual ~DownloadRequest();
    
    bool startDownload();
    
    bool isDownloaded();

	void setDownloadCmd(int cmd);
    
    unsigned long getDownloadID() const;
    
    const char* getDownloadUrl() const;
    
    const char* getFileName() const;
    
    unsigned int getConnectionTimeout();
    
    friend void* DownloadRequestDownloadAndUncompress(void*);
    
    friend int DownloadRequestProgressFunc(void *, double, double, double, double);

	bool checkDownloadStatus();
    
protected:
    
    unsigned long getLocalFileLenth(const std::string& fileName);
    
    bool downLoad();
    
    void checkStoragePath();
    
    bool uncompress();
    
    bool createDirectory(const char *path);
    
    void setSearchPath();
    
    void sendErrorMessage(DownloadManager::ErrorCode code);
    
private:
    
    typedef struct _Message
    {
        _Message() : what(0), obj(NULL){}
        
        unsigned int what;
        
        void* obj;
        
    }Message;
    
    class Helper : public CrossApp::CAObject
    {
    public:
        
        Helper();
        
        ~Helper();
        
        virtual void update(float dt);
        
        void sendMessage(Message *msg);
        
    private:
        
        void handleUpdateSucceed(Message *msg);
        
        std::list<Message*> *_messageQueue;
        
        pthread_mutex_t _messageQueueMutex;
    };
    
private:
    
    std::string _fileName;
    
    std::string _downloadUrl;
    
    CURL *_curl;
    
    curl_slist *m_headers;
    
    Helper *_schedule;
    
    pthread_t *_tid;

	int _downloadCmd;
    
	int _downloadStatus;
    
    unsigned int _connectionTimeout;
    
    unsigned long _download_id;
};



#pragma DownloadManager

static DownloadManager* _manager = NULL;

DownloadManager* DownloadManager::getInstance()
{
    if (_manager == NULL)
    {
        _manager = new DownloadManager();
    }
    return _manager;
}

void DownloadManager::destroyInstance()
{
    if (_manager)
    {
        delete _manager;
		_manager = NULL;
    }
}


DownloadManager::DownloadManager()
:m_pDelegate(NULL)
,m_nDownloadMaxCount(1)
{
	std::string fullPath = CCFileUtils::sharedFileUtils()->getWritablePath() + "downloadMgr.db";

	int nRet = sqlite3_open(fullPath.c_str(), (sqlite3**)&m_mpSqliteDB);
	CCAssert(nRet == SQLITE_OK, "");
	CCAssert(m_mpSqliteDB != NULL, "");
	sqlite3_busy_timeout((sqlite3*)m_mpSqliteDB, 60000);

	checkSqliteDB();
    loadDownloadTasks();
}

DownloadManager::~DownloadManager()
{
	sqlite3_close((sqlite3*)m_mpSqliteDB);
}

void DownloadManager::checkSqliteDB()
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
		cszSql = "CREATE TABLE [T_DownloadMgr] ([id] INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,[url] NVARCHAR(1024) NOT NULL,[filePath] NVARCHAR(512) NOT NULL,[fileSize] REAL NOT NULL,[startTime] NVARCHAR(64) NOT NULL,[isFinished] INT DEFAULT (0) NOT NULL,[textTag] NVARCHAR(1024) DEFAULT NULL NOT NULL)";
		nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, cszSql.c_str(), 0, 0, &szError);
		CCAssert(nRet == SQLITE_OK, "");
	}
}


double DownloadManager::getDownloadFileSize(const std::string& downloadUrl)
{
    double downloadFileSize = 0;
    
    CURL *handle = curl_easy_init();
    
    curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Referer: http://images.dmzj.com/");
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
    
    curl_easy_setopt(handle, CURLOPT_URL, downloadUrl.c_str());
    curl_easy_setopt(handle, CURLOPT_HEADER, 1);
    curl_easy_setopt(handle, CURLOPT_NOBODY, 1);
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


unsigned long DownloadManager::insertDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag)
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
    
    double fileSize = this->getDownloadFileSize(downloadUrl.c_str());
    char* szError = 0;
	char cszSql[4096] = { 0 };
	sprintf(cszSql, "INSERT INTO [T_DownloadMgr] (url, filePath, fileSize, startTime, textTag) values ('%s', '%s', %.0f, '%s', '%s');",
		downloadUrl.c_str(), fileName.c_str(), fileSize, startTime, textTag.c_str());
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, cszSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK, "");
    
	unsigned long download_id = sqlite3_last_insert_rowid((sqlite3*)m_mpSqliteDB);

    DownloadRecord v;
    v.download_id = download_id;
    v.download_Url = downloadUrl;
    v.filePath = fileName;
    v.fileSize = fileSize;
    v.startTime = startTime;
    v.isFinished = 0;
    m_mDownloadRecords.insert(std::map<unsigned long, DownloadRecord>::value_type(v.download_id, v));
    
    return download_id;
}

void DownloadManager::loadDownloadTasks()
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
		v.filePath = paszResults[(nCurrentRow*nCols) + nCols + 2];
		v.fileSize = atof(paszResults[(nCurrentRow*nCols) + nCols + 3]);
		v.startTime = paszResults[(nCurrentRow*nCols) + nCols + 4];
		v.isFinished = atoi(paszResults[(nCurrentRow*nCols) + nCols + 5]);
        v.textTag = paszResults[(nCurrentRow*nCols) + nCols + 6];
        m_mDownloadRecords.insert(std::map<unsigned long, DownloadRecord>::value_type(v.download_id, v));
	}
	sqlite3_free_table(paszResults);
}

void DownloadManager::deleteTaskFromDb(unsigned long download_id)
{
	char szSql[256] = { 0 };
	sprintf(szSql, "DELETE * FROM [T_DownloadMgr] WHERE id=%lu", download_id);

	char* szError = 0;
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, szSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK, "");
}

void DownloadManager::setTaskFinished(unsigned long download_id)
{
	char szSql[256] = { 0 };
	sprintf(szSql, "UPDATE [T_DownloadMgr] SET isFinished=1 WHERE id=%lu", download_id);

	char* szError = 0;
	int nRet = sqlite3_exec((sqlite3*)m_mpSqliteDB, szSql, 0, 0, &szError);
	CCAssert(nRet == SQLITE_OK, "");
}

std::vector<unsigned long> DownloadManager::selectIdFromTextTag(const std::string& textTag)
{
    std::vector<unsigned long> download_ids;
    
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
        unsigned long download_id = atol(paszResults[(nCurrentRow*nCols)]);
        download_ids.push_back(download_id);
    }
    sqlite3_free_table(paszResults);
    return download_ids;
}

unsigned long DownloadManager::enqueueDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag)
{
    unsigned long download_id = insertDownload(downloadUrl, fileName, textTag);
    DownloadRequest* quest = DownloadRequest::create(downloadUrl, fileName, download_id);
	this->enqueueDownload(quest);
    return  download_id;
}

void DownloadManager::enqueueDownload(DownloadRequest* request)
{
    CC_RETURN_IF(request == NULL);
    CC_RETURN_IF(!m_mDownloadRequests.insert(request->getDownloadID(), request));

	if (m_vDownloadingRequests.size() < m_nDownloadMaxCount)
	{
		if (request->startDownload())
		{
			m_vDownloadingRequests.pushBack(request);
		}
	}
	else
	{
		m_dWaitDownloadRequests.pushBack(request);
	}
}

void DownloadManager::resumeDownload(unsigned long download_id)
{
	DownloadRequest* pDownloadReq = m_mDownloadRequests.getValue(download_id);
	if (pDownloadReq && m_vPauseDownloadRequests.contains(pDownloadReq))
	{
        //如果暂停列表里包含此下载id
        if (m_vDownloadingRequests.size() < m_nDownloadMaxCount)
        {
            //如果正在下载个数未达到下载上限
            pDownloadReq->setDownloadCmd(DownloadCmd_resume);
            m_vDownloadingRequests.pushBack(pDownloadReq);
        }
        else
        {
            //如果正在下载个数达到下载上限,放在等待队列里
            m_dWaitDownloadRequests.pushBack(pDownloadReq);
        }
        m_vPauseDownloadRequests.eraseObject(pDownloadReq);
        if (m_pDelegate)
        {
            m_pDelegate->onResumeDownload(download_id);
        }
	}
    else if (m_mDownloadRecords.count(download_id))
    {
        //如果暂停列表里不包含此下载id，而下载信息中包含
        const DownloadRecord& record = m_mDownloadRecords.at(download_id);
        this->enqueueDownload(DownloadRequest::create(record.download_Url, record.filePath, download_id));
        if (m_pDelegate)
        {
            m_pDelegate->onResumeDownload(download_id);
        }
    }
}

void DownloadManager::pauseDownload(unsigned long download_id)
{
    DownloadRequest* pDownloadReq = m_mDownloadRequests.getValue(download_id);
    if (pDownloadReq && m_vDownloadingRequests.contains(pDownloadReq))
    {
        //如果下载列表里包含此下载id
        pDownloadReq->setDownloadCmd(DownloadCmd_Pause);
        m_vPauseDownloadRequests.pushBack(pDownloadReq);
        m_vDownloadingRequests.eraseObject(pDownloadReq);
        if (m_pDelegate)
        {
            m_pDelegate->onPauseDownload(download_id);
        }
    }
}

void DownloadManager::eraseDownload(unsigned long download_id)
{
    DownloadRequest* pDownloadReq = m_mDownloadRequests.getValue(download_id);
    if (pDownloadReq)
    {
        pDownloadReq->setDownloadCmd(DownloadCmd_Pause);
        if (m_vDownloadingRequests.contains(pDownloadReq))
        {
            //如果下载列表里包含此下载id
            m_vDownloadingRequests.eraseObject(pDownloadReq);
        }
        else if (m_dWaitDownloadRequests.contains(pDownloadReq))
        {
            //如果等待下载列表里包含此下载id
            m_dWaitDownloadRequests.eraseObject(pDownloadReq);
        }
        else if (m_vPauseDownloadRequests.contains(pDownloadReq))
        {
            //如果暂停下载列表里包含此下载id
            m_vPauseDownloadRequests.eraseObject(pDownloadReq);
        }
    }
    
	//(需添加内容)释放 DownloadRequest
    {
        m_mDownloadRequests.erase(download_id);
    }
	deleteTaskFromDb(download_id);
	m_mDownloadRecords.erase(download_id);
}

const char* DownloadManager::getDownloadUrl(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.download_Url.c_str();
}

const char* DownloadManager::getFilePath(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.filePath.c_str();
}

unsigned long DownloadManager::getFileSize(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? 0 : (unsigned long)(itr->second.fileSize);
}

const char* DownloadManager::getStartTime(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? NULL : itr->second.startTime.c_str();
}

bool DownloadManager::isFinished(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    return itr == m_mDownloadRecords.end() ? false : itr->second.isFinished;
}

void DownloadManager::clearOnSuccessDownloadAllRecord()
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.begin();
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

void DownloadManager::clearOnSuccessDownloadRecord(unsigned long download_id)
{
    std::map<unsigned long, DownloadRecord>::iterator itr = m_mDownloadRecords.find(download_id);
    if (itr != m_mDownloadRecords.end() && itr->second.isFinished)
    {
		deleteTaskFromDb(itr->second.download_id);
        m_mDownloadRecords.erase(itr);
    }
}

std::vector<unsigned long> DownloadManager::getDownloadIdsFromTextTag(const std::string& textTag)
{
    std::vector<unsigned long> download_ids;
    std::map<unsigned long, DownloadRecord>::iterator itr;
    for (itr=m_mDownloadRecords.begin(); itr!=m_mDownloadRecords.end(); itr++)
    {
        if (itr->second.textTag.compare(textTag) == 0)
        {
            download_ids.push_back(itr->second.download_id);
        }
    }
    return download_ids;
}

void DownloadManager::onError(DownloadRequest* request, DownloadManager::ErrorCode errorCode)
{
    if (m_pDelegate)
    {
        m_pDelegate->onError(request->getDownloadID(), errorCode);
    }
    m_mDownloadRequests.erase(request->getDownloadID());
    m_vDownloadingRequests.eraseObject(request);
    setTaskFinished(request->getDownloadID());
    
    if (!m_dWaitDownloadRequests.empty())
    {
        //如果等待下载列表有内容
        DownloadRequest* request = m_dWaitDownloadRequests.front();
        if (request->isDownloaded())
        {
            //如果此下载已经启动下载
            request->setDownloadCmd(DownloadCmd_resume);
            m_vDownloadingRequests.pushBack(request);
            m_dWaitDownloadRequests.popFront();
        }
        else if (request->startDownload())
        {
            //如果此下载没有启动下载且启动成功
            m_vDownloadingRequests.pushBack(request);
            m_dWaitDownloadRequests.popFront();
        }
    }
}

void DownloadManager::onProgress(DownloadRequest* request, int percent, unsigned long nowDownloaded, unsigned long totalToDownload)
{
    if (m_pDelegate)
    {
        m_pDelegate->onProgress(request->getDownloadID(), percent, nowDownloaded, totalToDownload);
    }
}

void DownloadManager::onSuccess(DownloadRequest* request)
{
    if (m_pDelegate)
    {
        m_pDelegate->onSuccess(request->getDownloadID());
    }
    //(需添加内容)更新对应数据库...
	setTaskFinished(request->getDownloadID());
    
    CCLog("ssss - %lu", request->getDownloadID());
    
    if (m_mDownloadRecords.count(request->getDownloadID()) > 0)
    {
        m_mDownloadRecords.at(request->getDownloadID()).isFinished = true;
    }
    if (m_mDownloadRequests.erase(request->getDownloadID()))
    {
        CCLog("ssss - %lu", request->getDownloadID());
    }

    m_vDownloadingRequests.eraseObject(request);
    
    if (!m_dWaitDownloadRequests.empty())
    {
        //如果等待下载列表有内容
        DownloadRequest* request = m_dWaitDownloadRequests.front();
        if (request->isDownloaded())
        {
            //如果此下载已经启动下载
            request->setDownloadCmd(DownloadCmd_resume);
            m_vDownloadingRequests.pushBack(request);
            m_dWaitDownloadRequests.popFront();
        }
        else if (request->startDownload())
        {
            //如果此下载没有启动下载且启动成功
            m_vDownloadingRequests.pushBack(request);
            m_dWaitDownloadRequests.popFront();
        }
    }
}

#pragma DownloadRequest

DownloadRequest::DownloadRequest(const std::string& downloadUrl, const std::string& fileName, unsigned long downloadId)
: _fileName(fileName)
, _downloadUrl(downloadUrl)
, _curl(NULL)
, _tid(NULL)
, _connectionTimeout(0)
, _schedule(NULL)
, _downloadCmd(DownloadCmd_Null)
, _downloadStatus(DownloadStatus_Running)
, _download_id(downloadId)
, m_headers(NULL)
{
    checkStoragePath();
    _schedule = new Helper();
    CCLog("DownloadRequest id = %lu", _download_id);
}

DownloadRequest::~DownloadRequest()
{
    CC_SAFE_RELEASE_NULL(_schedule);
    CCLog("~DownloadRequest id = %lu", _download_id);
}

DownloadRequest* DownloadRequest::create(const std::string& downloadUrl, const std::string& fileName, unsigned long downloadId)
{
    DownloadRequest* request = new DownloadRequest(downloadUrl, fileName, downloadId);
    if (request)
    {
        request->autorelease();
    }
    return request;
}

void DownloadRequest::checkStoragePath()
{

}

void* DownloadRequestDownloadAndUncompress(void *data)
{
    DownloadRequest* self = static_cast<DownloadRequest*>(data);
    do
    {
        CC_BREAK_IF(!self->downLoad());
        
//        if (!self->uncompress())
//        {
//            
//            self->sendErrorMessage(DownloadManager::kUncompress);
//            //break;
//        }

        DownloadRequest::Message *msg = new DownloadRequest::Message();
        msg->what = DownloadRequest_DOWNLOAD_FINISH;
        msg->obj = self;
        self->_schedule->sendMessage(msg);
        
    }
    while (0);
    
    if (self->_tid)
    {
        delete self->_tid;
        self->_tid = NULL;
    }

    return NULL;
}

bool DownloadRequest::startDownload()
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
    pthread_create(&(*_tid), NULL, DownloadRequestDownloadAndUncompress, this);
	return true;
}

bool DownloadRequest::isDownloaded()
{
    return (_tid != NULL);
}

void DownloadRequest::setDownloadCmd(int cmd)
{
	_downloadCmd = cmd;
}

bool DownloadRequest::checkDownloadStatus()
{
	if (_downloadCmd == DownloadCmd_Pause && _downloadStatus == DownloadStatus_Running)
	{
		curl_easy_pause(_curl, CURLPAUSE_ALL);
		_downloadCmd = DownloadCmd_Null;
		_downloadStatus = DownloadStatus_Waiting;
	}
	
	if (_downloadCmd == DownloadCmd_resume && _downloadStatus == DownloadStatus_Waiting)
	{
		curl_easy_pause(_curl, CURLPAUSE_CONT);
		_downloadCmd = DownloadCmd_Null;
		_downloadStatus = DownloadStatus_Running;
	}

	return _downloadStatus == DownloadStatus_Running;
}

bool DownloadRequest::uncompress()
{
    string outFileName = _fileName;
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
        
		string fullPath = _fileName;
		fullPath = fullPath.substr(0, fullPath.find_last_of('\\'));
		fullPath += "\\";
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

bool DownloadRequest::createDirectory(const char *path)
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

void DownloadRequest::setSearchPath()
{
    vector<string> searchPaths = CCFileUtils::sharedFileUtils()->getSearchPaths();
    vector<string>::iterator iter = searchPaths.begin();
    searchPaths.insert(iter, _fileName);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
}

static size_t downLoadPackage(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    FILE *fp = (FILE*)userdata;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

int DownloadRequestProgressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{
    DownloadRequest* request = (DownloadRequest*)ptr;
	if (!request->checkDownloadStatus())
	{
		return 0;
	}
	
    DownloadRequest::Message *msg = new DownloadRequest::Message();
    msg->what = DownloadRequest_PROGRESS;
    
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
    msg->obj = progressData;

    request->_schedule->sendMessage(msg);
    
	CCLog("downloading... %d%%  download_id = %lu", progressData->percent, request->_download_id);
    
    return 0;
}

unsigned long DownloadRequest::getLocalFileLenth(const std::string& fileName)
{
	unsigned long lCurFileSize = 0;

	FILE* fp = fopen(fileName.c_str(), "r");
	if (fp != NULL)
	{
		fseek(fp, 0L, SEEK_END);
		lCurFileSize = ftell(fp);
		fclose(fp);
	}
	return lCurFileSize;
}

bool DownloadRequest::downLoad()
{
    // Create a file to save package.
    string outFileName = _fileName+".tmp";
	unsigned long _curFileLength = getLocalFileLenth(outFileName.c_str());
    FILE *fp = fopen(outFileName.c_str(), "ab+");
    if (! fp)
    {
        sendErrorMessage(DownloadManager::kCreateFile);
        return false;
    }
    
	_curl = curl_easy_init();
	if (_curl == NULL)
	{
		sendErrorMessage(DownloadManager::kNetwork);
		fclose(fp);
		return false;
	}

    m_headers = curl_slist_append(m_headers,"Referer: http://images.dmzj.com/");
    curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, m_headers);
    
    // Download package
    CURLcode res;
    curl_easy_setopt(_curl, CURLOPT_URL, _downloadUrl.c_str());

	char cRange[32] = { 0 };
	sprintf(cRange, "%lu-", _curFileLength);
	curl_easy_setopt(_curl, CURLOPT_RANGE, cRange);
	curl_easy_setopt(_curl, CURLOPT_RESUME_FROM, _curFileLength);
    curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, downLoadPackage);
    curl_easy_setopt(_curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(_curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSFUNCTION, DownloadRequestProgressFunc);
    curl_easy_setopt(_curl, CURLOPT_PROGRESSDATA, this);
    
    res = curl_easy_perform(_curl);
    curl_easy_cleanup(_curl);
    if (m_headers)
    {
        curl_slist_free_all(m_headers);
        m_headers = NULL;
    }
    
    if (res != 0)
    {
        sendErrorMessage(DownloadManager::kNetwork);
        fclose(fp);
        return false;
    }
    
    fclose(fp);
	rename(outFileName.c_str(), _fileName.c_str());

    return true;
}

unsigned long DownloadRequest::getDownloadID() const
{
    return _download_id;
}

const char* DownloadRequest::getDownloadUrl() const
{
    return _downloadUrl.c_str();
}

const char* DownloadRequest::getFileName() const
{
    return _fileName.c_str();
}

unsigned int DownloadRequest::getConnectionTimeout()
{
    return _connectionTimeout;
}

void DownloadRequest::sendErrorMessage(DownloadManager::ErrorCode code)
{
    Message *msg = new Message();
    msg->what = DownloadRequest_ERROR;
    
    ErrorMessage *errorMessage = new ErrorMessage();
    errorMessage->code = code;
    errorMessage->request = this;
    msg->obj = errorMessage;
    
    _schedule->sendMessage(msg);
}

// Implementation of DownloadRequestHelper

DownloadRequest::Helper::Helper()
{
    _messageQueue = new list<Message*>();
    pthread_mutex_init(&_messageQueueMutex, NULL);
    CAScheduler::schedule(schedule_selector(DownloadRequest::Helper::update), this, 0);
}

DownloadRequest::Helper::~Helper()
{
    delete _messageQueue;
}

void DownloadRequest::Helper::sendMessage(Message *msg)
{
    pthread_mutex_lock(&_messageQueueMutex);
    _messageQueue->push_back(msg);
    pthread_mutex_unlock(&_messageQueueMutex);
}

void DownloadRequest::Helper::update(float dt)
{
    Message *msg = NULL;
    
    pthread_mutex_lock(&_messageQueueMutex);
    if (_messageQueue->empty())
    {
        pthread_mutex_unlock(&_messageQueueMutex);
        return;
    }

    msg = _messageQueue->front();
    _messageQueue->pop_front();
    pthread_mutex_unlock(&_messageQueueMutex);
    
    switch (msg->what)
    {
        case DownloadRequest_DOWNLOAD_FINISH:
        {
            this->handleUpdateSucceed(msg);
            DownloadRequest* request = static_cast<DownloadRequest*>(msg->obj);
            _manager->onSuccess(request);
        }
		break;

        case DownloadRequest_PROGRESS:
        {
            ProgressMessage* message = static_cast<ProgressMessage*>(msg->obj);
            _manager->onProgress(message->request, message->percent, message->nowDownloaded, message->totalToDownload);
            delete (ProgressMessage*)msg->obj;
        }
		break;

        case DownloadRequest_ERROR:
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

void DownloadRequest::Helper::handleUpdateSucceed(Message *msg)
{
    DownloadRequest* manager = (DownloadRequest*)msg->obj;

    manager->setSearchPath();

//    string zipfileName = manager->_fileName;
//    if (remove(zipfileName.c_str()) != 0)
//    {
//        CCLOG("can not remove downloaded zip file %s", zipfileName.c_str());
//    }
}

NS_CC_EXT_END;
#endif // CC_PLATFORM_WINRT
