
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
        explicit TaskBase( uint64_t taskId );
        virtual ~TaskBase();

        TaskBase( const TaskBase& other ) = delete;
        TaskBase& operator=( const TaskBase& other ) = delete;
        TaskBase( TaskBase&& other ) = delete;
        TaskBase& operator=( TaskBase&& other ) = delete;

        uint64_t getTaskId() const;
        void cancel();
        bool isCanceled() const;
        virtual void execute();

    protected:
        uint64_t mTaskId;
        std::atomic_bool mCanceled;
    };
}

#endif
