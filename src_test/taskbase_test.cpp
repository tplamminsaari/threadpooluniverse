/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "gtest/gtest.h"
#include "taskbase.h"

TEST( TaskBaseTest, Constructor )
{
    threadpooluniverse::TaskBase taskBase( 42 );
    EXPECT_EQ( taskBase.getTaskId(), 42 );
}

TEST( TaskBaseTest, Cancel )
{
    threadpooluniverse::TaskBase taskBase( 42 );
    EXPECT_FALSE( taskBase.isCanceled() );
    taskBase.cancel();
    EXPECT_TRUE( taskBase.isCanceled() );
    EXPECT_THROW( taskBase.cancel(), std::runtime_error );
}
