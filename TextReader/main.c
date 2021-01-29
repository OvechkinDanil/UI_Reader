#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "model.h"
#include "bufferParcer.h"
#include <locale.h>
#include <math.h>
#include "MyMenu.h"
#include "scroll.h"
#include "model.h"


/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;           /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = MAKEINTRESOURCE(MyMenu);                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("UI Lab"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           640,                 /* The programs width */
           480,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           lpszArgument                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    long iVscrollInc, iHscrollInc;
    long i = 0, begin, end;
    static long *lengthLineArr = NULL;
    static long *offsets = NULL, lengthOff = -1;
    long startLine = 0;
    long y, x;

    static RECT rect;
    static HMENU hMenu;
    CREATESTRUCT* myStruct = NULL;
    HDC hdc;
    static PAINTSTRUCT p;
    static TEXTMETRIC tm;
    static workWindow_t workWindow;
    static windows_t wind;
    static HFONT hfont;


    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            FreeModel(&wind, &workWindow);
            free(wind.windows);
            offsets = NULL;
            lengthLineArr = NULL;
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        case WM_CREATE: {

            myStruct = (CREATESTRUCT *) lParam;

            hdc = GetDC(hwnd);
            SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
            hMenu = GetMenu(hwnd);
            GetTextMetrics(hdc, &tm);

            wind.windows = malloc(sizeof(workWindow_t));
            workWindow = wind.windows[0];


            ReleaseDC(hwnd, hdc);
            char* filename = myStruct->lpCreateParams;
            if (strcmp(filename, "") != 0)
            {
                if (ReadTextFromFile(filename, &wind.text.str, &wind.text.length))
                {
                    InitModel(&wind, &workWindow, tm);
                    lengthOff = wind.text.offset.length;
                    lengthLineArr = workWindow.lengthLine;
                    offsets = wind.text.offset.OffsetString;

                    CheckMenuItem(hMenu, IDM_WRAP_OFF, MF_CHECKED);
                    CheckMenuItem(hMenu, IDM_WRAP_ON, MF_UNCHECKED);
                    EnableMenuItem(hMenu, IDM_WRAP_OFF, MF_DISABLED);
                    EnableMenuItem(hMenu, IDM_WRAP_ON, MF_ENABLED);

                    InvalidateRect(hwnd, 0, TRUE);
                    UpdateWindow(hwnd);
                }
                else
                    PostMessage(hwnd, WM_CLOSE, 0, 0L);
            }
            else
            {
                InitModel(&wind, &workWindow, tm);
                EnableMenuItem(hMenu, IDM_WRAP_ON, MF_DISABLED);
                EnableMenuItem(hMenu, IDM_WRAP_OFF, MF_DISABLED);
                EnableMenuItem(hMenu, IDM_EXIT, MF_DISABLED);
            }

            break;
        }
        case WM_SIZE :
            workWindow.windowParam.cxClient = LOWORD(lParam) - workWindow.windowParam.cxChar;
            workWindow.windowParam.cyClient = HIWORD(lParam);

            if (workWindow.mode == WRAP_ON)
            {
                makeWrap(&wind, &workWindow);
                offsets = workWindow.offsetWithWrap.OffsetString;
                lengthLineArr = workWindow.lengthLineWithWrap;
                lengthOff = workWindow.offsetWithWrap.length;
                InvalidateRect(hwnd, 0, TRUE);
                UpdateWindow(hwnd);
            }

            SetScroll(hwnd, &wind, &workWindow);

            break;

        case WM_VSCROLL:
            switch(LOWORD(wParam))
            {
            case SB_TOP:
                iVscrollInc = -workWindow.scroll.iVscrollPos;
                break;
            case SB_BOTTOM:
                iVscrollInc = workWindow.scroll.iVscrollMax - workWindow.scroll.iVscrollPos;
                break;
            case SB_LINEUP:
                iVscrollInc = -1;
                break;
            case SB_LINEDOWN:
                iVscrollInc = 1;
                break;
            case SB_PAGEUP:
                iVscrollInc = min(-1, -workWindow.windowParam.cyClient / workWindow.windowParam.cyChar);
                break;
            case SB_PAGEDOWN:
                iVscrollInc = max(1, workWindow.windowParam.cyClient / workWindow.windowParam.cyChar);
                break;
            case SB_THUMBTRACK:
                iVscrollInc = HIWORD(wParam) - workWindow.scroll.iVscrollPos;
                break;
            default :
                iVscrollInc = 0;
            }
            iVscrollInc = max(-workWindow.scroll.iVscrollPos,min(iVscrollInc, workWindow.scroll.iVscrollMax - workWindow.scroll.iVscrollPos));
            if (iVscrollInc != 0)
            {
                workWindow.scroll.iVscrollPos += iVscrollInc;
                ScrollWindow(hwnd, 0, -workWindow.windowParam.cyChar * (workWindow.scroll.iVscrollPos * workWindow.scroll.coef - workWindow.iFirstLine), NULL, NULL);
                SetScrollPos(hwnd, SB_VERT, workWindow.scroll.iVscrollPos, TRUE);

                workWindow.iFirstLine = workWindow.scroll.iVscrollPos * workWindow.scroll.coef;
                workWindow.iFirstChar = offsets[workWindow.iFirstLine - 1];

                UpdateWindow(hwnd);
            }
            break;
        case WM_HSCROLL :
            switch(LOWORD(wParam))
            {
                case SB_LINEUP :
                    iHscrollInc = -1;
                    break;
                case SB_LINEDOWN :
                    iHscrollInc = 1;
                    break;
                case SB_PAGEUP :
                    iHscrollInc = -8;
                    break;
                case SB_PAGEDOWN :
                    iHscrollInc = 8;
                    break;
                case SB_THUMBTRACK:
                    iHscrollInc = HIWORD(wParam) - workWindow.scroll.iHscrollPos;
                    break;
                default :
                    iHscrollInc = 0;
            }
            iHscrollInc = max(
                    -workWindow.scroll.iHscrollPos,
                    min(iHscrollInc, workWindow.scroll.iHscrollMax - workWindow.scroll.iHscrollPos)
            );
            if (iHscrollInc != 0)
            {
                workWindow.scroll.iHscrollPos += iHscrollInc;
                ScrollWindow(hwnd, -workWindow.windowParam.cxChar * iHscrollInc, 0, NULL, NULL);
                SetScrollPos(hwnd, SB_HORZ, workWindow.scroll.iHscrollPos, TRUE);
                workWindow.iFirstSymb = workWindow.scroll.iHscrollPos;

                InvalidateRect(hwnd, 0, TRUE);
            }
            return 0;
        case WM_PAINT:
            {
                hdc = BeginPaint(hwnd, &p);
                if (offsets == NULL || lengthOff == -1 || lengthLineArr == NULL)
                    break;

                begin = max(0, workWindow.iFirstLine);
                end = min(lengthOff, workWindow.iFirstLine + workWindow.windowParam.cyClient / workWindow.windowParam.cyChar  + 1);

                if (workWindow.mode == WRAP_ON)
                    end = end -1;

                for (i = begin; i < end; i++)
                {
                    if (i == 0)
                        startLine = 0;
                    else
                        startLine = offsets[i - 1];

                    x = workWindow.windowParam.cxChar * (0 - workWindow.iFirstSymb);
                    y = workWindow.windowParam.cyChar * (i - workWindow.iFirstLine);

                    TextOut(hdc, x, y, wind.text.str + startLine, lengthLineArr[i]);
                }
                EndPaint(hwnd, &p);
                break;
            }

        case WM_KEYDOWN:
            {
                switch(wParam)
                {
                case VK_PRIOR :
                    SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0L);
                    break;
                case VK_NEXT :
                    SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0L);
                    break;
                case VK_UP :
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0L);
                    break;
                case VK_DOWN :
                    SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0L);
                    break;
                case VK_LEFT :
                    SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0L);
                    break;
                case VK_RIGHT :
                    SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0L);
                    break;
                }
            }

        case WM_COMMAND :
        {
            HMENU hMenu;

            switch(LOWORD(wParam))
            {
                case IDM_OPEN :
                {

                    OPENFILENAME ofn;
                    char buff[260];

                    ZeroMemory(&ofn, sizeof(ofn));
                    ofn.lStructSize = sizeof(ofn);
                    ofn.hwndOwner = hwnd;
                    ofn.lpstrFile = buff;
                    ofn.lpstrFile[0] = '\0';
                    ofn.nMaxFile = sizeof(buff);
                    ofn.lpstrFilter = "Text\0*.TXT\0";
                    ofn.nFilterIndex = 1;
                    ofn.lpstrFileTitle = NULL;
                    ofn.nMaxFileTitle = 0;
                    ofn.lpstrInitialDir = NULL;
                    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

                    if (GetOpenFileName(&ofn) == TRUE)
                    {

                        FreeModel(&wind, &workWindow);

                        if (ReadTextFromFile(ofn.lpstrFile, &wind.text.str, &wind.text.length)) {

                            InitModel(&wind, &workWindow, tm);

                            lengthOff = wind.text.offset.length;
                            lengthLineArr = workWindow.lengthLine;
                            offsets = wind.text.offset.OffsetString;

                            GetClientRect(hwnd, &rect);


                            InvalidateRect(hwnd, 0, TRUE);
                            UpdateWindow(hwnd);

                            SetScroll(hwnd, &wind, &workWindow);

                            hMenu=GetMenu(hwnd);
                            CheckMenuItem(hMenu, IDM_WRAP_OFF, MF_CHECKED);
                            CheckMenuItem(hMenu, IDM_WRAP_ON, MF_UNCHECKED);
                            EnableMenuItem(hMenu, IDM_WRAP_OFF, MF_DISABLED);
                            EnableMenuItem(hMenu, IDM_WRAP_ON, MF_ENABLED);
                            EnableMenuItem(hMenu, IDM_EXIT, MF_ENABLED);
                        }
                        else
                            PostMessage(hwnd, WM_CLOSE, 0, 0L);
                    }

                    return 0;
                }

                case IDM_WRAP_ON :

                    if (workWindow.iFirstLine != 0)
                        workWindow.iFirstChar = wind.text.offset.OffsetString[workWindow.iFirstLine - 1];

                    FreeWrapData(&workWindow);

                    makeWrap(&wind, &workWindow);
                    SetScroll(hwnd, &wind, &workWindow);


                    offsets = workWindow.offsetWithWrap.OffsetString;
                    lengthLineArr = workWindow.lengthLineWithWrap;
                    lengthOff = workWindow.offsetWithWrap.length;
                    InvalidateRect(hwnd, 0, TRUE);
                    UpdateWindow(hwnd);

                    hMenu=GetMenu(hwnd);
                    CheckMenuItem(hMenu, IDM_WRAP_ON, MF_CHECKED);
                    CheckMenuItem(hMenu, IDM_WRAP_OFF, MF_UNCHECKED);
                    EnableMenuItem(hMenu, IDM_WRAP_ON, MF_DISABLED);
                    EnableMenuItem(hMenu, IDM_WRAP_OFF, MF_ENABLED);
                    return 0;

                case IDM_WRAP_OFF :

                    workWindow.mode = WRAP_OFF;

                    if (workWindow.iFirstLine != 0)
                        workWindow.iFirstChar = workWindow.offsetWithWrap.OffsetString[workWindow.iFirstLine - 1];

                    SetScroll(hwnd, &wind, &workWindow);

                    FreeWrapData(&workWindow);

                    offsets = wind.text.offset.OffsetString;
                    lengthOff = wind.text.offset.length;
                    lengthLineArr = workWindow.lengthLine;

                    InvalidateRect(hwnd, 0, TRUE);
                    UpdateWindow(hwnd);

                    hMenu=GetMenu(hwnd);

                    CheckMenuItem(hMenu, IDM_WRAP_OFF, MF_CHECKED);
                    CheckMenuItem(hMenu, IDM_WRAP_ON, MF_UNCHECKED);
                    EnableMenuItem(hMenu, IDM_WRAP_OFF, MF_DISABLED);
                    EnableMenuItem(hMenu, IDM_WRAP_ON, MF_ENABLED);
                    return 0;
                case IDM_EXIT:
                    SendMessage(hwnd, WM_CLOSE, 0, 0L);
                    return 0;
            }
            break;
        }
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
