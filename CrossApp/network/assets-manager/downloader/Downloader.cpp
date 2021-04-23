
#include "Downloader.h"
#include "Downloader-curl.h"
#include "platform/CAFileUtils.h"

#define DownloaderImpl  DownloaderCURL

NS_CC_BEGIN

DownloadTask::DownloadTask()
{
    DLLOG("Construct DownloadTask %p", this);
}

DownloadTask::~DownloadTask()
{
    DLLOG("Destruct DownloadTask %p", this);
}

////////////////////////////////////////////////////////////////////////////////
//  Implement Downloader
Downloader::Downloader() : Downloader(DownloaderHints{6, 45, ".tmp"}) { }

Downloader::Downloader(const DownloaderHints& hints)
{
    DLLOG("Construct Downloader %p", this);
     _impl.reset(new DownloaderImpl(hints));
    _impl->onTaskProgress = [this](const DownloadTask& task,
                                   int64_t bytesReceived,
                                   int64_t totalBytesReceived,
                                   int64_t totalBytesExpected,
                                   std::function<int64_t(void *buffer, int64_t len)>& /*transferDataToBuffer*/)
    {
        if (onTaskProgress)
        {
            onTaskProgress(task, bytesReceived, totalBytesReceived, totalBytesExpected);
        }
    };

    _impl->onTaskFinish = [this](const DownloadTask& task,
                                 int errorCode,
                                 int errorCodeInternal,
                                 const std::string& errorStr,
                                 std::vector<unsigned char>& data)
    {
        if (DownloadTask::ERROR_NO_ERROR != errorCode)
        {
            if (onTaskError)
            {
                onTaskError(task, errorCode, errorCodeInternal, errorStr);
            }
            return;
        }

        // success callback
        if (task.storagePath.length())
        {
            if (onFileTaskSuccess)
            {
                onFileTaskSuccess(task);
            }
        }
        else
        {
            // data task
            if (onDataTaskSuccess)
            {
                onDataTaskSuccess(task, data);
            }
        }
    };
}

Downloader::~Downloader()
{
    DLLOG("Destruct Downloader %p", this);
}

std::shared_ptr<const DownloadTask> Downloader::createDownloadDataTask(const std::string& srcUrl, const std::string& identifier/* = ""*/)
{
    DownloadTask *task_ = new (std::nothrow) DownloadTask();
    std::shared_ptr<const DownloadTask> task(task_);
    do
    {
        task_->requestURL    = srcUrl;
        task_->identifier    = identifier;
        if (0 == srcUrl.length())
        {
            if (onTaskError)
            {
                onTaskError(*task, DownloadTask::ERROR_INVALID_PARAMS, 0, "URL or is empty.");
            }
            task.reset();
            break;
        }
        task_->_coTask.reset(_impl->createCoTask(task));
    } while (0);

    return task;
}

std::shared_ptr<const DownloadTask> Downloader::createDownloadFileTask(const std::string& srcUrl,
                                                                       const std::string& storagePath,
                                                                       const std::string& identifier/* = ""*/)
{
    DownloadTask *task_ = new (std::nothrow) DownloadTask();
    std::shared_ptr<const DownloadTask> task(task_);
    do
    {
        task_->requestURL    = srcUrl;
        task_->storagePath   = storagePath;
        task_->identifier    = identifier;
        if (0 == srcUrl.length() || 0 == storagePath.length())
        {
            if (onTaskError)
            {
                onTaskError(*task, DownloadTask::ERROR_INVALID_PARAMS, 0, "URL or storage path is empty.");
            }
            task.reset();
            break;
        }
        task_->_coTask.reset(_impl->createCoTask(task));
    } while (0);

    return task;
}

NS_CC_END;
