#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Header.h"

using namespace std;

#define up 1
#define down 2
#define left 3
#define right 4

#define ENTER 13

#define open 5
#define mark 6


int keylist ();

int main (){
	int xSize , ySize , noMines;

	cout << "xSize: ";
	cin >> xSize;

	cout << "ySize: ";
	cin >> ySize;

	cout << "noMines: ";
	cin >> noMines;

	//creates the board
	Map map ( xSize , ySize , noMines );

	int started = 0;
	map.printmap ();

	while ( 1 ){
		int key = keylist ();

		int action = 0;

		switch ( key ){

			case up:
				map.moveUp ();
				break;

			case down:
				map.moveDown ();
				break;

			case left:
				map.moveLeft ();
				break;

			case right:
				map.moveRight ();
				break;

			case open:
				if ( !started ){		//if not started, place mines around the first square opened
					map.mineGen ();
					started = 1;
				}
				action = map.openSqr ();
				break;

			case mark:
				map.markSqr ();
				break;
		}
		map.printmap ();

		if ( !map.getDiff () ){
			cout << endl << "You WIN" << endl;
			system ( "PAUSE" );
			return 1;
		}

		if ( action == MINE ){
			cout << endl << "KABOOM" << endl;
			system ( "PAUSE" );
			return 1;
		}

	}
	return 0;

}




//wait for a keypress, returns the action (see defines)
int keylist (){
	int key = 0;

	while ( 1 ){
		key = _getch ();

		switch ( key ){

			case 'w':
				return up;
				break;

			case 's':
				return down;
				break;


			case 'a':
				return left;
				break;


			case 'd':
				return right;
				break;

			case ENTER:
				return open;
				break;

			case ' ':
				return mark;
				break;
		}
	}
}

