void calculateSlope(POINT start, POINT end, 
                    bool& bVerticalLine, float& m);
// For Bresenham's line drawing 
void drawLineSegment(HDC hdc, POINT start, POINT end, COLORREF clr);
void drawNextLineSegment(POINT end, COLORREF clrLine);
void setupLineSegmentDrawing(HDC hdc, POINT& start, POINT& end);
bool findNextPtInLineSegment(POINT end, POINT& nextPt);
// For scan line drawing 
void drawScanLineSegment(HDC hdc, POINT start,POINT end,COLORREF clr);
void drawNextScanLineSegment(POINT end, COLORREF clr);
void setupScanLineSegmentDrawing(HDC hdc, POINT& start, POINT& end);
bool findNextPtInScanLineSegment(POINT end, POINT& nextPt);
