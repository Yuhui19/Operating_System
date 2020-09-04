//
//  SerialQueueTest.cpp
//  ThreadSafeQueue
//
//  Created by Yuhui on 4/10/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include <stdio.h>
#include <cassert>
#include <iostream>
#include "SerialQueue.hpp"

void test_char_static(){
    SerialQueue<char> q{};
    assert(q.size()==0);
    q.enqueue('h');
    q.enqueue('e');
    q.enqueue('l');
    q.enqueue('l');
    q.enqueue('o');
    char ret;
//    assert(q.dequeue(&ret));
//    assert(ret == 'h');
    assert(q.dequeue(&ret));
    assert(ret == 'e');
    assert(q.dequeue(&ret));
    assert(ret == 'l');
    assert(q.dequeue(&ret));
    assert(ret == 'l');
    assert(q.dequeue(&ret));
    assert(ret == 'o');
    assert(q.dequeue(&ret));
    assert(q.size()==0);
}

void test_char_dynamic(){
    SerialQueue<char>* q=new SerialQueue<char>();
    assert(q->size()==0);
    q->enqueue('w');
    q->enqueue('o');
    q->enqueue('r');
    q->enqueue('l');
    q->enqueue('d');
    char ret;
//    assert(q->dequeue(&ret));
//    assert(ret == 'w');
    assert(q->dequeue(&ret));
    assert(ret == 'o');
    assert(q->dequeue(&ret));
    assert(ret == 'r');
    assert(q->dequeue(&ret));
    assert(ret == 'l');
    assert(q->dequeue(&ret));
    assert(ret == 'd');
    assert(q->dequeue(&ret));
    assert(q->size()==0);
}

void test_int_dynamic(){
    SerialQueue<int>* q=new SerialQueue<int>();
    assert(q->size()==0);
    q->enqueue(1);
    q->enqueue(2);
    q->enqueue(3);
    q->enqueue(4);
    q->enqueue(5);
    assert(q->size()==5);
    int ret;
//    assert(q->dequeue(&ret));
//    std::cout<<ret<<std::endl;
//    assert(ret == 1);
    assert(q->dequeue(&ret));
    assert(ret == 2);
    assert(q->dequeue(&ret));
    assert(ret == 3);
    assert(q->dequeue(&ret));
    assert(ret == 4);
    assert(q->dequeue(&ret));
    assert(ret == 5);
    assert(q->dequeue(&ret));
//    std::cout<<ret<<std::endl;
    assert(q->size()==0);
}

void test_int_static(){
    SerialQueue<int> q{};
    assert(q.size()==0);
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);
    q.enqueue(4);
    q.enqueue(5);
    int ret;
//    assert(q.dequeue(&ret));
//    assert(ret == 1);
    assert(q.dequeue(&ret));
    assert(ret == 2);
    assert(q.dequeue(&ret));
    assert(ret == 3);
    assert(q.dequeue(&ret));
    assert(ret == 4);
    assert(q.dequeue(&ret));
    assert(ret == 5);
    assert(q.dequeue(&ret));
    assert(q.size()==0);
}


//int main(int argc, const char * argv[]) {
//
//    test_int_dynamic();
//    test_int_static();
//
//    test_char_dynamic();
//    test_char_static();
//
//    std::cout << "SerialQueueTest result: true" << std::endl;
//    return 0;
//}

