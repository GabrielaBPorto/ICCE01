#!/bin/bash

#rm -rf /nobackup/bcc/jns15/nobackup/bench
#mkdir /nobackup/bcc/jns15/nobackup/bench

rm -rf bench
mkdir bench

versions=(gbp16-jsn15_v1 gbp16-jsn15_v2)
groups=(L2CACHE L3 FLOPS_AVX)
ns=(32 33 64 65 128 129 256 257 512 1000) #2000)


plot() {
    group=$1
    echo "set logscale x 2" >> $group.data
		if [ "$5" == "m" ]; then
    	echo "set key right center" >> $group.data
		else
    	echo "set key right bottom" >> $group.data
		fi


    echo "set logscale y" >> $group.data
    echo "set grid" >> $group.data
    #echo "set format x '2^{%L}'" >> $group.data
    echo "set xlabel \"n\"" >> $group.data
    echo "set ylabel \"$3\"" >> $group.data
    echo "set title \"$4\"" >> $group.data
    echo "set terminal png" >> $group.data
    echo "set output \"$group.png\"" >> $group.data
    echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'  lw 2 title 'v1-op1', 'tmp' u 1:3 w l lt rgb 'orange' lw 2 title 'v1-op2' ,\
                  'tmp' u 1:4 w l lt rgb 'green' lw 2 title 'v2-op1', 'tmp' u 1:5 w l  lt rgb 'blue' lw 2 title 'v2-op2' " >> $group.data

    for n in ${ns[@]}; do
      echo -n "$n " >> tmp
      for version in ${versions[@]}; do
        op2=$(cat $version.$n.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
        op1=$(cat $version.$n.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
        echo -n "$op1 $op2 " >> tmp
      done
      echo "" >> tmp
    done
    gnuplot $group.data
    rm tmp
}

plot_time() {
    group=$1
    echo "set logscale x 2" >> $group.data
    echo "set logscale y" >> $group.data
    #echo "set format x '2^{%L}'" >> $group.data
    echo "set key left" >> $group.data
    echo "set grid" >> $group.data
    echo "set xlabel \"n\"" >> $group.data
    echo "set ylabel \"$3\"" >> $group.data
    echo "set title \"$4\"" >> $group.data
    echo "set terminal png" >> $group.data
    echo "set output \"$group.png\"" >> $group.data
    echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'   lw 2 title 'v1-op1', 'tmp' u 1:3 w l lt rgb 'gold'   lw 2  title 'v1-op2' ,\
                  'tmp' u 1:4 w l lt rgb 'green'  lw 2  title 'v2-op1', 'tmp' u 1:5 w l lt rgb 'purple' lw 2 title 'v2-op2' " >> $group.data
    for n in ${ns[@]}; do
      echo -n "$n " >> tmp
      for version in ${versions[@]}; do
        op1=$(cat $version.$n.res | grep "op1" | cut -d":" -f2 | cut -d"<" -f2 | cut -d">" -f1 | sed -e 's/ //g')
        op2=$(cat $version.$n.res | grep "op2" | cut -d":" -f2 | cut -d"<" -f2 | cut -d">" -f1 | sed -e 's/ //g')
        
        x=$(echo "$op1 * 100" | bc -l | awk '{printf "%f", $0}')
        y=$(echo "$op2 * 100" | bc -l | awk '{printf "%f", $0}')
        echo -n "$x $y " >> tmp
        rm $version.$n.res
      done
      echo "" >> tmp
    done
    gnuplot $group.data
    rm tmp
}

for version in ${versions[@]}; do
  cd $version && make clean && make
  for n in ${ns[@]}; do
    for group in ${groups[@]}; do
      echo iniciando $n de $group;
      likwid-perfctr -C 0 -g $group -f -m ./invmat -r $n -i 10 -o ../bench/$version.$n.res  > ../bench/$version.$n.$group;
    done
  done
  cd ..
done

cd bench

gthumb TIME.png &

cd ..
plot ${groups[0]} "L2 miss ratio" "ratio" "$L2 Miss Ratio"
plot ${groups[1]} "${groups[1]} bandwidth" "Bandwidth [MBytes/s]" "${groups[1]} cache"
plot ${groups[2]} "Packed DP MFLOP/s" "MFLOP/s" "FLOPS AVX" "m"
plot_time "TIME" "RDTSC Runtime" "ms" "Runtime"
