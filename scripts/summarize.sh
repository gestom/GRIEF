rm features.txt
rm features.tmp
rm features*.txt
for j in stromovka
do
	for k in 01 10
	do
		rm results/*
		for i in $(ls $j\_$k/results/*.details|sed s/^.*results.//);
		do
			cat $j\_$k/results/$i|cut -f 10 -d ' ' >results/$k\_$i		
			cat $j\_00/results/$i|cut -f 10 -d ' ' >results/00_$i		
		done

		cd results
		for a in $(ls *.details)
		do
			h=0
			s=0
			c=$(cat $a |awk '{a=a+sqrt($1*$1)}END{OFMT = "%.2f";print a/NR}')
			for b in $(ls *.details)
			do
				if [ $a != $b ];then
					if [ $(../tools/t-test $a $b|grep -c higher ) -gt 0 ];then h=$(($h+1));fi
					if [ $(../tools/t-test $a $b|grep -c smaller) -gt 0 ];then s=$(($s+1));fi
				fi
			done
			echo $a $c $s $h
			echo $a $c $s $h >>../features\_$j\_$k.txt
		done
		cd ..
	done
done
