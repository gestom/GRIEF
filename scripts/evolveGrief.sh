date
f=0;
e=0;

#reset GRIEF comparison to BRIEF ones - for incremental training, comment out this line
#cp tools/grief/test_pairs.brief tools/grief/test_pairs.txt;

#train a given number or generations
for i in $(seq 1 $1);
do 
	#recompiles GRIEF code
	cd tools/;
	./generate.sh >/dev/null;
	cd ..;
	f=$e;
	e=$(./tools/evolve_grief 22 99|grep fitness|cut -f 3 -d ' ');
	echo Population $i Fitness $e;
	j=$(printf %05i $i);
	cp tools/grief/pair_stats.txt grief_history/$j\_$e.txt;
done
date
