//
//  DownloadManager.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-9-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __DownloadManager__
#define __DownloadManager__

#include "CrossApp.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class DownloadManagerDelegate;
class DownloadRequest;

class DownloadManager
{

    typedef struct _DownloadRecord
    {
        unsigned long   download_id;
        std::string     download_Url;
        std::string     filePath;
        unsigned long   fileSize;
        std::string     startTime;
        bool            isFinished;
    } DownloadRecord;
    
public:
    
    enum ErrorCode
    {
        kCreateFile,
        kNetwork,
        kNoNewVersion,
        kUncompress,
    };
    
    static DownloadManager* getInstance();
    
    static void destroyInstance();

    unsigned long enqueueDownload(const std::string& downloadUrl, const std::string& fileName);

    void resumeDownload(unsigned long download_id);
    
	void pauseDownload(unsigned long download_id);

	void eraseDownload(unsigned long download_id);
    
    const char* getDownloadUrl(unsigned long download_id);
    
    const char* getFilePath(unsigned long download_id);
    
    unsigned long getFileSize(unsigned long download_id);
    
    const char* getStartTime(unsigned long download_id);
    
    bool isFinished(unsigned long download_id);
    
    void clearOnSuccessDownloadAllRecord();
    
    void clearOnSuccessDownloadRecord(unsigned long download_id);
    
    CC_SYNTHESIZE(DownloadManagerDelegate*, m_pDelegate, DownloadManagerDelegate);
    
    CC_SYNTHESIZE(int, m_nDownloadMaxCount, DownloadMaxCount);
    
protected:
    
    DownloadManager();
    
    virtual ~DownloadManager();

	void checkSqliteDB();

    void loadDownloadTasks();
    
	unsigned long getDownloadFileSize(const char *url);

	unsigned long insertDownload(const std::string& downloadUrl, const std::string& fileName);

	unsigned long enqueueDownload(DownloadRequest* request);

    void onError(DownloadRequest* request, DownloadManager::ErrorCode errorCode);
    
    void onProgress(DownloadRequest* request, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded);
    
    void onSuccess(DownloadRequest* request);

    friend class DownloadRequest;
    
private:
    
	void* m_mpSqliteDB;
    
    CAMap<unsigned long, DownloadRequest*> m_mDownloadRequests;
    
    std::map<unsigned long, DownloadRecord> m_mDownloadRecords;
    
    CAVector<DownloadRequest*> m_vDownloadingRequests;
    
    CADeque<DownloadRequest*> m_dWaitDownloadRequests;
    
    CAVector<DownloadRequest*> m_vPauseDownloadRequests;
    
};

class DownloadManagerDelegate
{
public:
    
    void onError(unsigned long requestID, DownloadManager::ErrorCode errorCode){};
    
    void onProgress(unsigned long requestID, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded){};
    
    void onSuccess(unsigned long requestID){};
};


NS_CC_EXT_END;

#endif /* defined(__DownloadManager__) */
