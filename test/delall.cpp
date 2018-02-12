#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>

#define MAXBUF 1024*4096


/*
	./Client 127.0.0.1 7838
*/
#pragma pack(1)
typedef struct
{
	unsigned int head;
	unsigned int dIpAdrs;
	unsigned int sIpAdrs;
	unsigned int version;
	unsigned int invoke;
	unsigned int dialog;
	unsigned int seq;
	unsigned int length;
	unsigned char* pData;
} NIF_MSG_UNIT2;

typedef struct {
    char mdn[32];
} DelUser;

typedef struct {
	char cd[32];
	char cg[32];
	char smsCode;
	unsigned int content_len;
	unsigned char content[256];
} MO;
#pragma pack()


void print_hex(char* p_msg, int length)
{
	short i, j, len;
	char buf[2048];
	j   = 0;
	len = 0;
	memset(buf,0,2048);
	for (i = 0; (i<length)&&(i<512); i++)
	{
		sprintf(&buf[len], "%.2X ", *(p_msg + i));
		len += 3;
		j++;
		if (j >= 16)
		{
			buf[len] = '\r';
			len++;
			buf[len]= '\n';
			len++;
			j = 0;
		}
	}

	buf[len] = '\r';
	buf[len + 1] = '\n';
	buf[len + 2] = '\r';
	buf[len + 3] = '\n';
	buf[len + 4] = 0;

	printf("%s",buf);
}



int main(int argc, char **argv)
{
	int sockfd, len;
	struct sockaddr_in dest;
	char buffer[MAXBUF + 1];
	if (argc != 4) 
	{
		printf("args error! correct usage is��\n\t\t%s ip  port  filename\n\tfor example:\t%s 127.0.0.1 80 filename\nthis program\
			receive from ip port %d Bytes for most", argv[0], argv[0], MAXBUF);
		exit(0);
	}
	
	/* create a socket for tcp */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		perror("Socket");
		exit(errno);
	}
	
	printf("socket created\n");
	/* initialize peer ip and port */
	bzero(&dest, sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) 
	{
		perror(argv[1]);
		exit(errno);
	}
	
	printf("address created\n");
	/* ���ӷ����� */
	if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0)
	{
		perror("Connect ");
		exit(errno);
	}
	printf("server connected\n");

	std::ifstream ifs (argv[3]);
	if (!ifs.is_open ())
	{
	        printf("open file %s failed", argv[3]);
	        return false;
	}

	std::string str;

        while (std::getline (ifs, str))
        {
        	static unsigned int co=0; 
		co++;
		bzero(buffer, MAXBUF + 1);
	
		NIF_MSG_UNIT2* testMsg=(NIF_MSG_UNIT2*)buffer;
		DelUser user;
		//bzero(buffer, MAXBUF + 1);
		memset(user.mdn,0,sizeof(user.mdn));
		strcpy(user.mdn, str.c_str());

		printf("sizeof(msg_body) is %u\n", sizeof(user));
		printf("sizeof(NIF_MSG_UNIT2) is %u\n", sizeof(NIF_MSG_UNIT2));
		printf("sizeof(unsigned char *) is %u\n", sizeof(unsigned char *));
		
		testMsg->head=htonl(0x1a2b3c4d);
		testMsg->dIpAdrs=htonl(0xdddddd);
		testMsg->sIpAdrs=htonl(0xaaaaaa);
		testMsg->version=htonl(0x1);
		testMsg->invoke=htonl(0XEEEEEE02);
		testMsg->dialog=htonl(0x3);
		testMsg->seq=htonl(0x123456);
		testMsg->length=htonl(sizeof(user));
		char* p_user =(char*)&user;
		memcpy(buffer+sizeof(NIF_MSG_UNIT2)-8, p_user, sizeof(DelUser));

		if(100==co){
			usleep(100);
		}

		/* ����Ϣ�������� */

		len = send(sockfd, buffer, sizeof(NIF_MSG_UNIT2)-8+sizeof(user), 0);
		if(len < 0) {
			printf("send fail!  error code is %d,  error info is '%s'\n", errno, strerror(errno));
		}
		else{
			printf("send success,  sent %d Bytes  \n", len);
			print_hex(buffer, len);
		}
       }
		
	sleep(6);	
		
	/* �ر����� */
	close(sockfd);

	return 0;
}
