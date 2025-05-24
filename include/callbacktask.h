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
        using ExecuteCallback = std::function<void()>;
        using ErrorCallback = std::function<void()>;

        /**
         * @brief Constructs a CallbackTask with a unique task ID and a callback function.
         * @param taskId Unique identifier for the task.
         * @param callback The function to be executed when the task is run.
         * @param errorCallback Optional function to handle errors during execution.
         * 
         * Executes the callback function when the task is run. If callback function throws an
         * exception, the errorCallback is invoked.
         */
        CallbackTask( uint64_t taskId, ExecuteCallback callback, ErrorCallback errorCallback = ErrorCallback() );
        virtual ~CallbackTask();

    public :  // from TaskBase
        virtual void execute() override;
        virtual void handleError() override;

    private:
        ExecuteCallback mCallback;
        ErrorCallback mErrorCallback;
    };

}  // namespace threadpooluniverse
#endif
