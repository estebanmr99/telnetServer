#ifndef MAIN_h

#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <regex.h>

#define PORT 9666
#define BUFFER_SIZE 1024

// Struct para las lista enlazada doble para mantener trackeo de los hilos o procesos creados
typedef struct node {
   pthread_t thread;
   pid_t pid;
   int key;
   struct node *next;
   struct node *prev;
} node;

pthread_mutex_t listMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t serverThread;
int killFlag;
Server server;
struct node *headL = NULL; // Puntero a la cabeza de la lista
struct node *last = NULL; // Puntero a la cola de la lista
struct node *current = NULL;

#endif