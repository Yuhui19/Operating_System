//
//  alloc.cpp
//  malloc
//
//  Created by Yuhui on 3/22/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include "alloc.hpp"

msdalloc::msdalloc(){
    capacity = 4096/sizeof(pageEntry);
    size = 0;
    table = (pageEntry*)mmap(nullptr, capacity*sizeof(pageEntry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (table == MAP_FAILED) {
        perror("mmap table:");
        exit(-1);
    }
}

void msdalloc::grow(){
    capacity = capacity*2;
    pageEntry *newTable = (pageEntry*)mmap(nullptr, capacity*sizeof(pageEntry), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (newTable == MAP_FAILED) {
        perror("mmap newtable");
        exit(-1);
    }
    for(int i = 0; i < capacity/2; i++){
        pageEntry currect = table[i];
        add(newTable,currect);
        size--;
    }
    table = newTable;
}

void msdalloc::add(pageEntry *table, pageEntry entry){
    if((size+0.0)/(capacity+0.0) > 0.5){
        grow();
    }
    int hashIndex = ((unsigned long) entry.pointer >> 12) % capacity;
    int shiftIndex = 1;
    while(table[hashIndex].pointer != nullptr){
        hashIndex += shiftIndex * shiftIndex;
        shiftIndex++;
    }
    table[hashIndex] = entry;
    size++;
}

void* msdalloc::allocate(size_t bytesToAllocate){
    size_t size = 16;
    if(bytesToAllocate > size){
        size = (bytesToAllocate / 16) * 16;
    }
    if(bytesToAllocate % 16 != 0){
        size += 16;
    }
    void *ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap allocate");
        exit(-1);
    }
    pageEntry entry = pageEntry(ptr,bytesToAllocate);
    add(table,entry);
    return ptr;
}

void msdalloc::deallocate(void* ptr){
    if(ptr == nullptr){
        return;
    }
    int hashIndex = ((unsigned long) ptr >> 12) % capacity;
    int shiftIndex = 1;
    while(table[hashIndex].pointer != ptr){
        hashIndex += shiftIndex * shiftIndex;
        hashIndex = hashIndex % capacity;
        shiftIndex++;
    }
    table[hashIndex].pointer = nullptr;
    size--;
}

int msdalloc::getSize(){
    return size;
}

int msdalloc::getCapacity(){
    return capacity;
}

pageEntry msdalloc::getPageEntry(void* ptr){
    pageEntry result = pageEntry(nullptr,0);
    if(ptr == nullptr){
        return result;
    }
    int hashIndex = ((unsigned long) ptr >> 12) % capacity;
    int shiftIndex = 1;
    while(table[hashIndex].pointer != ptr && table[hashIndex].space != 0){
        hashIndex += shiftIndex * shiftIndex;
        hashIndex = hashIndex % capacity;
        shiftIndex++;
    }
    if(table[hashIndex].pointer == ptr){
        return table[hashIndex];
    }else{
        return result;
    }
}
