#!/bin/bash

diretorio='./results'

#rm -rf $diretorio
#mkdir $diretorio

groups=(L2CACHE L3 FLOPS_AVX)
ns=(10 32)

#  Fixar a frequencia do processador
plot() {
    echo "PLot plot plot"
    group=$1
    echo "Plotting $group for size $nBand"
    echo "set logscale x 2" >> $group.data
        if [ "$5" == "m" ]; then
        echo "set key right center" >> $group.data
        else
        echo "set key right bottom" >> $group.data
        fi


    echo "set logscale y" >> $group.data
    echo "set grid" >> $group.data
    echo "set format x '2^{%L}'" >> $group.data
    echo "set xlabel \"n\"" >> $group.data
    echo "set ylabel \"$3\"" >> $group.data
    echo "set title \"$4\"" >> $group.data
    echo "set terminal png" >> $group.data
    echo "set output \"$group.png\"" >> $group.data
    echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'  lw 2 title 'v0-mvv', 'tmp' u 1:3 w l lt rgb 'orange' lw 2 title 'v0-mmv' ,\
                  'tmp' u 1:4 w l lt rgb 'green' lw 2 title 'v4-mvv', 'tmp' u 1:5 w l  lt rgb 'blue' lw 2 title 'v4-mmv' " >> $group.data

    for n in ${ns[@]}; do
      echo -n "$n " >> tmp
        mvv=$(cat $n.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
        mmv=$(cat $n.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
        echo -n "$mvv $mmv " >> tmp
      echo "" >> tmp
    done
    gnuplot $group.data
    rm tmp

}

plot_time() {
  group=$1
  echo "set logscale x 2" >> $group.data
  echo "set logscale y" >> $group.data
  echo "set format x '2^{%L}'" >> $group.data
  echo "set key left" >> $group.data
  echo "set grid" >> $group.data
  echo "set xlabel \"n\"" >> $group.data
  echo "set ylabel \"$3\"" >> $group.data
  echo "set title \"$4\"" >> $group.data
  echo "set terminal png" >> $group.data
  echo "set output \"$group.png\"" >> $group.data
  echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'   lw 2 title 'v0-mvv', 'tmp' u 1:3 w l lt rgb 'gold'   lw 2  title 'v0-mmv' ,\
                'tmp' u 1:4 w l lt rgb 'green'  lw 2  title 'v4-mvv', 'tmp' u 1:5 w l lt rgb 'purple' lw 2 title 'v4-mmv' " >> $group.data
  for n in ${ns[@]}; do
    echo -n "$n " >> tmp
    loop=$(cat sistema_$n.res | grep "Tempo método CG" | cut -d":" -f2 | cut -d"<" -f2 | cut -d">" -f1 | sed -e 's/ //g')
    mvv=$(cat $n.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
    mmv=$(cat $n.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
    x=$(echo "$mvv * 100" | bc -l | awk '{printf "%f", $0}')
    y=$(echo "$mmv * 100" | bc -l | awk '{printf "%f", $0}')
    echo -n "$x $y " >> tmp
    #rm sistema_$n.res
    echo "" >> tmp
  done
  gnuplot $group.data
  rm tmp
}

cd $diretorio

echo "Plotting"

plot ${groups[0]} "L2 miss ratio" "ratio" "L2 Miss Ratio"
plot ${groups[1]} "${groups[1]} bandwidth" "Bandwidth [MBytes/s]" "${groups[1]} cache"
plot ${groups[2]} "Packed DP MFLOP/s" "MFLOP/s" "FLOPS AVX" "m"
plot_time "TIME" "RDTSC Runtime" "ms" "Runtime"
