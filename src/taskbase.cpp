/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

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
        if( mCanceled.load() )
        {
            return;
        }
        mCanceled.store( true );
    }

    bool TaskBase::isCanceled() const
    {
        return mCanceled.load();
    }

    void TaskBase::handleError()
    {
        // Default implementation does nothing.
    }

}  // namespace threadpooluniverse
