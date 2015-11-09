#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void ){

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
 
		if(write(stdin_Pipe[1],&uebertragung, uebertragungLen) < uebertragungLen){

			perror("write");
			return -1;
		}

		close(stdin_Pipe[1]);

		while(readPos < sizeof(buffer)-1 && (readResult = read(stdout_Pipe[0], buffer+readPos, sizeof(buffer)-1-readPos)) > 0){


			readPos += readResult;
		}			

		if(readResult < 0){

			perror("read");
			return -1;

		}

		buffer[readPos] = '\0';
		printf("\nKindprozess gibt aus: %s\n", buffer);

		return 1;
	}
}
