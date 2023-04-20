#include "SyncBase.h"

namespace CyclePipe
{
    void CyclePipe::SyncBase::Close()
    {
        std::lock_guard<std::shared_mutex> lock(mutex);
        if (closed) return;
        closed = true;
        condition.notify_all();
    }
    
    bool SyncBase::Closed()
    {
        std::shared_lock lk(mutex);
        return closed;
    }
}