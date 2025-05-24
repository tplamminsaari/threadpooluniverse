/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#ifndef THREADPOOLUNIVERSE_TASKBASE_H
#define THREADPOOLUNIVERSE_TASKBASE_H

#include <atomic>
#include <cstdint>

namespace threadpooluniverse
{
    /**
     * @brief Base class of all the tasks executed by the thread pool.
     */
    class TaskBase
    {
    public:
        /**
         * @brief Constructs a task with a unique ID.
         * @param taskId Unique ID for the task.
         */
        explicit TaskBase( uint64_t taskId );
        virtual ~TaskBase();

        TaskBase( const TaskBase& other ) = delete;
        TaskBase& operator=( const TaskBase& other ) = delete;
        TaskBase( TaskBase&& other ) = delete;
        TaskBase& operator=( TaskBase&& other ) = delete;

        /**
         * @brief Gets the unique task ID.
         * @return Unique task ID.
         */
        uint64_t getTaskId() const;

        /**
         * @brief Cancels the task. After this, the task should not be executed.
         *
         * The 'execute()' method can check the canceled state by calling 'isCanceled()' and
         * stop execution if it returns true.
         */
        void cancel();

        /**
         * @brief Checks if the task has been canceled.
         * @return True if the task is canceled, false otherwise.
         */
        bool isCanceled() const;

        /**
         * @brief Does the actual work of the task.
         *
         * The worker thread will call this method when it has a task for processing. If task execution
         * takes a long time, you should check the canceled state periodically by calling 'isCanceled()'
         * and stop execution if it returns true.
         *
         * If this function throws an exception, the worker thread will call 'handleError()' method
         * to handle the error.
         */
        virtual void execute() = 0;

        /**
         * @brief Handles errors that occur during task execution.
         */
        virtual void handleError();

    protected:
        uint64_t mTaskId;
        std::atomic_bool mCanceled;
    };
}

#endif
