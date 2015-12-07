/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Gemeinsam verwendete Module
 * 
 * rfc.c: Implementierung der Funktionen zum Senden und Empfangen von
 * Datenpaketen gemäß dem RFC
 */


#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>


#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "rfc.h"
#include "common/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define RFC_VERSION_USED 8




/* createLRQ */
MESSAGE createLRQ( const char* szName )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'L';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'Q';
	
	msg.data.lrq.RFCVersion = RFC_VERSION_USED;
	
	msg.head.Length = strlen(szName);
	memcpy( msg.data.lrq.Name, szName, msg.head.Length );
	
	msg.head.Length += 1;
	
	return msg;
}


/* createLOK */
MESSAGE createLOK( unsigned int client_id )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'L';
	msg.head.Type[1] = 'O';
	msg.head.Type[2] = 'K';
	
	msg.data.lok.RFCVersion = RFC_VERSION_USED;
	
	msg.data.lok.ClientID = client_id;
	
	msg.head.Length = 2;
	
	return msg;
}


/* createCRQ */
MESSAGE createCRQ()
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'C';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'Q';
	
	msg.head.Length = 0;
	
	return msg;
}


/* createCRE */
MESSAGE createCRE( const char* filename )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'C';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'E';
	
	msg.head.Length = strlen(filename);
	if(msg.head.Length > FS_MAX_FILENAME_LENGTH)
		msg.head.Length = FS_MAX_FILENAME_LENGTH;
	
	memcpy(msg.data.cre.Filename, filename, msg.head.Length);
	
	return msg;
}


/* createCCH */
MESSAGE createCCH( const char* filename )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'C';
	msg.head.Type[1] = 'C';
	msg.head.Type[2] = 'H';
	
	msg.head.Length = strlen(filename);
	if(msg.head.Length > FS_MAX_FILENAME_LENGTH)
		msg.head.Length = FS_MAX_FILENAME_LENGTH;
	
	memcpy(msg.data.cch.Filename, filename, msg.head.Length);
	
	return msg;
}


/* createLST */
MESSAGE createLST( struct player p_entries[4], int n_players )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'L';
	msg.head.Type[1] = 'S';
	msg.head.Type[2] = 'T';
	
	msg.head.Length = n_players * 37;
	int i;
	for(i = 0; i < n_players; i++)
	{
		msg.data.lst.p[i] = p_entries[i];
	}
	
	return msg;
}


/* createSTG */
MESSAGE createSTG( const char* filename )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'S';
	msg.head.Type[1] = 'T';
	msg.head.Type[2] = 'G';
	
	msg.head.Length = strlen(filename);
	if(msg.head.Length > FS_MAX_FILENAME_LENGTH)
		msg.head.Length = FS_MAX_FILENAME_LENGTH;
	
	memcpy(msg.data.stg.Filename, filename, msg.head.Length);
	
	return msg;
}


/* createQRQ */
MESSAGE createQRQ()
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'Q';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'Q';
	
	msg.head.Length = 0;
	
	return msg;
}


/* createQUE */

MESSAGE createQUE( QuestionMessage* question, int isEnd )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'Q';
	msg.head.Type[1] = 'U';
	msg.head.Type[2] = 'E';
	
	msg.head.Length = (isEnd) ? (0) : (769);
	
	memcpy(&(msg.data.que.question.question), &(question->question), sizeof(question->question));
	for(int i = 0; i < 4; i++)
	memcpy(&(msg.data.que.question.answers[i]), (&question->answers[i]), sizeof(question->answers[i]));
	msg.data.que.question.timeout = question->timeout;


	return msg;
}




/*createQAN */


MESSAGE createQAN(uint8_t selection)
{
	MESSAGE msg;
	msg.head.Type[0] = 'Q';
	msg.head.Type[1] = 'A';
	msg.head.Type[2] = 'N';
	msg.head.Length = 1;
	msg.data.qan.Selection = selection;
	return msg;	
}


/* createQRE */

MESSAGE createQRE(int isTimedOut, int correctAnswer)
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'Q';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'E';
	
	msg.head.Length = 2;
	
	msg.data.qre.TimedOut = isTimedOut;
	msg.data.qre.Correct = correctAnswer;
	
	return msg;
}


/* createGOV */
MESSAGE createGOV( int rank )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'G';
	msg.head.Type[1] = 'O';
	msg.head.Type[2] = 'V';
	
	msg.head.Length = 1;
	
	msg.data.gov.Rank = rank;
	
	return msg;
}



/* createERR */
MESSAGE createERR( ERR_SUBTYPE subtype, const char* szMessage )
{
	MESSAGE msg;
	
	msg.head.Type[0] = 'E';
	msg.head.Type[1] = 'R';
	msg.head.Type[2] = 'R';
	
	msg.data.err.Subtype = subtype;
	
	msg.head.Length = strlen(szMessage);
	if(msg.head.Length > 250)
		msg.head.Length = 250;
	
	memcpy(msg.data.err.Message, szMessage, msg.head.Length);
	
	msg.head.Length += 1;
	
	return msg;
}


