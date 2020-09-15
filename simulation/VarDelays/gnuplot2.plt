set terminal pdf
set output "plot2.pdf"
set title "graph of total cycles versus Delay N given a fixed HIT probability x"
set xlabel "Variable delay (N) in case of a MISS"
set ylabel "total cycles used in the program"
set boxwidth 0.05
set style fill solid
plot "vardelay.txt" using 2:3 with boxes title "" lt rgb "#000090", "vardelay.txt" using 2:3 pt 7 ps 0.5 title "points", "vardelay.txt" using 2:3 smooth csplines title "" lt rgb "#F00000"
