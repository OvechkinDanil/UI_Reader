#ifndef MODEL_H
#define MODEL_H
#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include "bufferParcer.h"


/** \brief
 *  Стуктура для хранения параметров скролла
 * \param iVscrollPos -  текущая позиция вертикального скролла
 * \param iVscrollMax - максимальная поизиция вертикального скролла
 * \param iHscrollPos - текущая позиция горизонтального скролла
 * \param iHscrollMax - максимальная поизиция горизонтального скролла
 * \param coef - в больших файлах вычисляется этот коэффициент для скрола
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
 *  Структура для хранения параметров окна
 * \param cxChar -  ширина символа
 * \param cyChar - высота символа
 * \param cxClient - ширина окна
 * \param cyClient - высота окна
 */
typedef struct WindowPar
{
   long cxChar;
   long cyChar;
   long cxClient;
   long cyClient;
} windowPar_t;


/** \brief
 *  Режим чтения
 * \param WRAP_OFF -  верстка отключена
 * \param WRAP_ON - верстка включена
 */
typedef enum mode
{
    WRAP_OFF,
    WRAP_ON
} mode_t;

/** \brief
 *  Структура для смещений
 * \param OffsetString -  массив, в котором хранятся индексы '\n'
 * \param length - размер массива
 */
typedef struct Offset
{
    long* OffsetString;
    long length;
} offset_t;


/** \brief
 *  Структура вывода
 * \param windowParam -  Структура для хранения параметров окна
 * \param iFirstLine - номер первой строки для отображения
 * \param iFirstChar - номер первого символа для отображения
 * \param maxWidth - ширина максимальной строки
 * \param WrapWidth - ширина строки при верстке
 * \param lengthLine - массив, в котором хранятся длины строк
 * \param lengthLineWithWrap -  массив, в котором хранятся длины строк( используется только при WRAP)
 * \param offsetWithWrap - структура для смещений при верстке
 * \param mode - мод чтения
 * \param scroll - cтуктура для хранения параметров скролла

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
 *  Структура для хранения исходного текста
 * \param str -  массив, в котором хранится полученный текст
 * \param length - размер массива
 * \param offset - структура для смещений без верстки
 */
typedef struct Text
{
    char* str;
    long length;
    offset_t offset;
} text_t;


/** \brief
 *  Главная структура
 * \param text -  структура для хранения исходного текста
 * \param windows - массив открытых окон
 */
typedef struct windows
{
    text_t text;
    workWindow_t* windows;
} windows_t;


/** \brief
 *  Функция создает верстку
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 * \return 0, если не было никаких ошибок
 *
 */

int makeWrap(windows_t* wind, workWindow_t* workWindow);

/** \brief
 *  Инициализация модели рабочего окна
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 * \param tm - структура TEXTMETRIC
 * \return
 *
 */

void InitModel(windows_t* wind, workWindow_t* wrkWind, TEXTMETRIC tm);


/** \brief
 * Ищет первую строку, после того, как произошла верстка
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 * \return номер найденнной строки
 *
 */

long FindNewFirstLine(windows_t* wind, workWindow_t* workWindow);

/** \brief
 * Во всем файле ищет максимальную ширину строки
 * \param lengthLine - массив, который хранит длины строк
 * \param numLines - кол-во элементов в этом массиве
 * \return ширина максимальной строки
 *
 */

long maxWidth(long* lengthLine,long numLines);

/** \brief
 * Очищаем все структуры
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 * \return
 *
 */

void FreeModel(windows_t* wind, workWindow_t* workWind);

/** \brief
 * Очищаем данные относящиеся к верстке(при переключении)
 * \param wind -  указатель на стрктуру, где хранятся окна и отображаемый текст
 * \param workWindow - указатель на структуру рабочего окна
 * \return
 *
 */

void FreeWrapData(workWindow_t* workWind);


#endif // MODEL_H
