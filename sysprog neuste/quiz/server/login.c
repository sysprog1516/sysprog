/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * login.c: Implementierung des Logins
 */

#include "login.h"



int acceptConnection(int listenSocket)
{
	printf("listensocket: %i \n", listenSocket);

	int sendsocket = accept(listenSocket, NULL, NULL);


	if( sendsocket < 0 )
	{
		errorPrint("//- WARNING: Incoming connection refused!\n");
		perror("(accept) ERROR");
		return -1;
	}
	else
	{
		return sendsocket;
	}
}


void* loginThreadHandler (void* port)
{
	int loginPort = (*((int*)port));
	pthread_t clientthread [MAX_PLAYERS];
	struct sockaddr_in serverAddr;
	int listensocket , tcpsocket;

	tcpsocket = socket(AF_INET,SOCK_STREAM,0);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(loginPort);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	 //Bind
    if( bind(tcpsocket,(struct sockaddr *)&serverAddr , sizeof(struct serverAddr)) < 0)
    {
        //print the error message
        perror("[server]bind failed. error\n");
        close(tcpsocket);
        return 1;
    }
    puts("[server]bind successfull");

    //Listen
    if( listen( tcpsocket, 3 ) < 0 )
	{
		errorPrint("[server]listen error ");
		close(tcpsocket);
		return -1;
	}
    puts("[server]listening ,waiting for incoming connections...\n");


	listensocket = accept(loginPort,NULL ,NULL);

	printf ("[login] listensocket: %d \n",listensocket);
	if (listensocket == -1)
	{
		printf ("[login]listensocket shitti , try new port.\n");
		exit(0);
	}
	

	initialisePlayers(); 	// Setzt alle Sockets auf -1
	//Warte bis vier Spieler verbunden. Vorher hörst du den Clients zu, guckst erstens ob der Startgame definitiv vom Leiter kommt
		//und ob auch vier Spieler (clientCount?) eingeloggt sind!
	//Dann könnts los gehen. Wenn startgame nachricht an nicht leiter geht, virtuel den Startbutton bei den dreien klicken (optional)

//====================================Los geht's, warten wir auf verbindungsfreudige Clienten==================================
	while (1)
	{
		printf ("[login] ready to connect\n");
		int sendsocket = acceptConnection (listensocket);

		printf ("[login] Sendsocket %d \n",sendsocket);

		printf ("[login] Waiting for message from client \n");
		MESSAGE lrqMsg;
		MTYPE type = 0;

		type = recvMSG (sendsocket, &lrqMsg);

		if (type == MTYPE_LRQ)
		{
		 
			if (1)
			{		
				printf("ok alles klar soweit");
			}
			else
			{
				MESSAGE errMsg = createERR (ERR_SUBTYPE_FATAL,"***[login] Ungültige RFC Version! ( !=8)***\n");
				sendMSG (sendsocket,&errMsg);
				close (sendsocket);
			}
		}
		else
		{
			MESSAGE errMsg;
			if (getID () == -1)
			{
				errMsg = createERR (ERR_SUBTYPE_FATAL,"***[login] Server voll!***\n");
			}
			sendMSG (sendsocket,&errMsg);
			close (sendsocket);
		}
	}
	return NULL;
}
