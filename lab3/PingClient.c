#include <stdlib.h> // provides functions such as exit() which are usually given by sys
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// for various socket utility types and functions
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define MAX 200
//#define PORT 8080
#define MAXLINE 1024


void error(const char *);
int main(int argc, char *argv[])
{
   int counter;
   counter = 0; 
   int timeouts;
   timeouts = 0; 
   int sock, n;
   unsigned int length;
   struct sockaddr_in server, from;
   struct hostent *hp;
   char buffer[128];
   
   if (argc != 3) { printf("Usage: server port\n");
                    exit(1);
   }
   sock= socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("socket");

   server.sin_family = AF_INET;
   hp = gethostbyname(argv[1]);
   if (hp==0) error("Unknown host");

   bcopy((char *)hp->h_addr, 
        (char *)&server.sin_addr,
         hp->h_length);
   server.sin_port = htons(atoi(argv[2]));
   length=sizeof(struct sockaddr_in);
   //printf("Please enter the message: ");
   //bzero(buffer,128);
   //fgets(buffer,127,stdin);
   //strcpy(buffer, "PING");
   float average;
   average = 0; 
   float min;
   min = 1; 
   float max;
   max = 0;  
   for (int i = 0; i < 10; i++) {
	bzero(buffer,128);
	strcpy(buffer, "PING ");
	//printf("%s\n", buffer);
	char append1[5];
	sprintf(append1, "%d ", (i + 1));
	strcat(buffer, append1); 
	time_t s, val = 1; 
    	struct tm* current_time; 
  
    	// time in seconds 
    	s = time(NULL); 
  
    	// to get current time 
    	current_time = localtime(&s); 
  
    	// print time in minutes, 
    	// hours and seconds 
    	char time[15];
	sprintf(time, "%02d:%02d:%02d", 
           current_time->tm_hour, 
           current_time->tm_min, 
           current_time->tm_sec);
	strcat(buffer, time); 
	//printf("%s\n", buffer);
	bool timeout;
	timeout = false;
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 999999;
	if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {
    		perror("Error");
	}
	char result[128];
	struct timeval start,end;
	n=sendto(sock,buffer,
        strlen(buffer),0,(const struct sockaddr *)&server,length);
   	if (n < 0) error("Sendto");
	gettimeofday(&start, NULL);
	if(recvfrom(sock, result,128, 0,
       (struct sockaddr *) &from, &length) < 0){
    		//timeout reached
   	 	//printf("%s\n", "Timout reached");
		timeouts++;
		timeout = true;
	}
	gettimeofday(&end, NULL);
	//long seconds = (end.tv_sec - start.tv_sec);
	//printf("%lld", seconds);
	long micros = (end.tv_usec) - (start.tv_usec);
	//printf("%lld\n", micros);
	long wait = 1000000 - micros;
	//printf("%lld\n", wait);
	if (timeout != true) {
		usleep(wait);
	}
	//printf("%lld", micros);
	//printf("%s", buffer);
	//n = recvfrom(sock,result,128,0,(struct sockaddr *)&from, &length);
   	//printf("%s\n", "receipt successful");
	//if (n < 0) error("recvfrom");
	// write(1,"Got an ack: ",12);
   	//printf("%s\n", "pastclock");
	//if (n > 0) {
	//	printf(result);
   	//}
	//else {
	//	printf("%s\n", "did not get anything from server");
	//}
	//write(1,buffer,n);
	if (timeout == true) {
		micros = 1000000;
	}
	else {
		counter++;
	}
	float output;
	float divisor;
	divisor = 1000;
	output = micros/divisor;
   	if (timeout != true) {
		printf("PING received from %s: seq#=%d time=%.3f ms\n", argv[1], (i+1), (output));
   	}
	else {
		printf("Request timeout for icmp_seq %d\n", (i+1));
	}
	if (timeout != true) { 
		average = (average*(counter - 1) + output)/(counter);
	}
	//printf("%.3f\n", average);
   	if ((output > max) & (timeout != true)) {
		max = output;
	}
	if ((output < min) & (timeout != true)) {
		min = output;
	}
	//printf("%.3f\n", min);
	//printf("%.3f\n", max);
   }
   close(sock);
   //printf("%.3f", average);
   printf("--- ping statistics --- 10 packages transmitted, %d received, %lld%% packet loss rtt min/avg/max = %.3f %.3f %.3f ms\n", (10 - timeouts), timeouts*10, min, average, max); 
   //printf("%d\n", timeouts);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

 

