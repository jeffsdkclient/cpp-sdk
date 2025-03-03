//
// Created by wwango on 2022/11/10.
//

#ifndef UNTITLED1_TA_EVENT_TASK_H
#define UNTITLED1_TA_EVENT_TASK_H

#include <iostream>
#include <functional>
#include <thread>
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "ta_sqlite.h"
#include "ta_cpp_send.h"


namespace thinkingdata {

    class TAHttpSend;
    class TATaskQueue;

    using namespace std;

    class TAITask {
    public:
        virtual ~TAITask() {
            
        }
        virtual void DoTask() = 0;
        virtual void Stop() = 0;
        bool isStop = false;
    };

    class TASqiteInsetTask : public TAITask {
    public:
        ~TASqiteInsetTask();
        void DoTask();
        void Stop();

        TASqiteInsetTask(TAHttpSend* m_httpSend, TASqliteDataQueue *sqliteQueue, string event, string appid);
    private:
        string m_event;
        string m_appid;
        TASqliteDataQueue *m_sqliteQueue;
        TAHttpSend *m_httpSend;

    };

    class TANetworkTask : public TAITask {
    public:
        ~TANetworkTask();
        void DoTask();
        void Stop();
        TANetworkTask(TASqliteDataQueue *sqliteQueue, TAHttpSend *httpSend, string appid);
    private:
        TASqliteDataQueue *m_sqliteQueue;
        TAHttpSend *m_httpSend;
        string m_appid;
    };

    class TAFlushTask : public TAITask {
    public:
        ~TAFlushTask();
        void DoTask();
        void Stop();
        TAFlushTask(TASqliteDataQueue *sqliteQueue, TAHttpSend *httpSend, string appid);
    private:
        TASqliteDataQueue *m_sqliteQueue;
        TAHttpSend *m_httpSend;
        string m_appid;
        string m_event;
    };

    class TADebugTask : public TAITask {
    public:
        ~TADebugTask();
        void DoTask();
        void Stop();
        TADebugTask(TAHttpSend *httpSend, string appid,string deviceId,string m_event,bool isDebugOnly);
    private:
        TAHttpSend *m_httpSend;
        string m_appid;
        string m_deviceId;
        string m_event;
        bool isDebugOnly = false;
    };


    class TATaskQueue {
    public:
        static TATaskQueue *m_ta_dataTaskQue;
        static TATaskQueue *m_ta_networkTaskQue;

        bool isStop = false;
        TATaskQueue();
        ~TATaskQueue();

        bool Start();
        void PushTask(const std::shared_ptr<TAITask> &task);
        void ThreadCallBack();
        void stopWait();

    private:
        mutex m_lock;
        thread *m_pThread;
        condition_variable cv;
        mutex mtx;
        queue<shared_ptr<TAITask>> m_taskQue;
    };

};
#endif //UNTITLED1_TA_EVENT_TASK_H
