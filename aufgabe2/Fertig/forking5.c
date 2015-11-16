#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>

int main(void){

	
	struct timespec timeout;
	timeout.tv_nsec = 0;
	timeout.tv_sec = 10;

	fd_set set;
	FD_ZERO(&set);

	int ret;
	
	const char *ort_program = "/home/user/c/wait4me";

	pid_t forkResult;
	
	int stdin_Pipe[2];
	int stdout_Pipe[2];

	int readPos = 0;
	int readResult = 0;

	char buffer[1024];

	char uebertragung[1024];
	size_t uebertragungLen;

	if(pipe(stdin_Pipe) == -1 || pipe(stdout_Pipe) == -1){

		perror("pipe");
		return -1;

	}
	forkResult = fork();		//erstellen des kindprozesses

	if(forkResult < 0){

		perror("forking");
		return -1;

	}else if(forkResult == 0){      //kindprozess

		if(dup2(stdin_Pipe[0], STDIN_FILENO) == -1){

			perror("dub2(in)");
			return -1;
		}
		if(dup2(stdout_Pipe[1], STDOUT_FILENO) == -1){

			perror("dub2(out)");
			return -1;
		}

		

		close(stdin_Pipe[0]);
		close(stdin_Pipe[1]);
		close(stdout_Pipe[0]);
		close(stdout_Pipe[1]);
		
		execl(ort_program, ort_program, NULL);
		perror("exec");  

	}else{				//elternprozess
		
		close(stdin_Pipe[0]);
		close(stdout_Pipe[1]);

		printf("Bitte Ziffer eingeben: ");
		scanf("%s", uebertragung);
		uebertragungLen = strlen(uebertragung);
		printf("\n");
 
		FD_SET(stdout_Pipe[0], &set);		
		if(!FD_ISSET(stdout_Pipe[0],&set))
		{
			perror("Filedeskrioper nicht in Menge  enthalten");
			return -1;			
		}
		
		if(write(stdin_Pipe[1],&uebertragung, uebertragungLen) < uebertragungLen){

			perror("write");
			return -1;
		}

		close(stdin_Pipe[1]);

		ret = pselect(stdout_Pipe[0]+1, &set, NULL, NULL, &timeout,NULL);
		//http://linux.die.net/man/2/pselect
		read(stdout_Pipe[0], buffer, sizeof(buffer)-1);

   		if(ret < 0){
      			perror("fehler beim Timeout");
      			return -1;
    		}else if(ret == 0){
      			perror("Timeout: Anwendung wird beendet");
      			return 0;
    		}else{
      			printf("Ausgabe des Kindprozesses: %s\n", buffer);
     			return 1;
    }
    
	}
}
