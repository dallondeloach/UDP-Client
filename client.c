#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  int sd;
  struct sockaddr_in server_address;
  socklen_t fromLength = sizeof(server_address);
  int rc;
  int portNumber;
  char serverIP[29];
  char buffer[18];
  char userInput[50];
  int converted_lenOfString;
  int seqNumber=0;
  char ackBuffer[11];
  

  if (argc < 3){
    printf("usage is client <ipaddr> <port>\n");
  }

  // Create a UDP socket
  sd = socket (AF_INET, SOCK_DGRAM, 0);
  portNumber = atoi(argv[2]);
  strcpy(serverIP, argv[1]);
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(portNumber);
  server_address.sin_addr.s_addr = inet_addr(serverIP);

  // Get string input & Determine length of string
  printf("\n");
  printf("Enter a string: ");
  fgets(userInput, sizeof(userInput), stdin);
  int lenOfString = strlen(userInput)-1;

  // Send length of string to server
  converted_lenOfString = htonl(lenOfString);
  rc = sendto(sd, &converted_lenOfString, sizeof(converted_lenOfString), 0, (struct sockaddr *)&server_address, sizeof(server_address));
  printf("wrote %d bytes to send the user string length\n", rc);
  if (rc<0){
    perror("writing string length");
  }
  
  // Send 2 bytes of data and receive ack
  if(lenOfString % 2 == 0){
    for(int i = 0; i < (lenOfString); i+=2){
      // Create a buffer with sequence number, segment length, and two characters
      memset(buffer, 0, 18);
      sprintf(buffer, "%11d%4d%c%c", seqNumber, 2, userInput[i], userInput[i+1]);
      // Send the buffer to the server
      sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));
      printf("Sent data: %s\n", buffer);
      time_t timeSent;
      timeSent=time(NULL);
      while(1){
        // Receive acknowledgment
        memset(ackBuffer, 0, 11);
        rc=recvfrom(sd, &ackBuffer, 11, MSG_DONTWAIT, (struct sockaddr *)&server_address, &fromLength);
        if(rc>0){
          int receivedAck;
          sscanf(ackBuffer, "%11d", &receivedAck);
          printf("Received Ack: %d\n", receivedAck);
          if(receivedAck==seqNumber){
            break;
          }
        }
        else{
          // Handle timeout and resend buffer
          time_t currentTime;
          currentTime=time(NULL);
          if(currentTime-timeSent>5){
            printf("TIMEOUT: Resending Buffer\n");
            sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));
            timeSent=time(NULL);
          }
        }
      }
      seqNumber++;
    }
  }  
  else if(lenOfString % 2 == 1){
    for(int i = 0; i < (lenOfString-1); i += 2){
      // Create a buffer with sequence number, segment length, and two characters
      memset(buffer, 0, 18);
      sprintf(buffer, "%11d%4d%c%c", seqNumber, 2, userInput[i], userInput[i+1]);
      // Send the buffer to the server
      sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));
      printf("Sent data: %s\n", buffer);
      time_t timeSent;
      timeSent=time(NULL);
      while(1){
        // Receive acknowledgment
        memset(ackBuffer, 0, 11);
        rc=recvfrom(sd, &ackBuffer, 11, MSG_DONTWAIT, (struct sockaddr *)&server_address, &fromLength);
        if(rc>0){
          int receivedAck;
          sscanf(ackBuffer, "%11d", &receivedAck);
          printf("Received Ack: %d\n", receivedAck);
          if(receivedAck==seqNumber){
            break;
          }
        }
        else{
          // Handle timeout and resend buffer
          time_t currentTime;
          currentTime=time(NULL);
          if(currentTime-timeSent>5){
            printf("TIMEOUT: Resending Buffer\n");
            sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));
            timeSent=time(NULL);
          }
        }
      }
      seqNumber++;
    }
    // Handle the last character of the odd-length string
    memset(buffer, 0, sizeof(buffer));
    sprintf(buffer, "%11d%4d%c", seqNumber+1, 2, userInput[lenOfString-1]);
    sendto(sd, buffer, sizeof(buffer), 0, (struct sockaddr *)&server_address, sizeof(server_address));
  }

  // Close the socket
  close(sd);
  return 0;
}
