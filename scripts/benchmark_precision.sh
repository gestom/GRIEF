#generates a table with error rates of different descriptor/detector combinations

printf "  \t" -
for des in $(cat settings/descriptors |head -n 1|tr ' ' \\n);
do
	printf "  %-10s \t" "$des "|tr '[:lower:]' '[:upper:] '
done
echo

for det in $(cat settings/detectors |head -n 1|tr ' ' \\n);
do 
	printf %-10s $(echo "$det "|tr '[:lower:]' '[:upper:] \n'|sed s/NORM.//|sed s/UP./u/)
	for des in $(cat settings/descriptors |head -n 1|tr ' ' \\n);
	do 
		if [ -e $1/results/$det\_$des.histogram ];
			then 
				x=$(grep ^16 $1/results/$det\_$des.histogram|cut -f 2 -d ' ';)
				#printf " &\mbox{\,\,\,\,%04.1f}" $x  |sed s/,0/',\\,\\,\\,'/g
				printf " & %4.1f \t" $x
			else	
				printf " & ----  \t" 
			fi
	done
	echo \\\\
done
