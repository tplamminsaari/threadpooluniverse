
#ifndef THREADPOOLUNIVER_THREADPOOL_H
#define THREADPOOLUNIVER_THREADPOOL_H

#include <atomic>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

namespace threadpooluniverse {

    class TaskBase;
    class WorkerThread;

    /**
     * @brief ThreadPool queues tasks and excutes them in worker threads.
     */
    class ThreadPool
    {
    public:
        ThreadPool( size_t numOfThreads, size_t maxQueueSize );
        ~ThreadPool();

        ThreadPool( const ThreadPool& ) = delete;
        ThreadPool& operator=( const ThreadPool& ) = delete;
        ThreadPool( ThreadPool&& ) = delete;
        ThreadPool& operator=( ThreadPool&& ) = delete;

    public:
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
         * If threadpool has been started, the task will go under execution as
         * soon as next available worker can pick it.
         * @param task The task to add.
         */
        void pushToQueue( std::unique_ptr<TaskBase> task );

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
         * @brief Waits until all tasks has been executed.
         */
        void waitAllTasks();

    private:
        void startWorkers();
        void shutdownWorkers();
        std::unique_ptr<TaskBase> getTaskForProcessing();
        void waitForNotify();

    private:
        size_t mMaxQueueSize {0};
        size_t mNumberOfThreads {5};
        std::vector<std::unique_ptr<TaskBase>> mTasks;
        std::vector<std::unique_ptr<WorkerThread>> mWorkers;
        std::mutex mTasksMutex;
        std::condition_variable mTasksCV;
        std::mutex mWorkersMutex;
        std::atomic_bool mStarted;
        std::atomic_size_t mNumberOfTasksInExecution;

        friend class WorkerThread;
    };
}

#endif
