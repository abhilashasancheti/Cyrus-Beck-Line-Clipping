#define ID_DRAW_RECTANGLE 40001
#define ID_CLEAR          40002
#define ID_EXIT           40003
#define ID_DRAW_LINE      40004
#define ID_CLIP           40005

const COLORREF CLR_BG = RGB(255,255,255);
const COLORREF CLR_RECT = RGB(255, 0, 0);
const COLORREF CLR_LINE = RGB(0, 0, 255);
const COLORREF CLR_B = RGB(255,0,0); 
typedef enum
{
  READY_MODE,
  DRAW_RECTANGLE_MODE,
  DRAW_LINE_MODE,
  CLIP_MODE,
  CLIPPED_MODE
} MODE;

typedef struct
{
  HDC hdcMem; 
  HBITMAP hbmp;
  MODE drawMode;
  POINT beginPt, endPt;
  POINT rectCornerPts[2];
  POINT clipMin, clipMax;
  POINT lineEndPts[2];
  SIZE maxBoundary;
} DRAWING_DATA;

extern DRAWING_DATA gDrawData; // global data

void reDraw(HWND hwnd);
void setDrawMode(MODE mode, HWND hwnd);
void  performCorrectionAtClipPts(HDC hdc, 
  POINT clipPt, COLORREF clrLine, COLORREF clrErase);
void drawRectangle(HDC hdc, POINT corner1, POINT corner2, 
                   COLORREF clr);
void clip(HWND hwnd);
bool isPointWithinClipBoundary(POINT pt);
bool isLineWithinClipBoundary(POINT start, POINT end);
bool isLineOutsideClipBoundary(POINT start, POINT end);
void computeClipDimensions();
