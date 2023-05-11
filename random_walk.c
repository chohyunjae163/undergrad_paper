//http://www.roguebasin.com/index.php/Random_Walk_Cave_Generation
/*
// initialize all map cells to walls.
// pick a map cell as the starting point.
// turn the selected map cell into floor.
 // while insufficient cells have been turned into floor,
// take one step in a random direction.
// if the new map cell is wall,
// turn the new map cell into floor and increment the count of floor tiles.
*/
#include <stdint.h> //int32_t
#include <stdlib.h>
#include <windows.h>

enum Direction {
  TOP = 0,
  LEFT = 1,
  RIGHT = 2,
  BOTTOM = 3,
};

void random_walk(void* cave, int32_t w, int32_t h){
    if(cave == NULL) {
        return;
    }

    if( w < 1 || h < 1) {
        return;
    }

    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    srand((unsigned int) ticks.QuadPart);

    const uint32_t floor_num = (w * h) / 4;
    //wall - black
    //floor - white
    uint32_t floor = 255;
    for(uint32_t i = 0; i < 2;++i) {
        floor = floor << 8 | 255;
    }
    const uint32_t wall = 0;
    // all map cells to walls.
    uint32_t* map_cells = cave;
    for(int i = 0; i < w * h; ++i) {
        map_cells[i] = wall;
    }

    //the center as the starting point
    uint32_t starting_index = ((( w * h ) + w) / 2);
    //turn the cell into floor
    map_cells[starting_index] = floor;
    uint32_t floor_count = 1;
    uint32_t current_index = starting_index;
    while(floor_count < floor_num) {
        //take a random direction
        uint32_t wander_try = 0;
        while(current_index == -1 ||
              map_cells[current_index] == floor){
            ++wander_try;
            if(wander_try > 10){
                break;
            }
            uint32_t index = 0;
            enum Direction direction = rand() % 4;
            switch(direction){
            case TOP:
                index-=w;
                break;
            case LEFT:
                --index;
                break;
            case RIGHT:
                ++index;
                break;
            case BOTTOM:
                index+=w;
            }
            current_index += index;
            if(current_index < 0 ||
               current_index > w * h - 1){
                current_index = -1;
            }
        }
        if(current_index < 0 || current_index > w * h - 1){
            current_index = w * (h / 2) + (rand() % w);
        }
        map_cells[current_index] = floor;
        ++floor_count;
    }
}
