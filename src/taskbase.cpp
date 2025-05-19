
#include <stdexcept>
#include "../include/taskbase.h"

namespace threadpooluniverse
{
    TaskBase::TaskBase( uint64_t taskId ) :
        mTaskId( taskId ),
        mCanceled( false )
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
        if( mCanceled )
        {
            throw std::runtime_error( "Already canceled" );
        }
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
