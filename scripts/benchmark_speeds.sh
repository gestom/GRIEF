#generates a table with average times of detection, description and matching for the individual features

rm detection.tmp 
rm description.tmp 
rm matching.tmp
rm table.tmp 

for i in $(head -n 1 settings/detectors);
do
	a=$(grep ^16 $1/results/$i\_*.histogram|awk 'BEGIN{a=0;i=0}{a+=$6;i=i+1}END{if (i==0) print -1; else print a/i;}');
	j=$(echo $i|tr [:lower:] [:upper:]);
	echo $j $a|sed s/NORM.//|sed s/UP.//>>detection.tmp
done 

for i in $(head -n 1 settings/descriptors);
do 
	a=$(grep ^16 $1/results/*_$i.histogram|awk 'BEGIN{a=0;i=0}{a+=$7;i=i+1}END{if (i==0) print -1; else print a/i;}');
	j=$(echo $i|tr [:lower:] [:upper:]);echo $j $a |sed s/ROOT.// >>description.tmp
done

for i in $(head -n 1 settings/descriptors);
do 
	a=$(grep ^16 $1/results/*_$i.histogram|awk 'BEGIN{a=0;i=0}{a+=$8;i=i+1}END{if (i==0) print -1; else print a/i;}');
	j=$(echo $i|tr [:lower:] [:upper:]);echo $j $a |sed s/ROOT.// >>matching.tmp
done

for i in $(cat detection.tmp|cut -f 1 -d ' ');
do 
	if [ "x$i" == "x$(grep $i description.tmp|cut -f 1 -d ' ')" ];
	then 
		a=$(printf %.1f $(grep $i detection.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'));
		b=$(printf %.1f $(grep $i description.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'))
		c=$(printf %.1f $(grep $i matching.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'))
		printf "$i \t\t& $a \t\t& $b \t\t& $c \\\\\\\\\n" >>table.tmp
	fi
done

for i in $(cat description.tmp|cut -f 1 -d ' ');
do
	if [ "x$i" == "x$(grep $i detection.tmp|cut -f 1 -d ' ')" ];
	then
	       echo -ne	
	else	
		b=$(printf %.1f $(grep $i description.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'))
		c=$(printf %.1f $(grep $i matching.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'))
		printf "$i \t\t& -- \t\t& $b \t\t& $c \\\\\\\\\n" >>table.tmp
	fi
done

for i in $(cat detection.tmp|cut -f 1 -d ' ');
do 
	if [ "x$i" == "x$(grep $i description.tmp|cut -f 1 -d ' ')" ];
	then
	       echo -ne	
	else
		a=$(printf %.1f $(grep $i detection.tmp|awk 'BEGIN{a=0;i=0}{a=a+$2;i=i+1}END{print a/i}'));
		printf "$i \t\t& $a \t\t& -- \t\t& -- \\\\\\\\\n" >>table.tmp
	fi
done
cat table.tmp |uniq|sed s/-1\.0/--/g|grep  -v "\-\-...\-\-...\-\-"
rm detection.tmp 
rm description.tmp 
rm matching.tmp
rm table.tmp 
