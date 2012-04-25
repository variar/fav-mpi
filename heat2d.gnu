#!/usr/bin/gnuplot -persist

set pm3d map

set terminal png size 800, 800
set output "out.png"

splot "./serial_result.txt" matrix
