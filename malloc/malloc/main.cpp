//
//  main.cpp
//  malloc
//
//  Created by Yuhui on 3/18/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <vector>
#include <chrono>
#include <ctime>
#include <stdlib.h>


struct AddressEntry {
    void *ptr;
    size_t size;
};

class Allocator {
private:
    
    AddressEntry *addressEntries;
    int *states;
    int capacity;
    int size;
    
    double loadFactor() {
        return (double)size / capacity;
    }
    
    void insert(void *ptr, size_t numOfBytes) {
    
        struct AddressEntry entry = {ptr, numOfBytes};

        int index = indexToInsert(states, capacity, entry);
        addressEntries[index] = entry;
        states[index] = 1;
        size++;
        
        if (loadFactor() > 0.5)
            grow();
        
        assert(loadFactor() <= 0.5);
    }
    
    long hash(void* ptr) {
        return (size_t)ptr >> 12;
    }
    
    int indexToInsert(int *states, int capacity, AddressEntry& toBeInserted) {
        int index = hash(toBeInserted.ptr) % capacity;
        
        while (true) {
            if (states[index] == -1 || states[index] == 0)
                return index;
            
            index++;
            if (index == capacity) {
                index = 0;
            }
        }
    }
    
    int lookup(void *ptr) {
        int index = hash(ptr) % capacity;
        
        while (states[index] != -1) {
            if (states[index] == 1 && addressEntries[index].ptr == ptr)
                return index;
            
            index++;
        }
        return -1;
    }
    
    void grow() {
        int new_capacity = capacity * 2;
        
        AddressEntry *new_addressEntries = (AddressEntry *) mmap(NULL,
                                                                 new_capacity * sizeof(struct AddressEntry),
                                                                 PROT_READ|PROT_WRITE|PROT_EXEC,
                                                                 MAP_ANON|MAP_SHARED, 0, 0);
        int *new_states = (int *) mmap(NULL,
                                       new_capacity * sizeof(int),
                                       PROT_READ|PROT_WRITE|PROT_EXEC,
                                       MAP_ANON|MAP_SHARED, 0, 0);
        for (int i = 0; i < new_capacity; i++)
            new_states[i] = -1;
        
        for (int i = 0; i < capacity; i++) {
            if (states[i] == 1) {
                int index = indexToInsert(new_states, new_capacity, addressEntries[i]);
                
                new_addressEntries[index] = addressEntries[i];
                new_states[index] = 1;
            }
        }
        
        if (munmap(addressEntries, capacity * sizeof(struct AddressEntry)) == -1) {
            perror("Old addressEntries deallocation failed");
            exit(1);
        }
        
        if (munmap(states, capacity * sizeof(int)) == -1) {
            perror("Old states deallocation failed");
            exit(1);
        }
        
        addressEntries = new_addressEntries;
        states = new_states;
        capacity = new_capacity;
    }
    
public:
    
    Allocator() {
        int pagesize = getpagesize();
        
        void *addressEntriesPtr = mmap(NULL,
                                       pagesize * sizeof(struct AddressEntry),
                                       PROT_READ|PROT_WRITE|PROT_EXEC,
                                       MAP_ANON|MAP_SHARED, 0, 0);
        if (addressEntriesPtr == MAP_FAILED) {
            perror("Allocation for the addressEntries array failed");
            exit(1);
        }
        addressEntries = (AddressEntry *)addressEntriesPtr;
        
        void *statesPtr = mmap(NULL,
                              pagesize * sizeof(int),
                              PROT_READ|PROT_WRITE|PROT_EXEC,
                              MAP_ANON|MAP_SHARED, 0, 0);
        
        if (statesPtr == MAP_FAILED) {
            perror("Allocation for the states array failed");
            exit(1);
        }
        
        states = (int *)statesPtr;
        for (int i = 0; i < pagesize; i++)
            states[i] = -1;
        
        capacity = pagesize;
        size = 0;
    }
    
    ~Allocator() {
        for (int i = 0; i < capacity; i++) {
            if (states[i] == 1) {
                if (munmap(addressEntries[i].ptr, addressEntries[i].size) == -1) {
                    perror("address entry deallocation failed");
                    exit(1);
                }
            }
        }
        
        if (munmap(addressEntries, capacity * sizeof(struct AddressEntry)) == -1) {
            perror("addressEntries deallocation failed");
            exit(1);
        }
        
        if (munmap(states, capacity * sizeof(int)) == -1) {
            perror("states deallocation failed");
            exit(1);
        }
    }
    
    void* allocate(size_t bytesToAllocate) {
        int pageSize = getpagesize();
        
        size_t actualBytesAllocated;
        if (bytesToAllocate % pageSize == 0)
            actualBytesAllocated = bytesToAllocate;
        else
            actualBytesAllocated = ((bytesToAllocate / pageSize) + 1) * pageSize;

        void *ptr = mmap(NULL,
                         actualBytesAllocated,
                         PROT_READ|PROT_WRITE|PROT_EXEC,
                         MAP_ANON|MAP_SHARED, 0, 0);
        if (ptr == MAP_FAILED) {
            perror("Memory allocation failed");
            exit(1);
        }
        insert(ptr, actualBytesAllocated);

        return ptr;
    }

    void deallocate(void* ptr) {
        
        int index = lookup(ptr);
        
        if (index == -1) {
            perror("The given ptr is not found");
            exit(1);
        }
        
        if (munmap(ptr, addressEntries[index].size) == -1) {
            perror("Memory deallocation failed");
            exit(1);
        }
        
        states[index] = 0;
        size--;
    }
};

void test() {
    Allocator allocator;
    
    std::vector<void*> vec;
    for (int i = 0; i < 100; i++) {
        void *ptr = allocator.allocate(1);
        vec.push_back(ptr);
    }
    
    for (int i = 0; i < vec.size(); i++)
        allocator.deallocate(vec[i]);
}

void timing() {
    
    std::vector<int> vec;
    for (int i = 0; i < 10000; i++) {
        vec.push_back(rand() % 10000 + 1);
    }
    
    Allocator allocator;
    
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed_seconds;
    
    std::vector<void*> ptrs_myallocator;
    start = std::chrono::system_clock::now();
    
    for (int i = 0; i < 10000; i++) {
        void *ptr = allocator.allocate(vec[i]);
        ptrs_myallocator.push_back(ptr);
    }
    
    for (int i = 0; i < 10000; i++) {
        allocator.deallocate(ptrs_myallocator[i]);
    }
    
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "My_malloc: " << elapsed_seconds.count() << "s\n";
    
    
    std::vector<void*> ptrs_defaultallocator;
    start = std::chrono::system_clock::now();
    for (int i = 0; i < 10000; i++) {
        void *ptr = malloc(vec[i]);
        ptrs_defaultallocator.push_back(ptr);
    }
    
    for (int i = 0; i < 10000; i++) {
        free(ptrs_defaultallocator[i]);
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    std::cout << "Build-in_malloc: " << elapsed_seconds.count() << "s\n";
    
}

int main(int argc, const char * argv[]) {

    test();
    
    timing();
    
    return 0;
}
