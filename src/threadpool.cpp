
#include "../include/threadpool.h"
#include "../include/threadpoolexceptions.h"
#include "../include/taskbase.h"
#include "workerthread.h"

namespace threadpooluniverse
{
    ThreadPool::ThreadPool( size_t numOfThreads, size_t maxQueueSize ) :
        mMaxQueueSize( maxQueueSize ),
        mNumberOfThreads( numOfThreads ),
        mStarted( false ),
        mNumberOfTasksInExecution( 0 )
    {
        startWorkers();
    }

    ThreadPool::~ThreadPool()
    {
        stopProcessing();
        shutdownWorkers();
    }

    uint64_t ThreadPool::generateId()
    {
        std::lock_guard<std::mutex> lock( mTaskIdMutex );
        return ++mTaskIdCounter;
    }

    void ThreadPool::startProcessing()
    {
        mStarted.store( true );
        mTasksCV.notify_all();
    }

    void ThreadPool::stopProcessing()
    {
        mStarted.store( false );
    }

    void ThreadPool::pushToQueue( std::unique_ptr<TaskBase> task )
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        if( mMaxQueueSize != 0 && mTasks.size() >= mMaxQueueSize )
        {
            // Queue is full, we cannot add more tasks.
            throw TaskQueueFullException( "Task queue full." );
        }
        mTasks.push_back( std::move( task ) );
        mTasksCV.notify_one();
    }

    void ThreadPool::clearQueue()
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        mTasks.clear();
    }

    bool ThreadPool::cancelTask( uint64_t taskId )
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        for( auto it = mTasks.begin(); it != mTasks.end(); ++it )
        {
            if( ( *it )->getTaskId() == taskId )
            {
                ( *it )->cancel();
                mTasks.erase( it );
                return true;
            }
        }
        return false;
    }

    size_t ThreadPool::getNumberOfTasks()
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        size_t numTasksInQueue = mTasks.size();
        return numTasksInQueue + mNumberOfTasksInExecution.load();;
    }

    size_t ThreadPool::getNumberOfIdleThreads()
    {
        size_t numIdleThreads = 0;
        for( auto& worker : mWorkers )
        {
            if( worker->isIdle() )
            {
                ++numIdleThreads;
            }
        }
        return numIdleThreads;
    }

    bool ThreadPool::allThreadsRunning()
    {
        std::lock_guard<std::mutex> lock( mWorkersMutex );
        return mNumberOfRunningWorkerThreads == mNumberOfThreads;
    }

    size_t ThreadPool::getNumberOfThreads()
    {
        return mNumberOfThreads;
    }

    void ThreadPool::waitAllTasks()
    {
        while(getNumberOfTasks() > 0)
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
    }

    void ThreadPool::startWorkers()
    {
        for( size_t i = 0; i < mNumberOfThreads; ++i )
        {
            mWorkers.emplace_back( std::make_unique<WorkerThread>( *this ) );
        }
    }

    void ThreadPool::shutdownWorkers()
    {
        // Ask worker threads to exit.
        for( auto& worker : mWorkers )
        {
            worker->requestExit();
        }

        // Wake up all worker threads to process exit request.
        mTasksCV.notify_all();

        // Join all worker threads.
        for( auto& worker : mWorkers )
        {
            if( worker->accessThread().joinable() )
            {
                worker->accessThread().join();
            }
        }
        mWorkers.clear();
    }

    std::unique_ptr<TaskBase> ThreadPool::getTaskForProcessing()
    {
        // Return null task if task processing not started.
        if( !mStarted.load() )
        {
            return nullptr;
        }

        // Get the next task from the queue.
        std::lock_guard<std::mutex> lock( mTasksMutex );
        if( mTasks.empty() )
        {
            return nullptr;
        }
        std::unique_ptr<TaskBase> task = std::move( mTasks.front() );
        mTasks.pop_front();

        ++mNumberOfTasksInExecution;
        return std::move( task );
    }

    void ThreadPool::taskCompleted()
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        --mNumberOfTasksInExecution;
    }

    void ThreadPool::waitForNotify()
    {
        std::unique_lock<std::mutex> lock( mTasksMutex );
        mTasksCV.wait( lock );
    }

    void ThreadPool::registerRunningWorkerThread()
    {
        std::lock_guard<std::mutex> lock( mWorkersMutex );
        ++mNumberOfRunningWorkerThreads;
    }

}  // namespace threadpooluniverse
