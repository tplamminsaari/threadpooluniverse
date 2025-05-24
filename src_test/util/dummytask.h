/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#ifndef THREADPOOLUNIVERSE_TESTS_DUMMYTASK_H
#define THREADPOOLUNIVERSE_TESTS_DUMMYTASK_H

#include "taskbase.h"
namespace threadpooluniverse
{
    class DummyTask : public TaskBase
    {
    public:
        explicit DummyTask( uint64_t taskId );

        virtual ~DummyTask() = default;
        virtual void execute() override;
    };

}  // namespace threadpooluniverse

#endif
