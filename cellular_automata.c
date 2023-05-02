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
#include <windows.h> 
static const int32_t BIRTH_THRESHOLD = 3;
static const int32_t SURVIVAL_THRESHOLD = 4;
static const int32_t SIMULATION_COUNT = 4;

void cellular_automata(void* organism,uint32_t w,uint32_t h) {
    if(organism == NULL) {
        return;
    }
    
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    srand((unsigned int)ticks.QuadPart);
    
    //set alive and dead code
    const uint32_t alive = 0;
    uint32_t dead = 255;
    {
        for(int i = 0; i < 2;++i) {
            dead = dead << 8 | 255;
        }
    }
    
    //make a chaos, generate alive and dead cells randomly.
    uint32_t* cells = (uint32_t *)organism;
    uint32_t count = 0;
    while(count < (w * h)) {
		      const int r = rand() % 100000;
        cells[count] = r % 2 == 0 ? alive : dead;
        count++;
    }
    
    //let's get into the algorithm! For each cell,
    //calculate n of alive neighbors
    const int32_t ROW_NUM = h;
    const int32_t COLUMN_NUM = w;
    for(int32_t t = 0; t < SIMULATION_COUNT; ++t) {
        for(int32_t r = 0; r < ROW_NUM; ++r) {
            for(int32_t c = 0; c < COLUMN_NUM;++c) {
                // count alive number of neighbors
                int32_t  alive_count = 0;
                for(int32_t i = -1; i <= 1; ++i) {
                    for(int32_t j = -1; j <= 1;++j) {
                        int32_t neighbor_x = c + j;
                        int32_t neighbor_y = r + i;
                        if( i == 0 && j == 0) {
                            //self case do nothing
                        } else if (neighbor_x < 0 || 
                                   neighbor_y < 0 || 
                                   neighbor_x >= COLUMN_NUM ||
                                   neighbor_y >= ROW_NUM) {
                            alive_count += 2; //make boundaries alive
                        } else if(cells[neighbor_x + COLUMN_NUM * neighbor_y] == alive) {
                            alive_count += 1;
                        }
                    }
                }
                const uint32_t current_index = c +  COLUMN_NUM * r;
                if(cells[current_index] == dead && alive_count > BIRTH_THRESHOLD) {
                    cells[current_index] = alive;
                } else if(cells[current_index] == alive && alive_count > SURVIVAL_THRESHOLD) {
                    cells[current_index] = alive;
                } else {
                    cells[current_index] = dead;
                }
            }
            
        };
    } 
}
