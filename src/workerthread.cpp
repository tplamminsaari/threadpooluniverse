
#include "workerthread.h"

namespace threadpooluniverse
{
    WorkerThread::WorkerThread()
    {

    }

    WorkerThread::~WorkerThread()
    {
    }

    void WorkerThread::requestExit()
    {
        mRequestExit.store( true );
    }
}
