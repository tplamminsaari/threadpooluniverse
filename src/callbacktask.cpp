/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "../include/callbacktask.h"

namespace threadpooluniverse
{
CallbackTask::CallbackTask( uint64_t taskId, ExecuteCallback callback, ErrorCallback errorCallback )
    :
        TaskBase( taskId ),
        mCallback( std::move( callback ) ),
        mErrorCallback( std::move( errorCallback ) )
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

    void CallbackTask::handleError()
    {
        if( mErrorCallback )
        {
            mErrorCallback();
        }
    }

}  // namespace threadpooluniverse