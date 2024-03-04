#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>

#define BUF_LEN 2
int semaphore_routine();
int set_led(char color, char state);
void INThandler(int);
int msec = 0, triggers[5] = {4000, 2000, 2000, 9000, 1500};
int ret_val;
int sent_val;
int file_desc;
int M;

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
		printf("Ts%d:%ds\n", (i+1), triggers[i]/1000);
		fflush(stdout);
	}

	/* Read M value of driver */
	FILE *fp;
	fp = fopen("/sys/module/gpio_driver/parameters/M", "r");
	if(!fp) {
		printf("Error opening file: %s", strerror(errno));
		fflush(stdout);
		M=15;
		exit(1);
	} else {
		if(fscanf(fp, "%d", &M) != 1)
        	printf("Error reading file.\n");
	}
	fclose(fp);

	while(1)
		semaphore_routine();

	/* Clear all lights */
	set_led('G', '0');
	set_led('Y', '0');
	set_led('R', '0');

    return 0;
}

int semaphore_routine() {
	char string_from_file[20] = "";
	clock_t before = clock();
	set_led('G', '1');
	set_led('R', '0');
	set_led('Y', '0');

	/* Clearing button presses */
    file_desc = open("/dev/gpio_driver", O_RDWR);
    if(file_desc < 0)
    {
    	printf("Error, 'gpio_driver' not opened\n");
    	return -1;
    }
	char tmp[2] = "C";
    ret_val = write(file_desc, tmp, BUF_LEN);
	close(file_desc);

	do {
    	file_desc = open("/dev/gpio_driver", O_RDWR);
    	if(file_desc < 0)
    	{
    		printf("Error, 'gpio_driver' not opened\n");
    		return -1;
    	}

    	sent_val = read(file_desc, string_from_file, 10);

		if((atoi(string_from_file)) == M) {
			char tmp[2] = "C";
    		ret_val = write(file_desc, tmp, BUF_LEN);
			printf("Button was pressed 15 times\n");
			fflush(stdout);
			close(file_desc);
			break;
		}

		close(file_desc);
			
		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[0] );
	memset(string_from_file, 0, sizeof(string_from_file));

	set_led('G', '2');
	set_led('R', '0');
	set_led('Y', '0');
	before = clock();

	do {
		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[1] );

	set_led('G', '0');
	set_led('R', '0');
	set_led('Y', '1');
	before = clock();

	do {
		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[2] );

	set_led('G', '0');
	set_led('R', '1');
	set_led('Y', '0');
	before = clock();

	do {
		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[3] );

	set_led('G', '0');
	set_led('R', '1');
	set_led('Y', '1');
	before = clock();

	do {
		clock_t difference = clock() - before;
	 	msec = difference * 1000 / CLOCKS_PER_SEC;
	} while ( msec < triggers[4] );
	return 1;
}

int set_led(char color, char state) {
    char tmp[BUF_LEN];
	
	tmp[0] = color;
	tmp[1] = state;

	/* Write to gpio_driver */
    file_desc = open("/dev/gpio_driver", O_RDWR);
    if(file_desc < 0)
    {
    	printf("Error, 'gpio_driver' not opened\n");
    	return -1;
    }
    ret_val = write(file_desc, tmp, BUF_LEN);
	close(file_desc);
	return 1;
}

/* Clear lights nad close file upon exiting app */
void INThandler(int sig) {
	signal(sig, SIG_IGN);
	set_led('G', '0');
	set_led('R', '0');
	set_led('Y', '0');
    close(file_desc);
	exit(0);
}
