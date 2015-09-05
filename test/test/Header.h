#include <Windows.h>
#include <time.h>

using namespace std;

#define maxSize 256 
#define CLEAR 0
#define MINE 1


class square{
public:
	int opened;
	int mine;
	int marked;
	int number;

	square (){
		opened = 0;
		mine = 0;
		marked = 0;
		number = 0;
	}
};



class Map{
private:
	square *grid[ maxSize ][ maxSize ];
	int xPos;
	int yPos;
	int noMines;
	int xSize;
	int ySize;
	int marked;
	int correct;

	void setNumbers ( int xIn , int yIn ){
		square *curr;
		for ( int x = -1; x < 2; x++ ){
			for ( int y = -1; y < 2; y++ ){
				if ( ( xIn + x >= 0 ) && ( xIn + x < xSize ) && ( yIn + y >= 0 ) && ( yIn + y < ySize ) ){//if within the gameboard
					curr = grid[ xIn + x ][ yIn + y ];
					curr->number++;
				}
			}
		}
	}

	void openBlanks ( int xIn , int yIn ){		//ToDo
		square *curr = grid[ xIn ][ yIn ];

		if ( ( curr->number == 0 ) && ( !curr->opened ) ){
			for ( int x = -1; x < 2; x++ ){
				for ( int y = -1; y < 2; y++ ){

					if ( ( xIn + x >= 0 ) && ( xIn + x < xSize ) &&
						 ( yIn + y >= 0 ) && ( yIn + y < ySize ) ){
							curr->opened = 1;
							openBlanks ( xIn + x , yIn + y );
						}
				}
			}
		}
		else{
			curr->opened = 1;
			if ( curr->marked ){
				curr->marked = 0;
				marked--;
			}
		}
	}

public:
	Map ( int xIn , int yIn , int mines ){
		for ( int x = 0; x < xIn; x++ ){
			for ( int y = 0; y < yIn; y++ ){
				grid[ x ][ y ] = new square;
			}
		}
		xPos = 0;
		yPos = 0;

		xSize = xIn;
		ySize = yIn;

		noMines = mines;
		marked = 0;
		correct = 0;

	}

	void mineGen (){
		int mines = noMines;
		srand ( time ( 0 ) );
		int x , y;
		square *curr;

		while ( mines ){
			x = rand () % ( xSize - 1 );
			y = rand () % ( ySize - 1 );

			if ( ( x != xPos ) && ( y != yPos ) ){
				curr = grid[ x ][ y ];
				if ( !curr->mine ){
					curr->mine = 1;
					setNumbers ( x , y );
					mines--;
				}
			}
		}
	}

	int openSqr (){
		square *curr;
		curr = grid[ xPos ][ yPos ];

		if ( !curr->marked ){
			if ( curr->number == 0 ){
				openBlanks ( xPos , yPos );
			}
			curr->opened = 1;

			if ( curr->mine ){
				return MINE;
			}
		}

		return CLEAR;
	}

	void markSqr (){
		square *curr;

		curr = grid[ xPos ][ yPos ];

		if ( curr->marked == 0 ){
			curr->marked = 1;
			marked++;
			if ( curr->mine )
				correct++;

			return;
		}
		curr->marked = 0;
		marked--;
		if ( curr->mine )
			correct--;

	}

	//returns difference between correctly marked and ammount of mines. If 0 you win
	int getDiff (){
		return noMines - correct;
	}

	int moveRight (){
		if ( xPos < xSize - 1 ){
			xPos += 1;
			return 1;
		}
		return 0;
	}

	int moveLeft (){
		if ( xPos > 0 ){
			xPos -= 1;
			return 1;
		}
		return 0;
	}

	int moveUp (){
		if ( yPos > 0 ){
			yPos -= 1;
			return 1;
		}
		return 0;
	}

	int moveDown (){
		if ( yPos < ySize - 1 ){
			yPos += 1;
			return 1;
		}
		return 0;
	}

	//Prints the board on the console
	void printmap (){						//ToDo - Colors
		HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		WORD saved_attributes;

		square *curr;

		SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );

		system ( "cls" );
		int out = noMines - marked;
		cout << "Mines: " << out << endl;
		for ( int x = 0; x < xSize + 2; x++ ){
			cout << "#";
		}
		cout << endl;
		for ( int y = 0; y < ySize; y++ ){
			cout << "#";
			for ( int x = 0; x < xSize; x++ ){
				curr = grid[ x ][ y ];

				if ( ( x == xPos ) && ( y == yPos ) ){
					cout << "@";
				}
				else{
					if ( curr->opened ){
						SetConsoleTextAttribute ( hConsole , curr->number + 7 );
						cout << curr->number;
						SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );
					}
					else if ( curr->marked ){
						SetConsoleTextAttribute ( hConsole , FOREGROUND_RED );
						cout << "!";
						SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );
					}
					else{
						cout << " ";
					}
				}


			}
			cout << "#" << endl;
		}
		for ( int x = 0; x < xSize + 2; x++ ){
			cout << "#";
		}
		cout << endl;

	}

	//Erases all squares from the board
	void clear (){
		for ( int x = 0; x < xSize; x++ ){
			for ( int y = 0; y < ySize; y++ ){
				delete grid[ x ][ y ];
			}
		}
	}


};