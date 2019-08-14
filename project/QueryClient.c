/*
 * Modified by Dan Williams 8/12/2019
 * Inspired by: Beej's Guide to Network Programming
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "MovieIndex.h"
#include "Movie.h"
#include "MovieReport.h"

#include "QueryProtocol.h"
#define BUFFER_SIZE 1000

char *port_string = "1500";
unsigned short int port;
char *ip = "127.0.0.1";
int yes = 1;
int responses = 0;
char movie[BUFFER_SIZE];
char goodbye[BUFFER_SIZE];

void RunQuery(char *query) {
  int sockfd;

  if (strlen(query) > BUFFER_SIZE) {
    printf("Query length exceeded.");
    return;
  }

  // Fill an addrinfo struck for the client given the information provided
  struct addrinfo hints;
  struct addrinfo *servinfo;
  int info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Find the address
  if ((info = getaddrinfo(ip, port_string, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
    return;
  }

  // Create the socket
  sockfd = socket(
          servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  // Bind the socket
  bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  if (servinfo == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                 sizeof yes) == -1) {  // If socket already in use
    perror("setsockopt");
    freeaddrinfo(servinfo);
    return;
  }
  // Connect to the server
  if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
    perror("connect");
    freeaddrinfo(servinfo);
    return;  // Failed to connect to server
  }
  char ack[100];
  int len = read(sockfd, ack, BUFFER_SIZE - 1);
  ack[len] = '\0';
  if (CheckAck(ack) != 0) {
    return;
  }
  // Do the query-protocol
  write(sockfd, query, strlen(query));
  read(sockfd, (void *) &responses, sizeof(int));
  SendAck(sockfd);
  Index movie_results = CreateIndex();  // Create index for results

  LinkedList ll = CreateLinkedList();

  for (int i = 0; i < responses; i++) {
    int sen = read(sockfd, movie, BUFFER_SIZE);
    movie[sen] = '\0';
    Movie * currentMovie = CreateMovieFromRow(movie);
    if (AddMovieToIndex(movie_results, currentMovie, Year) != 0) {
      return;
    }
    InsertLinkedList(ll, currentMovie);

    // DestroyMovie(currentMovie);
    SendAck(sockfd);
  }

  if (responses > 0) {
    PrintReport(movie_results);
  }
  DestroyIndex(movie_results);
  DestroyLinkedList(ll, DestroyMovieWrapper);
  // Wait for GOODBYE
  len = read(sockfd, movie, BUFFER_SIZE);
  movie[len] = '\0';
  if (CheckGoodbye(movie) != 0) {
    printf("No goodbye? \n");
    return;
  }
  printf("Goodbye received. \n");

  // Close the connection
  freeaddrinfo(servinfo);
  close(sockfd);
  printf("Socket Closed. \n");
}

void RunPrompt() {
  char input[BUFFER_SIZE];

  while (1) {
    printf("Enter a term to search for, or q to quit: ");
    scanf("%s", input);

    printf("input was: %s\n", input);

    if (strlen(input) == 1) {
      if (input[0] == 'q') {
        printf("Thanks for playing! \n");
        return;
      }
    }
    printf("\n\n");
    RunQuery(input);
  }
}

// This function connects to the given IP/port to ensure
// that it is up and running, before accepting queries from users.
// Returns 0 if can't connect; 1 if can.
int CheckIpAddress(char *ip, char *port) {
  // Fill an addrinfo struck for the client given the information provided
  struct addrinfo hints;
  struct addrinfo *servinfo;
  int sockfd;
  int info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Find the address
  if ((info = getaddrinfo(ip, port_string, &hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
    return 0;
  }

  // Create the socket
  sockfd = socket(
          servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);

  // Bind the socket
  bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
  if (servinfo == NULL) {
    fprintf(stderr, "listener: failed to bind socket\n");
    return 0;
  }

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                 sizeof yes) == -1) {  // If socket already in use
    perror("setsockopt");
    freeaddrinfo(servinfo);
    return 0;
  }
  // Connect to the server
  if (connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
    perror("connect");
    freeaddrinfo(servinfo);
    return 0;  // Failed to connect to server
  }
  // Wait for ACK
  char ack[BUFFER_SIZE];
  int len = read(sockfd, ack, BUFFER_SIZE);
  ack[len] = '\0';
  if (CheckAck(ack) != 0) {
    return -1;
  }
  SendGoodbye(sockfd);
  freeaddrinfo(servinfo);
  close(sockfd);
  return 1;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Incorrect number of arguments. \n");
    printf("Correct usage: ./queryclient [IP] [port]\n");
  } else {
      ip = argv[1];
      port_string = argv[2];
  }
  if (CheckIpAddress(ip, port_string)) {
    RunPrompt();
  }
}

