
#ifndef THREADPOOLUNIVERSE_WORKERTHREAD_H
#define THREADPOOLUNIVERSE_WORKERTHREAD_H

#include <atomic>
#include <thread>

namespace threadpooluniverse
{
    class WorkerThread
    {
    public:
        WorkerThread();
        ~WorkerThread();

        WorkerThread(const WorkerThread&) = delete;
        WorkerThread& operator=(const WorkerThread&) = delete;
        WorkerThread(WorkerThread&&) = delete;
        WorkerThread& operator=(WorkerThread&&) = delete;

    public:
        void requestExit();
        std::thread& accessThread();

    private:
        static void threadFunction( WorkerThread* threadObject );
        void threadMain();
        std::atomic_bool mRequestExit;
    };
}

#endif
