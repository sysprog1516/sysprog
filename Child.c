#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
  const char *program_name = "/bin/wc";
  pid_t forkResult;
  int stdinPipe[2];
  int stdoutPipe[2];
  char readBuffer[1024];
  int readPos = 0;
  int readResult;
  const char *message = "Das ist ein Test!\n";
  const size_t messageLen = strlen(message);

  /* Pipes erzeugen */
  if(pipe(stdinPipe) == -1 || pipe(stdoutPipe) == -1)
  {
    perror("pipe");
    return 3;
  }

  /* Kindprozess abspalten */
  forkResult = fork();
  if(forkResult < 0)		/* Fehlerfall */
  {
    perror("fork");
    return 1;
  }
  else if(forkResult == 0)	/* im Kindprozess */
  {
    /* Umleitung der Standardeingabe */
    if(dup2(stdinPipe[0], STDIN_FILENO) == -1)
    {
      perror("dup2(stdinPipe[0], STDIN_FILENO)");
      return 4;
    }

    /* Umleitung der Standardausgabe */
    if(dup2(stdoutPipe[1], STDOUT_FILENO) == -1)
    {
      perror("dup2(stdoutPipe[1], STDOUT_FILENO)");
      return 5;
    }

    /*** NEU: Schließen aller Pipe-Deskriptoren.
     *   Nach dem exec kennt der Kindprozess diese nicht mehr und spricht
     *   die Pipes selbst über stdin und stdout an. */
    close(stdinPipe[0]); close(stdinPipe[1]);
    close(stdoutPipe[0]); close(stdoutPipe[1]);

    /* Anderes Programm in die vorbereitete Prozessumgebung laden */
    execl(program_name, program_name, NULL);	/* Neues Programm läuft... */
    perror("exec");				/* ...oder auch nicht, dann war's aber ein Fehler */
    return 2;
  }
  else				/* im Elternprozess */
  {
    /*** NEU: Schließen der hier nicht benötigten Enden der Pipes,
     *   also stdinPipe[0] und stdoutPipe[1] */
    close(stdinPipe[0]);
    close(stdoutPipe[1]);

    /* Daten über Schreibende von stdinPipe an Kindprozess schicken */
    if(write(stdinPipe[1], message, messageLen) < messageLen)
    {
      perror("write");
      return 6;
    }

    /* Filedeskriptor schließen, da wc erst am Dateiende mit der Arbeit beginnt */
    close(stdinPipe[1]);

    /* Daten aus dem Leseende von stdoutPipe von Kindprozess lesen */
    while(readPos < sizeof(readBuffer)-1 &&
          (readResult = read(stdoutPipe[0], readBuffer+readPos,
                             sizeof(readBuffer)-1-readPos)) > 0)
    {
      readPos += readResult;
    }

    if(readResult < 0)
    {
      perror("read");
      return 7;
    }

    /* Lesepuffer nullterminieren */
    readBuffer[readPos] = '\0';

    /* Ergebnis ausgeben */
    printf("Ausgabe des Kindprozesses: %s\n", readBuffer);
    return 0;
  }
}