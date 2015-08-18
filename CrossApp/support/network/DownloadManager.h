//
//  DownloadManager.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-9-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CADownloadManager__
#define __CADownloadManager__

#include "basics/CAObject.h"
#include "basics/CASTLContainer.h"
#include "platform/platform.h"
#include <curl/curl.h>
#include <curl/easy.h>
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>

#if (CC_TARGET_PLATFORM != CC_PLATORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif


NS_CC_BEGIN

class CADownloadManagerDelegate;
class CADownloadResponse;

class CC_DLL CADownloadManager
{

    typedef struct _DownloadRecord
    {
        unsigned long   download_id;
        std::string     download_Url;
		std::string		download_header;
        std::string     filePath;
        double			fileSize;
        std::string     startTime;
        bool            isFinished;
        std::string     textTag;
    } DownloadRecord;
    
public:
    
    enum ErrorCode
    {
        kCreateFile,
        kNetwork,
        kNoNewVersion,
        //kUncompress
    };
    
    static CADownloadManager* getInstance();
    
    static void destroyInstance();

    unsigned long enqueueDownload(const std::string& downloadUrl,
                                  const std::string& fileName,
                                  const std::string& headers = "",
                                  const std::string& textTag = "");

	unsigned long enqueueDownloadEx(const std::string& downloadUrl,
                                    const std::string& fileName,
                                    const std::string& headers = "",
                                    const std::string& textTag = "");

    void resumeDownload(unsigned long download_id);
    
	void pauseDownload(unsigned long download_id);

	void eraseDownload(unsigned long download_id);
    
    const char* getDownloadUrl(unsigned long download_id);

	const char* getDownloadHeader(unsigned long download_id);
    
    const char* getFilePath(unsigned long download_id);
    
    unsigned long getTotalFileSize(unsigned long download_id);
    
    unsigned long getLocalFileSize(unsigned long download_id);
    
    const char* getStartTime(unsigned long download_id);
    
    bool isFinished(unsigned long download_id);
    
    bool isDownloading(unsigned long download_id);

    void clearOnSuccessDownloadAllRecord();
    
    void clearOnSuccessDownloadRecord(unsigned long download_id);
    
    std::vector<unsigned long> getDownloadIdsFromTextTag(const std::string& textTag);
    
    std::vector<std::string> getDownloadAllTextTags();
    
    CC_SYNTHESIZE(CADownloadManagerDelegate*, m_pDelegate, DownloadManagerDelegate);
    
    CC_SYNTHESIZE(int, m_nDownloadMaxCount, DownloadMaxCount);
    
protected:
    
    CADownloadManager();
    
    virtual ~CADownloadManager();

	void checkSqliteDB();

    void loadDownloadTasks();

	void deleteTaskFromDb(unsigned long download_id);

	void deleteTaskFromDb(const std::string& cszUrl);

	void setTaskFinished(unsigned long download_id);
    
    std::vector<unsigned long> selectIdFromTextTag(const std::string& textTag);
    
	double getDownloadFileSize(const std::string& downloadUrl, const std::string& downloadHeader);

	unsigned long insertDownload(const std::string& downloadUrl, const std::string& downloadHeader, const std::string& fileName, const std::string& textTag);

	void enqueueDownload(CADownloadResponse* request);

    void onError(CADownloadResponse* request, CADownloadManager::ErrorCode errorCode);
    
    void onProgress(CADownloadResponse* request, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded);
    
    void onSuccess(CADownloadResponse* request);

    friend class CADownloadResponse;
    
private:
    
	void* m_mpSqliteDB;
    
    CAMap<unsigned long, CADownloadResponse*> m_mCADownloadResponses;
    
    std::map<unsigned long, DownloadRecord> m_mDownloadRecords;
    
    CAVector<CADownloadResponse*> m_vDownloadingRequests;
    
    CADeque<CADownloadResponse*> m_dWaitCADownloadResponses;
    
    CAVector<CADownloadResponse*> m_vPauseCADownloadResponses;
    
};

class CC_DLL CADownloadResponse : public CAObject
{
private:

	friend class CADownloadManager;

	static CADownloadResponse* create(const std::string& downloadUrl,
		const std::string& fileName,
		unsigned long downloadId,
		const std::string& headers = "");

	CADownloadResponse(const std::string& downloadUrl, const std::string& fileName, unsigned long downloadId, const std::string& downHeaders = "");

	virtual ~CADownloadResponse();

	bool startDownload();

	bool isDownloaded();

	void setDownloadCmd(int cmd);

	unsigned long getDownloadID() const;

	const std::string& getDownloadUrl() const;

	const std::string& getFileName() const;

	unsigned int getConnectionTimeout();

	friend void* CADownloadResponseDownloadAndUncompress(void*);

	friend int CADownloadResponseProgressFunc(void *, double, double, double, double);

	bool isDownloadAbort();

	bool checkDownloadStatus();

	CC_SYNTHESIZE_READONLY(double, _initialFileSize, InitialFileSize);

	CC_SYNTHESIZE(double, _localFileSize, LocalFileSize);

	CC_SYNTHESIZE(double, _totalFileSize, TotalFileSize);

private:

	bool downLoad();

	void checkStoragePath();

	bool uncompress();

	bool createDirectory(const char *path);

	void setSearchPath();

	void sendErrorMessage(CADownloadManager::ErrorCode code);

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
	std::string _downHeaders;

	CURL *_curl;

	curl_slist *m_headers;

	Helper *_schedule;

	pthread_t *_tid;

	int _downloadCmd;

	int _downloadStatus;

	unsigned int _connectionTimeout;

	unsigned long _download_id;

	float m_fDelay;

	struct cc_timeval m_pLastUpdate;
};

class CC_DLL CADownloadManagerDelegate
{
public:
    
    virtual void onError(unsigned long download_id, CADownloadManager::ErrorCode errorCode){};
    
    virtual void onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded){};
    
    virtual void onPauseDownload(unsigned long download_id){};
    
    virtual void onResumeDownload(unsigned long download_id){};
    
    virtual void onSuccess(unsigned long download_id){};
};


NS_CC_END;

#endif /* defined(__CADownloadManager__) */
