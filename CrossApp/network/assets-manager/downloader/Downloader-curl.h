
#pragma once

#include "IDownloaderImpl.h"
#include "basics/CAScheduler.h"

NS_CC_BEGIN
class DownloadTaskCURL;
class DownloaderHints;

class DownloaderCURL : public IDownloaderImpl
{
public:
    DownloaderCURL(const DownloaderHints& hints);
    virtual ~DownloaderCURL();

    virtual IDownloadTask *createCoTask(std::shared_ptr<const DownloadTask>& task) override;

protected:
    class Impl;
    std::shared_ptr<Impl>   _impl;

    // for transfer data on schedule
    DownloadTaskCURL* _currTask;        // temp ref
    std::function<int64_t(void*, int64_t)> _transferDataToBuffer;

    // scheduler for update processing and finished task in main schedule
    void _onSchedule(float);
    std::string             _schedulerKey;
    CAScheduler*              _scheduler;
};

NS_CC_END;

