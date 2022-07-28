#include <iostream>
#include "screen.h"

const int GAME_WIDTH = 64;
const int GAME_HEIGHT = 64;


int getRandomCellState(){
    int cellState;
    cellState = rand() % 10==0 ? 1:0;
    return cellState;
}

int getAliveNeighbourCount(
    std::array< std:: array<int,GAME_HEIGHT> , GAME_WIDTH >& game,
    const int x, const int y){
    bool isAlive = false;
    int liveCellCount = 0;

    // Check left of given cell
    if ( x>0 && game[x-1][y] == 1) liveCellCount+=1;
    // Check right of given cell
    if ( x < GAME_WIDTH && game[x+1][y] == 1) liveCellCount +=1;
    // Check top of given cell
    if ( y > 0 && game[x][y-1] == 1 ) liveCellCount +=1;
    // Check bottom of given cell
    if ( y < GAME_HEIGHT && game[x][y+1] == 1 ) liveCellCount +=1;


    // Check top-left of given cell
    if ( x > 0 && y > 0 && game[x-1][y-1] == 1 ) liveCellCount +=1;
    // Check top-right of given cell
    if ( x < GAME_WIDTH && y > 0 && game[x+1][y-1] == 1 ) liveCellCount +=1;
    // Check bottom-left of given cell
    if ( x > 0 && y < GAME_HEIGHT && game[x-1][y+1] == 1 ) liveCellCount +=1;
    // Check bottom-right of given cell
    if ( x < GAME_WIDTH && y < GAME_HEIGHT && game[x+1][y+1] == 1 ) liveCellCount +=1;

    return liveCellCount;
}

// Here go the rules. Change as u wish
bool isAlive(int liveNeighbourCount, bool currCellState){
    
    // If Current Cell is alive and less than 2 neighbours are alive then current cell dies.
    if ( currCellState && liveNeighbourCount < 2) return false;

    // If Current cell is alive and has 2 or 3 alive neighbours then current cell stays alive.
    if ( currCellState && ( liveNeighbourCount >1 && liveNeighbourCount < 4 ) )   return true;

    // If Current Cell is dead and has exactly 3 alive neighbours then the current cell becomes alive
    if ((!currCellState) && liveNeighbourCount==3 )    return true;

    // Anything else the current cell dies.
    return false;
}

int main(){

    std::array< std:: array<int,GAME_HEIGHT> , GAME_WIDTH > display {};
    std::array< std:: array<int,GAME_HEIGHT> , GAME_WIDTH> buffer {};
    bool currCellState;
    int liveNeighbourCount;
    G screen;

    // Generate Random Points

    for (auto& row : display){
        std::generate(row.begin(),row.end(),getRandomCellState );
    }

    // for (auto& row: display){
    //     for (int col:row){
    //         std::cout << col;
    //         std::cout << "|";
    //     }
    //     std:: cout << std::endl << std::string(GAME_WIDTH*2,'-') << std::endl;
    // }

    while( true ){
        // Use the display 2D array as input. Apply rules and update to a buffer 2D array then swap buffer and display arrays

        for (int i=0; i< GAME_WIDTH;++i){
            for (int j=0 ; j< GAME_HEIGHT; ++j){
                currCellState = (display[i][j] == 1)  ? true : false;
                liveNeighbourCount = getAliveNeighbourCount(display,i,j);
                // Update current Cell State in buffer
                buffer[i][j] = isAlive(liveNeighbourCount,currCellState) ? 1 : 0 ; 
            }
        }

        // Draw Buffer to screen
        for (int i=0; i< GAME_WIDTH;++i){
            for (int j=0 ; j< GAME_HEIGHT; ++j){
                if (buffer[i][j]){
                    screen.drawpixel(i,j);
                }
            }
        }
        // Swap the updated buffer array to display array and then show the display array
        std::copy(buffer.begin(),buffer.end(), display.begin());

        // Display to the screen.
        screen.update();
        SDL_Delay(10);
        screen.input();
        screen.clearpixels();
    }
}