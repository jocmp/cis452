//
// Created by Hans Dulimarta on 2/17/16.
//

#include "gv_semaphore.h"

namespace gvsu {

semaphore::semaphore(int c) : count(c) {

}

void semaphore::wait() {
    /* this unique_lock will lock the entire function */
    std::unique_lock<std::mutex> lock (mtx);
#ifdef WITHOUT_LAMBDA
    while (count <= 0)
                cv.wait (lock);
#else
    /* the following code using lambda expressions is shorter */
    /* this->count is outside the scope of lambda, we must capture
     * "this" */
    cv.wait (lock, [this] { return count > 0; });
#endif
    count--;

}

void semaphore::signal() {
    /* this unique_lock will lock the entire function */
    std::unique_lock<std::mutex> lock (mtx);
    count++;
    cv.notify_one();

}
}
