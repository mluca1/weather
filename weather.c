/*
    Matt Luca 
    Date: 5/15/2019 @ 8:06pm
    Subject: C Programming
    Lab 12: Weather.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <libsocket/libinetsocket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

char foo [50];
float ftemp;
char *start;

/*
   URL has 3 parts, the protocol, host, and path.
   Protocol: Determines port we connect to
   HTTP: port is 80
   The HOST and the PATH below correspond to the URL.
*/

/*

 http://api.aerisapi.com/observations/ZIPCODE?client_id=CLIENT_ID&client_secret=CLIENT_SECRET
 CLIENT_ID: kk1CU9UdOTJ050z63ZuDh
 CLIENT_SECRET: 1y2kRweAUVgYoanjcWypsGBfbHPezYTOyQMDARXK
 
*/

#define CLIENT_ID = kk1CU9UdOTJ050z63ZuDh;
#define CLIENT_SECRET = rUOW0GEyf5bT9JhUzro2WQAuUpj3A7nFHgVCRGEK;

const int PORT = 80;

const char HOST [] = "api.aerisapi.com";
const char PATH [] = "/api/kk1CU9UdOTJ050z63ZuDh/conditions/q/";


/*
   NOTE: This program uses "hard-wird" values for PORT,HOST, and PATH.
   For a program that needs to request a document depending on what the user needs you'll,
   want to put them into variables.
*/

char * parse( char *str );


int main (int argc, char **argv) {

   if ( argc != 2 )
   {
       printf("ERROR ZIPCODE not present\n");
       printf("usage ./weather ZIPCODE\n");
       return 0;
   }

   struct sockaddr_in sa;

   int sockfd;

   printf("\nStarting..\n");
   printf("Looking up IP address for %s\n", HOST);
   printf("1");
    

   struct hostent *he = gethostbyname(HOST);

   printf("1");

   struct in_addr *ip = (struct in_addr *) he->h_addr_list[0];

   printf("2");
   printf("IP address is: %s\n", inet_ntoa(*ip));
   printf("Creating structure...\n");
   

   sa.sin_family = AF_INET;
   sa.sin_port = htons(PORT); //PORT 80 for http
   sa.sin_addr = *((struct in_addr *)ip);
  
  
   printf("Creating socket...\n");

   sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

   if (sockfd == -1) {
            fprintf(stderr, "Can't create socket.\n");
            exit(3);
   }  


            printf("Connecting...\n");

   int res = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa));
        if (res ==-1) {
           fprintf(stderr, "Can't Connect.\n");
           exit(2);
        }

   //Once connected, convert the sockfd into a FILE *
        //that we can read/write to.

   FILE *sock = fdopen (sockfd, "r+");
  
   //Now we can use the fprintf, fgets, fscanf, etc. to interact
        //with the server.


            printf("Sending GET\n");

   char NEW_PATH[128]="";
   strcat(NEW_PATH, PATH );
   strcat(NEW_PATH, argv[1] );
   strcat(NEW_PATH, ".json" );

   fprintf(sock, "GET %s HTTP/1.0\n", NEW_PATH);
   fprintf(sock, "Host: %s\n", HOST);
   fprintf(sock, "\n");
  
       
       printf("Recieving response...\n\n");

   char buf[1000];
   char wind_dir[10]="";

   int location_set = 1 ;
   int observation_time_set = 1 ;
   int wind_set = 1 ;

   while ((fgets(buf, 1000, sock))!=NULL) {

          
               
               //printf("%s", buf);

           if (strstr(buf, "wind_dir") && wind_set)                      
           {
               strcpy(wind_dir, parse(buf)) ;
               wind_set = 0;
            }

           if (strstr(buf, "full" ) && location_set)  
           {
               printf("Location : %s", parse(buf));
               printf("\n");

               location_set = 0;
           }

           if (strstr(buf, "observation_time") && observation_time_set)
           {
               printf("Observation Time : %s", parse(buf));
               printf("\n");

               observation_time_set=0;
           }

           if ( strstr( buf, "temp_f"))
           {
               printf("Temperature : %s", parse(buf));  
               printf("\n");
           }

           if ( strstr(buf, "wind_mph" ))
           {
               printf("Wind : %s %s mph",wind_dir,parse( buf));  
               printf("\n\n");
           }

           if ( strstr(buf, "relative_humidity"))
           {
               printf("Humidity : %s", parse(buf));  
               printf("\n");
           }
   }
    
    printf("Forcast:\n");
    printf("Coming soon!\n");
    printf("Coming soon!\n");
    printf("Coming soon!\n");
    printf("Coming soon!\n");
  
   //close the connection
   fclose(sock);
   close(sockfd);
}


char * parse(char *str)
{
   // Breaking string based on :
   while (*str++ != ':');

   // Removed the New line char from the end
   str[strlen(str) - 1] = '\0';

        // Removed the comma char from the end
        str[strlen(str) - 1] = '\0';

   // removing quotes from the beginning and comma/quotes from end if it is there
   if (*str == '"')
   {
       ++str;

       // Removed the quote char from the end
       str[strlen(str) - 1] = '\0';

       return str;
   }

   return str;
}