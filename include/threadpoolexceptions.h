/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#ifndef THREADPOOLUNIVERSE_THREADPOOLEXCEPTIONS_H
#define THREADPOOLUNIVERSE_THREADPOOLEXCEPTIONS_H

#include <stdexcept>

namespace threadpooluniverse
{
    /**
     * @brief ThreadPoolBaseException is the base for all thread pool related exceptions.
     */
    class ThreadPoolBaseException : public std::runtime_error
    {
    public:
        explicit ThreadPoolBaseException( const std::string& message );
        virtual ~ThreadPoolBaseException() noexcept = default;

        ThreadPoolBaseException( const ThreadPoolBaseException& ) = default;
        ThreadPoolBaseException& operator=( const ThreadPoolBaseException& ) = default;
        ThreadPoolBaseException( ThreadPoolBaseException&& ) = default;
        ThreadPoolBaseException& operator=( ThreadPoolBaseException&& ) = default;
    };

    /**
     * @brief Exception thrown when the task queue is full.
     */
    class TaskQueueFullException : ThreadPoolBaseException
    {
    public:
        explicit TaskQueueFullException( const std::string& message );
        virtual ~TaskQueueFullException() noexcept = default;
        TaskQueueFullException( const TaskQueueFullException& ) = default;
        TaskQueueFullException& operator=( const TaskQueueFullException& ) = default;
        TaskQueueFullException( TaskQueueFullException&& ) = default;
        TaskQueueFullException& operator=( TaskQueueFullException&& ) = default;
    };

    /**
     * @brief Exception thrown when a canceling a task that has already been canceled.
     */
    class AlreadyCanceledException : ThreadPoolBaseException
    {
    public:
        explicit AlreadyCanceledException( const std::string& message );
        virtual ~AlreadyCanceledException() noexcept = default;
        AlreadyCanceledException( const AlreadyCanceledException& ) = default;
        AlreadyCanceledException& operator=( const AlreadyCanceledException& ) = default;
        AlreadyCanceledException( AlreadyCanceledException&& ) = default;
        AlreadyCanceledException& operator=( AlreadyCanceledException&& ) = default;
    };

}  // namespace threadpooluniverse
#endif
