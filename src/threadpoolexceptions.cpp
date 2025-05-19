#include "../include/threadpoolexceptions.h"

namespace threadpooluniverse
{
TaskQueueFullException::TaskQueueFullException( const std::string& message )
    : std::runtime_error( message )
{
}

}  // namespace threadpooluniverse
