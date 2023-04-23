#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <stdbool.h>
#include "bsp_gen.c"

#define DISPLAY_WIDTH 800
#define DISPLAY_HEIGHT 600
#define BITMAP_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT
#define BITS_PER_PIXEL 32
#define BITS_PER_BYTE 8
#define BYTES_PER_PIXEL BITS_PER_PIXEL / BITS_PER_BYTE
#define BITMAP_MEMORY_SIZE_IN_BYTES BITMAP_SIZE * BYTES_PER_PIXEL

static bool bIsRunning = true;
static void* BitmapMemory;
BITMAPINFO BitmapInfo = { 0 } ;
static long ClientWidth = 0;
static long ClientHeight = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    
    // Register the window class.
    const wchar_t CLASS_NAME[]    = L"MyProgram";
	const wchar_t CLASS_TEXT[]	= L"RDE";
    
    WNDCLASS wc = { 0 };
    
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    RegisterClass(&wc);
    
    // Create the window.
    
    HWND hwnd = CreateWindowEx(
                               0,                              // Optional window styles.
                               CLASS_NAME,                     // Window class
                               CLASS_TEXT,    					// Window text
                               WS_OVERLAPPEDWINDOW,            // Window style
                               
                               // Size and position
                               CW_USEDEFAULT, CW_USEDEFAULT, DISPLAY_WIDTH + 50, DISPLAY_HEIGHT + 50,
                               
                               NULL,       // Parent window    
                               NULL,       // Menu
                               hInstance,  // Instance handle
                               NULL        // Additional application data
                               );
    
    if (hwnd == NULL){
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    
	//initial setup
	
	BITMAPINFOHEADER BitmapInfoHeader = { 0 };
	BitmapInfoHeader.biSize = sizeof(BitmapInfoHeader);
	BitmapInfoHeader.biWidth = DISPLAY_WIDTH;
	BitmapInfoHeader.biHeight = -DISPLAY_HEIGHT;
	BitmapInfoHeader.biPlanes = 1; // Specifies the number of planes for the target device. This value must be set to 1.
	BitmapInfoHeader.biBitCount = BITS_PER_PIXEL; //Specifies the number of bits per pixel (bpp). 
	BitmapInfoHeader.biCompression = BI_RGB;
	BitmapInfo.bmiHeader = BitmapInfoHeader;
	
	BitmapMemory  = VirtualAlloc(
                                 NULL, // If this parameter is NULL, the system determines where to allocate the region.
                                 BITMAP_MEMORY_SIZE_IN_BYTES,//The size of the region, in bytes. 
                                 MEM_RESERVE | MEM_COMMIT,// DWORD  flAllocationType,
                                 PAGE_READWRITE);// DWORD  flProtect
    
	memset(BitmapMemory,0,BITMAP_MEMORY_SIZE_IN_BYTES);
    // Run the message loop.
    
    RECT ClientRect;
    GetClientRect(hwnd,&ClientRect);
	ClientWidth = ClientRect.right - ClientRect.left;
    ClientHeight = ClientRect.bottom - ClientRect.top;
	while(bIsRunning){
		MSG Message = { 0 };
		while(PeekMessage(&Message,hwnd,0,0,PM_REMOVE)){
			if(Message.message == WM_QUIT){
				bIsRunning = false;
			}
	        TranslateMessage(&Message);
			DispatchMessage(&Message);			
		}
	}
	
    VirtualFree(
                BitmapMemory,
                0, //If the dwFreeType parameter is MEM_RELEASE, this parameter must be 0 (zero).
                MEM_RELEASE);
    
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    
    switch (uMsg){
		case WM_KEYDOWN:{
            //Key Code 'A'
			if(wParam == 0x41){
				memset(BitmapMemory,126,BITMAP_MEMORY_SIZE_IN_BYTES);
                const int Iteration = 3;
				GenerateBSPRooms(BitmapMemory,
                                 DISPLAY_WIDTH,
                                 DISPLAY_HEIGHT,
                                 Iteration);
				InvalidateRect(hwnd,NULL,TRUE);
			}
		} break;
		case WM_CLOSE:{
			bIsRunning = false;
		} break;
		
		case WM_DESTROY:{
			bIsRunning = false;
		} break;
        
		case WM_PAINT:{
			PAINTSTRUCT ps;
			HDC DeviceContext = BeginPaint(hwnd, &ps);
            HBRUSH hBrush = CreateSolidBrush(RGB(0,200,0));
            RECT rect;
            FillRect(DeviceContext, &rect, hBrush);
			// All painting occurs here, between BeginPaint and EndPaint.
            
			StretchDIBits(
                          DeviceContext,
                          15,//xDest,
                          5,//yDest,
                          DISPLAY_WIDTH,//DestWidth,
                          DISPLAY_HEIGHT,//DestHeight,
                          0,//xSrc,
                          0,//ySrc,
                          DISPLAY_WIDTH,//SrcWidth,
                          DISPLAY_HEIGHT,//SrcHeight,
                          BitmapMemory,//const VOID       *lpBits,
                          &BitmapInfo,//const BITMAPINFO *lpbmi,
                          DIB_RGB_COLORS,
                          SRCCOPY //raster operation code
                          );
			
			
			EndPaint(hwnd, &ps);
		} break;
        
    }
	
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
