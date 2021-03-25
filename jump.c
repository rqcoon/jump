//rqcoon, 20/3/21
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
int spike_xco[5] = {0,0,0,0,0};
int score = 0;

int main() {
	while (1) {
		usleep(53000);
		//system("clear");
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

void game(){
	struct winsize ts;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
	int cols = ts.ws_col;
	int lines = ts.ws_row;

	system("stty cbreak -echo"); //yes, I know this is a horrible solution
	if (kbhit()) {
		int ch = getch();

		if (ch == 'j' && yco == 1) {
		       	jump = 3;
		} else if (ch == 'q') {
			system("clear");
			exit(0);
		}
	}
	system("stty cooked echo"); //and that I shouldn't run system() if i want to make this portable at all
	
	
	for (int i=0; i < 5; i++) {
		if (spike_xco[i] < 0) {
			spike_xco[i] = (rand() % cols) + cols - 1;
		}
	}
	

	if (jump > 0 && !(score % 2)) {
		jump -= 1;
		yco += 1; 
	} else if (yco != 1 && jump != 3 && !(score % 2)) {
		yco -= 1;
	} else if (yco > 3) {
		yco = 3;
	}

	printf("score:%d\n",score);
	for(int i=1; i<(lines-9); i++,printf("\n"));
	/* Blankness takes up almost the entire screen, minus the bottom 8 lines. This gives us 8 lines for the ground and the play "area" 
	 Because I have chosen to make the ground 3 lines, this gives me 5 lines where the game will occur*/

	for(int i=0;i<cols*5;i++) {
		if (i == xco + cols*(5-yco)) {
			printf("\033[1;32m");
			printf("@");
			printf("\033[0m");
		} else if (i == spike_xco[0] + cols*4 || i == spike_xco[1] + cols*4 || i == spike_xco[2] + cols*4 || i == spike_xco[3] + cols*4 || i == spike_xco[4] + cols*4){ //yes, I know
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

	for (int i=0;i<5;i++) {
		if ((spike_xco[i] == xco || spike_xco[i] == xco + 1) && yco == 1) {
			system("clear");
			printf("game over, score: %d\n",score);
			exit(0);
		}
	}

	score++;
	for (int i = 0; i < 5; i++) {
		spike_xco[i] -= 2;
	}
}
