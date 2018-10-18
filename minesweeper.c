// Aleksi Martikainen 1706215
// minesweeper program with most of the original functionalities
// most notably missing user choice size of grid and bomb count
// and in addition it is not guaranteed that the first "click"
// won't be a bomb, unlike in normal minesweeper

#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <conio.h> 

//edit values to change sizes
//MAX - 1 will be width of grid
//BOMBS indicates bomb count
#define MAX 11
#define BOMBS 10

int newGame(void);
void printer(char array[MAX][MAX]);
void bombPlacer(char grid[MAX][MAX]);
void gridFiller(char array[MAX][MAX], char c);
void simpleChecker(char playerGrid[MAX][MAX], char grid[MAX][MAX], int y, int x);
void zeroChecker(char playerGrid[MAX][MAX], char grid[MAX][MAX], int y, int x);

int main(void){
	char c;

	printf("\nWelcome to Minesweeper\n");

	while (c != '0') {
		printf("\nPress any key to play minesweeper, exit with '0'\n");
		c = getch();
		if (c != '0') {
			newGame();
		}
	}

	printf("\nThanks for playing\n");
	return 0;
}


//Function makes the grid, places bombs, takes user input
int newGame(void) {
	char grid[MAX][MAX];
	char playerGrid[MAX][MAX];
	grid[MAX][MAX] = '\0';
	playerGrid[MAX][MAX] = '\0';

	int i = 0;
	int j = 0;
	char c;
	int y = 1;
	int x = 1;
	int winCounter = 0;
	int winner = 0;
	char temp = 'x';

	/*PLAYER SEEN GRID*/
	//fill grid with x
	gridFiller(playerGrid, 'x');
	
	/*BOMB GRID*/
	//fill with '0'
	gridFiller(grid, '0');

	//placing bombs
	bombPlacer(grid);
	
	
	//set spacebars to cells that are '0' for easier visual while playing
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < MAX; j++) {
			if (grid[i][j] == '0') {
				grid[i][j] = ' ';
			}
		}
	}
	
	//printing grid to see during testing
	//won't be printed in final version
	
	printf("\nPlayer Grid:\n");


	//Playing the game
	//check with win condition to ensure that all cells have been
	//opened before we can declare the player a winner
	while (winner != (MAX - 1) * (MAX - 1)) {
		
		//printing grids
		//printer(grid);
		printer(playerGrid);

		/*Move in console with wasd to select cell*/
		//getch() way of inputting without having to press enter
		//makes gameplay more smooth, conio.h was necessary
		//to get the getch() function
		c = getch();
		if (c == 'w') {
			//first set the current cell to = temp
			playerGrid[y][x] = temp;

			//this if makes the 'cursor' wrap around the field
			if ((y - 1) < 1) {
				y = (MAX - 1);
			}
			else {
				y--;
			}

			//after 'moving' we set temp to the new cell
			temp = playerGrid[y][x];

			//then we need to display where we are with the 'cursor'
			playerGrid[y][x] = '#';
		}
		else if (c == 's') {
			playerGrid[y][x] = temp;
			if ((y + 1) == MAX) {
				y = 1;
			}
			else {
				y++;
			}
			temp = playerGrid[y][x];
			playerGrid[y][x] = '#';
		}
		else if (c == 'a') {
			playerGrid[y][x] = temp;
			if ((x - 1) < 1) {
				x = (MAX - 1);
			}
			else {
				x--;
			}
			temp = playerGrid[y][x];
			playerGrid[y][x] = '#';
		}
		else if (c == 'd') {
			playerGrid[y][x] = temp;
			if ((x + 1) == MAX) {
				x = 1;
			}
			else {
				x++;
			}
			temp = playerGrid[y][x];
			playerGrid[y][x] = '#';
		}

		//flag
		else if (c == 'f') {
			if (grid[y][x] == 'B') {
				//prevent player from pressing f on the same cell to win
				if (playerGrid[y][x] != 'f') {
					winCounter++;
				}

				playerGrid[y][x] = 'f';
				temp = playerGrid[y][x];
			}
			else {
				//this case exists so that when player misplaces a
				//flag it won't count towards winning
				playerGrid[y][x] = 'f';
				temp = playerGrid[y][x];
			}
		}

		//opening cell
		else if (c == 'r') {
			//if cell contains a number
			if (grid[y][x] != 'B' && grid[y][x] != ' ') {
				playerGrid[y][x] = grid[y][x];
				temp = playerGrid[y][x];
			}
			//if cell cointains empty space
			else if (grid[y][x] == ' ') {
				//simpleChecker(playerGrid, grid, y, x);
				zeroChecker(playerGrid, grid, y, x);
				temp = playerGrid[y][x];
			}
			//if user opened a bomb
			else {
				printer(grid);
				printf("Game Over, you opened a bomb\nPlay again?\n");
				return 0;
			}
		}

		//exit
		else if (c == '0') {
			return 0;
		}
		
		//check that all cells are open before ending game
		if (winCounter == BOMBS) {
			winner = 0;
			//check all cells for 'x' or '#'
			for (i = 1; i < MAX; i++) {
				for (j = 1; j < MAX; j++) {
					if (playerGrid[i][j] != 'x' && playerGrid[i][j] != '#') {
						winner++;
					}
				}
			}
		}
	}
	printer(grid);
	printf("CONGRATULATIONS\nYOU WIN\n");
	//print original grid to show user the truly won
}

