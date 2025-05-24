/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "../include/threadpool.h"
#include "../include/threadpoolexceptions.h"
#include "../include/taskbase.h"
#include "workerthread.h"

namespace threadpooluniverse
{
    ThreadPool::ThreadPool( size_t numOfThreads, const std::optional<size_t> maxQueueSize ) :
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
        if( mMaxQueueSize.has_value() && mTasks.size() >= mMaxQueueSize.value() )
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

        // Wait until all worker threads are running.
        while( allThreadsRunning() == false )
        {
            std::this_thread::sleep_for( std::chrono::milliseconds( 0 ) );
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
        // Wait for new tasks to be added to the queue or for a timeout. The timeout is used to
        // avoid apparent glitches the condition variable seems to have. When notify_all() is called, it
        // seems that always not every thread wakes although they are waiting on the same condition variable.
        std::unique_lock<std::mutex> lock( mTasksMutex );
        mTasksCV.wait_for( lock, std::chrono::seconds( 1 ) );
    }

    void ThreadPool::registerRunningWorkerThread()
    {
        std::lock_guard<std::mutex> lock( mWorkersMutex );
        ++mNumberOfRunningWorkerThreads;
    }

}  // namespace threadpooluniverse
