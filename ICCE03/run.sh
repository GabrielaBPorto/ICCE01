#!/bin/bash

diretorio='/nobackup/ibm/gbp16/nobackup/bench'

rm -rf $diretorio
mkdir $diretorio

groups=(L2CACHE L3 FLOPS_AVX)
ns=(10 32 50 64 100 128 200 250 256 300 400 512 600 1000 1024 2000 2048 3000 4096)

make;

#  Fixar a frequencia do processador
echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

plot() {
  for nBand in ${nBands[@]}; do
    group=$1
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

  done
}

# plot_time() {
#   for nBand in ${nBands[@]}; do
#     group=$1
#     echo "set logscale x 2" >> $group.data
#     echo "set logscale y" >> $group.data
#     echo "set format x '2^{%L}'" >> $group.data
#     echo "set key left" >> $group.data
#     echo "set grid" >> $group.data
#     echo "set xlabel \"n\"" >> $group.data
#     echo "set ylabel \"$3\"" >> $group.data
#     echo "set title \"$4\"" >> $group.data
#     echo "set terminal png" >> $group.data
#     echo "set output \"$group.png\"" >> $group.data
#     echo -n "plot 'tmp' w l lt rgb 'red'   lw 2 title 'v0-mvv', 'tmp' u 1:3 w l lt rgb 'gold'   lw 2  title 'v0-mmv' ,\
#                   'tmp' w l lt rgb 'green'  lw 2  title 'v4-mvv', 'tmp' u 1:5 w l lt rgb 'purple' lw 2 title 'v4-mmv' \" >> $group.data
#     for n in ${ns[@]}; do
#       echo -n "$n " >> tmp
#       loop=$(cat $n.res | grep "Tempo m??todo CG" | cut -d":" -f2 | cut -d"<" -f2 | cut -d">" -f1 | sed -e 's/ //g')
#       mvv=$(cat $n.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 1 | sed -e 's/ //g')
#       mmv=$(cat $n.FLOPS_AVX | grep "$2" | cut -d\| -f 3 | tr '\n' '\t' | cut -f 2 | sed -e 's/ //g')
#       x=$(echo "$mvv * 100" | bc -l | awk '{printf "%f", $0}')
#       y=$(echo "$mmv * 100" | bc -l | awk '{printf "%f", $0}')
#       echo -n "$x $y " >> tmp
#       rm sistema_$n.res
#       echo "" >> tmp
#     done
#     gnuplot $group.data
#     rm tmp

#   done
# }

for n in ${ns[@]}; do
  ./gerarSL $n >>  $diretorio/sistema_$n.res
  for group in ${groups[@]}; do
    likwid-perfctr -C 3 -g $group -f -m ./gaussJacobi-likwid $diretorio/sistema_$n.res > $diretorio/$n.$group
  done
done

cd $diretorio

plot ${groups[0]} "L2 miss ratio" "ratio" "$L2 Miss Ratio"
plot ${groups[1]} "${groups[1]} bandwidth" "Bandwidth [MBytes/s]" "${groups[1]} cache"
plot ${groups[2]} "Packed DP MFLOP/s" "MFLOP/s" "FLOPS AVX" "m"
plot_time "TIME" "RDTSC Runtime" "ms" "Runtime"


echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor 
cd /home/gabi/Faculdade/ICC/ICCE01/ICCE03
make clean;
make purge faxina;