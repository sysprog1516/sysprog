/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * user.h: Header für die User-Verwaltung
 */

#ifndef USER_H
#define USER_H
#include "common/rfc.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_PLAYERS 4
#define NICKNAME_LENGTH = 32

typedef struct clientInfos
{
	char nickname[NICKNAME_LENGTH];
	int clientID;
	int socket;
	//int score;	
} ClientInfos;

int getClientID();
int testClientName(char name[]);


#endif
