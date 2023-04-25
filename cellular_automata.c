/*
// cellualr automata algorithm
// preparation
// -- make a chaos first --
//1. calculate the number of its alive neighbors
//2. if the cell is dead has at least birth-threshold alive neighbors,
 //it becomes alive.
//3. if the cell is alive and has at least survival-threshold alive
//neighbors,  it stays alive.
//4. the cell is dead otherwise.
//5. Simulate. repeat 1 - 5, n times
*/

#include <stdint.h> //uint32_t
#include <stdlib.h> //srand()

const int BIRTH_THRESHOLD = 3;
const int SURVIVAL_THRESHOLD = 5;


void cellular_automata(void* display_buffer,uint32_t w,uint32_t h) {
    if(display_buffer == NULL) {
        return;
    }
    
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    srand(ticks.QuadPart);
    
    uint32_t alive = 255;
    for(int i = 0; i < 2;++i) {
        open = open << 8 | 255;
    }
    
    const uint32_t dead = 0;
    //make a chaos, generate alive and dead cells randomly.
    uint32_t* ptr_buffer = (uint32_t *)display_buffer;
    uint32_t count = 0;
    while(count < (w * h)) {
        ptr_buffer[count] = rand() % 2 == 1 ? alive : dead;
        count++;
    }
    
    //let's get into the algorithm! For each cell,
    //1. calculate n of alive neighbors
    const int ROW = h;
    const int COLUMN = w;
    for(uint32_t r = 0; r < ROW; ++r) {
        for(uint32_t c = 0; c < COLUMN;++c) {
            // checking neighbors
            int alive_count = 0;
            for(uint32_t i = -1; i <= 1; ++i) {
                for(uint32_t j = -1; j <= 1;++j) {
                    uint32_t neighbor_x = r + i;
                    uint32_t neighbor_y = c + j;
                    if( i == 0 && j == 0) {
                        //self case do nothing
                    } else if (x < 0 || 
                               y < 0 || 
                               neighbor_x >= ROW
                               neighbor_y >= COLUMN) {
                        alive_count += 2; //make boundaries alive
                    } else if(ptr_buffer[neighbor_x][neighbor_y] == alive) {
                        alive_count += 1;
                    }
                }
            }
            if(ptr_buffer[r][c] == dead && count > BIRTH_THRESHOLD){
                ptr_buffer[r][c] = alive;
            }else if(ptr_buffer[r][c] == alive && count > SURVIVAL_THRESHOLD){
                ptr_buffer[r][c] = alive;
            }else {
                ptr_buffer[r][c] = dead;
            }
        }
    } 
}
