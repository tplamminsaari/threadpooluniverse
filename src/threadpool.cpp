
#include "../include/threadpool.h"
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
        mTasks.push_back( std::move( task ) );
        mTasksCV.notify_one();
    }

    size_t ThreadPool::getNumberOfTasks()
    {
        std::lock_guard<std::mutex> lock( mTasksMutex );
        size_t numTasksInQueue = mTasks.size();
        return numTasksInQueue + mNumberOfTasksInExecution.load();;
    }

    size_t ThreadPool::getNumberOfThreads()
    {
        return mNumberOfThreads;
    }

    void ThreadPool::waitAllTasks()
    {
        while (getNumberOfTasks() > 0)
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        }
    }

    void ThreadPool::startWorkers()
    {
        for ( size_t i = 0; i < mNumberOfThreads; ++i )
        {
            mWorkers.emplace_back( new WorkerThread( this ) );
        }
    }

    void ThreadPool::shutdownWorkers()
    {
        for ( auto& worker : mWorkers )
        {
            worker->cancel();
        }
    }

}
