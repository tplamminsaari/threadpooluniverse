#ifndef THREADPOOLUNIVERSE_CALLBACKTASK_H
#define THREADPOOLUNIVERSE_CALLBACKTASK_H

#include "taskbase.h"

#include <functional>

namespace threadpooluniverse
{
    /**
     * @brief A task that executes a callback function.
     */
    class CallbackTask : public TaskBase
    {
    public:
        using Callback = std::function<void()>;

        explicit CallbackTask( uint64_t taskId, Callback callback );
        ~CallbackTask() override;
        void execute() override;

    private:
        Callback mCallback;
    };

}  // namespace threadpooluniverse
#endif
