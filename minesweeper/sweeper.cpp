#include <iostream>
#include <stdio.h>
#include <conio.h>
#include "Header.h"

using namespace std;

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ENTER 13

#define OPEN 5
#define MARK 6

int keylist();

int main() {
    int xSize , ySize , noMines;

    cout << "xSize: ";
    cin >> xSize;

    cout << "ySize: ";
    cin >> ySize;

    cout << "noMines: ";
    cin >> noMines;

    //creates the board
    Map map ( xSize , ySize , noMines );

    bool started = false;
    map.printmap();

    while ( 1 ) {
        int key = keylist();

        bool isMine = false;

        switch ( key ) {
            case UP:
                map.moveUp();
                break;
            case DOWN:
                map.moveDown();
                break;
            case LEFT:
                map.moveLeft();
                break;
            case RIGHT:
                map.moveRight();
                break;
            case MARK:
                map.markSqr();
                break;
            case OPEN:
                if ( !started ) {        //if not started, place mines around the first square opened
                    map.mineGen();
                    started = true;
                }
                isMine = map.openSqr();
                break;
        }
        
        map.printmap();

        if ( map.isVictory() ) {
            cout << endl << "You WIN" << endl;
            int t = map.getTime();
            if ( t > 60 ) {
                cout << endl << t / 60 << " Minute, " << t % 60 << " Seconds" << endl;
            }
            else{
                cout << endl << t << " Seconds to win" << endl;
            }
            system ( "PAUSE" );
            return 1;
        }

        if ( isMine ) {
            cout << endl << "KABOOM" << endl;
            system ( "PAUSE" );
            return 1;
        }
    }
    
    return 0;
}

//wait for a keypress, returns the action (see defines)
int keylist() {
    int key = 0;

    while ( 1 ) {
        key = _getch();

        switch ( key ) {

            case 'w':
                return UP;
            case 's':
                return DOWN;
            case 'a':
                return LEFT;
            case 'd':
                return RIGHT;
            case ENTER:
                return OPEN;
            case ' ':
                return MARK;
        }
    }
}