void printer(char array[MAX][MAX]) {
	int i = 0;
	int	j = 0;
	printf("\nControls: w,a,s,d to move, 'f' to mark flag, 'r' to open cell, '0' to exit\n");
	//printing desired grid
	for (i = 1; i < MAX; i++) {
		for (j = 1; j < MAX; j++) {
			printf(" %c", array[i][j]);
		}
		printf("\n");
	}
}

//function places the bombs on the grid
void bombPlacer(char grid[MAX][MAX]) {
	int bombs = 0;
	int r1, r2;
	srand(time(0));
	while (bombs < BOMBS) {

		//get random values where to place
		r1 = rand() % (MAX - 1) + 1;
		r2 = rand() % (MAX - 1) + 1;

		//check that there won't be overlap of bombs
		if (grid[r1][r2] != 'B') {

			//set cell to bomb
			grid[r1][r2] = 'B';

			//calculate the numbers for the grid on cells that aren't bombs
			//to indicate how many bombs are next to the cell
			//some additional checks to prevent wrapping around the 
			if (grid[r1 - 1][r2 - 1] != 'B' && (r2 - 1) >= 1 && (r1 - 1) >= 1) {
				grid[r1 - 1][r2 - 1]++;
			}
			if (grid[r1 - 1][r2] != 'B' && (r1 - 1) >= 1) {
				grid[r1 - 1][r2]++;
			}
			if (grid[r1 - 1][r2 + 1] != 'B' && (r2 + 1) < MAX && (r1 - 1) >= 1) {
				grid[r1 - 1][r2 + 1]++;
			}
			if (grid[r1][r2 - 1] != 'B' && (r2 - 1) >= 1) {
				grid[r1][r2 - 1]++;
			}
			if (grid[r1][r2 + 1] != 'B' && (r2 + 1) < MAX) {
				grid[r1][r2 + 1]++;
			}
			if (grid[r1 + 1][r2 - 1] != 'B' && (r2 - 1) >= 1) {
				grid[r1 + 1][r2 - 1]++;
			}
			if (grid[r1 + 1][r2] != 'B') {
				grid[r1 + 1][r2]++;
			}
			if (grid[r1 + 1][r2 + 1] != 'B' && (r2 + 1) < MAX) {
				grid[r1 + 1][r2 + 1]++;
			}

			//increment bombs
			bombs++;
		}
	}
}

//fills the grid with desired char
void gridFiller(char array[MAX][MAX], char c) {
	int i, j;
	for (i = 1; i < MAX; i++) {
		for (j = 1; j < MAX; j++) {
			array[i][j] = c;
		}
	}
}

