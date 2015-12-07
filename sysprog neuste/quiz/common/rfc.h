/*
 * Systemprogrammierung
 * Multiplayer-Quiz
 *
 * Gemeinsam verwendete Module
 * 
 * rfc.h: Definitionen für das Netzwerkprotokoll gemäß dem RFC
 */

#ifndef RFC_H
#define RFC_H


#ifndef RFC_H
#define RFC_H

#include "common/question.h"
#include <stdint.h>
#include <sys/types.h>



// Message types zur Identifikation
typedef uint8_t MTYPE;

#define MTYPE_UNKNOWN				0
#define MTYPE_LRQ				1
#define MTYPE_LOK				2
#define MTYPE_CRQ				3
#define MTYPE_CRE				4
#define MTYPE_CCH				5
#define MTYPE_LST				6
#define MTYPE_STG				7
#define MTYPE_QRQ				8
#define MTYPE_QUE				9
#define MTYPE_QAN				10
#define MTYPE_QRE				11
#define MTYPE_GOV				12
#define MTYPE_ERR				13


// Error-Subtypes
typedef uint8_t ERR_SUBTYPE;
#define ERR_SUBTYPE_WARNING		0
#define ERR_SUBTYPE_FATAL		1


// Sonstige Konstanten
#define FS_MAX_FILENAME_LENGTH	255


// Message-Strukturen
// ----->
#pragma pack(push, 1)

/* MSG_HEAD */
typedef struct
{
	uint8_t		Type[3];
	uint16_t	Length;
}
MSG_HEAD;


/* MSG_LRQ */
typedef struct
{
	uint8_t		RFCVersion;
	char		Name[31];
} 
MSG_LRQ;


/* MSG_LOK */
typedef struct
{
	uint8_t 	RFCVersion;
	uint8_t 	ClientID;
} 
MSG_LOK;


/* MSG_CRQ */
typedef struct
{
}
MSG_CRQ;


/* MSG_CRE */
typedef struct
{
	char 		Filename[FS_MAX_FILENAME_LENGTH];
}
MSG_CRE;


/* MSG_CCH */
typedef MSG_CRE MSG_CCH;



/* MSG_LST */
typedef struct
{	
	struct player
	{
		char 		name[32];
		uint32_t 	score;
		uint8_t 	PlayerID;
		
	}p[4];
} 
MSG_LST;


/* MSG_STG */
typedef MSG_CRE MSG_STG;


/* MSG_QRQ */
typedef MSG_CRQ MSG_QRQ;


/* MSG_QUE */
typedef struct 
{
	QuestionMessage question;
}
MSG_QUE;


/* MSG_QAN */
typedef struct
{
	uint8_t		Selection;
}
MSG_QAN;


/* MSG_QRE */
typedef struct
{
	uint8_t		TimedOut;
	uint8_t		Correct;
}
MSG_QRE;


/* MSG_GOV */
typedef struct
{
	uint8_t		Rank;
}
MSG_GOV;


/* MSG_ERR */
typedef struct
{
	uint8_t 	Subtype;
	char 		Message[250];
} 
MSG_ERR;


/* MESSAGE */
typedef struct
{
	MSG_HEAD	head;
	union Data
	{
		MSG_LRQ lrq;
		MSG_LOK lok;
		MSG_CRQ crq;
		MSG_CRE cre;
		MSG_CCH cch;
		MSG_LST lst;
		MSG_STG stg;
		MSG_QRQ qrq;
		MSG_QUE que;
		MSG_QAN qan;
		MSG_QRE qre;
		MSG_GOV gov;
		MSG_ERR err;
	} data;
	MTYPE type;
}
MESSAGE;

#pragma pack(pop)
// <-----|



// Prototypen
// ----->

MESSAGE createLRQ( const char* szName );
MESSAGE createLOK( unsigned int client_id );
MESSAGE createCRQ( );
MESSAGE createCRE( const char* filename );
MESSAGE createCCH( const char* filename );
MESSAGE createLST( struct player p_entries[4], int n_players );
MESSAGE createSTG( const char* filename );
MESSAGE createQRQ( );
MESSAGE createQUE( QuestionMessage* question, int isEnd );
MESSAGE createQRE(int isTimedOut, int correctAnswer);

MESSAGE createGOV( int rank );

MESSAGE createQAN(uint8_t selection);



MESSAGE createERR( ERR_SUBTYPE subtype, const char* szMessage );


MTYPE getMessageType(MSG_HEAD header);
MTYPE recvMSG(int tcpsocket, MESSAGE *msg );

char* nullterm(char* source, uint8_t length);

int	sendMSG(int tcpsocket, MESSAGE *msg);

int establishConnectionTo(char ip[], int port);


int broadcast(int sockets[4], MESSAGE *msg);



// <-----|


#endif
