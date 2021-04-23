
#pragma once

#include <functional>
#include <string>
#include <memory>
#include <vector>

#include "platform/CCPlatformMacros.h"

NS_CC_BEGIN
    class IDownloadTask;
    class IDownloaderImpl;
    class Downloader;

    class CC_DLL DownloadTask final
    {
    public:
        const static int ERROR_NO_ERROR = 0;
        const static int ERROR_INVALID_PARAMS = -1;
        const static int ERROR_FILE_OP_FAILED = -2;
        const static int ERROR_IMPL_INTERNAL = -3;

        std::string identifier;
        std::string requestURL;
        std::string storagePath;

        DownloadTask();
        virtual ~DownloadTask();

    private:
        friend class Downloader;
        std::unique_ptr<IDownloadTask> _coTask;
    };

    class CC_DLL DownloaderHints
    {
    public:
        uint32_t countOfMaxProcessingTasks;
        uint32_t timeoutInSeconds;
        std::string tempFileNameSuffix;
    };

    class CC_DLL Downloader final
    {
    public:
        Downloader();
        Downloader(const DownloaderHints& hints);
        ~Downloader();

        std::function<void(const DownloadTask& task,
                           std::vector<unsigned char>& data)> onDataTaskSuccess;

        std::function<void(const DownloadTask& task)> onFileTaskSuccess;

        std::function<void(const DownloadTask& task,
                           int64_t bytesReceived,
                           int64_t totalBytesReceived,
                           int64_t totalBytesExpected)> onTaskProgress;

        std::function<void(const DownloadTask& task,
                           int errorCode,
                           int errorCodeInternal,
                           const std::string& errorStr)> onTaskError;
        
        void setOnFileTaskSuccess(const std::function<void(const DownloadTask& task)>& callback) {onFileTaskSuccess = callback;};
        
        void setOnTaskProgress(const std::function<void(const DownloadTask& task,
                                                  int64_t bytesReceived,
                                                  int64_t totalBytesReceived,
                                                  int64_t totalBytesExpected)>& callback) {onTaskProgress = callback;};
        
        void setOnTaskError(const std::function<void(const DownloadTask& task,
                                               int errorCode,
                                               int errorCodeInternal,
                                               const std::string& errorStr)>& callback) {onTaskError = callback;};

        std::shared_ptr<const DownloadTask> createDownloadDataTask(const std::string& srcUrl, const std::string& identifier = "");

        std::shared_ptr<const DownloadTask> createDownloadFileTask(const std::string& srcUrl, const std::string& storagePath, const std::string& identifier = "");

    private:
        std::unique_ptr<IDownloaderImpl> _impl;
    };

NS_CC_END;
