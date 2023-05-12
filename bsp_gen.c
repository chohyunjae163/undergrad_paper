//https://eskerda.com/bsp-dungeon-generation/ author: hjcho

// divides a surface into two for a number of Iterations
// each Iteration, a vertical or a horizontal partition randomly
// one room -> two room -> four rooms -> eight rooms -> sixteen rooms
// root -> two nodes -> four nodes -> eight nodes

#include <stdlib.h> //srand()
#include <stdint.h> //uint32_t
#include <assert.h> //assert()
#include <stdbool.h> //boolean
#include <windows.h>

#define MAX(x,y) ((x) > (y) ? (y) : (x))
#define MIN(x,y) ((x) < (y) ? (y) : (x))
#define MIN_MAX(x,min,max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))

#define internal static

internal const uint32_t MIN_ROOM_WIDTH = 100;
internal const uint32_t MIN_ROOM_HEIGHT = 100;

internal const int32_t PADDING = 15;
internal const int32_t MARGIN = 30;
internal const int32_t CORRIDOR_WIDTH = 25;
internal const int32_t CORRIDOR_HEIGHT = 25;
internal uint32_t DisplayWidth;
internal uint32_t DisplayHeight;

struct tag_position {
    int32_t X;
    int32_t Y;
};

internal struct tag_position
BinarySpacePartition(void *Parent, 
                     const uint32_t ParentWidth, 
                     const uint32_t ParentHeight) {
    
    if(Parent == NULL) {
        return ((struct tag_position){.X=-1,.Y=-1});
    }

    if(ParentWidth <= (MIN_ROOM_WIDTH * 2)  ||
       ParentHeight <= (MIN_ROOM_HEIGHT * 2)) {

        const uint32_t MaxWidth = ParentWidth - MARGIN;
        const uint32_t RandomWidth = rand() % MaxWidth;
        const uint32_t RoomWidth  =  MIN(RandomWidth,MIN_ROOM_WIDTH);
        assert(MIN_ROOM_WIDTH <= RoomWidth && RoomWidth <= MaxWidth);

        const uint32_t MaxHeight = ParentHeight - MARGIN;
        const uint32_t RandomHeight = rand() % MaxHeight;
        const uint32_t RoomHeight =  MIN(RandomHeight,MIN_ROOM_HEIGHT);
        assert(MIN_ROOM_HEIGHT <= RoomHeight && RoomHeight <= MaxHeight);

        const int32_t MaxPosX = ParentWidth - RoomWidth - PADDING;
        assert(MaxPosX > 0);
        const int32_t RoomPosX = MIN(PADDING,rand() % MaxPosX);

        const int32_t MaxPosY = ParentHeight - RoomHeight - PADDING;
        assert(MaxPosY > 0);
        const int32_t RoomPosY = MIN(PADDING,rand() % MaxPosY);

        //4 bytes per pixel
        uint32_t *Bitmap = (uint32_t *) Parent;
        uint32_t *Begin = Bitmap + RoomPosX + (RoomPosY * DisplayWidth);
        int8_t Color_r = rand() % 255;
        int8_t Color_g = rand() % 255;
        int8_t Color_b = rand() % 255;
        for(uint32_t row = 0; row < RoomHeight;++row) {
            uint8_t *Pixel = (uint8_t *)(Begin + (DisplayWidth * row));
            for(uint32_t column = 0; column < RoomWidth; ++column) {
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
        return ((struct tag_position){ .X=RoomPosX + (RoomWidth / 2), .Y=RoomPosY + (RoomHeight / 2)});
    }
    //partition - divide a room into two rooms
    {
        const int r = rand() % 100000;
        bool bSplitVertical = r % 2 == 0 ? true : false;
        uint32_t *Begin = Parent;
        if(bSplitVertical){
            const uint32_t HalfWidth = ParentWidth / 2;
            //left
            struct tag_position LeftRoomCenterPos = BinarySpacePartition(Begin,
                                 HalfWidth, 
                                 ParentHeight);
            const int32_t CorridorHeight = 30;
            if(LeftRoomCenterPos.Y + LeftRoomCenterPos.X > 0){
                const int32_t CorridorWidth = HalfWidth - LeftRoomCenterPos.X;
                const int32_t* CorridorBegin = Begin + LeftRoomCenterPos.X +
                    ((LeftRoomCenterPos.Y - CorridorHeight) * DisplayWidth);
                const int32_t* CorridorEnd = CorridorBegin + CorridorWidth + CorridorHeight * DisplayWidth;
                for(uint32_t r = 0; r < CorridorHeight; ++r){
                    uint8_t * Pixel = (uint8_t *) (CorridorBegin + (DisplayWidth * r));
                    for(uint32_t c = 0; c < CorridorWidth;++c){
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
            }
            //right
            struct tag_position RightRoomCenterPos = BinarySpacePartition(Begin + HalfWidth,
                                 HalfWidth, 
                                 ParentHeight);
            {
                if(RightRoomCenterPos.X + RightRoomCenterPos.Y > 0){
                    const int32_t* RightCorridorBegin = Begin + HalfWidth - (CorridorHeight/2) +
                        ((RightRoomCenterPos.Y - CorridorHeight) * DisplayWidth );
                    const int32_t RightCorridorWidth = RightRoomCenterPos.X;
                    for(uint32_t r = 0; r < CorridorHeight; ++r){
                        uint8_t * Pixel =(uint8_t*) (RightCorridorBegin + (DisplayWidth * r));
                        for(uint32_t c = 0; c < RightCorridorWidth;++c){
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
                    //vertical corridor
                    const int32_t VerticalCorridorX = HalfWidth;
                    const int32_t VerticalCorridorY = RightRoomCenterPos.Y < LeftRoomCenterPos.Y ?
                        RightRoomCenterPos.Y : LeftRoomCenterPos.Y;
                    int32_t VerticalCorridorHeight = RightRoomCenterPos.Y - LeftRoomCenterPos.Y;
                    if(VerticalCorridorHeight < 0){
                        VerticalCorridorHeight *= -1;
                    }
                    const int32_t* VerticalCorridorBegin = Begin + VerticalCorridorX -
                        (CorridorHeight/2) + ((VerticalCorridorY - CorridorHeight) * DisplayWidth);
                    for(uint32_t r = 0; r < VerticalCorridorHeight; ++r){
                        uint8_t * Pixel =(uint8_t*) (VerticalCorridorBegin + (DisplayWidth * r));
                        for(uint32_t c = 0; c < CorridorHeight;++c){
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
                }
            }
        } else {
            const uint32_t HalfHeight = ParentHeight / 2;
            //top
            BinarySpacePartition(Begin,
                                 ParentWidth,
                                 HalfHeight);
            //bottom
            BinarySpacePartition(Begin + (DisplayWidth * HalfHeight),
                                 ParentWidth,
                                 HalfHeight);
        }

    }
    return ((struct tag_position){ParentWidth/2,ParentHeight/2});
}

void
GenerateBSPRooms(void* Parent,
                 const uint32_t InDisplayWidth,
                 const uint32_t InDisplayHeight)
{
    LARGE_INTEGER ticks;
    QueryPerformanceCounter(&ticks);
    srand((unsigned int)ticks.QuadPart);
    DisplayWidth = InDisplayWidth;
    DisplayHeight = InDisplayHeight;
    BinarySpacePartition(Parent,DisplayWidth,DisplayHeight);
}
