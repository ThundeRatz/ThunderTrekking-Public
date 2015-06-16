unset key

set xzeroaxis
set xtics axis

set yzeroaxis
set ytics axis

set grid front

set xrange [-400:400]
set yrange [-400:400]

set size square
set xlabel 'X'
set ylabel 'Z'
plot "hmc_xz.dat" with points, "hmc_xz.dat" with lines
pause -1
