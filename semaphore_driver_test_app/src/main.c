#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#define BUF_LEN 2
void semaphore_routine();
void set_led(char color, char state);
void INThandler(int);
int msec = 0, triggers[5] = {4000, 2000, 2000, 9000, 1500};
int ret_val;
int file_desc;

int main(int argc, char* argv[]) {
	signal(SIGINT, INThandler);

	/* Clear all lights */
	set_led('G', '0');
	set_led('Y', '0');
	set_led('R', '0');

	/* Read passed arguments */
	for(int i=1;i<argc && i<6;i++) {
		triggers[i-1] = atoi(argv[i]);
	}
	for(int i=0;i<5;i++) {
		printf("%d ", triggers[i]);
		fflush(stdout);
	}


    /* Open device driver file(e.g. sem_driver). */
    file_desc = open("/dev/sem_driver", O_RDWR);
    if(file_desc < 0)
    {
    	printf("Error, 'sem_driver' not opened\n");
    	return -1;
    }

	while(1)
		semaphore_routine();

    /* /1* Read from dummy file. *1/ */
    /* ret_val = read(file_desc, tmp, BUF_LEN); */

    /* Close dummy file. */
    close(file_desc);

	/* Clear all lights */
	set_led('G', '0');
	set_led('Y', '0');
	set_led('R', '0');

    return 0;
}

void semaphore_routine() {
	clock_t before = clock();
	set_led('G', '1');
	set_led('R', '0');
	set_led('Y', '0');
	do {
		/* TODO: Provjera da li je dugme pritisnuto M puta */

		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[0] );

	set_led('G', '2');
	set_led('R', '0');
	set_led('Y', '0');
	before = clock();

	do {
		/* TODO: Provjera da li je dugme pritisnuto M puta */

		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[1] );

	set_led('G', '0');
	set_led('R', '0');
	set_led('Y', '1');
	before = clock();

	do {
		/* TODO: Provjera da li je dugme pritisnuto M puta */

		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[2] );

	set_led('G', '0');
	set_led('R', '1');
	set_led('Y', '0');
	before = clock();

	do {
		/* TODO: Provjera da li je dugme pritisnuto M puta */

		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[3] );

	set_led('G', '0');
	set_led('R', '1');
	set_led('Y', '1');
	before = clock();

	do {
		/* TODO: Provjera da li je dugme pritisnuto M puta */

		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[4] );
}

void set_led(char color, char state) {
    char tmp[BUF_LEN];
	
	tmp[0] = color;
	tmp[1] = state;

	/* Write to sem_driver */
    ret_val = write(file_desc, tmp, BUF_LEN);
}

/* Clear lights upon exiting app */
void INThandler(int sig) {
	signal(sig, SIG_IGN);
	set_led('G', '0');
	set_led('R', '0');
	set_led('Y', '0');
	exit(0);
}
