/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

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