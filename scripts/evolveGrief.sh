date
f=0;
e=0;

#reset GRIEF comparison to BRIEF ones - for incremental training, comment out the following line
#cp tools/grief/test_pairs.brief tools/grief/test_pairs.txt;

#train a given number or generations
for i in $(seq 1 $2);
do 
	#recompiles GRIEF code
	cd tools/;
	./generate.sh >/dev/null;
	cd ..;
	f=$e;
	./tools/evolve_grief $1 |grep fitness >store.tmp
	e=$(cat store.tmp|cut -f 3 -d ' ');
	r=$(cat store.tmp|cut -f 4 -d ' ');
	echo Population $i Fitness $e Error $r;
	j=$(printf %05i $i);
	cp tools/grief/pair_stats.txt grief_history/$j\_$e.txt;
done
date
