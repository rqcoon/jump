#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>

void game();
int jump = 0;
int xco = 20;
int yco = 1;
int spike_xco = 255;
int spike2_xco = 322;
int score = 0;

int main() {
	while (1) {
		usleep(53000);
		system("clear");
		game();
	}
	system("clear");
	game();

	return 0;
}

int getch(void)
{
   struct termios oldattr, newattr;
   int ch;
   tcgetattr(0, &oldattr);
   newattr=oldattr;
   newattr.c_lflag &= ~( ICANON | ECHO );
   tcsetattr( 0, TCSANOW, &newattr);
   ch=getchar();
   tcsetattr(0, TCSANOW, &oldattr);
   return(ch);
}


bool kbhit()
{
    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);
    return byteswaiting > 0;
}

int newrand(void) {
	return rand() % 100;
}

void game(){
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	int cols = ts.ws_col;
	int lines = ts.ws_row;

	system("stty cbreak -echo");
	if (kbhit()) {
		int ch = getch();

		if (ch == 'j' && yco == 1) {
		       	jump = 3;
		} else if (ch == 'q') {
			exit(0);
		}
	}
	system("stty cooked echo");	

	if (jump > 0 && !(score % 2)) {
		jump -= 1;
		yco += 1; 
	} else if (yco != 1 && jump != 3 && !(score % 2)) {
		yco -= 1;
	} else if (yco > 3) {
		yco = 3;
	}

	printf("score:%d\n",score);
	for(int i=1; i<(lines-9); i++,printf("\n"));	//print the nothing
	/* The nothing takes up almost the entire screen, minus the bottom 8 lines. This gives us 8 lines for the ground and the play "area" 
	 Because I have chosen to make the ground 3 lines, this gives me 5 lines where the game will occur*/
	

	if (spike_xco > 1) {
		spike_xco -= 2;
	} else if (rand()%5 == 3) {
	spike_xco = cols - 1 + rand() % 100;
	}

	if (spike2_xco > 1) {
		spike2_xco -= 2;
	} else {
		spike2_xco = cols - 1 + newrand() % 100;
	}

	for(int i=0;i<cols*5;i++) {
		if (i == xco + cols*(5-yco)) {
			printf("\033[1;32m");
			printf("@");
			printf("\033[0m");
		} else if (i == (spike_xco + cols*4)){
			printf("\033[1;31m");
			printf("$");
			printf("\033[0m");
		} else if (i == (spike2_xco + cols*4)){
			printf("\033[1;31m");
			printf("$");
			printf("\033[0m");
		} else if (i<(cols*5)-1) {
			printf(" ");
		} else {
			printf(" \n");
		}
	}	

	for(int i=0;i<cols*3;i++) {			//print the ground
		if (i<(cols*3)-1) {
			printf("#");
		} else {
			printf("#\n");
		}
	}
	score++;

	if (yco == 1 && (spike_xco == 20 || spike2_xco == 20 || spike_xco == 21 || spike2_xco == 21)) {
		printf("game over, score: %d\n", score-1);
		exit(0);
	}
	
}
