#ifndef UNICODE
#define UNICODE
#endif

#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

void simple_room_placement(void* area, int32_t w, int32_t h){
  if(area == NULL){
    return;
  }

  if( w < 1 || h < 1 ){
    return;
  }

  LARGE_INTEGER ticks;
  QueryPerformanceCounter(&ticks);
  srand((unsigned int) ticks.QuadPart);
  const int32_t room_num = 6;
  int32_t current_room = 0;

  uint32_t floor = 255;
  for(uint32_t i = 0; i < 2;++i) {
      floor = floor << 8 | 255;
  }
  const uint32_t wall = 0;
  int32_t* map = (int32_t *)area;
  for(int i = 0; i < w * h; ++i) {
      map[i] = wall;
  }
  const int32_t max_room_length = 100;
  const int32_t min_room_length = 50;
  while (current_room < room_num) {
setup_room:
    int32_t room_width = rand() % max_room_length;
    if (room_width < min_room_length) {
      room_width = min_room_length;
    }
    int32_t room_height = rand() % max_room_length;
    if (room_height < min_room_length) {
      room_height = min_room_length;
    }
    const int32_t pos_x = rand() % w - 1;
    const int32_t pos_y = rand() % h - 1;
    const int32_t start_pos = pos_x + (w * pos_y);
    int32_t current_index = start_pos;
    for(int32_t r = 0; r < room_height; ++r){
      current_index += w;
      for(int32_t c = 0; c < room_width;++c){
        if(map[current_index + c] == floor){
         //overlap case. let's go back and setup a room again.
         goto setup_room;
        }
      }
    }
    //place a new room
    current_index = start_pos;
    for(int32_t r = 0; r < room_height; ++r){
      current_index += w;
      for(int32_t c = 0; c < room_width;++c){
        map[current_index + c] = floor;
      }
    }
    //make 4-way corridors
    ++current_room;
  }
}
