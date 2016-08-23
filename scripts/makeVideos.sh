#this script can be used to create videos of matchings

function process
{
rm -rf $1/$2_$3
mkdir $1/$2_$3
./tools/match_all $2 $3 $1 save >$1/$2_$3/log.txt
for i in $(ls $1/*A.bmp|sed s/.A.bmp//);
do
convert $i-B.bmp -morphology dilate Disk:1:1 -transparent black a.png 
convert $i-A.bmp -draw "image src-over 0,0 1024,768 'a.png'" $i.jpg
done
rm a.png
rm $1/*.bmp
mv $1/*.jpg $1/$2_$3/
}

function combine
{
	convert -size 445x330 xc:black -pointsize 24 -fill white -font Helvetica-Bold -gravity Center -draw "Text 0,0 'No heading estimation failure (yet).'" $1/up-sift_root-sift/f-00-01.jpg
	convert -size 445x330 xc:black -pointsize 24 -fill white -font Helvetica-Bold -gravity Center -draw "Text 0,0 'No heading estimation failure (yet).'" $1/star_grief/f-00-01.jpg
	f=f-00-01.jpg
	for a in $(ls $1/star_grief/0*|sed s/-..-..\..*//|sed s/.*star_grief.//);
	do
		f0=$(grep Report $1/up-sift_root-sift/log.txt|grep $a|cut -f 8 -d ' ');
		f1=$(grep Report $1/star_grief/log.txt|grep $a|cut -f 8 -d ' ');
		b=$(ls $1/star_grief/0*|grep $a|sed s/.*star_grief.//)
		echo $a $b A: $f0 B: $f1
		if [ $((f0+f1)) -gt 0 ];
		then f=$b;
		fi
		convert -size 920x700 xc:black \
			-pointsize 24 -fill white -font Helvetica-Bold \
			-draw "Image src-over 10,10,445,330 '$1/star_grief/$b'" \
			-draw "Image src-over 465,10,445,330 '$1/up-sift_root-sift/$b'" \
			-draw "Image src-over 10,360,445,330 '$1/star_grief/$f'" \
			-draw "Image src-over 465,360,445,330 '$1/up-sift_root-sift/$f'" \
			-fill black -draw "Rectangle 0,335,930,359" \
			-fill white -gravity Center -draw "Text -232,0 'STAR + GRIEF '" \
			-gravity Center -draw "Text +232,0 'Upright-Root-SIFT '" \
			$1/$a.jpg
	done
}

process $1 star grief 
process $1 up-sift root-sift
combine $1 
