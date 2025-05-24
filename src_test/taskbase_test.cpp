/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "gtest/gtest.h"
#include "taskbase.h"

#include "util/dummytask.h"

TEST( TaskBaseTest, Constructor )
{
    threadpooluniverse::DummyTask task( 42 );
    EXPECT_EQ( task.getTaskId(), 42 );
}

TEST( TaskBaseTest, Cancel )
{
    threadpooluniverse::DummyTask task( 42 );
    EXPECT_FALSE( task.isCanceled() );
    task.cancel();
    EXPECT_TRUE( task.isCanceled() );
}
