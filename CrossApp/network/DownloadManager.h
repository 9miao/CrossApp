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
#include <pthread.h>
#include <stdio.h>
#include <string>
#include <list>
#include <set>
#include <vector>
#include <chrono>

#if (CC_TARGET_PLATFORM != CC_PLATORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#endif


NS_CC_BEGIN

class CADownloadManagerDelegate;
class CADownloadResponse;
class DownloadResponseHelper;
class CC_DLL CADownloadManager
{
	friend class DownloadResponseHelper;
    typedef struct _DownloadRecord
    {
        ssize_t   download_id;
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

    ssize_t enqueueDownload(const std::string& downloadUrl,
                                  const std::string& fileName,
                                  const std::string& headers = "",
                                  const std::string& textTag = "");

	ssize_t enqueueDownloadEx(const std::string& downloadUrl,
                                    const std::string& fileName,
                                    const std::string& headers = "",
                                    const std::string& textTag = "");

    void resumeDownload(ssize_t download_id);
    
	void pauseDownload(ssize_t download_id);

	void eraseDownload(ssize_t download_id);
    
    const char* getDownloadUrl(ssize_t download_id);

	const char* getDownloadHeader(ssize_t download_id);
    
    const char* getFilePath(ssize_t download_id);
    
    ssize_t getTotalFileSize(ssize_t download_id);
    
    ssize_t getLocalFileSize(ssize_t download_id);
    
    const char* getStartTime(ssize_t download_id);
    
    bool isFinished(ssize_t download_id);
    
    bool isDownloading(ssize_t download_id);

    void clearOnSuccessDownloadAllRecord();
    
    void clearOnSuccessDownloadRecord(ssize_t download_id);
    
    std::vector<ssize_t> getDownloadIdsFromTextTag(const std::string& textTag);
    
    std::vector<std::string> getDownloadAllTextTags();
    
    CC_SYNTHESIZE(CADownloadManagerDelegate*, m_pDelegate, DownloadManagerDelegate);
    
    CC_SYNTHESIZE(int, m_nDownloadMaxCount, DownloadMaxCount);
    
protected:
    
    CADownloadManager();
    
    virtual ~CADownloadManager();

	void checkSqliteDB();

    void loadDownloadTasks();

	void deleteTaskFromDb(ssize_t download_id);

	void deleteTaskFromDb(const std::string& cszUrl);

	void setTaskFinished(ssize_t download_id);
    
    std::vector<ssize_t> selectIdFromTextTag(const std::string& textTag);
    
	double getDownloadFileSize(const std::string& downloadUrl, const std::string& downloadHeader);

	ssize_t insertDownload(const std::string& downloadUrl, const std::string& downloadHeader, const std::string& fileName, const std::string& textTag);

	void enqueueDownload(CADownloadResponse* request);

    void onError(CADownloadResponse* request, CADownloadManager::ErrorCode errorCode);
    
    void onProgress(CADownloadResponse* request, int percent, ssize_t nowDownloaded, ssize_t totalToDownloaded);
    
    void onSuccess(CADownloadResponse* request);

    friend class CADownloadResponse;
    
private:
    
	void* m_mpSqliteDB;
    
    CAMap<ssize_t, CADownloadResponse*> m_mCADownloadResponses;
    
    std::map<ssize_t, DownloadRecord> m_mDownloadRecords;
    
    CAVector<CADownloadResponse*> m_vDownloadingRequests;
    
    CADeque<CADownloadResponse*> m_dWaitCADownloadResponses;
    
    CAVector<CADownloadResponse*> m_vPauseCADownloadResponses;
    
};


class CC_DLL CADownloadResponse : public CAObject
{
private:
	friend class DownloadResponseHelper;
	friend class CADownloadManager;

	static CADownloadResponse* create(const std::string& downloadUrl,
		const std::string& fileName,
		ssize_t downloadId,
		const std::string& headers = "");

	CADownloadResponse(const std::string& downloadUrl, const std::string& fileName, ssize_t downloadId, const std::string& downHeaders = "");

	virtual ~CADownloadResponse();

	bool startDownload();

	bool isDownloaded();

	void setDownloadCmd(int cmd);

	ssize_t getDownloadID() const;

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

	std::string _fileName;

	std::string _downloadUrl;
	std::string _downHeaders;

	DownloadResponseHelper *_schedule;

	pthread_t *_tid;

	int _downloadCmd;

	int _downloadStatus;

	unsigned int _connectionTimeout;

	ssize_t _download_id;

	float m_fDelay;

	std::chrono::high_resolution_clock::time_point m_lastUpdate;
};

class CC_DLL CADownloadManagerDelegate
{
public:
    
    virtual void onError(ssize_t download_id, CADownloadManager::ErrorCode errorCode){};
    
    virtual void onProgress(ssize_t download_id, int percent, ssize_t nowDownloaded, ssize_t totalToDownloaded){};
    
    virtual void onPauseDownload(ssize_t download_id){};
    
    virtual void onResumeDownload(ssize_t download_id){};
    
    virtual void onSuccess(ssize_t download_id){};
};


NS_CC_END;

#endif /* defined(__CADownloadManager__) */
