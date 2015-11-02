


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <const.h>

main(){
	int listenerS, clientS;
	int rec, length;
	char buf[BUF_SIZE];
	struct sockaddr_in server;

	listenerS = socket(AF_INET, SOCK_STREAM, 0);
	
	if(listenerS < 0){
		printf("Socket konnte nicht erstellt werden.");
		exit(1);
	}

	//IPv4
	server.sin_family = AF_INET;
	//alle Verbindungen akzeptieren
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);
	//socket binden
 


	if(bind(listenerS, (struct sockaddr*) &server, sizeof(struct sockaddr_in))){    // Socket-Deskr, Addr&Port
		perror("Socket konnte nicht an Adresse gebunden werden.");                  // Größe
		exit(1);
	}

    printf("\nServer laufi laufi... \n");
    
	listen(listenerS, MAX_QUEUE);
	//Verbindungsanfrage akzeptieren
	clientS = accept(listenerS, 0, 0);     
	
	if(clientS < 0){
		perror("Client konnte nicht verbunden werden.");
		exit(1);
	}
	//Daten empfangen
	while(rec = read(clientS, buf, BUF_SIZE)){
		if(rec < 0){
			perror("Fehler beim Empfangen von Daten.");
			exit(1);
		}
		else{
			printf("Versende: %s", buf);
			send(clientS, buf, rec, 0);
		}
	}
	printf("Verbindung wurde beendet.");
	close(listenerS);
	close(clientS);
}
