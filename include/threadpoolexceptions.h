#ifndef THREADPOOLUNIVERSE_THREADPOOLEXCEPTIONS_H
#define THREADPOOLUNIVERSE_THREADPOOLEXCEPTIONS_H

#include <stdexcept>

namespace threadpooluniverse
{
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
