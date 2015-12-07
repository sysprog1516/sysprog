/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * main.c: Hauptprogramm des Servers
 */
#include "common/util.h"
#include "common/rfc.h"
#include "score.h"
#include "login.h"
#include "clientthread.h"
#include "catalog.h"

#include <sys/types.h>
#include <sys/mman.h>

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <signal.h>

#define		SERVER_DEFAULT_LISTEN_PORT			8111
#define 	MAX_LOADERPATH_LENGTH				255

void starthelp()
{
	infoPrint ("[server] server gestartet              \n");
	infoPrint ("[help] help: -p 12345 <- port          \n");
	infoPrint ("[help] server_init : ./server -p 12345 \n");	
	infoPrint ("[server] gruppe 10\n");
}

void getStartArguments(int argc, char **argv, int *logport)
{
		int command = -1;	
		while ((command = getopt(argc, argv,"p:?")) != -1)
		{		
			switch(command)
			{
				
				case 'p':
				{
					if(optarg)
					{
						//atoi convert char* to int
						*logport = atoi(optarg);	
						printf("[main] angegebener port: %d\n", *logport);
					}

					break;
				}
				case '?':
				{
					printf("[server] port [p] \n");
					exit(1);
					break;
				}				
				default:
				{
					puts("undefined argument");	
					exit(0);			
				}
			 }	
	 }
}


int main(int argc, char **argv)
{
	int loginport = 0;
	pthread_t loginthread;

	starthelp();

	setProgName(argv[0]);
	/* debugEnable() */

	getStartArguments(argc,argv ,&loginport);

	if(loginport == 0)
	{
		loginport = SERVER_DEFAULT_LISTEN_PORT;		
	puts("loginport : %d",loginport);
	}
	

	pthread_create(&loginthread, NULL, loginThreadHandler, &loginport);

    pthread_join (loginthread, NULL);
    puts("[server]server closed\n");
	return 0;
}
