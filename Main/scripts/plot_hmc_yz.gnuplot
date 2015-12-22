unset key

set xzeroaxis
set xtics axis

set yzeroaxis
set ytics axis

set grid front

set xrange [-400:400]
set yrange [-400:400]

set size square
set xlabel 'Y'
set ylabel 'Z'
plot "hmc_yz.dat" with points, "hmc_yz.dat" with lines
pause -1
