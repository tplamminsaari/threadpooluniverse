/**
 * Copyright (c) 2025 Tomi Lamminsaari
 *
 * This software is licensed under the MIT License.
 * See the accompanying LICENSE file for more details.
 */

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

        /**
         * @brief Constructs a CallbackTask with a unique task ID and a callback function.
         * @param taskId Unique identifier for the task.
         * @param callback The function to be executed when the task is run.
         */
        explicit CallbackTask( uint64_t taskId, Callback callback );
        ~CallbackTask() override;

    public :  // from TaskBase
        void execute() override;

    private:
        Callback mCallback;
    };

}  // namespace threadpooluniverse
#endif
