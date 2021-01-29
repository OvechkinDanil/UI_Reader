#include "bufferParcer.h"

long int* FindLines(char** buf, long size, long int* offsetSize, long int* maxWidthLine, long int** _lengthLine)
{
  long i = 0, j = 0;
  long *lengthLine;
  long *offset;
  long sizeOff = 0;
  long maxWidth = -1;
  for (i = 0; i < size; i++)
  {
    if ((*buf)[i] == '\n' || (*buf)[i] == '\0' || i == size - 1)
      sizeOff += 1;
  }
  offset = malloc(sizeOff * sizeof(long));
  lengthLine = malloc(sizeOff * sizeof(long));
  for (i = 0; i < size; i++)
  {
    if ((*buf)[i] == '\n' || (*buf)[i] == '\0' || i == size - 1)
    {
        offset[j] = i;
        if (j == 0)
            lengthLine[j] = i;
        else
            lengthLine[j] = offset[j] - offset[j-1];

        if (lengthLine[j] > maxWidth)
            maxWidth = lengthLine[j];
        j += 1;
    }
  }
  *maxWidthLine = maxWidth;
  *offsetSize = sizeOff;
  *_lengthLine = lengthLine;
  return offset;
}
