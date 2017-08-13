#include <Windows.h>
#include <time.h>

using namespace std;

#define maxSize 256 
#define CLEAR 0
#define MINE 1


class Square {
public:
    bool opened;
    bool mine;
    bool marked;
    int number;
//    int unopenedAround;

    Square() {
        opened = false;
        mine = false;
        marked = false;
        number = 0;
    }
};

class Player {
private:
    int xPosition;
    int yPosition;

public:
    Player() {
        xPosition = 0;
        yPosition = 0;
    }
    void moveDown() {
        yPosition++;
    }
    void moveUp() {
        yPosition--;
    }
    void moveRight() {
        xPosition++;
    }
    void moveLeft() {
        xPosition--;
    }
    
    int getPositionY() {
        return yPosition;
    }
    int getPositionX() {
        return xPosition;
    }
};

class Map {
private:
    Square *grid[ maxSize ][ maxSize ];
    Player player;
    int noMines;
    int xSize;
    int ySize;
    int marked;
    int correct;
    unsigned long long startTime;


    //Places the numbers on the grid after the mines have been placed
    void setNumbers ( int xIn , int yIn ) {
        Square *curr;
        for ( int x = -1; x < 2; x++ ) {
            for ( int y = -1; y < 2; y++ ) {
                if ( ( xIn + x >= 0 ) && ( xIn + x < xSize ) //if within the gameboard
                     && ( yIn + y >= 0 ) && ( yIn + y < ySize ) ) {
                    curr = grid[ xIn + x ][ yIn + y ];
                    curr -> number++;
                }
            }
        }
    }

    //If you open an empty Square, open all other empty Squares.
    void openBlanks ( int xIn , int yIn ) {
        Square *curr = grid[ xIn ][ yIn ];

        //Open blanks if selected square have no neighboring mines
        if ( !curr -> opened && curr -> number == 0 ) {
            for ( int x = -1; x < 2; x++ ) {
                for ( int y = -1; y < 2; y++ ) {
                    if ( ( xIn + x >= 0 ) && ( xIn + x < xSize ) &&
                         ( yIn + y >= 0 ) && ( yIn + y < ySize ) ) {
                        curr -> opened = true;
                        openBlanks ( xIn + x , yIn + y );
                    }
                }
            }
        }
        else{
            curr -> opened = true;
            if ( curr -> marked ) {
                curr -> marked = 0;
                marked--;
            }
        }
    }

    /*
    void setUnop ( int xIn , int yIn ) {
        Square *curr = grid[ xIn ][ yIn ];
        int counter = 0;

        for ( int x = -1; x < 2; x++ ) {
            for ( int y = -1; y < 2; y++ ) {
                if ( ( xIn + x >= 0 ) && ( xIn + x < xSize ) &&
                     ( yIn + y >= 0 ) && ( yIn + y < ySize ) ) {

                    if ( !grid[ xIn + x ][ yIn + y ] -> opened )
                        counter++;

                }
            }
        }
        curr -> unopenedAround = counter;
    }
    */

public:

    //Constructor, takes x size, y size and nr of mines.
    Map ( int xSize, int ySize , int noMines ) {
        for ( int x = 0; x < xSize; x++ ) {
            for ( int y = 0; y < ySize; y++ ) {
                grid[ x ][ y ] = new Square;
            }
        }

        this -> xSize = xSize;
        this -> ySize = ySize;

        this -> noMines = noMines;
        marked = false;
        correct = false;
    }

    //Places mines around your current place
    void mineGen() {
        int mines = noMines;
        srand ( time( 0 ) );
        int currentX , currentY;
        Square *curr;

        while ( mines ) {
            currentX = rand() % ( xSize - 1 );
            currentY = rand() % ( ySize - 1 );

            if ( currentX != player.getPositionX() && currentY != player.getPositionY() ) {
                curr = grid[ currentX ][ currentY ];
                if ( !curr -> mine ) {
                    curr -> mine = 1;
                    setNumbers( currentX , currentY );
                    mines--;
                }
            }
        }
        startTime = time( 0 );
    }

    //Open the current Square
    int openSqr() {
        Square *curr = grid[ player.getPositionX() ][ player.getPositionY() ];

        if ( !curr -> marked ) {
            if ( curr -> number == 0 ) {
                openBlanks ( player.getPositionX() , player.getPositionY());
            }
            curr -> opened = true;
        }

        return curr -> mine;
    }

    //Mark the current Square
    void markSqr() {
        Square *curr = grid[ player.getPositionX() ][ player.getPositionY() ];

        if ( curr -> opened ) {
            return;
        }

        if ( !curr -> marked ) {
            curr -> marked = true;
            marked++;
            if ( curr -> mine ) {
                correct++;
            }
            return;
        }
        curr -> marked = false;
        marked--;
        if ( curr -> mine ) {
            correct--;
        }
    }

    //returns difference between correctly marked and ammount of mines. If 0 you win
    int isVictory() {
        return !(noMines - correct);
    }

    void moveRight() {
        if ( player.getPositionX() < xSize - 1 ) {
            player.moveRight();
        }
    }

    void moveLeft() {
        if ( player.getPositionX() > 0 ) {
            player.moveLeft();
        }
    }

    void moveUp() {
        if ( player.getPositionY() > 0 ) {
            player.moveUp();
        }
    }

    void moveDown() {
        if ( player.getPositionY() < ySize - 1 ) {
            player.moveDown();
        }
    }

    //Prints the board on the console
    void printmap() {
        HANDLE hConsole = GetStdHandle ( STD_OUTPUT_HANDLE );
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;


        SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );

        system ( "cls" );
        int minesLeft = noMines - marked;
        cout << "Mines: " << minesLeft << endl;
        
        for ( int x = 0; x < xSize + 2; x++ ) {
            cout << "#";
        }
        cout << endl;
        Square *curr;
        for ( int currentY = 0; currentY < ySize; currentY++ ) {
            cout << "#";
            for ( int currentX = 0; currentX < xSize; currentX++ ) {
                curr = grid[ currentX ][ currentY ];

                if ( currentX == player.getPositionX() &&  currentY == player.getPositionY() ) {
                    cout << "@";
                }
                else{
                    if ( curr  ->  opened ) {
                        SetConsoleTextAttribute ( hConsole , curr  ->  number + 7 );
                        cout << curr  ->  number;
                        SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );
                    }
                    else if ( curr  ->  marked ) {
                        SetConsoleTextAttribute ( hConsole , FOREGROUND_RED );
                        cout << "!";
                        SetConsoleTextAttribute ( hConsole , FOREGROUND_INTENSITY );
                    }
                    else {
                        cout << " ";
                    }
                }
            }
            cout << "#" << endl;
        }
        
        for ( int x = 0; x < xSize + 2; x++ ) {
            cout << "#";
        }
        cout << endl;
    }

    //Erases all Squares from the board
    void clear() {
        for ( int x = 0; x < xSize; x++ ) {
            for ( int y = 0; y < ySize; y++ ) {
                delete grid[ x ][ y ];
            }
        }
    }

	/*
    void cheat() {
        Square *curr;
        for ( int x = 0; x < xSize; x++ ) {
            for ( int y = 0; y < xSize; y++ ) {
                setUnop ( x , y );
            }
        }
    }
	*/

    int getTime() {
        return ( time ( 0 ) - startTime );
    }
};