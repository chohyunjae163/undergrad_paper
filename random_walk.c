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

void random_walk(void* cave, int32_t w, int32_t h){
    if(cave == NULL) {
        return;
    }

    if( w < 1 || h < 1) {
        return;
    }
    
    const uint32_t floor_num = (w * h) / 5;
    
    //wall - black
    //floor - white
    // all map cells to walls.
    const uint32_t wall = 0;
    uint32_t* map_cells = cave;
    for(int i = 0; i < w * h; ++i) {
        map_cells[i] = wall;
    }
    uint32_t floor = 255;
    for(uint32_t i = 0; i < 2;++i) {
        floor = floor << 8 | 255;
    }
    //the center as the starting point
    uint32_t starting_index = (( w * h ) / 2);
    //turn the cell into floor
    map_cells[starting_index] = floor;
    uint32_t floor_count = 1;
    while(floor_count < floor_num) {
        //take a random direction
        //++starting_point
        //--starting_point
        //starting_point + w;
        //starting_point - w;
        
        ++floor_count;
    }
}
