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
        std::string     textTag;
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

    unsigned long enqueueDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag = "");

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
    
    std::vector<unsigned long> getDownloadIdsFromTextTag(const std::string& textTag);
    
    CC_SYNTHESIZE(DownloadManagerDelegate*, m_pDelegate, DownloadManagerDelegate);
    
    CC_SYNTHESIZE(int, m_nDownloadMaxCount, DownloadMaxCount);
    
protected:
    
    DownloadManager();
    
    virtual ~DownloadManager();

	void checkSqliteDB();

    void loadDownloadTasks();

	void deleteTaskFromDb(unsigned long download_id);

	void setTaskFinished(unsigned long download_id);
    
    std::vector<unsigned long> selectIdFromTextTag(const std::string& textTag);
    
	unsigned long getDownloadFileSize(const char *url);

	unsigned long insertDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag);

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
    
    virtual void onError(unsigned long download_id, DownloadManager::ErrorCode errorCode){};
    
    virtual void onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded){};
    
    virtual void onPauseDownload(unsigned long download_id){};
    
    virtual void onResumeDownload(unsigned long download_id){};
    
    virtual void onSuccess(unsigned long download_id){};
};


NS_CC_EXT_END;

#endif /* defined(__DownloadManager__) */
