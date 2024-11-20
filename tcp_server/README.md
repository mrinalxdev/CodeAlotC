# TCP Server Project

A simple TCP server implementation in C with testing capabilities.


Demo : 

## Project Structure
- `src/`: Source code files
- `include/`: Header files
- `tests/`: Test files
- `build/`: Compiled binaries and object files
  - `bin/`: Executable files
  - `obj/`: Object files

## Building
```bash
# Build the server
make

# Run tests
make test

# Clean build files
make clean
```

## Running
```bash
# Start the server
./build/bin/server
```

# include/server.h
```c
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Server functions
int initialize_server(void);
int accept_connection(int server_fd);
int handle_client(int client_fd);
void close_server(int server_fd);

#endif
```

# tests/test_utils.h

```c
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <assert.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "../include/server.h"

// Test helper functions
void assert_success(int result, const char* message) {
    if (result < 0) {
        printf("Test failed: %s\n", message);
        exit(1);
    }
}

#endif
```