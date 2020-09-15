set terminal pdf
set output "plot1.pdf"
set title "graph of total cycles(delay) versus HIT probability (x) for given N"
set xlabel "probability(x) of completing the operation in one cycle (a HIT)"
set ylabel "total cycles used in the program"
set boxwidth 0.05
set style fill solid
plot "vardelay.txt" using 1:3 with boxes title "" lt rgb "#000090", "vardelay.txt" using 1:3 pt 7 ps 0.5 title "points", "vardelay.txt" using 1:3 smooth csplines title "" lt rgb "#F00000"
