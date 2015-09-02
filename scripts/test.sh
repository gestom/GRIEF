echo Tests started 
date
descriptors=$(head -1 settings/descriptors)
detectors=$(head -1 settings/detectors)
n=$(echo $descriptors|wc -w)
for m in $(seq 1 $n);
do
	a=$(echo $detectors|cut -f $m -d ' ')
	b=$(echo $descriptors|cut -f $m -d ' ')
	./tools/test_all $a $b $1 
done;
echo Tests completed
date 
