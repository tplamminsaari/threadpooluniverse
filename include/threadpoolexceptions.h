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
     * @brief Exception thrown when the task queue is full.
     */
    class TaskQueueFullException : public std::runtime_error
    {
    public:
        explicit TaskQueueFullException( const std::string& message );
        virtual ~TaskQueueFullException() noexcept = default;
        TaskQueueFullException( const TaskQueueFullException& ) = default;
        TaskQueueFullException& operator=( const TaskQueueFullException& ) = default;
        TaskQueueFullException( TaskQueueFullException&& ) = default;
        TaskQueueFullException& operator=( TaskQueueFullException&& ) = default;
    };
}

#endif
