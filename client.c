
//
// by fU9ANg
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <memory.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERV_IP     "127.0.0.1"
#define SERV_PORT    9999 
#define CON_NUM      2000

typedef struct sMsgTest
{
    int iLen;
    char message[128];
}   MSG_TEST;

#define MSG_TEST_LEN sizeof(MSG_TEST)

int exit_flag = 0;

void sig_handler (int sig)
{
    exit_flag = 1;
}

int main(int argc, char* argv[]){

    (void) signal (SIGINT, sig_handler);

    int  cntFd[CON_NUM] = {0};
    int  i = 0;
    long idx = 1;

    for (i = 0; i< CON_NUM; ++i)
    {
        cntFd[i] = socket (AF_INET, SOCK_STREAM, 0);
    }

    struct sockaddr_in addr;
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = inet_addr (SERV_IP);
    addr.sin_port        = htons (SERV_PORT);

    for( i = 0; i< CON_NUM; ++i)
    {
        connect(cntFd[i], (struct sockaddr*)&addr, sizeof (struct sockaddr));
    }

    while (1)
    {
        if (exit_flag == 1) break;
        if (idx == 32768) idx = 1;
        #ifdef _FAST_TEST
            usleep (100);
        #else
            usleep (100000);
        #endif
        printf("Send Test Packet to Server\n");
        MSG_TEST test;

        (void) memset (&test, 0x00, MSG_TEST_LEN);

        test.iLen = MSG_TEST_LEN;
        sprintf (test.message, "DATA(%ld)", idx);

        for( i = 0; i< CON_NUM; ++i)
        {
            send (cntFd[i], &test, test.iLen, 0);
        }

        idx++;
    }

    for (i = 0; i< CON_NUM; ++i)
    {
        close (cntFd[i]);
    }

    return (0);
}
