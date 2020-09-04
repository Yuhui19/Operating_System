//
//  main.cpp
//  Assembly
//
//  Created by Yuhui on 1/22/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include <iostream>
#include <sys/time.h>

extern "C" {

  void sayHello();
void myPuts();
timeval myGTOD();
}

//void sayHello(){
//    std::cout<<"hello\n";
//}

int main(int argc, const char * argv[]) {
    sayHello();
    std::cout<<std::endl;
    myPuts();
    std::cout<<std::endl;
    timeval time=myGTOD();
    std::cout<<time.tv_sec<<" "<<time.tv_usec<<std::endl;
    timeval realtime;
    gettimeofday(&realtime, nullptr);
    std::cout<<realtime.tv_sec<<" "<<realtime.tv_usec<<std::endl;
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
