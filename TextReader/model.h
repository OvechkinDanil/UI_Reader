#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "bufferParcer.h"


/** \brief
 *  �������� ��� �������� ���������� �������
 * \param iVscrollPos -  ������� ������� ������������� �������
 * \param iVscrollMax - ������������ �������� ������������� �������
 * \param iHscrollPos - ������� ������� ��������������� �������
 * \param iHscrollMax - ������������ �������� ��������������� �������
 * \param coef - � ������� ������ ����������� ���� ����������� ��� ������
 */
typedef struct Scroll
{
    long iVscrollPos;
    long iVscrollMax;
    long iHscrollPos;
    long iHscrollMax;
    long coef;
} Scroll_t;


/** \brief
 *  ��������� ��� �������� ���������� ����
 * \param cxChar -  ������ �������
 * \param cyChar - ������ �������
 * \param cxClient - ������ ����
 * \param cyClient - ������ ����
 */
typedef struct WindowPar
{
   long cxChar;
   long cyChar;
   long cxClient;
   long cyClient;
} windowPar_t;


/** \brief
 *  ����� ������
 * \param WRAP_OFF -  ������� ���������
 * \param WRAP_ON - ������� ��������
 */
typedef enum mode
{
    WRAP_OFF,
    WRAP_ON
} mode_t;

/** \brief
 *  ��������� ��� ��������
 * \param OffsetString -  ������, � ������� �������� ������� '\n'
 * \param length - ������ �������
 */
typedef struct Offset
{
    long* OffsetString;
    long length;
} offset_t;


/** \brief
 *  ��������� ������
 * \param windowParam -  ��������� ��� �������� ���������� ����
 * \param iFirstLine - ����� ������ ������ ��� �����������
 * \param iFirstChar - ����� ������� ������� ��� �����������
 * \param maxWidth - ������ ������������ ������
 * \param WrapWidth - ������ ������ ��� �������
 * \param lengthLine - ������, � ������� �������� ����� �����
 * \param lengthLineWithWrap -  ������, � ������� �������� ����� �����( ������������ ������ ��� WRAP)
 * \param offsetWithWrap - ��������� ��� �������� ��� �������
 * \param mode - ��� ������
 * \param scroll - c������� ��� �������� ���������� �������

 */
typedef struct WorkWindow
{
    windowPar_t windowParam;
    long iFirstLine;
    long iFirstSymb;
    long maxWidth;
    long WrapWidth;
    long* lengthLine;
    long* lengthLineWithWrap;
    long iFirstChar;
    offset_t offsetWithWrap;
    mode_t mode;
    Scroll_t scroll;
} workWindow_t;


/** \brief
 *  ��������� ��� �������� ��������� ������
 * \param str -  ������, � ������� �������� ���������� �����
 * \param length - ������ �������
 * \param offset - ��������� ��� �������� ��� �������
 */
typedef struct Text
{
    char* str;
    long length;
    offset_t offset;
} text_t;


/** \brief
 *  ������� ���������
 * \param text -  ��������� ��� �������� ��������� ������
 * \param windows - ������ �������� ����
 */
typedef struct windows
{
    text_t text;
    workWindow_t* windows;
} windows_t;


/** \brief
 *  ������� ������� �������
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 * \return 0, ���� �� ���� ������� ������
 *
 */

int makeWrap(windows_t* wind, workWindow_t* workWindow);

/** \brief
 *  ������������� ������ �������� ����
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 * \param tm - ��������� TEXTMETRIC
 * \return
 *
 */

void InitModel(windows_t* wind, workWindow_t* wrkWind, TEXTMETRIC tm);


/** \brief
 * ���� ������ ������, ����� ����, ��� ��������� �������
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 * \return ����� ���������� ������
 *
 */

long FindNewFirstLine(windows_t* wind, workWindow_t* workWindow);

/** \brief
 * �� ���� ����� ���� ������������ ������ ������
 * \param lengthLine - ������, ������� ������ ����� �����
 * \param numLines - ���-�� ��������� � ���� �������
 * \return ������ ������������ ������
 *
 */

long maxWidth(long* lengthLine,long numLines);

/** \brief
 * ������� ��� ���������
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 * \return
 *
 */

void FreeModel(windows_t* wind, workWindow_t* workWind);

/** \brief
 * ������� ������ ����������� � �������(��� ������������)
 * \param wind -  ��������� �� ��������, ��� �������� ���� � ������������ �����
 * \param workWindow - ��������� �� ��������� �������� ����
 * \return
 *
 */

void FreeWrapData(workWindow_t* workWind);


#endif // MODEL_H
