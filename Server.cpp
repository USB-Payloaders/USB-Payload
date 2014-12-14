#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <strings.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

using namespace std;


void receive(int sock, int len, char* buf);
void send(int sock, char* buffer);

int main()
{
   struct sockaddr_in socketInfo;
   int socketHandle;
   int portNumber = 10000;
   char* buf;
   bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

   if((socketHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      close(socketHandle);
      exit(EXIT_FAILURE);
   }

   // Load system information into socket data structures

   socketInfo.sin_family = AF_INET;
   socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
   socketInfo.sin_port = htons(portNumber);      // Set port number

   // Bind the socket to a local socket address

   if( bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0)
   {
      close(socketHandle);
      perror("bind");
      exit(EXIT_FAILURE);
   }

   listen(socketHandle, 1);
   cout << "Server is running!\n";
   int socketConnection;
   if((socketConnection = accept(socketHandle, NULL, NULL)) < 0)
   {
      exit(EXIT_FAILURE);
   }
   sleep(2);
   receive(socketHandle, 1024, buf);
   free(buf);
   cout << buf;
   close(socketHandle);
   return 0;
}

void receive(int sock, int len, char* buf)
{
    int rc = 0;  // Actual number of bytes read
    buf = (char*) malloc(len);
   // rc is the number of characters returned.
   // Note this is not typical. Typically one would only specify the number
   // of bytes to read a fixed header which would include the number of bytes
   // to read. See "Tips and Best Practices" below.

   rc = recv(sock, buf, len, 0);
   buf[rc]= (char) NULL;
}

