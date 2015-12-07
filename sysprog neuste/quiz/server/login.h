/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * login.h: Header für das Login
 */

#ifndef LOGIN_H
#define LOGIN_H
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <signal.h>
#include "common/rfc.h"
#include "clientthread.h"
#include "user.h"
 
int listenandbind(int port);
int acceptConnection(int listenSocket);

void* loginThreadHandler (void*);

#endif
