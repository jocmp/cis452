//
// Created by Hans Dulimarta
//
// Reference: https://gist.github.com/sguzman/9594227
//
#ifndef CONCURRENCY_SEMAPHORE_H
#define CONCURRENCY_SEMAPHORE_H
#include <mutex>
#include <condition_variable>

namespace gvsu {
    class semaphore {
    public:
        semaphore(int c = 0);
        void wait();
        void signal();

    private:
        int count;
        std::mutex mtx;
        std::condition_variable cv;
    };
}

#endif //CONCURRENCY_SEMAPHORE_H
