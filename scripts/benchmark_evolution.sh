for i in $(ls grief_history/[0-9]*.txt);
do 
./scripts/restoreGrief.sh $i;
./tools/match_all star grief $1 >/dev/null;
a=$(tail -n 1 $1/results/star_grief.histogram);
echo $i $1 $a 
echo $i $1 $a >>grief_stats.txt;
done
