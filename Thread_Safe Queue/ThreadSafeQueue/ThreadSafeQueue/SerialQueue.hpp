#pragma once

template <typename T>
class SerialQueue{
private:
    struct Node{
        T data;
        Node* next;
    };
    
    Node* head;
    Node* tail;
    int size_;
    
public:
    SerialQueue()
    :head(new Node{T{}, nullptr}), size_(0)
    {
        tail = head;
    }
    
    void enqueue(const T& x){
        Node *new_node = new Node{x, nullptr};
        tail->next = new_node;
        tail = tail->next;
        size_++;
    }
    
    bool dequeue(T* ret){
        if (size_ == 0)
            return false;
        
        Node *temp = head;
        head = head->next;
        delete temp;
        size_--;
        if (size_ > 0)
            *ret = head->next->data;
        
        return true;
    }
    
    ~SerialQueue(){
        
        while(head){
            Node* temp = head->next;
            delete head;
            head = temp;
        }
    }
    
    int size() const{ return size_;}
};
