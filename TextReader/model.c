#include "model.h"


long maxWidth(long* lengthLine,long numLines)
{
    long maxWidth = -1;
    for (int i = 0; i < numLines; i++)
    {
        if (lengthLine[i] > maxWidth)
            maxWidth = lengthLine[i];
    }
    return maxWidth;
}


void InitModel(windows_t* wind, workWindow_t* wrkWind, TEXTMETRIC tm)
{

    wind->text.offset.OffsetString = FindLines(&wind->text.str, wind->text.length, &wind->text.offset.length, &wrkWind->maxWidth, &wrkWind->lengthLine);
    wrkWind->iFirstLine = 0;
    wrkWind->iFirstSymb = 0;
    wrkWind->mode = WRAP_OFF;

    wrkWind->windowParam.cxChar = tm.tmAveCharWidth;
    wrkWind->windowParam.cyChar = tm.tmHeight + tm.tmExternalLeading;

    wrkWind->WrapWidth = 0;

    wrkWind->lengthLineWithWrap = NULL;

    wrkWind->offsetWithWrap.OffsetString = NULL;
    wrkWind->offsetWithWrap.length = 0;

    wrkWind->scroll.iVscrollPos = 0;

    wrkWind->scroll.iHscrollPos = 0;

    wrkWind->iFirstChar = 1;

}
long FindNewFirstLine(windows_t* wind, workWindow_t* workWindow)
{
    long i;
    offset_t offsets;

    if (workWindow->mode == WRAP_ON)
        offsets = workWindow->offsetWithWrap;
    else
        offsets = wind->text.offset;

    if (workWindow->iFirstLine == 0)
        return 0;

    for (i = 0; offsets.OffsetString[i] <= workWindow->iFirstChar; i++);

    return i;
}



int makeWrap(windows_t* wind, workWindow_t* workWindow)
{
    long i, numChars = 0, numLines = 0;
    workWindow->mode = WRAP_ON;
    workWindow->WrapWidth = workWindow->windowParam.cxClient / workWindow->windowParam.cxChar;

    if(workWindow->offsetWithWrap.OffsetString != NULL)
        free(workWindow->offsetWithWrap.OffsetString);

    if (workWindow->lengthLineWithWrap != NULL)
        free(workWindow->lengthLineWithWrap);

    for(i = 0; i < wind->text.length; i++)
    {
        if(wind->text.str[i] == '\n' || wind->text.str[i] == '\0' || numChars == workWindow->WrapWidth)
        {
            numLines++;
            numChars=-1;
        }
        numChars++;
    }


    workWindow->offsetWithWrap.OffsetString=(long*)malloc((numLines + 1) * sizeof(long));

    workWindow->lengthLineWithWrap = (long*)malloc((numLines + 1) * sizeof(long));

    numChars=0;
    numLines=0;


    for(i=0; i<wind->text.length; i++)
    {
        if(wind->text.str[i]=='\n' || wind->text.str[i]=='\0' || numChars==workWindow->WrapWidth)
        {
            workWindow->offsetWithWrap.OffsetString[numLines]=i;

            if (numLines == 0)
                workWindow->lengthLineWithWrap[0] = workWindow->offsetWithWrap.OffsetString[0];
            else
                workWindow->lengthLineWithWrap[numLines] = workWindow->offsetWithWrap.OffsetString[numLines] - workWindow->offsetWithWrap.OffsetString[numLines-1];

            numChars=-1;
            numLines++;
        }
        numChars++;
    }

    workWindow->offsetWithWrap.OffsetString[numLines]=i;

    workWindow->offsetWithWrap.length = numLines + 1;

    workWindow->iFirstLine=FindNewFirstLine(wind, workWindow);

    return 0;
}

void FreeModel(windows_t* wind, workWindow_t* workWind)
{
     if (wind->text.str != NULL)
     {
        free(wind->text.str);
     }


    if (wind->text.offset.OffsetString != NULL)
        free(wind->text.offset.OffsetString);

    if (workWind->offsetWithWrap.OffsetString != NULL)
        free(workWind->offsetWithWrap.OffsetString);

    if (workWind->lengthLine != NULL)
        free(workWind->lengthLine);

    if (workWind->lengthLineWithWrap != NULL)
        free(workWind->lengthLineWithWrap);

}

void FreeWrapData(workWindow_t* workWind)
{
    if (workWind->offsetWithWrap.OffsetString != NULL)
    {
        free(workWind->offsetWithWrap.OffsetString);
        workWind->offsetWithWrap.OffsetString = NULL;
    }


    if (workWind->lengthLineWithWrap != NULL)
    {
        free(workWind->lengthLineWithWrap);
        workWind->lengthLineWithWrap = NULL;
    }

}
