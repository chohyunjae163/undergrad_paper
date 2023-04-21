//https://eskerda.com/bsp-dungeon-generation/
//author: hjcho

// divides a surface into two for a number of Iterations
// each Iteration, a vertical or a horizontal partition randomly
// one room -> two room -> four rooms -> eight rooms -> sixteen rooms
// root -> two nodes -> four nodes -> eight nodes

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#define MIN_ROOM_WIDTH 100
#define MIN_ROOM_HEIGHT 100
#define PADDING 10

#define MAX(x,y) ((x) > (y) ? (x) : (y))

#define internal static


internal uint32_t DisplayWidth;
internal uint32_t DisplayHeight;

internal void 
BinarySpacePartition(void *Parent, 
                     const uint32_t ParentWidth, 
                     const uint32_t ParentHeight, 
                     const uint32_t Iteration) {
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    srand(ticks.QuadPart);
    
	if(Parent == NULL) {
		return;
	}
	
	if(ParentWidth < MIN_ROOM_WIDTH ||
       ParentHeight < MIN_ROOM_HEIGHT ||
       Iteration < 0) {
		return;
	}
	
	if(Iteration == 0){
		const uint32_t Margin = PADDING * 2;
		const uint32_t PossibleWidth = rand() % (ParentWidth - Margin);
		const uint32_t RoomWidth  =  MAX(MIN_ROOM_WIDTH  , PossibleWidth);
		assert(RoomWidth >= MIN_ROOM_WIDTH);
		const uint32_t RoomHeight =  MAX(MIN_ROOM_HEIGHT , rand() % (ParentHeight - Margin));
		assert(RoomHeight >= MIN_ROOM_HEIGHT);
		const uint32_t RoomPositionX = PADDING + (rand() % (ParentWidth - Margin - RoomWidth));
		const uint32_t RoomPositionY = PADDING + (rand() % (ParentHeight - Margin - RoomHeight));
				
        //4 bytes per pixel
		uint32_t *Bitmap = (uint32_t *) Parent;
		uint32_t *Begin = Bitmap + RoomPositionX + (RoomPositionY * DisplayWidth);
		
		for(int row = 0; row < RoomHeight;++row) {
			uint8_t *Pixel = (uint8_t *)(Begin + (DisplayWidth * row));
			for(int column = 0; column < RoomWidth; ++column) {
				*Pixel = 255;
				++Pixel;
				*Pixel = 255;
				++Pixel;
				*Pixel = 255;
				++Pixel;
				*Pixel = 0;
				++Pixel;				
			}
		}
        
        return;
	}
    
    //partition - divide a room into two rooms
    {
        bool bSplitVertical = rand() % 2;
        uint32_t *Begin = Parent;
        if(bSplitVertical){
            const uint32_t HalfWidth = ParentWidth / 2;
            //left
            BinarySpacePartition(Begin, HalfWidth, ParentHeight, Iteration - 1);
            //right
            BinarySpacePartition(Begin + (ParentWidth / 2), HalfWidth, ParentHeight,Iteration - 1);
        }else {
            const uint32_t HalfHeight = ParentHeight / 2;
            //top
            BinarySpacePartition(Begin,ParentWidth, HalfHeight, Iteration - 1);
            //bottom
            BinarySpacePartition(Begin + (ParentWidth * (ParentHeight / 2)),ParentWidth,HalfHeight, Iteration - 1);
        }
    }
}

void
GenerateBSPRooms(void* Parent,
                 const uint32_t InDisplayWidth,
                 const uint32_t InDisplayHeight,
                 const uint32_t Iteration)
{
    DisplayWidth = InDisplayWidth;
    DisplayHeight = InDisplayHeight;
    BinarySpacePartition(Parent,DisplayWidth,DisplayHeight,Iteration);
}