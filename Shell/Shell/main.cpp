//
//  main.cpp
//  Shell
//
//  Created by Yuhui on 2/13/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include <iostream>
#include "shelpers.hpp"
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <algorithm>

void changeDirectory(Command &input);
void handleVariable(Command &input);

int main(int argc, const char * argv[]) {
    
    std::cout<< "Shell is running."<<std::endl;
    std::string s = "shous-MacBook-Pro:~ Yu$ ";
    
    int status;
    std::vector<pid_t> background_commands;
    while(true){
        char *buffer = readline(s.c_str());
        std::string input=buffer;
        
        if (background_commands.size() != 0) {
            std::cout << "There are " << background_commands.size() << " commands running in the background." << std::endl;
        }
        
        std::vector<Command> commands=getCommands(tokenize(input));
        
        for(int i=0;i<commands.size();i++){
            Command c = commands.at(i);
            
            if(c.exec=="cd"){
                changeDirectory(c);
                continue;
            }
            
            if(c.exec=="exit")
                exit(0);
            
            if(c.exec.find("=") != std::string::npos){
                handleVariable(c);
                continue;
            }
            
            if(c.exec=="echo" && c.argv.at(1)[0]=='$'){
                const char *str = c.argv.at(1);
                str++;
                std::cout<<getenv(str)<<std::endl;
                continue;
            }
            
            if(c.background)
                background_commands.push_back(getpid());
            
            int rc=fork();
            if(rc<0){
                fprintf(stderr, "Fail to fork.\n");
                exit(1);
            }else if(rc==0){
                std::cout<<"Running: "<<c.exec<<std::endl;
                std::cout<<c.exec<<": "<<"Stdin: "<<c.fdStdout<<" "<<"Stdout: "<<c.fdStdout<<std::endl;
                
                if(c.fdStdin!=0){
                    if(dup2(c.fdStdin, 0)==-1){
                        fprintf(stderr, "Fail to copy.\n");
                        exit(1);
                    }
                }
                if (c.fdStdout != 1) {
                    if (dup2(c.fdStdout, 1) == -1) {
                        fprintf(stderr, "Failed to copy the file descriptor.\n");
                        exit(1);
                    }
                }
                int rt=execvp(c.exec.c_str(), const_cast<char**>(c.argv.data()));
                if(rt==-1){
                    fprintf(stderr, "Fail to run this command.\n");
                    std::cout<<c<<std::endl;
                    exit(1);
                }
            }else{
                
                // if it is a background command, don't wait
                // if it is not, wait
                if(!c.background)
                    waitpid(rc, &status, 0);
                if (c.fdStdin != 0) {
                    if (close(c.fdStdin) == -1){
                        fprintf(stderr, "Failed to close the file descriptor.\n");
                        exit(1);
                    }
                }
                if (c.fdStdout != 1) {
                    if (close(c.fdStdout) == -1) {
                        fprintf(stderr, "Failed to close the file descriptor.\n");
                        exit(1);
                    }
                }
            }
        }
    }
    // insert code here...
//    std::cout << "Hello, World!\n";
    return 0;
}

void changeDirectory(Command &input){
    if(input.argv.size()==2){
        char* path=getenv("HOME");
        chdir(path);
        std::cout<< "Change to home directory: "<<path<<std::endl;
    }else{
        chdir(input.argv.at(1));
        std::cout<< "Change directory to: "<<input.argv.at(1)<<std::endl;
    }
}

void handleVariable(Command &input){
    std::string s = input.exec;
    std::string name=s.substr(0, s.find("="));
    
    std::string value=s.substr(s.find("=")+1, std::string::npos);
    int rt = setenv(name.c_str(), value.c_str(), 1);
    if(rt == -1){
        fprintf(stderr, "Fail to set variable.\n");
        exit(1);
    }
}
