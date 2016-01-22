#restore GRIEF to given version, either used in the ECMR 2015 paper "Image features and seasons revisisted", or an arbitrary one 
if [ $# == 0 ];
then 
echo Restore original GRIEF
cp tools/grief/test_pairs.grief tools/grief/test_pairs.txt;
else
echo Creating GRIEF feature from $1
cat $1|cut -f 1,2,3,4 -d ' ' 	>tools/grief/test_pairs.txt;
tail -n 256 tools/grief/test_pairs.grief >>tools/grief/test_pairs.txt;
fi
cd tools/;
./generate.sh 
cd ..;
