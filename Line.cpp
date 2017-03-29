#include <windows.h>

typedef struct
{
  HDC hdc;
  bool bVerticalLine;
  float m;
  long d, ine, inne;
  int linetype;
  float x, y;
} LINE_SEGMENT_DATA;

typedef struct
{
  HDC hdc;
  bool bVerticalLine;
  float m;
  int linetype;
  float x, y;
} SCANLINE_SEGMENT_DATA;

// global data for scan line drawing as contiguous segments 
SCANLINE_SEGMENT_DATA gscl;

// global data for Bresenham's line drawing as contiguous segments 
LINE_SEGMENT_DATA gbrl;

void calculateSlope(POINT start, POINT end, 
                    bool& bVerticalLine, float& m)
{
  if (end.x == start.x)
    bVerticalLine = true;
  else
  {
    m = ((float)(end.y - start.y))/((float)(end.x - start.x));
    bVerticalLine = false;
  }
}

void rearrangeOnX(long &sx, long &sy, long &ex, long &ey)
{
  long tempx, tempy;
  if(sx > ex)
  {
    tempx=ex;tempy=ey;
    ex=sx;ey=sy;
    sx=tempx;sy=tempy;
  }
}

void rearrangeOnY(long &sx, long &sy, long &ex, long &ey)
{ 
  long tempx, tempy;
  if(sy > ey)
  {
    tempx=ex;tempy=ey;
    ex=sx;ey=sy;
    sx=tempx;sy=tempy;
  }
}

// for Bresenham's line drawing
void setupLineSegmentDrawing(HDC hdc, POINT& start, POINT& end)
{
  // setup global line segment data for entire line stretch
  // start and end points of line get rearranged after the call

  long dx;
  long dy;

  gbrl.hdc = hdc;
  calculateSlope(start, end, gbrl.bVerticalLine, gbrl.m);

  if(gbrl.bVerticalLine)
  {
    rearrangeOnY(start.x,start.y,end.x,end.y);
    gbrl.linetype = 1;
  }
  else
  {
    // non-vertical line
    if((gbrl.m >= 0) && (gbrl.m <= 1))
    {
      rearrangeOnX(start.x,start.y,end.x,end.y);
      gbrl.linetype = 2;
    }
    else if((gbrl.m >= -1) && (gbrl.m < 0))
    {
      rearrangeOnX(start.x,start.y,end.x,end.y);
      gbrl.linetype = 3;
    }
    else if(gbrl.m > 1)
    {
      rearrangeOnY(start.x,start.y,end.x,end.y);
      gbrl.linetype = 4;
    }
    else
    {
      // gbrl.m < -1
      rearrangeOnY(start.x,start.y,end.x,end.y);
      gbrl.linetype = 5;
    }
  }

  dx = end.x - start.x;
  dy = end.y - start.y;  
  switch(gbrl.linetype)
  {
    case 2:
    case 3:
      if (dy < 0)
        dy = -dy;
      gbrl.d = 2*dy-dx;
      gbrl.ine = 2*dy;
      gbrl.inne = 2*(dy-dx);
      break;

    case 4:
    case 5:
      if (dx < 0)
        dx = -dx;
      gbrl.d = 2*dx-dy;
      gbrl.ine = 2*dx;
      gbrl.inne = 2*(dx-dy);
      break;
    
    default:
      break;
  }
  // set current position
  gbrl.x = (float) start.x;
  gbrl.y = (float) start.y;
}

bool findNextPtInLineSegment(POINT end, POINT& nextPt)
{
  // find next pt in the line segment from current pt 
  // upto and including end pt
  // return true if next point exists, false otherwise

  float ex, ey;

  ex = (float) end.x;
  ey = (float) end.y;

  nextPt.x = (int) gbrl.x;
  nextPt.y = (int) gbrl.y;

  switch(gbrl.linetype)
  {
    case 1:
      if (gbrl.y <= ey)
      {
        gbrl.y++;
        return true;
      }
      break;

    case 2:
    case 3:
      if (gbrl.x <= ex)
      {
        gbrl.x++;
        if (gbrl.d <= 0)
        {
          gbrl.d = gbrl.d + gbrl.ine;
        }
        else
        {
          gbrl.d = gbrl.d + gbrl.inne;
          if (gbrl.linetype == 2)
            gbrl.y++;
          else
            gbrl.y--;
        }
        return true;
      }
      break;

    case 4:
    case 5:
      if (gbrl.y <= ey)
      {
        gbrl.y++;
        if (gbrl.d <= 0)
        {
          gbrl.d = gbrl.d + gbrl.ine;
        }
        else
        {
          gbrl.d = gbrl.d + gbrl.inne;
          if (gbrl.linetype == 4)
            gbrl.x++;
          else
            gbrl.x--;
        }
        return true;
      }
      break;

    default:
      break;
  }
  return false;
}

void drawNextLineSegment(POINT end, COLORREF clr)
{
  // draw upto the end pt (including the end pt)
  // draws line segment from current position (gbrl.x, gbrl.y)
  // to (ex, ey)
  POINT ptNext;
  while (findNextPtInLineSegment(end, ptNext))
  {
    SetPixel(gbrl.hdc, ptNext.x, ptNext.y, clr);
  }
}

void drawLineSegment(HDC hdc, POINT start, POINT end, COLORREF clr)
{
  setupLineSegmentDrawing(hdc, start, end);
  drawNextLineSegment(end, clr);
}

// for scanline drawing
void setupScanLineSegmentDrawing(HDC hdc, POINT& start, POINT& end)
{
  // setup global line segment data for entire line stretch
  // start and end points of line get rearranged after the call

  gscl.hdc = hdc;
  calculateSlope(start, end, gscl.bVerticalLine, gscl.m);

  if((!gscl.bVerticalLine) && (abs((int)gscl.m)<=1))
  {
    rearrangeOnX(start.x,start.y,end.x,end.y);
    gscl.linetype = 1;
  }
  else
  {
    rearrangeOnY(start.x,start.y,end.x,end.y);
    gscl.linetype = 0;
  }
  // set current position
  gscl.x = (float) start.x;
  gscl.y = (float) start.y;
}

bool findNextPtInScanLineSegment(POINT end, POINT& nextPt)
{
  // find next pt in the line segment from current pt 
  // upto and including end pt
  // return true if next point exists, false otherwise
  float ex, ey;

  ex = (float) end.x;
  ey = (float) end.y;
  nextPt.x = (int) gscl.x;
  nextPt.y = (int) gscl.y;

  if(gscl.linetype == 1)
  {
    // |ey-sy| <= |ex-sx|
    if (gscl.x <= ex)
    {
      gscl.x++;
      gscl.y += gscl.m;
      return true;
    }
  }
  else
  {
    // ex==sx OR |ey-sy| >= |ex-sx| 
    // sx could be equal to ex
    if (gscl.y <= ey)
    {
      if (!gscl.bVerticalLine)
        gscl.x += 1/gscl.m;
      gscl.y++;
      return true;
    }
  }
  return false;
}

void drawNextScanLineSegment(POINT end, COLORREF clr)
{
  // draw upto the end pt (including the end pt)
  // draws line segment from current position (gbrl.x, gbrl.y)
  // to (ex, ey)
  POINT ptNext;
  while (findNextPtInScanLineSegment(end, ptNext))
  {
    SetPixel(gscl.hdc, ptNext.x, ptNext.y, clr);
  }
}

void drawScanLineSegment(HDC hdc, POINT start, POINT end, COLORREF clr)
{
  setupScanLineSegmentDrawing(hdc, start, end);
  drawNextScanLineSegment(end, clr);
}
