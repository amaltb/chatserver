#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void* writ(int sock);
void* red(int sock);

int main(int argc,char* argv[])
{
	int sock,n,port_no;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[255];
	int flag=0;
	
	if(argc<3)
	{
		printf("Error,enter hostname and port\n");
		exit(0);
	}
	
	port_no=atoi(argv[2]);
//	char method=argv[3];
	
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock == -1)
	{
		printf("Socket can't be created.Aborting.....");
		return -1;
	}
	
	server=gethostbyname(argv[1]);
	if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char*)&serv_addr,sizeof(serv_addr));
    
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(port_no);
	 bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
	
	if((connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)))<0)
	{
		printf("Can't connect.Aborting......\n");
		exit(0);
	}
	
/*	if(method=='r')
	{
		n=write(sock,"Register",8);
	}
	else if(method=='l')
	{
		n=write(sock,"Login",5);
	}
	
*/	
	printf("Please enter the username:");
	bzero(buffer,255);
	gets(buffer);
	
	n=write(sock,buffer,255);
	if(n<0)
	{
		printf("write error.Aborting......");
		exit(1);
	}
	
	
	
	n=read(sock,buffer,255);
	
//	puts(buffer);
/*	if((strcmp(buffer,"Username already exist",22))
	{
		return 0;
	}
*/	
		
	if(strcmp(buffer,"Enter the password")==0)
	{
		printf("Please enter the password:");
		bzero(buffer,255);
		gets(buffer);
		n=write(sock,buffer,255);		
	}
	bzero(buffer,255);
	n=read(sock,buffer,255);
	puts(buffer);
	if(strcmp(buffer,"Wrong Password")==0)
	{
		return 0;
	}
/*	else if(strcmp(buffer,"Registration complete")==0)
	{
		return 0;
	}

*/	
//	printf("%d\n",(strcmp(buffer,"Login Successful")));
	
//	puts(buffer);
	
	if(strcmp(buffer,"Login Successful")==0)
	flag=1;

	pthread_t pt1,pt2;
	
	pthread_create(&pt2,NULL,&red,sock);
	pthread_create(&pt1,NULL,&writ,sock);
	
	pthread_join(pt1,NULL);
	pthread_join(pt2,NULL);	
	
	return 0;
}


void* writ(int sock)
{
	int n;
	char buffer[255];
	
	while(1)
	{
		bzero(buffer,255);
		gets(buffer);
//		puts(buffer);
		if(strcmp(buffer,"Logout")==0 || strcmp(buffer,"logout")==0)
		{
			break;
		}
//		puts(buffer);
		n=write(sock,buffer,255);
	}
	n=write(sock,buffer,255);
	return NULL;
}


void* red(int sock)
{
	int n;
	char buffer[255];
//	printf("\n");
	while(1)
	{
		bzero(buffer,255);
		n=read(sock,buffer,255);
		if(n>0)
		{
		puts(buffer);
		if(strcmp(buffer,"Logout")==0)
		{
			break;
		}
		}
//		puts(buffer);	
	}
	return NULL;
}
