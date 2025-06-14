/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "workerthread.h"
#include "../include/taskbase.h"
#include "../include/threadpool.h"

namespace threadpooluniverse
{
    WorkerThread::WorkerThread( ThreadPool& owningThreadPool ) :
        mOwningThreadPool( owningThreadPool ),
        mIdle( true )
    {
        mRequestExit.store( false );
        mWorkerThread = std::thread( &WorkerThread::threadFunction, this );
    }

    WorkerThread::~WorkerThread()
    {
    }

    void WorkerThread::requestExit()
    {
        mRequestExit.store( true );
    }

    std::thread& WorkerThread::accessThread()
    {
        return mWorkerThread;
    }

    bool WorkerThread::isIdle() const
    {
        return mIdle.load();
    }

    void WorkerThread::threadFunction( WorkerThread* threadObject )
    {
        threadObject->threadMain();
    }

    void WorkerThread::threadMain()
    {
        mOwningThreadPool.registerRunningWorkerThread();

        // Main thread loop.
        while( !mRequestExit.load() )
        {
            auto task = mOwningThreadPool.getTaskForProcessing();
            if( task )
            {
                mIdle.store( false );
                try
                {
                    task->execute();
                }
                catch( const std::exception& )
                {
                    // Don't let exceptions propagate out of the thread because it would
                    // terminate thread. Call the error handling method of the task instead.
                    try
                    {
                        task->handleError();
                    }
                    catch( const std::exception& )
                    {
                        // Handle error threw an exception. Ignore it for now.
                    }
                }
                mOwningThreadPool.taskCompleted();
            }
            else
            {
                mIdle.store( true );
                mOwningThreadPool.waitForNotify();
            }
        }
    }

}  // namespace threadpooluniverse
