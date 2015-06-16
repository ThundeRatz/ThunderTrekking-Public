unset key

set xzeroaxis
set xtics axis

set yzeroaxis
set ytics axis

set zzeroaxis
set ztics axis

set grid front

set ticslevel 0

#set xrange [-400:400]
#set yrange [-400:400]
#set zrange [-400:400]

set size square
splot "hmc.dat" with points, "hmc.dat" with lines
pause -1
