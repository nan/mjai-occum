#ifndef mjaiC___socket_h
#define mjaiC___socket_h

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "MJAIGame.hpp"

class Socket {
    
public:
    int start(char *ip, int port, MJAIGame *game) {
        
        int sockfd = 0, n = 0;
        char recvBuff[65535];
        struct sockaddr_in serv_addr;
        
        memset(recvBuff, '0', sizeof(recvBuff));
        
        /* a socket is created through call to socket() function */
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            printf("\n Error : Could not create socket \n");
            return -1;
        }
        
        memset(&serv_addr, '0', sizeof(serv_addr));
        
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);
        
        if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0) {
            printf("\n inet_pton error occured. ip = %s port = %d\n", ip, port);
            return -1;
        }
        
        /* Information like IP address of the remote host and its port is
         * bundled up in a structure and a call to function connect() is made
         * which tries to connect this socket with the socket (IP address and port)
         * of the remote host
         */
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            printf("\n Error : Connect Failed \n");
            return -1;
        }
        
#ifdef TENHOUPORT
        // tv.tv_sec = 600;
#else
        // TIMEOUT
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
#endif
        
        /* Once the sockets are connected, the server sends the data (date+time)
         * on clients socket through clients socket descriptor and client can read
         * it
         * through normal read call on the its socket descriptor.
         */
        
        char sendBuff[65535];
        
        time_t tReserve = time(NULL);
        
        while (true) {
            int n = 0;
            if ((n = read(sockfd, recvBuff, sizeof(recvBuff) - 1)) > 0) {
                recvBuff[n] = 0;
#ifndef RELEASE_BUILD
                cout << "read size = " << n << endl;
#endif
                if (fputs(recvBuff, stdout) == EOF) {
                    printf("\n Error : Fputs error\n");
                }
#ifdef TENHOUPORT
                if (strcmp(recvBuff, "health check\n") == 0) {
                    
                    if (difftime(time(NULL), tReserve) >= 15.0f * 60.0f) {
                        printf("recv health check after 15min no request from Server\n");
                        shutdown(sockfd, 2);
                        return -1;
                    }
                    
                    send(sockfd, "health check\n", strlen("health check\n"),
                         MSG_DONTWAIT);
                    
                    continue;
                }
#endif
                
                if (strlen(recvBuff) < strlen("{type:none}")) {
#ifndef RELEASE_BUILD
                    printf("json data deha nai\n");
#endif
                    continue;
                }
                // save last time when recive json
                tReserve = time(NULL);
                
                std::string s = game->updateStateAndRespondToAction(recvBuff, false);
                
                sprintf(sendBuff, "%s", s.c_str());
                
                int sendsize = send(sockfd, sendBuff, strlen(sendBuff), MSG_DONTWAIT);
#ifndef RELEASE_BUILD
                cout << "sendsize = " << sendsize << endl;
#endif
                if (sendsize > 0) {
#ifndef RELEASE_BUILD
                    
                    printf("\n send : %s : sendsize  : %d strlen : %zu\n", sendBuff,
                           sendsize, strlen(sendBuff));
#endif
                    memset(recvBuff, '0', sizeof(recvBuff));
                } else {
                    printf("\n socket send error (%d) char : %s strlen : %zuu", sendsize,
                           sendBuff, strlen(sendBuff));
                    
                    break;
                }
            } else {
                printf("socket read timeout\n");
                break;
            }
        }
        
        shutdown(sockfd, 2); // 送受信を無効にする
        sleep(1);
        
        return 0;
    }
};

#endif
