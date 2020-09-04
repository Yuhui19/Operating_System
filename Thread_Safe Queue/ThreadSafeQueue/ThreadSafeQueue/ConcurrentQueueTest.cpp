//
//  ConcurrentQueueTest.cpp
//  ThreadSafeQueue
//
//  Created by Yuhui on 4/10/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//
#include "ConcurrentQueue.hpp"
#include <vector>
#include <thread>
#include <iostream>
#include <stdio.h>

bool testQueue(int num_producers, int num_consumers, int num_ints){
    std::vector<std::thread> v;
    ConcurrentQueue<int> q;

    for(int i = 0; i < num_consumers * num_ints; i++)
        q.enqueue(i);

    for(int i = 0; i < num_producers; i++){
        v.push_back(std::thread([&q, &num_ints]{
            for(int k = 0; k < num_ints; k++)
                q.enqueue(k);
        }));
    }
    for(int i = 0; i < num_consumers; i++){
        v.push_back(std::thread([&q, &num_ints]{
            int ret;
            for(int k = 0; k < num_ints; k++)
                q.dequeue(&ret);
        }));
    }
    for(int i = 0; i < num_producers + num_consumers; i++)
        v[i].join();
    return q.size() == (num_producers - num_consumers) * num_ints + num_consumers * num_ints;
}

int main(int argc, const char * argv[]){
    if(testQueue(3, 3, 10))
        std::cout << "ConcurrentQueueTest result: true" << std::endl;
    else
        std::cout << "ConcurrentQueueTest result: false" << std::endl;
    return 0;
}
