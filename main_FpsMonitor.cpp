/*
 * Copyright (C) 2024 The Android Open Source Project
 */

#define LOG_TAG "FpsMonitor"

#include <android-base/logging.h>
#include <android/gui/BnFpsListener.h>
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <ui/DynamicDisplayInfo.h>
#include <utils/Log.h>

#include <chrono>
#include <sstream>
#include <iomanip>
#include <ctime>

#include "RefreshRateOverlay.h"

using namespace android;

int mTaskId = 0;
int mMode = 0;

std::string getTimeAsString() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
 
    std::ostringstream oss;
    oss << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S");

    auto duration = now.time_since_epoch();
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration) % 1000;
    oss << '.' << std::setfill('0') << std::setw(3) << millis.count();
 
    return oss.str();
}

struct TaskFpsCallback : public gui::BnFpsListener {
    binder::Status onFpsReported(float fps) override {
        if(mMode == 0 || mMode == 1)
            fprintf(stderr, "%s   FPS=\033[31m%f\033[m\n",getTimeAsString().c_str(), fps);
        if(mMode == 0 || mMode == 2)
            mRefreshRateOverlay->changeRefreshRate(mDisplayInfo.renderFrameRate, fps);
        return binder::Status::ok();
    }
    
    TaskFpsCallback() {
        const auto ids = SurfaceComposerClient::getPhysicalDisplayIds();
        CHECK(!ids.empty());
        uint64_t displayId = ids.front().value;
        
        status_t err = SurfaceComposerClient::getDynamicDisplayInfoFromId(displayId, &mDisplayInfo);
        if (err != OK)
            mDisplayInfo.getActiveDisplayMode()->resolution.set(1920, 1080);

        ui::Size resolution = mDisplayInfo.getActiveDisplayMode()->resolution;
        ftl::Flags<RefreshRateOverlay::Features> features;
        //features |= RefreshRateOverlay::Features::ShowInMiddle;
        features |= RefreshRateOverlay::Features::RenderRate;
        //features |= RefreshRateOverlay::Features::Spinner;
        mRefreshRateOverlay = std::make_unique<RefreshRateOverlay>(features);
        mRefreshRateOverlay->setLayerStack(ui::LayerStack::fromValue(0));
        mRefreshRateOverlay->setViewport(resolution);
    }

    std::unique_ptr<RefreshRateOverlay> mRefreshRateOverlay;
    ui::DynamicDisplayInfo mDisplayInfo;
};

static void usage(const char *me)
{
    fprintf(stderr, "\nusage: \t%s [options]\n"
                    "\t--------------------------------------- options ------------------------------------------------\n"
                    "\t[-h] help\n"
                    "\t[-d] task id of app that you want to observe \n"
                    "\t[-m] 1: only print log, 2: only show ui, default: print log and show ui \n"
                    "\t------------------------------------------------------------------------------------------------\n",
                    me);
    exit(1);
}

void parseOptions(int argc, char **argv) {
    const char *me = argv[0];
    if(argc < 2) {
        usage(me);
        exit(0);
    }
    int res;
    while((res = getopt(argc, argv, "d:m:")) >= 0) {
        switch(res) {
            case 'd':
                mTaskId = atoi(optarg);
                break;
            case 'm':
                mMode = atoi(optarg);
                break;
            case 'h':
            default:
            {
                usage(me);
            }
        }
    }
}

int main(int argc, char ** argv) {
    parseOptions(argc, argv);

    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    sp<TaskFpsCallback> callback = new TaskFpsCallback();
    if (SurfaceComposerClient::addFpsListener(mTaskId, callback) != OK) {
        ALOGD("addFpsListener error!");
        return -1;
    }

    fprintf(stderr, "-- Monitor Task#%d's FPS -- \n", mTaskId);

    while(getchar() != 'q');

    return 0;
}