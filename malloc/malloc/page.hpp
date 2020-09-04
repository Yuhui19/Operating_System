//
//  page.hpp
//  malloc
//
//  Created by Yuhui on 3/22/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#ifndef page_hpp
#define page_hpp

#include <stdio.h>

class pageEntry{
public:
    void *pointer;
    size_t space;
    pageEntry(void *pointer, size_t space);
};

#endif /* page_hpp */
