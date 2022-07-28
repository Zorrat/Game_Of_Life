#include <SDL2/SDL.h>
#include <cstdlib>
#include <iostream>
#include <time.h>  
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;

// Setup SDL library
class G {

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_bool done;
    vector<SDL_FPoint> points;
    vector<SDL_Color> colors;
    SDL_Event event;
    public:
    G() 
    {
       SDL_Init(SDL_INIT_VIDEO); 
       SDL_CreateWindowAndRenderer(1280, 720, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer);
       SDL_RenderSetScale(renderer,8,8);
    }

    void drawpixel(double xm, double ym, uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) 
    {
        points.emplace_back(xm,ym);
        colors.emplace_back(r,g,b,a);
    }
    void clearpixels()
    {
        points.clear();
    }

    void update() 
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        
        for(long unsigned int i = 0; i < points.size(); i++)
        {
            SDL_SetRenderDrawColor(renderer, colors[i].r, colors[i].g, colors[i].b, colors[i].a);
            SDL_RenderDrawPointF(renderer, points[i].x,points[i].y);
        }


        SDL_RenderPresent(renderer);
    }
    void input() {
        while( SDL_PollEvent( &event ) )
        {  
            switch( event.type )
            {
                /* SDL_QUIT event (window close) */
                case SDL_QUIT:
                    SDL_Quit();
                    exit(0);
                    break;

                default:
                    break;
            }
        }
    }

};


const int GAME_WIDTH = 160;
const int GAME_HEIGHT = 90;
const float FRAME_RATE = 24;
const float FRAME_INTERVAL = (1/FRAME_RATE) * 1000 ;


int getRandomCellState(){
    int cellState;
    int rn = 0;;
    rn = rand();
    cellState = (rn % 10==0 || rn%3 ==0 ) ? 1:0;
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
bool isAlive(int liveNeighbourCount, int currCellState){
    
    // If Current Cell is alive and less than 2 neighbours are alive then current cell dies.
    if ( currCellState==1 && liveNeighbourCount < 2) return false;

    // If Current cell is alive and has 2 or 3 alive neighbours then current cell stays alive.
    if ( currCellState==1 && ( liveNeighbourCount >1 && liveNeighbourCount < 4 ) )   return true;

    // If Current Cell is dead and has exactly 3 alive neighbours then the current cell becomes alive
    if ((currCellState ==0) && liveNeighbourCount==3 )    return true;

    // Anything else the current cell dies.
    return false;
}

int main(){

    std::array< std:: array<int,GAME_HEIGHT> , GAME_WIDTH > display {};
    std::array< std:: array<int,GAME_HEIGHT> , GAME_WIDTH> buffer {};
    int currCellState;
    int liveNeighbourCount;
    G screen;

    // Generate Random Points

    for (auto& row : display){
        std::generate(row.begin(),row.end(),getRandomCellState );
    }

    while( true ){
        // Use the display 2D array as input. Apply rules and update to a buffer 2D array then swap buffer and display arrays

        for (int i=0; i< GAME_WIDTH;++i){
            for (int j=0 ; j< GAME_HEIGHT; ++j){
                currCellState = display[i][j];
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
        SDL_Delay(FRAME_INTERVAL);
        screen.input();
        screen.clearpixels();
    }
}