/*
 * generate series of random integers in the format
 * N
 * a[0]
 * a[1]
 * ...
 * a[N-1]
 * 
 * a[N-3] = min(a)
 * a[N-2] = max(a)
 * a[N-1] = average(a)
 * 
 * Author: Anton Filimonov (anton.filimonov@gmail.com)
 * Year: 2013
 * Arguments: N
 * Result is printed to stdout
 * 
 * 
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain worldwide.
 *
 * This software is distributed without any warranty.
 * You should have received a copy of the CC0 Public Domain Dedication along with this software. If not, see 
 * http://creativecommons.org/publicdomain/zero/1.0/
 */

#include <iostream>
#include <limits>
#include <stdlib.h>
#include <time.h>

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cout << "Usage: gen_array <size>"<<std::endl;
    return 1;
  }


  int N = atoi(argv[1]);

  std::cout << N << std::endl;

  int min = std::numeric_limits<int>::max();
  int max = std::numeric_limits<int>::min();
  int sum = 0;

  srand(time(NULL));

  for (int i=0; i<N-3; ++i)
  {
    int rnd = rand() % 1000;
    sum += rnd;
    min = std::min(min,rnd);
    max = std::max(max,rnd);
    std::cout << rnd << std::endl;
  }

  std::cout << min << std::endl;
  std::cout << max << std::endl;
  std::cout << sum/N << std::endl;
  
  return 0;
}