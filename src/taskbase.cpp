
#include "../include/taskbase.h"

namespace threadpooluniverse
{
    TaskBase::TaskBase( uint64_t taskId )
    {
    }

    TaskBase::~TaskBase()
    {
    }

    uint64_t TaskBase::getTaskId() const
    {
        return mTaskId;
    }

    void TaskBase::cancel()
    {
        mCanceled = true;
    }

    bool TaskBase::isCanceled() const
    {
        return mCanceled;
    }

    void TaskBase::execute()
    {
    }
}
