unset key

set xtics axis
set ytics axis

set grid front

set size square
set xlabel 'X'
set ylabel 'Y'
plot "location.dat" with points, "location.dat" with lines
pause -1
