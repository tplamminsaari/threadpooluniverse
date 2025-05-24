/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#ifndef THREADPOOLUNIVERSE_THREADPOOL_H
#define THREADPOOLUNIVERSE_THREADPOOL_H

#include <atomic>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

namespace threadpooluniverse
{
    class TaskBase;
    class WorkerThread;

    /**
     * @brief ThreadPool queues tasks and excutes them in worker threads.
     */
    class ThreadPool
    {
    public:
        /**
         * @brief Creates a thread pool with given number of threads and maximum queue size.
         * @param numOfThreads Number of worker threads.
         * @param maxQueueSize Maximum number of tasks in queue. Pass std::nullopt for unlimited
         * queue size.
         */
        ThreadPool( size_t numOfThreads, const std::optional<size_t> maxQueueSize );

        ~ThreadPool();

        ThreadPool( const ThreadPool& ) = delete;
        ThreadPool& operator=( const ThreadPool& ) = delete;
        ThreadPool( ThreadPool&& ) = delete;
        ThreadPool& operator=( ThreadPool&& ) = delete;

    public:
        /**
         * @brief Generates an unique task ID. The ID is guaranteed to be unique within a thread pool instance.
         * @return Unique task ID.
         */
        uint64_t generateId();

        /**
         * @brief Starts processing tasks in worker threads.
         */
        void startProcessing();

        /**
         * @brief Stops processing tasks in worker threads.
         *
         * The tasks tash are currently being processed will be completed but no
         * new task is taken under execution.
         */
        void stopProcessing();

        /**
         * @brief Appends new task to processing queue.
         *
         * If threadpool has been started, the task will go under execution as soon as next available
         * worker can pick it.
         * @param task The task to add. Takes the ownership of the task instance.
         * @throws TaskQueueFullException if the task queue is full and cannot accept more tasks.
         */
        void pushToQueue( std::unique_ptr<TaskBase> task );

        /**
         * @brief Empties the task queue. Tasks currently in-processing will continue processing.
         */
        void clearQueue();

        /**
         * @brief Cancels the task with given ID if it is still in queue.
         * @return True if task was canceled, false if it was already in processing or processed.
         */
        bool cancelTask( uint64_t taskId );

        /**
         * @brief Gets the number of tasks are in queue or under execution.
         * @return Number of tasks in queue or under execution.
         */
        size_t getNumberOfTasks();

        /**
         * @brief Returns the number of worker threads this threadpool has.
         */
        size_t getNumberOfThreads();

        /**
         * @brief Returns the number of idle worker threads.
         * @return Number of idle worker threads.
         */
        size_t getNumberOfIdleThreads();

        /**
         * @brief Returns true if all worker threads are running.
         * @return True if all worker threads are running.
         */
        bool allThreadsRunning();

        /**
         * @brief Waits until all tasks has been executed.
         */
        void waitAllTasks();

    private:
        /**
         * @brief Starts the worker threads. The threads will not start processing tasks yet.
         */
        void startWorkers();

        /**
         * @brief Shuts down the worker threads. Blocks until all threads are exited.
         */
        void shutdownWorkers();

        /**
         * Gets the next task from queue for processing. Returns an empty pointer if there
         * are no tasks in queue.
         *
         * @param The task for processing. Empty pointer if no tasks available.
         */
        std::unique_ptr<TaskBase> getTaskForProcessing();

        /**
         * Called by the worker thread when it has completed a task.
         */
        void taskCompleted();

        /**
         * To be called only from worker threads. Blocks until new tasks get added to the queue.
         * Can return even if no tasks are added due to spurious wakeups.
         */
        void waitForNotify();

        /**
         * To be called only from worker threads when thread has been started and is ready
         * to start processing tasks.
         */
        void registerRunningWorkerThread();

    private:
        std::optional<size_t> mMaxQueueSize;
        size_t mNumberOfThreads{ 5 };
        size_t mNumberOfRunningWorkerThreads{ 0 };
        std::list<std::unique_ptr<TaskBase>> mTasks;
        std::vector<std::unique_ptr<WorkerThread>> mWorkers;
        std::mutex mWorkersMutex;
        std::mutex mTasksMutex;
        std::condition_variable mTasksCV;
        std::atomic_bool mStarted;
        std::atomic_size_t mNumberOfTasksInExecution;

        uint64_t mTaskIdCounter{ 0 };
        std::mutex mTaskIdMutex;

        friend class WorkerThread;
    };
}

#endif  // THREADPOOLUNIVERSE_THREADPOOL_H
