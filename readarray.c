/* 
 * Author: Anton Filimonov (anton.filimonov@gmail.com)
 * Year: 2012
 * 
 * 
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain worldwide.
 *
 * This software is distributed without any warranty.
 * You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see 
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <stdio.h>
#include <stdlib.h>

int countLines(FILE* file)
{
  int lines = 0;
  char buf[256];
  while(fgets(buf, 256, file) != NULL) {
    lines++;
  }
  rewind(file);
  return lines;
}

int readArray(char* filename, int** array)
{
  FILE* file = fopen(filename, "r");
  int lines = countLines(file);
  *array = (int*)malloc(lines*sizeof(int));
  int line = 0;
  while ( fscanf(file, "%d\n", (*array)+line) != EOF) {
    line++;
  }
  fclose(file);
  return lines;
}

int main(int argc, char** argv)
{
  int* array = NULL;
  int count = 0;
  int i=0;
  
  count = readArray(argv[1], &array);
  
  for (i=0; i<count; ++i) {
    printf("%d\n", array[i]);
  }
  free(array);
  return 0;
}
