//CLIENT

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <const.h>


main(int argc, char *argv[]){
	int sock, ret;
	char buf[BUF_SIZE];
	struct sockaddr_in server;
	struct hostent *hostAddress;
   
    //Prüfen des Argument Counters auf ungleich 2
	if(argc != 2){
    printf(stderr, "usage: client <hostname>\n");
	exit(2);
	}

	//Socket erstellen , ipv4, sock stream und 0 = tcp
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0){  
        perror("Fehler beim Anfordern eines Sockets.");
		exit(1);
	}

	server.sin_family = AF_INET;                    // Alle Adresse von allen Netzen dürfen zugreifen
	hostAddress = gethostbyname(argv[1]);           // Übernehmen der Adresse der Eingabe

    printf("\nVerbinde zu %s..\n" , argv[1]);

        if(hostAddress == NULL){                                    // Prüfen auf Programmfehler -> Erste if?
		printf(stderr, "Host %s nicht gefunden.\n", argv[1]);
		exit(2);
	}

	bcopy(hostAddress->h_addr, &server.sin_addr, hostAddress->h_length);    // Stellt Verbindung her
	server.sin_port = htons(PORT);                                          // Setzen des vordefinierten Ports
    
	if(connect(sock,(struct sockaddr*) &server, sizeof(struct sockaddr_in)) < 0){       // Client will Verbindung zum Serversocket herstellen  
    perror("Verbindung fehlgeschlagen.. Server down?");                                 // -1 = Fehlschlag
		exit(1);
	}

	while(ret = read(stdin, buf, BUF_SIZE)){

		if(ret < 0){
			perror("Fehler bei der Eingabe.");
			exit(1);
		}

	    //Senden der Eingabe Flag = 0
		if(send(sock, buf, ret, 0) < 0){
			perror("Fehler beim Senden.");
			exit(1);
		}       

		//Empfangen der Daten
		ret = recv(sock, buf, BUF_SIZE, 0);
		if(ret < 0){
			perror("Fehler beim Empfangen.");
			exit(1);
		}
		printf("Antwort: %s", buf);
	}
	close(sock);
}
