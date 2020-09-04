//
//  ConcurrentQueue.hpp
//  ThreadSafeQueue
//
//  Created by Yuhui on 4/10/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//
#pragma once
#include <stdio.h>
#include<mutex>

template <typename T>
class ConcurrentQueue{
private:
    struct Node{
        T data;
        Node* next;
    };
    std::mutex m;
    
    Node* head;
    Node* tail;
    int size_;
    
public:
    ConcurrentQueue()
    :head(new Node{T{}, nullptr}), size_(0)
    {
        tail = head;
    }
    
    void enqueue(const T& x){
        Node *n = new Node{x, nullptr};
        {
            std::unique_lock<std::mutex> lock(m);
            tail->next = n;
            tail = tail->next;
            size_++;
        }
    }
    
    bool dequeue(T* ret){
        {
            std::unique_lock<std::mutex> lock(m);
            if (size_ == 0)
                return false;
            Node *temp = head;
            head = head->next;
            size_--;
            if (size_ > 0)
                *ret = head->next->data;
            delete temp;
        }
        
        return true;
    }
    
    ~ConcurrentQueue(){
        
        while(head){
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }
    
    int size() const{ return size_;}
};
