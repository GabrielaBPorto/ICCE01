#!/bin/bash

rm -rf /nobackup/bcc/gbp16/nobackup/bench
mkdir /nobackup/bcc/gbp16/nobackup/bench

versions=(v2_icc_comzero v4_icc_SIMDv2)
groups=(L2CACHE L3 FLOPS_AVX)
nBands=(7 27)
ns=(10, 32, 50, 64, 100, 128, 200, 250, 256, 300, 400, 512, 600, 1000, 1024, 2000, 2048, 3000, 4096)


plot() {
  for nBand in ${nBands[@]}; do
    group=$1
    echo "set logscale x 2" >> $group.$nBand.data
		if [ "$5" == "m" ]; then
    	echo "set key right center" >> $group.$nBand.data
		else
    	echo "set key right bottom" >> $group.$nBand.data
		fi


    echo "set logscale y" >> $group.$nBand.data
    echo "set grid" >> $group.$nBand.data
    echo "set format x '2^{%L}'" >> $group.$nBand.data
    echo "set xlabel \"n\"" >> $group.$nBand.data
    echo "set ylabel \"$3\"" >> $group.$nBand.data
    echo "set title \"$4\"" >> $group.$nBand.data
    echo "set terminal png" >> $group.$nBand.data
    echo "set output \"$group.$nBand.png\"" >> $group.$nBand.data
    echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'  lw 2 title 'v0-mvv', 'tmp' u 1:3 w l lt rgb 'orange' lw 2 title 'v0-mmv' ,\
                  'tmp' u 1:4 w l lt rgb 'green' lw 2 title 'v4-mvv', 'tmp' u 1:5 w l  lt rgb 'blue' lw 2 title 'v4-mmv' " >> $group.$nBand.data

    for n in ${ns[@]}; do
      echo -n "$n " >> tmp
      for version in ${versions[@]}; do
        mvv=$(cat $version.$n.$nBand.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
        mmv=$(cat $version.$n.$nBand.$group | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
        echo -n "$mvv $mmv " >> tmp
      done
      echo "" >> tmp
    done
    gnuplot $group.$nBand.data
    rm tmp

  done
}

plot_time() {
  for nBand in ${nBands[@]}; do
    group=$1
    echo "set logscale x 2" >> $group.$nBand.data
    echo "set logscale y" >> $group.$nBand.data
    echo "set format x '2^{%L}'" >> $group.$nBand.data
    echo "set key left" >> $group.$nBand.data
    echo "set grid" >> $group.$nBand.data
    echo "set xlabel \"n\"" >> $group.$nBand.data
    echo "set ylabel \"$3\"" >> $group.$nBand.data
    echo "set title \"$4\"" >> $group.$nBand.data
    echo "set terminal png" >> $group.$nBand.data
    echo "set output \"$group.$nBand.png\"" >> $group.$nBand.data
    echo -n "plot 'tmp' u 1:2 w l lt rgb 'red'   lw 2 title 'v0-mvv', 'tmp' u 1:3 w l lt rgb 'gold'   lw 2  title 'v0-mmv' ,\
                  'tmp' u 1:4 w l lt rgb 'green'  lw 2  title 'v4-mvv', 'tmp' u 1:5 w l lt rgb 'purple' lw 2 title 'v4-mmv' " >> $group.$nBand.data
    for n in ${ns[@]}; do
      echo -n "$n " >> tmp
      for version in ${versions[@]}; do
        loop=$(cat $version.$n.$nBand.res | grep "Tempo método CG" | cut -d":" -f2 | cut -d"<" -f2 | cut -d">" -f1 | sed -e 's/ //g')
        mvv=$(cat $version.$n.$nBand.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
        mmv=$(cat $version.$n.$nBand.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
        x=$(echo "$mvv * 100" | bc -l | awk '{printf "%f", $0}')
        y=$(echo "$mmv * 100" | bc -l | awk '{printf "%f", $0}')
        echo -n "$x $y " >> tmp
        rm $version.$n.$nBand.res
      done
      echo "" >> tmp
    done
    gnuplot $group.$nBand.data
    rm tmp

  done
}

for version in ${versions[@]}; do
  cd $version && make clean && make
  for nBand in ${nBands[@]}; do
    for n in ${ns[@]}; do
      for group in ${groups[@]}; do
        likwid-perfctr -C 0 -g $group -f -m ./cgSolver $n $nBand -o /nobackup/bcc/gbp16/nobackup/bench/$version.$n.$nBand.res -i 10 > /nobackup/bcc/gbp16/nobackup/bench/$version.$n.$nBand.$group
      done
    done
  done
  cd -
done

cd /nobackup/bcc/jns15/nobackup/bench

plot ${groups[0]} "L2 miss ratio" "ratio" "$L2 Miss Ratio"
plot ${groups[1]} "${groups[1]} bandwidth" "Bandwidth [MBytes/s]" "${groups[1]} cache"
plot ${groups[2]} "Packed DP MFLOP/s" "MFLOP/s" "FLOPS AVX" "m"
plot_time "TIME" "RDTSC Runtime" "ms" "Runtime"
