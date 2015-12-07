/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Server
 * 
 * user.c: Implementierung der User-Verwaltung
 */

#include "user.h"

static pthread_mutex_t datamut;

static int currentPlayers =0;
static clientTyp players  [MAX_PLAYERS];


int testClientName (char name [])
{
	pthread_mutex_lock (&datamut);    // -------Kritische Sektion-------------------

    printf ("[login] Name wird verglichen.\n");
	int back = 0;
	for (int i=0;i<MAX_PLAYERS;i++)
	{
		if ((strcmp (players[i].nickname, name) == 0)&&(back == 0))
		{
			printf ("[user] %s\n",players [i].nickname);
			printf ("[user] %s\n",name);
			back = -1;
		}
	}

	pthread_mutex_unlock (&datamut); // -----------------------------

    return back;
}

int getClientID ()
{
    pthread_mutex_lock (&datamut);    // -------Kritische Sektion-------------------

    for (int i=0;i<MAX_PLAYERS;i++)
	{
		if(players[i].socket == -1)
		{
			printf("[login] Recently asced Id: %d\n", i);

            pthread_mutex_unlock (&datamut); // -----------------------------
			return i;
		}
	}
	pthread_mutex_unlock (&datamut); // -----------------------------
    return -1;
}

void initialisePlayers()
{
    pthread_mutex_lock (&datamut);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		players[i].socket = -1;
	}
    pthread_mutex_unlock (&datamut);
}

void giveSockets (int currentSockets [MAX_PLAYERS])
{
	clientTyp clients[MAX_PLAYERS];
	getPlayers(clients);
    pthread_mutex_lock (&datamut);
	for (int i=0;i<currentPlayersCount;i++)
	{
		currentSockets[i] = clients[i].socket;
	}
    pthread_mutex_unlock (&datamut);
}