//simpler version of zeroChecker(), second version of opening
//cells i made, before doing the more complex one
//not used in final version
void simpleChecker(char playerGrid[MAX][MAX], char grid[MAX][MAX], int y, int x) {

	playerGrid[y][x] = grid[y][x];
	if (playerGrid[y][x] == ' ') {
		if ((x - 1) >= 1) {
			playerGrid[y + 1][x - 1] = grid[y + 1][x - 1];
			playerGrid[y - 1][x - 1] = grid[y - 1][x - 1];
			playerGrid[y][x - 1] = grid[y][x - 1];
		}
		if ((x + 1) < MAX) {
			playerGrid[y + 1][x + 1] = grid[y + 1][x + 1];
			playerGrid[y - 1][x + 1] = grid[y - 1][x + 1];
			playerGrid[y][x + 1] = grid[y][x + 1];
		}
		playerGrid[y + 1][x] = grid[y + 1][x];
		playerGrid[y - 1][x] = grid[y - 1][x];
	}
}

//causes many crashes(fixed(hopefully))
//sometimes 'corrupts' the arrays with a odd symbol
//fixed by having the arrays 'start' from 1 instead of 0,
//problems only occured on the first row, so not 
//using that row "fixed" the issue
void zeroChecker(char playerGrid[MAX][MAX], char grid[MAX][MAX], int y, int x) {

	//return if the cell has already been checked by the function
	if (playerGrid[y][x] == ' ') {
		return;
	}
	
	if (grid[y][x] == ' ') {
		playerGrid[y][x] = grid[y][x];

		//check that wrapping around the arrays won't happen
		//with the >= 1 and < MAX, so we don't reveal cells on
		//the other side of the grid

		//if cell is ' ' then we need to check the cells 
		//surrounding the new empty cell found by repeating
		//zeroChecker() with the new values.

		//need to check all 8 cells surrounding current one,
		//so pretty much same thing is repeated 8 times.
		if (grid[y][x - 1] == ' ' && (x - 1) >= 1) {
			zeroChecker(playerGrid, grid, y, x - 1);
		}
		else if ((x - 1) >= 1) {
			playerGrid[y][x - 1] = grid[y][x - 1];
		}

		if (grid[y - 1][x - 1] == ' ' && (x - 1) >= 1) {
			zeroChecker(playerGrid, grid, y - 1, x - 1);
		}
		else if ((x - 1) >= 1) {
			playerGrid[y - 1][x - 1] = grid[y - 1][x - 1];
		}

		if (grid[y + 1][x - 1] == ' ' && (x - 1) >= 1) {
			zeroChecker(playerGrid, grid, y + 1, x - 1);
		}
		else if ((x - 1) >= 1) {
			playerGrid[y + 1][x - 1] = grid[y + 1][x - 1];
		}

		if (grid[y][x + 1] == ' ' && (x + 1) < MAX) {
			zeroChecker(playerGrid, grid, y, x + 1);
		}
		else if ((x + 1) < MAX) {
			playerGrid[y][x + 1] = grid[y][x + 1];
		}

		if (grid[y + 1][x + 1] == ' ' && (x + 1) < MAX) {
			zeroChecker(playerGrid, grid, y + 1, x + 1);
		}
		else if ((x + 1) < MAX) {
			playerGrid[y + 1][x + 1] = grid[y + 1][x + 1];
		}

		if (grid[y - 1][x + 1] == ' ' && (x + 1) < MAX) {
			zeroChecker(playerGrid, grid, y - 1, x + 1);
		}
		else if ((x + 1) < MAX) {
			playerGrid[y - 1][x + 1] = grid[y - 1][x + 1];
		}

		if (grid[y - 1][x] == ' ' && (y - 1) >= 1) {
			zeroChecker(playerGrid, grid, y - 1, x);
		}
		else if ((y - 1) >= 1) {
			playerGrid[y - 1][x] = grid[y - 1][x];
		}

		if (grid[y + 1][x] == ' ' && (y + 1) < MAX) {
			zeroChecker(playerGrid, grid, y + 1, x);
		}
		else if ((y + 1) < MAX) {
			playerGrid[y + 1][x] = grid[y + 1][x];
		}
	}
}