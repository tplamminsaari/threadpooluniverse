/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

#include "../include/threadpoolexceptions.h"

namespace threadpooluniverse
{
    ThreadPoolBaseException::ThreadPoolBaseException( const std::string& message )
        : std::runtime_error( message )
    {
    }

    TaskQueueFullException::TaskQueueFullException( const std::string& message )
        : ThreadPoolBaseException( message )
    {
    }

    AlreadyCanceledException::AlreadyCanceledException( const std::string& message )
        : ThreadPoolBaseException( message )
    {
    }

}  // namespace threadpooluniverse
