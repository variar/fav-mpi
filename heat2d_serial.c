/*
 * 2d heat equation
 * Author: Anton Filimonov (anton.filimonov@gmail.com)
 * Year: 2012
 * Arguments: T, N, a, b
 * Result in serial_result.txt


 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain worldwide.
 *
 * This software is distributed without any warranty.
 * You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see 
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct _grid {
  double* u;
  int size;
  double dh;
  double dt;
  double a;
  double b;
  
} Grid;

int utime()
{
struct timeval tv;
gettimeofday(&tv, NULL);
return (int)tv.tv_sec*1000000+(int)tv.tv_usec;
}

double f(double x, double y, double a, double b)
{
  return exp (-1/(a*a) * (x*x - 2*b*x*y + y*y));
}

void init(Grid* grid)
{
  int x=0; 
  int y=0;
  double cur_x = 0.0;
  double cur_y = 0.0;
  
  for (y = 0; y<grid->size; ++y) {
    cur_y = y*grid->dh;
    for (x=0; x<grid->size; ++x) {
      cur_x = x*grid->dh;
      
      *(grid->u+y*grid->size+x) = f(cur_x, cur_y, grid->a, grid->b);
    }
  }
}

double val(Grid* g, int x, int y)
{
  return *(g->u+g->size*y + x);
}

void step(Grid* g0, Grid* g1)
{
  int size = g0->size;
  double C = g0->dt / (g0->dh*g0->dh);

  int y=0;
  int x=0;
  for (y=1; y<size-1; ++y) {
    for (x=1; x<size-1; ++x) {
     *(g1->u+y*size+x) =
        val(g0,x,y) +
        C * (val(g0,x,y-1) - 2 * val(g0,x,y) + val(g0,x,y+1)) + 
        C * (val(g0,x-1,y) - 2 * val(g0,x,y) + val(g0,x+1,y)); 
    }
  }
}

void save(Grid* g, char* filename)
{
  FILE* file = fopen(filename, "w");
  int y=0;
  int x=0;
  for (y=0; y<g->size; ++y) {
    for (x=0; x<g->size; ++x) {
      fprintf(file, "%8.3f\t", val(g, x, y));
      }
    fprintf(file, "\n");
  }
  fclose(file);
}

int main(int argc, char* argv[])
{
  const double T = atof(argv[1]);
  const int N = atoi(argv[2]);
  const double A = atof(argv[3]);
  const double B = atof(argv[4]);
  
  const double h = 1.0 / (N - 1);
  const double t = (h * h) / 4;
  const int i_ceil = ceil(T / t) + 1;
  int i=0;
  int z=0;
  
  int startTime = utime();
  
  Grid grid[2];
  grid[0].u = (double*)malloc(sizeof(double)*N*N);
  grid[1].u = (double*)malloc(sizeof(double)*N*N);
  
  grid[0].size = grid[1].size = N;
  grid[0].dh = grid[1].dh = h;
  grid[0].dt = grid[1].dt = t;
  grid[0].a = grid[1].a = A;
  grid[0].b = grid[1].b = B;
  
  init(&grid[0]);
  init(&grid[1]);
  
  for (i=0; i<i_ceil; ++i) {
    step(&grid[z], &grid[1-z]);
    z = 1-z;
  }
  
  printf("Time: %f\n", (utime()-startTime)/1000000.0);
  
  save(&grid[z], "serial_result.txt");
}
