#check the IV.c section of the paper to interpret this script
function generate
{
	#resize images to match them with the style-transferred ones
	rm -rf $1_00
	mkdir $1_00
	for j in season_00 season_01;
	do
		cp -r $1/$j $1_00/$j
		cd $1_00/$j
		for k in $(ls *.bmp);
		do 
			convert $k -resize 50% $k 
			echo converting $1_00/$j/$k
		done
		cd ..
		cd ..
	done

	#restructure the datasets and update the ground truth files accordingly
	rm -rf $1_01
	rm -rf $1_10
	for i in 00 01 10;
	do
		mkdir $1_$i
		mkdir $1_$i/results
		for j in season_00 season_01;
		do
			if [ $i != 00 ];then cp -r $1_00/$j $1_$i/$j;fi
			cat $1/$j/displacements.txt |awk '{print $1/2,$2/2}' >$1_$i/$j/displacements.txt

		done
	done

	#incorporate the style tranferred images
	cd $1/00to01
	for i in $(ls *.png|sed s/.png//);
	do
		convert $i.png ../../$1_01/season_00/$i.bmp
		echo $1_01/season_01/$i.bmp
	done
	cd ../../

	#incorporate the style tranferred images
	cd $1/01to00
	for i in $(ls *.png|sed s/.png//);
	do
		convert $i.png ../../$1_10/season_01/$i.bmp
		echo $1_10/season_00/$i.bmp
	done
	cd ../../
}

generate nclt
generate stromovka 
