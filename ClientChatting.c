#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#define BUFFER 512

int a =10;
#define SERV_IP "127.0.0.1"
#define SERV_PORT 4000

void message(int fd);
void *MessageSend(void* fd);
void *MessageRev(void* fd);

int main(int argc, char *argv[]){
    int sockfd; /*will hold the destination addr */
    struct sockaddr_in dest_addr;
    //sockaddr_in는 소켓주소를 담는 기본 구조체 틀의 역할을 한다
    //
    int n = 0;

    int rcv_byte;
    char buf[512];
    char msg[512];

    char id[20];
    char pw[20];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //소켓을 생성한다 socket()은 system-call 소켓 생성을 의미한다

    if(sockfd == -1){
        //소켓함수를 사용하고 올바르게 소켓 생성시 0, 오류 발생시 -1을 반환한다
        perror("Client_socket() error lol!");
        exit(1);
    }
    else printf("Client-socket() sockfd is OK...\n");

    /* host byte order */
    dest_addr.sin_family = AF_INET;
    //sin_family는 주소체제를 저장하는 필드인다.
    //ipv4를 위한 주소체제이다. 
    //AF_INET은 ipv4주소 체제를 사용하는 인터넷 망에 접속한다는 의미이다.


    /* short, network byte order */
    dest_addr.sin_port = htons(SERV_PORT);
    //htons함수는 2바이트 데이터 네트워크를 byte order로 변경한다는 의미이다
    dest_addr.sin_addr.s_addr = inet_addr(SERV_IP);
    //inet_addr()함수는 ipv4주소 체제를 network byte order의 binary주소로 변환하는 함수이다


    /* zero the rest of the struct */
    memset(&(dest_addr.sin_zero),0 ,0);
    //memset은 memory setting의 약자로 메모리를 특정 값으로 세팅하는 함수이다

    /* connect */
    if(connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr)) == -1){
       //연결을 요청하는 함수이다. 성공시 0 실패시 -1을 반환한다
        perror("Client-connection() error lol");
        exit(1);
    }
    else printf("Client-connection() is OK...\n\n");

    /* receive INIT_MSG */
    rcv_byte = recv(sockfd, buf, sizeof(buf), 0);
    //recv함수를 통하여 다름 프로세스로 부터 데이터를 수신할 수 있다

    printf("%s\n",buf);

    while(1){
        printf("ID : ");
        scanf("%s",id);
        send(sockfd, id, strlen(id) + 1, 0);
        printf("PW : ");
        scanf("%s",pw);
        send(sockfd, pw, strlen(pw) + 1, 0);
        recv(sockfd,buf,sizeof(buf),0);
        if(strcmp(buf,"log-in")==0){
            printf("welcome\n");
            message(sockfd);
            break;
        }
        printf("login again\n");
    }
    
    

    //close(sockfd);

    return 0;
}
void message(int fd){
    int status;
    int threadErr;
    pthread_t SendThread, RevThread;
    printf("채팅 시작! 대화내용을 입력해주세요!\n");
    if(threadErr = pthread_create(&SendThread,NULL,MessageSend,(void*)&fd)!=0){
        printf("Thread ERR = %d",threadErr);
    }
    if(threadErr = pthread_create(&RevThread,NULL,MessageRev,(void*)&fd)!=0){
        printf("Thread ERR = %d",threadErr);
    }
    pthread_join(SendThread, (void **)&status);
    pthread_join(RevThread, (void **)&status);

}
void *MessageSend(void *fd){
    int newfd = *((int *)fd);
    char SEND_BUFF[BUFFER];
    while(1){
            scanf("%s",SEND_BUFF);
            send(newfd, SEND_BUFF, strlen(SEND_BUFF) + 1, 0);

        }
}
void *MessageRev(void *fd){
    int newfd = *((int *)fd);
    char RECV_BUFF[BUFFER];
    while(1){
        recv(newfd,RECV_BUFF,sizeof(RECV_BUFF),0);
        printf("%s\n",RECV_BUFF);   
    }
}