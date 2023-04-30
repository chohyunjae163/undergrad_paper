//https://eskerda.com/bsp-dungeon-generation/
//author: hjcho

// divides a surface into two for a number of Iterations
// each Iteration, a vertical or a horizontal partition randomly
// one room -> two room -> four rooms -> eight rooms -> sixteen rooms
// root -> two nodes -> four nodes -> eight nodes

#include <stdlib.h> //srand()
#include <stdint.h> //uint32_t
#include <assert.h> //assert()

const uint8_t MIN_WIDTH = 50;
const uint8_t MIN_HEIGHT = 50;
const uint8_t MAX_WIDTH = 50;
const uint8_t MAX_HEIGHT = 50;

const uint8_t PADDING = 25;
const uint8_t MARGIN = 50;

#define MAX(x,y) ((x) > (y) ? (x) : (y))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define MIN_MAX(x,min,max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

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
	
	if(ParentWidth <= MAX_WIDTH + MARGIN  ||
       ParentHeight <= MAX_HEIGHT + MARGIN ||
       Iteration < 0) {
		return;
	}
	
	if(Iteration == 0) {
        const uint32_t Width = rand() % (ParentWidth - MARGIN);
		const uint32_t RoomWidth  =  MIN_MAX(Width,MIN_WIDTH,MAX_WIDTH);
		assert(MIN_WIDTH <= RoomWidth && RoomWidth <= MAX_WIDTH);
        
        const uint32_t Height = rand() % (ParentHeight - MARGIN);
		const uint32_t RoomHeight =  MIN_MAX(Height,MIN_HEIGHT,MAX_HEIGHT);
		assert(MIN_HEIGHT <= RoomHeight && RoomHeight <= MAX_HEIGHT);
        
        const int32_t MaxPosX = ParentWidth - RoomWidth - MARGIN;
        assert(MaxPosX > 0);
        const int32_t RandPosX = rand() % MaxPosX;
		const int32_t RoomPosX = PADDING + RandPosX;
        
        const int32_t MaxPosY = ParentHeight - RoomHeight - MARGIN;
        assert(MaxPosY > 0);
        const int32_t RandPosY = rand() % MaxPosY;
		const int32_t RoomPosY = PADDING + RandPosY; 
        
        //4 bytes per pixel
		uint32_t *Bitmap = (uint32_t *) Parent;
		uint32_t *Begin = Bitmap + RoomPosX + (RoomPosY * DisplayWidth);
		
		for(int row = 0; row < RoomHeight;++row) {
			uint8_t *Pixel = (uint8_t *)(Begin + (DisplayWidth * row));
			for(int column = 0; column < RoomWidth; ++column) {
				*Pixel = 255;
				++Pixel;
				*Pixel = 0;
				++Pixel;
				*Pixel = 0;
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
            BinarySpacePartition(Begin, 
                                 HalfWidth, 
                                 ParentHeight, 
                                 Iteration - 1);
            //right
            BinarySpacePartition(Begin + (ParentWidth / 2), 
                                 HalfWidth, 
                                 ParentHeight,
                                 Iteration - 1);
        }else {
            const uint32_t HalfHeight = ParentHeight / 2;
            //top
            BinarySpacePartition(Begin,
                                 ParentWidth,
                                 HalfHeight,
                                 Iteration - 1);
            //bottom
            BinarySpacePartition(Begin + (ParentWidth * (ParentHeight / 2)),
                                 ParentWidth,
                                 HalfHeight,
                                 Iteration - 1);
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
