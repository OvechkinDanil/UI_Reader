#include <stdio.h>
#include <malloc.h>
#ifndef BUFFERPARCER_H
#define BUFFERPARCER_H


/** \brief
 * Пробегается по всему буферу и находит все переводы строк
 * \param buf - буффер, где хранятся все символы
 * \param  size - размер этого буфера
 * \return offsetSize - размер массива индексов всех '\n'
 * \return maxWidthLine - максимальная ширина строки
 * \return _lengthLine - массив длин строк
   \return массив индексов всех '\n'
 */

long int* FindLines(char** buf, long size, long int* offsetSize, long int* maxWidthLine, long int** _lengthLine);
#endif // BUFFERPARCER_H
