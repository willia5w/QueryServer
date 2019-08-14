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
#include <signal.h>

#include "DirectoryParser_MT.h"
#include "QueryProtocol.h"
#include "MovieSet.h"
#include "MovieIndex.h"
#include "MovieTitleIndex.h"
#include "DocIdMap.h"
#include "Hashtable.h"
#include "QueryProcessor.h"
#include "FileParser.h"
#include "FileCrawler.h"
#include "DirectoryParser.h"
#include "Util.h"

#define kLog 10
#define kArgs 5
#define BUFFER_SIZE 1000
#define SEARCH_RESULT_LENGTH 1500
char movieSearchResult[SEARCH_RESULT_LENGTH];
char *port_string = "1500";
int Cleanup();

DocIdMap docs;
MovieTitleIndex docIndex;
struct addrinfo *servinfo;

void sigint_handler(int sig) {
  write(0, "Exit signal sent. Cleaning up...\n", 34);
    Cleanup();
  exit(0);
}

int HandleClient(int sock_fd) {
  // Step 5: Accept connection
  int socket_fd = accept(sock_fd, NULL, NULL);
  // Send ACK
  SendAck(socket_fd);
  // Step 6: Read, then write if you want
  char search[BUFFER_SIZE];
  int len = read(socket_fd, search, BUFFER_SIZE);   // Listen for query
  search[len] = '\0';

  char *query = (char*)malloc(sizeof(char)*strlen(search) + 1);
  strcpy(query, search);

  if (CheckGoodbye(query) == 0) {  // If query is GOODBYE close connection
    close(socket_fd);
    free(query);
    return 0;
  }

  SearchResultIter iter = FindMovies(docIndex, query);

  int num_search_results;
  if (iter == NULL) {
    num_search_results = 0;
  } else {
    num_search_results = NumResultsInIter(iter);
  }
  printf("Receiving %d results\n", num_search_results);

  write(socket_fd, (void*)&num_search_results, sizeof(int));
  // Wait for ACK
  char ack[BUFFER_SIZE];
  len = read(socket_fd, ack, BUFFER_SIZE);
  ack[len] = '\0';
  if (CheckAck(ack) != 0) {
    free(query);
    printf("Ack not received.\n");
    close(socket_fd);
    return -1;
  }

  if (num_search_results == 0) {
    SendGoodbye(socket_fd);
    close(socket_fd);
    free(query);
  }

  int more = 1;
  struct searchResult currentSearchResult;  // Get payload

  do {
    if (!SearchResultIterHasMore(iter)) {
      more = 0;
    }
    SearchResultGet(iter, &currentSearchResult);
    CopyRowFromFile(&currentSearchResult, docs, movieSearchResult);
    write(socket_fd, movieSearchResult, strlen(movieSearchResult));
    // Wait for ACK
    len = read(socket_fd, ack, BUFFER_SIZE);
    ack[len] = '\0';
    if (CheckAck(ack) != 0) {
      printf("Ack never received.\n");
      free(query);
      DestroySearchResultIter(iter);
      close(socket_fd);
      return -1;
    }
    if (more) {
      SearchResultNext(iter);
    }
  } while (more);
  // Done iterating
  DestroySearchResultIter(iter);
  free(query);
  SendGoodbye(socket_fd);
  close(socket_fd);
  return 0;
}

int Setup(char *dir) {
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
  freeaddrinfo(servinfo);
  return 0;
}

int main(int argc, char **argv) {
  if (argc != kArgs) {
    printf("Expected \t-f <file_path> -p <port>\n");
    return 1;
  }

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

  // Setup graceful exit
  struct sigaction kill;

  kill.sa_handler = sigint_handler;
  kill.sa_flags = 0;  // or SA_RESTART
  sigemptyset(&kill.sa_mask);

  if (sigaction(SIGINT, &kill, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  int num_entries = Setup(dir_to_crawl);
  if (num_entries == 0) {
    printf("No entries in index. Quitting. \n");
    exit(0);
  }
  int s;
  struct addrinfo hints;
  int sock_fd;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // Find the address
  s = getaddrinfo("localhost", "1500", &hints, &servinfo);
  if (s != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
    exit(1);
  }

  // Create the socket
  sock_fd = socket(AF_INET, SOCK_STREAM, 0);

  // Bind the socket
 if (bind(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen) != 0) {
   perror("bind\n");
   freeaddrinfo(servinfo);
   exit(1);
  }

  // Step 4: Listen on the socket
  if (listen(sock_fd, kLog) != 0) {  // bind() beforehand
    perror("listen()");
    freeaddrinfo(servinfo);
    exit(1);
  }  else {
    while (1) {
      int run = HandleClient(sock_fd);
      if (run != 0) {
        break;
      }
    }
  }

  // Got Kill signal
  close(sock_fd);
  Cleanup();
  return 0;
}

