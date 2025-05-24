/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include <chrono>
#include "gtest/gtest.h"

#include "callbacktask.h"
#include "threadpool.h"
#include "threadpoolexceptions.h"
#include "util/dummytask.h"
using threadpooluniverse::DummyTask;

TEST( ThreadPoolTest, CreateDestroy )
{
    threadpooluniverse::ThreadPool threadPool( 4, std::nullopt );
    EXPECT_EQ( threadPool.getNumberOfThreads(), 4 );
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
    EXPECT_EQ( threadPool.getNumberOfIdleThreads(), 4 );
}

TEST( ThreadPoolTest, PushToQueueThrowsWhenFull )
{
    threadpooluniverse::ThreadPool threadPool( 4, 2 );
    ASSERT_NO_THROW(
        threadPool.pushToQueue( std::make_unique<DummyTask>( threadPool.generateId() ) ) );
    ASSERT_NO_THROW(
        threadPool.pushToQueue( std::make_unique<DummyTask>( threadPool.generateId() ) ) );
    EXPECT_THROW(
        threadPool.pushToQueue( std::make_unique<DummyTask>( threadPool.generateId() ) ),
        threadpooluniverse::TaskQueueFullException );
}

TEST( ThreadPoolTest, ProcessTasks )
{
    threadpooluniverse::ThreadPool threadPool( 4, std::nullopt );
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

    // Start processing tasks and give some time to finish.
    threadPool.startProcessing();
    auto startTime = std::chrono::steady_clock::now();
    do
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    } while( tasksExecuted.load() < 50 &&
             std::chrono::steady_clock::now() - startTime < std::chrono::seconds( 10 ) );
    EXPECT_EQ( tasksExecuted.load(), 50 );
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
}

TEST( ThreadPoolTest, CancelTasks )
{
    threadpooluniverse::ThreadPool threadPool( 4, std::nullopt );
    std::atomic_int tasksExecuted{ 0 };
    for( int i = 0; i < 150; ++i )
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

    // Ensure that all threads start processing tasks.
    do
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
    } while( threadPool.getNumberOfIdleThreads() > 0 );
    EXPECT_EQ( threadPool.getNumberOfIdleThreads(), 0 );

    // Cancel the tasks from the queue.
    for( uint64_t fakeId = 0; fakeId < 150; ++fakeId )
    {
        threadPool.cancelTask( fakeId );
    }
    EXPECT_TRUE( threadPool.getNumberOfTasks() > 0 );
    EXPECT_TRUE( tasksExecuted.load() < 150 );
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

TEST( ThreadPoolTest, TaskCanHandleErrors )
{
    threadpooluniverse::ThreadPool threadPool( 4, 50 );
    std::atomic_int errorsHandled{ 0 };
    for( int i = 0; i < 50; ++i )
    {
        auto task = std::make_unique<threadpooluniverse::CallbackTask>(
            threadPool.generateId(), []() { throw std::runtime_error( "Error for the test" ); },
            [&errorsHandled]() { errorsHandled.fetch_add( 1 ); } );
        threadPool.pushToQueue( std::move( task ) );
    }
    threadPool.startProcessing();
    threadPool.waitAllTasks();
    EXPECT_EQ( threadPool.getNumberOfTasks(), 0 );
    EXPECT_EQ( errorsHandled.load(), 50 );
}
