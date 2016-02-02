#draws the dependence of heading estimation error on the number of extracted features
#this script actually generates another script for gnuplot 
cp scripts/rates.gnu rates.tmp

#reads which detectors and descriptors to use
descriptors=$(head -1 settings/descriptors)
detectors=$(head -1 settings/detectors)
n=$(echo $descriptors|wc -w)
d=$(echo $1|sed s/\\/$//|sed s/^.*\\///|sed 's/.*/\u&/')
echo set title \'"$d " dataset\' >>rates.tmp
echo plot '[0:16] [0:80] \' >>rates.tmp
for m in $(seq 1 $n);
do
	a=$(echo $detectors|cut -f $m -d ' ')
	b=$(echo $descriptors|cut -f $m -d ' ')
	at=$(echo $a |tr [:lower:] [:upper:])
	bt=$(echo $b |tr [:lower:] [:upper:])
	echo \'"$1"/results/$a\_$b.histogram\'  'using ($1):2  with lines lw 2 title' \'$at+$bt\'\,\\ >>rates.tmp
done;
echo	>>rates.tmp

#are there two datasets to draw ?
if [ $# == 2 ];
then
d=$(echo $2|sed s/\\/$//|sed s/^.*\\///|sed 's/.*/\u&/')
echo set title \'"$d " dataset\' >>rates.tmp
echo unset ylabel >>rates.tmp
echo set size 0.5,1.0 >>rates.tmp
echo set origin 0.5,0.0 >>rates.tmp
echo plot '[0:16] [0:80] \' >>rates.tmp
for m in $(seq 1 $n);
do
	a=$(echo $detectors|cut -f $m -d ' ')
	b=$(echo $descriptors|cut -f $m -d ' ')
	at=$(echo $a |tr [:lower:] [:upper:])
	bt=$(echo $b |tr [:lower:] [:upper:])
	echo \'"$2"/results/$a\_$b.histogram\'  'using ($1):2  with lines lw 2 title' \'$at+$bt\'\,\\ >>rates.tmp
done
fi

#call gnuplot to draw the results
gnuplot rates.tmp >rates.fig;fig2dev -Lpdf rates.fig >rates.pdf
echo "Generated rates.fig and rates.pdf files, check them out"
