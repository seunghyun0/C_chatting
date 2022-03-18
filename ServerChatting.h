#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 4000
#define BACKLOG 10

#define INIT_MSG  "================================================\nHello! I'm P2P File sharing server...\nPlease, LOG-IN!\n==============================================="

#define BUFFER 512
#define LOGIN_BUFFER 20

#define USER1_ID "user1"
#define USER1_PW "passwd1"
#define USER2_ID "user2"
#define USER2_PW "passwd2"

void login(int new_fd);
void message(int fd);
void *MessageSend(void* fd);
void *MessageRev(void* fd);
