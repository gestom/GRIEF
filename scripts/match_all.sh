echo Tests started 
date
descriptors=$(head -1 settings/descriptors)
detectors=$(head -1 settings/detectors)

for k in $(echo $detectors);
do
for m in $(echo $descriptors);
do
	./tools/match_all $k $m $1 
done;
done;
echo Tests completed
date 
