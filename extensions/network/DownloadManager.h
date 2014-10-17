//
//  DownloadManager.h
//  CrossApp
//
//  Created by Li Yuanfeng on 14-9-28.
//  Copyright (c) 2014 http://9miao.com All rights reserved.
//

#ifndef __CADownloadManager__
#define __CADownloadManager__

#include "CrossApp.h"
#include "ExtensionMacros.h"

NS_CC_EXT_BEGIN

class CADownloadManagerDelegate;
class CADownloadRequest;

class CADownloadManager
{

    typedef struct _DownloadRecord
    {
        unsigned long   download_id;
        std::string     download_Url;
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
        kUncompress,
    };
    
    static CADownloadManager* getInstance();
    
    static void destroyInstance();

    unsigned long enqueueDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag = "");

	unsigned long enqueueDownloadEx(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag = "");

    void resumeDownload(unsigned long download_id);
    
	void pauseDownload(unsigned long download_id);

	void eraseDownload(unsigned long download_id);
    
    const char* getDownloadUrl(unsigned long download_id);
    
    const char* getFilePath(unsigned long download_id);
    
    unsigned long getTotalFileSize(unsigned long download_id);
    
    unsigned long getLocalFileSize(unsigned long download_id);
    
    const char* getStartTime(unsigned long download_id);
    
    bool isFinished(unsigned long download_id);
    
    bool isDownloading(unsigned long download_id);

    void clearOnSuccessDownloadAllRecord();
    
    void clearOnSuccessDownloadRecord(unsigned long download_id);
    
    std::vector<unsigned long> getDownloadIdsFromTextTag(const std::string& textTag);
    
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
    
    double getDownloadFileSize(const std::string& downloadUrl);

	unsigned long insertDownload(const std::string& downloadUrl, const std::string& fileName, const std::string& textTag);

	void enqueueDownload(CADownloadRequest* request);

    void onError(CADownloadRequest* request, CADownloadManager::ErrorCode errorCode);
    
    void onProgress(CADownloadRequest* request, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded);
    
    void onSuccess(CADownloadRequest* request);

    friend class CADownloadRequest;
    
private:
    
	void* m_mpSqliteDB;
    
    CAMap<unsigned long, CADownloadRequest*> m_mCADownloadRequests;
    
    std::map<unsigned long, DownloadRecord> m_mDownloadRecords;
    
    CAVector<CADownloadRequest*> m_vDownloadingRequests;
    
    CADeque<CADownloadRequest*> m_dWaitCADownloadRequests;
    
    CAVector<CADownloadRequest*> m_vPauseCADownloadRequests;
    
};

class CADownloadManagerDelegate
{
public:
    
    virtual void onError(unsigned long download_id, CADownloadManager::ErrorCode errorCode){};
    
    virtual void onProgress(unsigned long download_id, int percent, unsigned long nowDownloaded, unsigned long totalToDownloaded){};
    
    virtual void onPauseDownload(unsigned long download_id){};
    
    virtual void onResumeDownload(unsigned long download_id){};
    
    virtual void onSuccess(unsigned long download_id){};
};


NS_CC_EXT_END;

#endif /* defined(__CADownloadManager__) */
