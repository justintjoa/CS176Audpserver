/* This is taken directly from discussion slides.
All code within the horizontal lines is my own work. */

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#define MAX 200  
//#define PORT     8080 
#define MAXLINE 1024 


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
   srand(time(NULL));
   int sock, length, n;
   socklen_t fromlen;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buffer[128];

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }
   
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("binding");
   fromlen = sizeof(struct sockaddr_in);
   int counter;
   counter = 0; 
   while (1) {
	//printf("%s\n", "entering function");
	if (counter >= 10) {
		return 0;
	}
       n = recvfrom(sock,buffer,128,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("recvfrom");
       printf("%s", buffer);
	//write(1,"Received a datagram: ",21);
       //write(1,buffer,n);

       // -------------------------------
	/*
      int sum = 0;
           char totalbuf[10][15];
           char output[128];
           // Alright, so here's where we're gonna calculate the sum
           int iter = 0;
           do {
               sum = 0;
               for (int i = 0; i < 128; i++){
                  //printf("i is %i\n",i);
                  if (buffer[i] == '\n' || buffer[i] == '\0'){
                      //printf("Loop was null.\n");
                      break;
                  } else if (isdigit(buffer[i])){
                      //printf("Current number is: %i\n", buffer[i] - '0');
                      sum += (buffer[i] - '0');
                      //printf("Sum is now %i\n",sum);
                      //printf("Sum is now %i",sum);
                  } else {
                      //printf("Now trying to write.\n");
                      //n = write(newsockfd,"Sorry, cannot compute!\n",23);
                      n = sendto(sock, "Sorry, cannot compute!\n", 23, 0, (struct sockaddr *)&from, fromlen);
                  }
                  //printf("Loop finished\n");

               }
               //printf("For loop completed.\n");
               //printf("\n\nSum = %i\n\n", sum);
               // final = final + sum + "\n";
               // char temp_buff[15];
               bzero(buffer, 128);
               char writebuf[15];
               // char* curr_sum;
               unsigned int len = sprintf(writebuf, "%i", sum);
               sprintf(buffer, "%d", sum);
               int len2 = sprintf(totalbuf[iter], "%i", sum);
               totalbuf[iter][len2] = '\n';
               // sprintf(totalbuf + iter*5, "%i", sum);
               // printf("WRITEBUF: %s", writebuf);
               // strcpy(buffer, temp_buff);
               // strncpy(buffer, curr_sum, strlen(curr_sum) + 1);
               writebuf[len] = ' ';
               // writebuf[len]
               // n = write(newsockfd, writebuf, len + 1);
               if (n < 0) error ("ERROR writing to socket");
               iter++;

           } while (sum > 9);
           int counter = 0;
           for (int i = 0; i < iter; i++){
              for (int j = 0; j < 15; j++){

                  output[counter]=totalbuf[i][j];
                  counter++;
                  if (totalbuf[i][j] == '\n'){

                      j = 15;
                  }
              }
           }
           output[counter] = '\0';
        */ int randomnumber;
	randomnumber = (rand() % 10) + 1;
	printf("%d\n", randomnumber);
	//randomnumber = 2;
	if (randomnumber < 4) {
		counter++;
		continue;
	}
	else {  
		n = sendto(sock, buffer, 127, 0, (struct sockaddr *)&from, fromlen);
       		printf("%s\n", "went OK");
	}
	   // n = write(newsockfd, output, 127);

      // -----------------------------------

       //n = sendto(sock,"Got your message\n",17,
        //          0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("sendto");
	counter++;
   }
   return 0;
 }


