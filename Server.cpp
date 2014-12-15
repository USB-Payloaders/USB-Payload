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


string receive(int sock, int len);
void send(int sock, string buffer);

int main()
{
   struct sockaddr_in socketInfo;
   int socketHandle;
   int socketConnection;
   int portNumber = 10003;

   bzero(&socketInfo, sizeof(sockaddr_in));  // Clear structure memory

   (socketHandle = socket(AF_INET, SOCK_STREAM, 0)

   socketInfo.sin_family = AF_INET;
   socketInfo.sin_addr.s_addr = htonl(INADDR_ANY); // Use any address available to the system
   socketInfo.sin_port = htons(portNumber);      // Set port number

   if( bind(socketHandle, (struct sockaddr *) &socketInfo, sizeof(socketInfo)) < 0)
   {
      close(socketHandle);
      perror("bind");
      exit(EXIT_FAILURE);
   }

   listen(socketHandle, 1);
   cout << "Server is running!\n";


   if((socketConnection = accept(socketHandle, NULL, NULL)) < 0)
   {
      exit(EXIT_FAILURE);
   }

   string data = receive(socketConnection, 1024);
   send(socketConnection, data);

   close(socketHandle);
   close(socketConnection);

   return 0;
}

string receive(int sock, int len)
{
    int rc = 0;
    char* buf = (char*) malloc(len);
    string answer;
    while (rc <= 0)
     rc = recv(sock, buf, len, 0);
    buf[rc]= (char) NULL;
    answer = buf;
    free(buf);
    return answer;
}

void send(int sock, string buffer)
{
    send(sock, buffer.c_str(), buffer.length() , 0);
}
