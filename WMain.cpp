//Code listing for WMain.cpp
#include <windows.h>

extern LRESULT CALLBACK WindowF (HWND,UINT,WPARAM,LPARAM);
char szWinName[] = "MyWin";
char szTitle[] = "Cyrus Beck Line Clipping";

int WINAPI WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst,
                    LPSTR lpszArgs, int nWinMode)
{
  //Defining window class
  HWND hwnd;
  MSG msg;
  WNDCLASSEX wcl;

  wcl.cbClsExtra = 0;//extra bytes
  wcl.cbSize = sizeof(WNDCLASSEX); //size of the struct
  wcl.cbWndExtra = 0;//extra bytes
  wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
  wcl.hCursor = LoadCursor(NULL,IDC_CROSS);
  wcl.hIcon = LoadIcon(NULL,IDI_WINLOGO);
  wcl.hIconSm = NULL;
  wcl.hInstance = hThisInst;
  wcl.lpfnWndProc = WindowF;
  wcl.lpszClassName = szWinName;
  wcl.lpszMenuName = "MyMenu";
  wcl.style = 0;

  //Registration of class
  if(!RegisterClassEx(&wcl))
    return 0;

  //Create Window
  hwnd = CreateWindow(
    szWinName,
    szTitle,
    WS_OVERLAPPEDWINDOW|WS_SYSMENU,// no scroll bar
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    400, // specific value instead of CW_USEDEFAULT 
    300, // specific value instead of CW_USEDEFAULT 
    HWND_DESKTOP,
    NULL,
    hThisInst,
    NULL
   );

  ShowWindow(hwnd, SW_RESTORE);
  UpdateWindow(hwnd);

  //Serve Messages
  while(GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }//end while
  return msg.wParam;
}//end WinMain
