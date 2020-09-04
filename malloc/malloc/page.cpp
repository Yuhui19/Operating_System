//
//  page.cpp
//  malloc
//
//  Created by Yuhui on 3/22/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include "page.hpp"

pageEntry::pageEntry(void *pointer, size_t space){
    this->pointer = pointer;
    this->space = space;
}
