
#ifndef THREADPOOLUNIVERSE_WORKERTHREAD_H
#define THREADPOOLUNIVERSE_WORKERTHREAD_H

#include <atomic>
#include <memory>
#include <thread>

namespace threadpooluniverse
{
    class ThreadPool;

    class WorkerThread
    {
    public:
        WorkerThread(ThreadPool& owningThreadPool);
        ~WorkerThread();

        WorkerThread(const WorkerThread&) = delete;
        WorkerThread& operator=(const WorkerThread&) = delete;
        WorkerThread(WorkerThread&&) = delete;
        WorkerThread& operator=(WorkerThread&&) = delete;

    public:
        void requestExit();
        std::thread& accessThread();
        bool isIdle() const;

    private:
        static void threadFunction( WorkerThread* threadObject );
        void threadMain();

    private:
        std::atomic_bool mRequestExit;
        std::atomic_bool mIdle;
        ThreadPool& mOwningThreadPool;
        std::thread mWorkerThread;
    };
}

#endif
