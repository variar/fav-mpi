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
  return 0;
}