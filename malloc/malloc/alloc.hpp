//
//  alloc.hpp
//  malloc
//
//  Created by Yuhui on 3/22/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#ifndef alloc_hpp
#define alloc_hpp

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include "page.hpp"

class msdalloc{
private:
    pageEntry *table;
    int size;
    int capacity;
public:
    msdalloc();
    int getSize();
    int getCapacity();
    void* allocate(size_t bytesToAllocate);
    void deallocate(void* ptr);
    pageEntry getPageEntry(void* ptr);
private:
    void add(pageEntry *table, pageEntry entry);
    void grow();
    
};

#endif /* alloc_hpp */
