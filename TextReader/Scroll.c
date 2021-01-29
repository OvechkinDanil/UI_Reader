#include <windows.h>
#include "scroll.h"

void SetScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind)
{
    UpdateScroll(hwnd, wind, workWind);

    UpdateVScroll(wind, workWind);

    //ScrollWindow(hwnd, 0, -workWind->windowParam.cyChar * (workWind->scroll.iVscrollPos * workWind->scroll.coef - workWind->iFirstLine), NULL, NULL);

    SetScrollPos(hwnd, SB_VERT, workWind->scroll.iVscrollPos, TRUE);

    SetScrollPos(hwnd, SB_HORZ, workWind->scroll.iHscrollPos, TRUE);


}

void UpdateVScroll(windows_t* wind, workWindow_t* workWind)
{
    long i;
    i=FindNewFirstLine(wind, workWind);
    workWind->scroll.iVscrollPos = i / workWind->scroll.coef;
    workWind->iFirstLine=i;

}

void UpdateScroll(HWND hwnd, windows_t* wind, workWindow_t* workWind)
{
    int numLines = 0;
    long* lengthLine;

    if (workWind->mode == WRAP_ON)
    {

        numLines = workWind->offsetWithWrap.length;
        lengthLine = workWind->lengthLineWithWrap;
    }
    else
    {
        numLines = wind->text.offset.length;
        lengthLine = workWind->lengthLine;
    }


    if(numLines - workWind->windowParam.cyClient / workWind->windowParam.cyChar + 1 > 0xffff)
        workWind->scroll.coef = ceil(((long double)(numLines - workWind->windowParam.cyClient / workWind->windowParam.cyChar + 1) / 0xffff));
    else
        workWind->scroll.coef = 1;

    workWind->scroll.iVscrollMax = max(0, ceil((long double)(numLines - workWind->windowParam.cyClient/workWind->windowParam.cyChar + 1) / workWind->scroll.coef));
    workWind->scroll.iVscrollPos = min(workWind->scroll.iVscrollPos, workWind->scroll.iVscrollMax);

    SetScrollRange(hwnd, SB_VERT, 0, workWind->scroll.iVscrollMax, FALSE);
    SetScrollPos(hwnd, SB_VERT, workWind->scroll.iVscrollPos, TRUE);


    if (workWind->mode == WRAP_ON)
    {
        workWind->scroll.iHscrollPos = 0;
        workWind->scroll.iHscrollMax = 0;
        workWind->iFirstSymb = 0;
    }
    else
    {
        workWind->scroll.iHscrollPos = min(workWind->scroll.iHscrollPos, workWind->scroll.iHscrollMax);
        workWind->scroll.iHscrollMax = max(0,workWind->maxWidth - workWind->windowParam.cxClient / workWind->windowParam.cxChar);
    }

    SetScrollRange(hwnd, SB_HORZ, 0, workWind->scroll.iHscrollMax, FALSE);
    SetScrollPos(hwnd, SB_HORZ, workWind->scroll.iHscrollPos, TRUE);
}
