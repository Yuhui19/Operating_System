//
//  main.c
//  Pipes
//
//  Created by Yuhui on 2/10/20.
//  Copyright © 2020 Yuhui. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, const char * argv[]) {
    
    int filedes[2];
    
    long length = strlen(argv[1]);
    if(pipe(filedes)<0){
        fprintf(stderr, "Failed to create a pipe.\n");
        exit(1);
    }
    
    int rc=fork();
    if(rc<0){
        fprintf(stderr, "Fork failed.\n");
        exit(1);
    }else if (rc == 0){
        if(close(filedes[1])<0){
            fprintf(stderr, "Child failed to close the end.\n");
            exit(1);
        }
    
    char string[length];
    long bytes_read = read(filedes[0], string, length);
    
    if (bytes_read == -1) {
        fprintf(stderr, "Child failed to read the message.\n");
        exit(1);
    } else if (bytes_read < length) {
        fprintf(stderr, "Child failed to read the message.\n");
        fprintf(stderr, "Message length: %ld; Bytes_read: %ld.\n", length, bytes_read);
        exit(1);
    }
    
    printf("Message received: %s Message length: %ld\n", string, length);
    } else {
    if (close (filedes[0]) < 0) {
        fprintf(stderr, "Parent failed to close the end.\n");
        exit(1);
    }
    
    long bytes_written = write(filedes[1], argv[1], length);
    
    if (bytes_written == -1) {
        fprintf(stderr, "Parent failed to close the end.\n");
        exit(1);
    } else if (bytes_written < length) {
        fprintf(stderr, "Parent failed to read the message.\n");
        fprintf(stderr, "Message length: %ld; Bytes_written: %ld.\n", length, bytes_written);
        exit(1);
    }
    
    printf("Sending a message from parent(%d) to child(%d).\n", (int)getpid(), rc);
    printf("Message sent: %s Message length: %ld\n", argv[1], length);
    }
    
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
