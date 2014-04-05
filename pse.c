#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<time.h>


void* doprocessing (int sock);
void* timesp(double t1);
//int cnt=0;

int gift=1;

struct s{
	char username[10];
	char password[10];
	int sockno;
	int status;
	char msg[100];
	
};

struct s a[10];
int b[10][10]={{0}};

int main( int argc, char *argv[] )
{
	clock_t t1;
	t1=clock();	
	pthread_t tim;
	pthread_create(&tim,NULL,&timesp,t1);	
    
    int sockfd, newsockfd, portno, clilen;
//   char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
//    int  n;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        perror("ERROR opening socket");
        exit(1);
    }
    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 5001;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
 
    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                          sizeof(serv_addr)) < 0)
    {
         perror("ERROR on binding");
         exit(1);
    }
    /* Now start listening for the clients, here 
     * process will go in sleep mode and will wait 
     * for the incoming connection
     */
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    while (1) 
    {
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        
        printf("%f , %d\n",t1,newsockfd);
        
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }
        pthread_t pts;
        pthread_create(&pts,NULL,&doprocessing,newsockfd);
    }
}


void* doprocessing (int sock)
{
    int n,i;
    char buffer[255],uname[10],pword[10],buffer1[255],buffer3[255],buff[255];
    char buffer2[255]={"From "};
    bzero(buffer,255);
    int count=0;
    n = read(sock,buffer,255);
    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }
    for(i=0;i<10;i++)
    {
//		printf("(%s , %d)",a[i].username,(strcmp(a[i].username,buffer)));
		if(strcmp(a[i].username,buffer)==0)
		{
			printf("%d\n",i);
			
			a[i].sockno=sock;
			n = write(sock,"Enter the password",18);
			break;
		}
	}
    
    puts(a[i].username);
    int z;
    if(i==10)
    {
//		puts(a[0].username);
		printf("%d %d\n",i,sock);
		struct s node;
	    strcpy(uname,buffer);
	    n = write(sock,"Enter the password",18);
	    n = read(sock,buffer,255);
	    strcpy(pword,buffer);
	    
//	    t2=clock();
	    
	    strcpy(node.username,uname);
	    strcpy(node.password,pword);
	    node.sockno=sock;
	    node.status=1;
//	    n = write(sock,"Login Successful",16);
	    for(z=0;z<10;z++)
	    {
			if(a[z].status==1)
			{
				bzero(buffer,255);
				strcpy(buffer,a[z].username);
				n=write(sock,buffer,255);
			}	
		}
	    
	    
	    int cnt=atoi(&uname[5]);
//	    printf("%d\n",cnt);
	    a[cnt]=node;
	    i=cnt;
	    printf("%s %d %d %d\n",a[i].username,i,a[i].status,a[i].sockno);
	 }
	 else
	 {
		n = read(sock,buffer,255);
		if(strcmp(a[i].password,buffer)!=0)
		{
			n=write(sock,"Wrong Password",24);
			pthread_exit(NULL);	
		}
		else
		{
//			t2=clock();
			for(z=0;z<10;z++)
			{
				if(a[z].status==1)
				{
					bzero(buffer,255);
					strcpy(buffer,a[z].username);
					n=write(sock,buffer,255);
				}	
			}
		}	
	}
		
   if(count < 5)
    {
//		printf("%f\n",t2);
//		double spenttime=(double)(t2-t1)/1000000;
//		printf("%f\n",spenttime);
		a[i].status=1;
		char bf[255];
		bzero(bf,255);
		if(gift==1)
		{
			strcpy(bf,"Login Successful and getting a gift......YOUR INBOX IS:");
			strcat(bf,a[i].msg);
			n=write(sock,bf,255);
		}
		else
		{
			strcpy(bf,"Login Successful but no gift......YOUR INBOX IS:");
			strcat(bf,a[i].msg);
			n=write(sock,bf,255);
		}
		while((a[i].status)==1)
		{
			int block=0;
			int j=3,k=0;
			bzero(buffer,255);
			n = read(sock,buffer,255);
			puts(buffer);
			
			if(strcmp(buffer,"Logout")==0 || strcmp(buffer,"logout")==0)
			{
				a[i].status=0;
				n=write(sock,"Logout",6);
				pthread_exit(NULL);
			}
			
			if(strcmp(buffer,"Get users")==0 || strcmp(buffer,"get users")==0)
			{
				do
				{
				for(z=0;z<10;z++)
				{
					if(a[z].status==1 && z!=i)
					{
						puts(a[z].username);
						bzero(buffer1,255);
						strcpy(buffer1,a[z].username);
						puts(buffer1);
						n=write(sock,buffer1,255);
						if(n<0)
						break;
					}	
				}
				bzero(buffer,255);
				n = read(sock,buffer,255);
				}
				while(strcmp(buffer,"Get users")==0 || strcmp(buffer,"get users")==0);
				
				if(strcmp(buffer,"Logout")==0 || strcmp(buffer,"logout")==0)
				{
					a[i].status=0;
					n=write(sock,"Logout",6);
					pthread_exit(NULL);
				}
			}
			char buf2[255];
			bzero(buf2,255);
			char buf3[255];
			bzero(buf3,255);
			
			
			bzero(buffer1,255);
			
			while(buffer[j]!=':')
			{
				buffer1[k]=buffer[j];
				j++;
				k++;
			}
			if(strcmp(buffer1,"server")==0)
			{
				for(k=16;k<strlen(buffer);k++)
				{
					buf2[k-16]=buffer[k];
				}
				puts(buf2);
				int c=atoi(&buf2[5]);
				bzero(buf3,255);
				strcpy(buf3,a[i].username);
				puts(buf3);
				int d=atoi(&buf3[5]);
//				printf("%d,%d",d,c);
				
				b[d][c]=1;
				continue;
				
			}
			
			puts(buffer1);
			int c=atoi(&buffer1[5]);
			bzero(buf3,255);
			strcpy(buf3,a[i].username);
			puts(buf3);
			int d=atoi(&buf3[5]);
			printf("%d,%d\n",d,c);
			
			if(b[c][d]==1)
			continue;
	
			bzero(buff,255);	
			for(k=j;k<strlen(buffer);k++)
			{
				buff[k-j]=buffer[k];
			}
			puts(buff);
//			puts(a[i].username);
			
			bzero(buffer3,255);
			
			sprintf(buffer3,"%s%s%s",buffer2,a[i].username,buff);
			
			puts(buffer3);
//			printf("%s\n",a[0].username);
//			printf("%d\n",(strcmp(a[0].username,buffer1)));	
			
			char buffer4[255];
			
			
			int l=0;
			
			printf("%d,%d\n",l,strlen(buffer1));
			
			while(l<strlen(buffer1))
			{	
				int ct=0;
				bzero(buffer4,255);
				while(buffer1[l]!=',' && l<strlen(buffer1))
				{
					buffer4[ct]=buffer1[l];
					l++;
					ct++;
				}
				l++;
				puts(buffer4);
				
				char b[255];
				bzero(b,255);
				
			for(j=0;j<10;j++)
			{
				if(strcmp(a[j].username,buffer4)==0 && a[j].status==1)
				{
					int temp=a[j].sockno;
					n=write(temp,buffer3,255);
//					printf("%d,%d\n",temp,n);
					break;
				}
				else if(strcmp(a[j].username,buffer4)==0 && a[j].status==0)
				{
					bzero(b,255);
					strcat(a[j].msg,"## ");
					strcat(a[j].msg,buffer3);
					strcpy(b,buffer4);
					strcat(b," is not logged in. Message added to his inbox.");
					n=write(sock,b,255);
					break;
				}
			}
			if(j==10)
			{
				n=write(sock,"User is not regestered",22);
			}
			
			printf("%d\n",j);
		}
			
			if (n < 0)
			{
				perror("ERROR reading from socket");
				exit(1);
			}
			if (n < 0) 
			{
				perror("ERROR writing to socket");
				exit(1);
			}
		}
//		puts(a[i].username);
	}
	return NULL;
}


void* timesp(double t1)
{
	clock_t t2;
	while(1)
	{
		t2=clock();
		double timespent=(double)(t2-t1)/CLOCKS_PER_SEC;
		if(timespent>10)
		{
			gift=0;
			break;
		}
	}
	pthread_exit(NULL);
}
