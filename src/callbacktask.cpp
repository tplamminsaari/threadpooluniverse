#include "../include/callbacktask.h"

namespace threadpooluniverse
{
    CallbackTask::CallbackTask( uint64_t taskId, Callback callback ) :
        TaskBase( taskId ),
        mCallback( std::move( callback ) )
    {
    }

    CallbackTask::~CallbackTask()
    {
    }

    void CallbackTask::execute()
    {
        if( mCallback && !isCanceled() )
        {
            mCallback();
        }
    }

}  // namespace threadpooluniverse