MTYPE getMessageType(MSG_HEAD header)
{
	char lrq[] = "LRQ";
	char lok[] = "LOK";
	char crq[] = "CRQ";
	char cre[] = "CRE";
	char cch[] = "CCH";
	char lst[] = "LST";
	char stg[] = "STG";
	char qrq[] = "QRQ";
	char que[] = "QUE";
	char qan[] = "QAN";
	char qre[] = "QRE";
	char gov[] = "GOV";
	char err[] = "ERR";

	if( !memcmp( &(header.Type), lrq, 3 ) )
		return MTYPE_LRQ;

	if( !memcmp( &(header.Type), lok, 3 ) )
		return MTYPE_LOK;

	if( !memcmp( &(header.Type), crq, 3 ) )
		return MTYPE_CRQ;

	if( !memcmp( &(header.Type), cre, 3 ) )
		return MTYPE_CRE;

	if( !memcmp( &(header.Type), cch, 3 ) )
		return MTYPE_CCH;

	if( !memcmp( &(header.Type), lst, 3 ) )
		return MTYPE_LST;

	if( !memcmp( &(header.Type), stg, 3 ) )
		return MTYPE_STG;

	if( !memcmp( &(header.Type), qrq, 3 ) )
		return MTYPE_QRQ;

	if( !memcmp( &(header.Type), que, 3 ) )
		return MTYPE_QUE;

	if( !memcmp( &(header.Type), qan, 3 ) )
		return MTYPE_QAN;

	if( !memcmp( &(header.Type), qre, 3 ) )
		return MTYPE_QRE;

	if( !memcmp( &(header.Type), gov, 3 ) )
		return MTYPE_GOV;

	if( !memcmp( &(header.Type), err, 3 ) )
		return MTYPE_ERR;

	return MTYPE_UNKNOWN;

}

/**
 * recv a message ...
 * socket and msg ptr must be initialized
 */
MTYPE recvMSG(int tcpsocket, MESSAGE *msg )
{

	// check msg and socket
	if(msg == NULL)
	{
		errorPrint("Failed to recvMSG: uninitialized message pointer!");
		return MTYPE_UNKNOWN;
	}
	else if(tcpsocket < 0)
	{
		errorPrint("Failed to recvMSG: tcpsocket is shit!");
		return MTYPE_UNKNOWN;
	}

	if( recv(tcpsocket, &(msg->head), sizeof(msg->head), 0) <= 0 )
	{
		errorPrint("Failed to receive header data!");
		msg->type = MTYPE_UNKNOWN;
		return MTYPE_UNKNOWN;
	}

	msg->head.Length = ntohs(msg->head.Length);
		
	if(msg->head.Length == 0)
	{
		msg->type = getMessageType(msg->head);
		return msg->type;
	}

	// recv data part

	if( recv(tcpsocket, &(msg->data), msg->head.Length, 0) < 0 )
	{
		errorPrint("Failed to receive message data!");
		msg->type = MTYPE_UNKNOWN;
		return MTYPE_UNKNOWN;
	}
	else
	{

		msg->type = getMessageType(msg->head);
		return msg->type;
	}
}


/**
 * sends a message ...
 * socket and msg ptr must be initialized
 */
int	sendMSG(int tcpsocket, MESSAGE *msg)
{
	// check msg and socket

	if(msg == NULL)
	{
		errorPrint("Failed to sendMSG: uninitialized message pointer!");
		return MTYPE_UNKNOWN;
	}
	else if(tcpsocket < 0)
	{
		errorPrint("Failed to sendMSG: tcpsocket is shit!");
		return MTYPE_UNKNOWN;
	}

	msg->head.Length = htons(msg->head.Length);

	// send message
	if( send(tcpsocket, msg, ntohs(msg->head.Length)+sizeof(msg->head), 0) < 0 )
	{
		errorPrint("Failed to send message!");
		return EXIT_FAILURE;
	}
	msg->head.Length = ntohs(msg->head.Length);

	return EXIT_SUCCESS;
}

/**
 * Returns a tcp (listen)-socket
 */

/**
 * Returns send-socket ;)
 */
int establishConnectionTo(char ip[], int port)
{
	//- Setting up... ")

	int tcpsocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( tcpsocket < 0 )
	{
		errorPrint("establishConnection (socket) ERROR");
		return EXIT_FAILURE;
	}

	struct sockaddr_in tcps_addr;
	tcps_addr.sin_family = AF_INET;
	tcps_addr.sin_port = htons( port );

	if((tcps_addr.sin_addr.s_addr = inet_addr(ip)) == -1)
	{
		errorPrint("(Fehlerhafte IP-Adresse) ERROR");
		return EXIT_FAILURE;
	}


	//- Connecting...

	if( connect(tcpsocket, (const struct sockaddr *)&tcps_addr, sizeof(tcps_addr)) < 0 )
	{
		errorPrint("establishConnection (connect) ERROR");
		printf("\nStop.\n");
		close(tcpsocket);
		return EXIT_FAILURE;
	}
	printf("Connected to %s:%d (TCP)\n", ip, port );


	return tcpsocket;
}



int broadcastTo(int tcpsockets[], int cNumber, MESSAGE *msg )
{
	for(int i = 0; i < cNumber; i++)
	{
		if( sendMSG(tcpsockets[i], msg) == EXIT_FAILURE )
		{
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

//string nullterminieren
char* nullterm(char* source, uint8_t length)
{	
	char* newchar;
	uint16_t i;
	newchar = (char*)malloc(sizeof(char)* (length+1));
	for(i = 0; i < (length); i++)
	{
		newchar[i] = source[i];
	}
	newchar[length] = 0;
	return newchar;
}



int broadcast(int sockets[4], MESSAGE *msg)
{
	printf("Broadcast to all Clients \n");
	for(int i = 0; i < 4; i++)
	{
		if(sockets[i] != -1)
		{
			printf("socket %i \n", sockets[i]);
			if( sendMSG(sockets[i], msg) == EXIT_FAILURE )
				{
					return EXIT_FAILURE;
				}
		}
	}
	return EXIT_SUCCESS;
}


