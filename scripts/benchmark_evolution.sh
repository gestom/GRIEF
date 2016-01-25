for i in $(ls $1/[0-9]*.txt);
do 
./scripts/restoreGrief.sh $i;
./tools/match_all star grief $2 >/dev/null;
a=$(tail -n 1 $2/results/star_grief.histogram);
echo $i $2 $a 
echo $i $2 $a >>grief_stats.txt;
done
