

#ifndef __AssetsManager__
#define __AssetsManager__

#include <string>
#include <mutex>
#include "basics/CAObject.h"

NS_CC_BEGIN
class Downloader;
class AssetsManagerDelegateProtocol;

class AssetsManager : public CAObject
{
public:
    enum class ErrorCode
    {
        CREATE_FILE,
        NETWORK,
        NO_NEW_VERSION,
        UNCOMPRESS,
    };

    AssetsManager(const char* packageUrl = NULL, const char* versionFileUrl = NULL, const char* storagePath = NULL);

    virtual ~AssetsManager();
    
    typedef std::function<void(int)> ErrorCallback;
    typedef std::function<void(int)> ProgressCallback;
    typedef std::function<void(void)> SuccessCallback;

    static AssetsManager* create(const char* packageUrl, const char* versionFileUrl, const char* storagePath, ErrorCallback errorCallback, ProgressCallback progressCallback, SuccessCallback successCallback );

    virtual bool checkUpdate();
    
    virtual void update();

    const char* getPackageUrl() const;

    void setPackageUrl(const char* packageUrl);

    const char* getVersionFileUrl() const;

    void setVersionFileUrl(const char* versionFileUrl);

    std::string getVersion();

    void deleteVersion();

    const char* getStoragePath() const;
    void setStoragePath(const char* storagePath);

    void setDelegate(AssetsManagerDelegateProtocol *delegate);

    AssetsManagerDelegateProtocol* getDelegate() const { return _delegate ;}

    void setConnectionTimeout(unsigned int timeout);

    unsigned int getConnectionTimeout();

    void setSearchPath();
    
protected:
    
    void checkStoragePath();
    bool uncompress();
    void downloadAndUncompress();

private:
    
    std::string _storagePath;
    std::string _version;
    
    std::string _packageUrl;
    std::string _versionFileUrl;
    
    std::string _downloadedVersion;
    
    Downloader* _downloader;

    unsigned int _connectionTimeout;
    
    AssetsManagerDelegateProtocol *_delegate; 
    
    bool _isDownloading;
    bool _shouldDeleteDelegateWhenExit;
    
    std::string keyOfVersion() const;
    std::string keyOfDownloadedVersion() const;
};

class AssetsManagerDelegateProtocol
{
public:
    virtual ~AssetsManagerDelegateProtocol(){};
public:

    virtual void onError(AssetsManager::ErrorCode errorCode) {};

    virtual void onProgress(int percent) {};

    virtual void onSuccess() {};
};

NS_CC_END;

#endif /* defined(__AssetsManager__) */
