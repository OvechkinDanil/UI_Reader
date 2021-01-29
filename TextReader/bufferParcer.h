#include <stdio.h>
#include <malloc.h>
#ifndef BUFFERPARCER_H
#define BUFFERPARCER_H


/** \brief
 * ����������� �� ����� ������ � ������� ��� �������� �����
 * \param buf - ������, ��� �������� ��� �������
 * \param  size - ������ ����� ������
 * \return offsetSize - ������ ������� �������� ���� '\n'
 * \return maxWidthLine - ������������ ������ ������
 * \return _lengthLine - ������ ���� �����
   \return ������ �������� ���� '\n'
 */

long int* FindLines(char** buf, long size, long int* offsetSize, long int* maxWidthLine, long int** _lengthLine);
#endif // BUFFERPARCER_H
