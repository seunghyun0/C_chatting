#include "ServerChatting.h"
int main (void){
  /*listen on sock_fd, new connection on new_fd*/
  int sockfd, new_fd;
  /*my address information, address where I run this program*/
  struct sockaddr_in my_addr;
  /*remote address information */
  struct sockaddr_in their_addr;
  unsigned int sin_size;
  
  /* bufffer */
  int rcv_byte;

  int val = 1;

  pid_t  pid;
  /* socket */

  sockfd = socket(AF_INET,SOCK_STREAM,0);//소켓 생성
  if(sockfd == -1){
    perror("Server-socket() error lol!");
    exit(1);
  }
  else printf("Server-socket() sockfd is OK...\n");
  
  /*host byte order*/
  my_addr.sin_family = AF_INET;

  /*short, network byte order*/
  my_addr.sin_port = htons(SERV_PORT);
  //my_addr.sin_addr.s_addr = inet_addr(SERV_IP);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  //INADDR_ANY의 의미는 자동으로 이 컴퓨터에 존재하는 랜카드중 사용가능한 랜카드의 IP주소를 사용하라는 의미이다.
  //사용하는 이유는 여러 ip주소에 들어오는 데이터를 모두 수신하기 위해서이다

  /*zero the rest of the struct*/
  memset(&(my_addr.sin_zero),0,8);

  /*to prevent 'Address already in use...'*/
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val))<0){
    //setsockopt()함수를 통하여 소켓의 송수신 동작을 다양한 옵션으로 제어 할 수 있다
    perror("setsockopt");
    close(sockfd);
    return -1;
  }

  /*bind*/
  if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
    //bind 함수는 소켓에 서버 소켓에 필요한 정보를 할당하고 커널에 등록하는 함수입니다.
    perror("Server-bind() error lol!");
    exit(1);
  }
  else printf("Server-bind() is OK...\n");

  /*listen*/
  if(listen(sockfd, BACKLOG) == -1){
    //listen함수는 연결 요청을 할 수 있는 상태로 만들어주는 함수 이다

    perror("listen() error lol!");
    exit(1);
  }
  else printf("listen() is OK...\n\n");
while(1){
  sin_size = sizeof(struct sockaddr_in);
  new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
    pid = fork();
    if(pid == 0){
      login(new_fd);
      message(new_fd);
    }
}
  close(sockfd);

  return 0;
  
}
void login(int fd){
  char id[LOGIN_BUFFER];
  char pw[LOGIN_BUFFER];
  int new_fd = fd;
  send(new_fd, INIT_MSG, strlen(INIT_MSG) + 1, 0);
    //연결된 서버나 클라이언트로 데이터를 전송한다
    while(1){  
      recv(new_fd,id,sizeof(id),0);
      recv(new_fd,pw,sizeof(pw),0);
      if(strcmp(id,USER1_ID) == 0 && strcmp(pw,USER1_PW)==0){
          printf("log-in success\n");
          send(new_fd, "log-in", strlen("log-in") + 1, 0);
          break;
         }
      else if(strcmp(id,USER2_ID) == 0&&strcmp(pw,USER2_PW)==0){
          printf("log-in success\n");
          send(new_fd, "log-in", strlen("log-in") + 1, 0);
          break;
         }    
      send(new_fd, "login again", strlen("login again") + 1, 0);
  }
}
void message(int fd){
    char RECV_BUFF[BUFFER];
    char SEND_BUFF[BUFFER];
    pid_t  pid_1;   
    pid_1 = fork();
    if(pid_1 == 0){
      while(1){
          recv(fd,RECV_BUFF,sizeof(RECV_BUFF),0);
          printf("CLIENT : %s\n",RECV_BUFF);
          sleep(1);
      }
    }
    if(pid_1 == 1){
        while(1){
            printf("SERVER : ");
            scanf("%s",SEND_BUFF);
            send(fd, SEND_BUFF, strlen(SEND_BUFF) + 1, 0);
            sleep(1);
        }
    }

}