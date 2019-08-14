/*
 * Modified by Dan Williams 8/13/2019
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>


#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "DocIdMap.h"
#include "Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "DirectoryParser.h"
#include "FileCrawler.h"
#include "Util.h"

#define kLog 10
#define kSleep 10
#define BUFFER_SIZE 1000

int Cleanup();

DocIdMap docs;
MovieTitleIndex docIndex;

#define SEARCH_RESULT_LENGTH 1500

char movieSearchResult[SEARCH_RESULT_LENGTH];

void sigchld_handler(int s) {
  write(0, "Handling zombies...\n", 20);
  int saved_errno = errno;

  while (waitpid(-1, NULL, WNOHANG) > 0);

  errno = saved_errno;
}


void sigint_handler(int sig) {
  write(0, "Ahhh! SIGINT!\n", 14);
  Cleanup();
  exit(0);
}

/**
 * Return 0 for successful connection;
 * Return -1 for some error.
 */
int HandleClient(int client_fd, char* query) {
  // Send number of responses
  SearchResultIter iter = FindMovies(docIndex, query);
  int num_search_results;
  if (iter == NULL) {
    num_search_results = 0;
  } else {
    num_search_results = NumResultsInIter(iter);
  }

  write(client_fd, (void*)&num_search_results, sizeof(int));
  // Wait for ACK
  char ack[BUFFER_SIZE];
  int len = read(client_fd, ack, BUFFER_SIZE);
  ack[len] = '\0';
  if (CheckAck(ack) != 0) {
    printf("Ack not received.\n");
    close(client_fd);
    free(query);
    return -1;
  }
  if (num_search_results == 0) {
    printf("No results for your query");
    SendGoodbye(client_fd);
    close(client_fd);
    free(query);
    return 0;
  }

  struct searchResult currentSearchResult;
  int more = 1;

  // For each search result
  do {  // While IterHasMore
    if (!SearchResultIterHasMore(iter)) {
      more = 0;
    }
    // Get and send current search result
    SearchResultGet(iter, &currentSearchResult);
    CopyRowFromFile(&currentSearchResult, docs, movieSearchResult);
    write(client_fd, movieSearchResult, strlen(movieSearchResult));
    // Wait for ACK
    len = read(client_fd, ack, BUFFER_SIZE);
    ack[len] = '\0';
    if (CheckAck(ack) != 0) {
      printf("Ack never received.\n");
      free(query);
      DestroySearchResultIter(iter);
      close(client_fd);
      return -1;
    }
    if (more) {  // If IterHasMore
      SearchResultNext(iter);
    }
  } while (more);
  DestroySearchResultIter(iter);  // Done iterating, destroy iterator
  free(query);
  SendGoodbye(client_fd);
  close(client_fd);
  return 0;
}

int HandleConnections(int sock_fd, int debug) {
  // Step 5: Accept connection
  int socket_fd = accept(sock_fd, NULL, NULL);
  if (socket_fd == -1) {
    perror("Accept failed, connection not handled..\n");
    exit(1);
  }
  // Fork on every connection
  int pid = fork();
  if (pid < 0) {
    perror("Fork failed.\n");
    exit(1);
  }
  if (pid == 0) {
    close(sock_fd);
    if (debug) {
      sleep(kSleep);
    }
    SendAck(socket_fd);
    char search[BUFFER_SIZE];
    int len = read(socket_fd, search, BUFFER_SIZE);  // Listen for query
    search[len] = '\0';

    // Query must be char*
    char *query = (char*)malloc(sizeof(char)*strlen(search) + 1);
    strcpy(query, search);

    // Handle GOODBYE case
    if (CheckGoodbye(query) == 0) {  // If query is GOODBYE close connection
      close(socket_fd);
      free(query);
      Cleanup();
      exit(0);
    }
    HandleClient(socket_fd, query);
    Cleanup();
    exit(0);
  } else {
    // free query?
    close(socket_fd);
  }
  return 0;
}

int Setup(char *dir) {
  struct sigaction sa;

  sa.sa_handler = sigchld_handler;  // reap all dead processes
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  printf("Crawling directory tree starting at: %s\n", dir);
  // Create a DocIdMap
  docs = CreateDocIdMap();
  CrawlFilesToMap(dir, docs);
  printf("Crawled %d files.\n", NumElemsInHashtable(docs));

  // Create the index
  docIndex = CreateMovieTitleIndex();

  if (NumDocsInMap(docs) < 1) {
    printf("No documents found.\n");
    return 0;
  }

  // Index the files
  printf("Parsing and indexing files...\n");
  ParseTheFiles(docs, docIndex);
  printf("%d entries in the index.\n", NumElemsInHashtable(docIndex->ht));
  return NumElemsInHashtable(docIndex->ht);
}

int Cleanup() {
  DestroyMovieTitleIndex(docIndex);
  DestroyDocIdMap(docs);
  return 0;
}

int main(int argc, char **argv) {
  // port
  char *port = NULL;
  char *dir_to_crawl = NULL;

  int debug_flag = 0;
  int c;

  while ((c = getopt (argc, argv, "dp:f:")) != -1) {
    switch (c)
      {
      case 'd':
        debug_flag = 1;
        break;
      case 'p':
        port = optarg;
        break;
      case 'f':
        dir_to_crawl = optarg;
        break;
      }
  }

  if (port == NULL) {
    printf("No port provided; please include with a -p flag.\n");
    exit(0);
  }

  if (dir_to_crawl == NULL) {
    printf("No directory provided; please include with a -f flag.\n");
    exit(0);
  }

  int num_entries = Setup(dir_to_crawl);
  if (num_entries == 0) {
    printf("No entries in index. Quitting. \n");
    exit(0);
  }

  // Step 1: Get address stuff
  struct addrinfo hints;
  struct addrinfo *clientinfo;
  int info;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Find the address
  if ((info = getaddrinfo("localhost", "1500", &hints, &clientinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(info));
    exit(1);
  }

  // Step 2: Open socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Step 3: Bind socket
  if (bind(sock_fd, clientinfo->ai_addr, clientinfo->ai_addrlen) != 0) {
    perror("bind\n");
    freeaddrinfo(clientinfo);
    exit(1);
  }

  // Step 4: Listen on the socket
  if (listen(sock_fd, kLog) != 0) {  // bind() beforehand
    perror("listen()");
    freeaddrinfo(clientinfo);
    exit(1);
  }

  while (1) {
    HandleConnections(sock_fd, debug_flag);
  }
  // Got Kill signal
  close(sock_fd);
  Cleanup();
  return 0;
}

