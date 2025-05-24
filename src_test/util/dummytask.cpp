/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "dummytask.h"

namespace threadpooluniverse
{
    DummyTask::DummyTask( uint64_t taskId ) : TaskBase( taskId )
    {
    }

    void DummyTask::execute()
    {
        // Dummy task does nothing.
    }
}