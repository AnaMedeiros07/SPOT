#include  <sys/syslog.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
#include <fcntl.h>
#include <time.h>
#include <signal.h>
#include "UPSHat.h"
#include "CHB100.h"

int main(int argc, char *argv[]){
	pid_t pid, sid;
	int len, fd;
	const time_t timebuf;

    openlog("SPOT Deamon Test",LOG_PID,LOG_DEAMON);
	pid = fork(); // create a new process

	if (pid < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "fork");
		exit(EXIT_FAILURE);
	}

	if (pid > 0){  
		printf("Deamon PID: %d\n", pid);	
		exit(EXIT_SUCCESS); // parent process (exit)
	}
	sid = setsid(); // create a new session

	if (sid < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "setsid");
		exit(EXIT_FAILURE);
	}
	// make '/' the root directory
	if (chdir("/") < 0) { // on error exit
		syslog(LOG_ERR, "%s\n", "chdir");
		exit(EXIT_FAILURE);
	}
	umask(0); //read and write permission
	close(STDIN_FILENO);  // close standard input file descriptor
	close(STDOUT_FILENO); // close standard output file descriptor
	close(STDERR_FILENO); // close standard error file descriptor


    UPSHat charger;
    CHB100 motion_sensor;
    float voltage;
    float batery;
    float temperature;
    float humidity;
    bool motion;
    bool smoke;

	while (1) {
		voltage = charger.get_voltage();
        batery = charger.batery();
        motion =
        // smoke =
        // temperature =
        // humidity =
		
            // enviar valores por message queu 
		

		sleep(0.5);
	}
exit(EXIT_SUCCESS);		
}
