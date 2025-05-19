#include <chrono>
#include "gtest/gtest.h"

#include "callbacktask.h"
#include "threadpool.h"
#include "threadpoolexceptions.h"

TEST( ThreadPoolTest, CreateDestroy )
{
    threadpooluniverse::ThreadPool threadPool( 4, 12 );
    auto startTime = std::chrono::high_resolution_clock::now();
    while( threadPool.allThreadsRunning() == false )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
        auto currentTime = std::chrono::high_resolution_clock::now();
        if( std::chrono::duration_cast<std::chrono::milliseconds>( currentTime - startTime ).count() > 1000 )
        {
            FAIL() << "Thread pool did not start within 1 second.";
            return;
        }
    }
    EXPECT_EQ( threadPool.getNumberOfThreads(), 4 );
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
    EXPECT_EQ( threadPool.getNumberOfIdleThreads(), 4 );
}

TEST( ThreadPoolTest, ProcessTasks )
{
    threadpooluniverse::ThreadPool threadPool( 4, 50 );
    std::atomic_int tasksExecuted{ 0 };
    for( int i = 0; i < 50; ++i )
    {
        auto task = std::make_unique<threadpooluniverse::CallbackTask>(
            threadPool.generateId(),
            [&tasksExecuted]() {
                tasksExecuted.fetch_add( 1 );
            } );
        ASSERT_NO_THROW( threadPool.pushToQueue( std::move( task ) ) );
    }
    ASSERT_THROW( threadPool.pushToQueue( std::make_unique<threadpooluniverse::CallbackTask>(
                      threadPool.generateId(), []() {} ) ),
                  threadpooluniverse::TaskQueueFullException );

    // Start processing tasks and give some time to finish.
    threadPool.startProcessing();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    EXPECT_EQ( tasksExecuted.load(), 50 );
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
}

TEST( ThreadPoolTest, CancelTasks )
{
    threadpooluniverse::ThreadPool threadPool( 4, 50 );
    std::atomic_int tasksExecuted{ 0 };
    for( int i = 0; i < 50; ++i )
    {
        auto task = std::make_unique<threadpooluniverse::CallbackTask>(
            threadPool.generateId(),
            [&tasksExecuted]() {
                std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
                tasksExecuted.fetch_add( 1 );
            } );
        ASSERT_NO_THROW( threadPool.pushToQueue( std::move( task ) ) );
    }
    threadPool.startProcessing();
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    // No all thread threads should be processing the tasks. Check that none of them is idle.
    EXPECT_EQ( threadPool.getNumberOfIdleThreads(), 0 );

    // Cancel the tasks from the queue.
    for( uint64_t fakeId = 0; fakeId < 50; ++fakeId )
    {
        threadPool.cancelTask( fakeId );
    }
    EXPECT_TRUE( threadPool.getNumberOfTasks() > 0 );
    EXPECT_TRUE( tasksExecuted.load() < 50 );
}

TEST( ThreadPoolTest, TaskCanThrowExceptions )
{
    threadpooluniverse::ThreadPool threadPool( 4, 50 );
    for( int i = 0; i < 50; ++i )
    {
        auto task = std::make_unique<threadpooluniverse::CallbackTask>(
            threadPool.generateId(), []() { throw std::runtime_error( "Error" ); } );
        ASSERT_NO_THROW( threadPool.pushToQueue( std::move( task ) ) );
    }
    threadPool.startProcessing();
    threadPool.waitAllTasks();
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
}

