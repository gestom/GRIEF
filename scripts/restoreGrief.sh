#restore GRIEF to the version used in the ECMR 2015 paper "Image features and seasons revisisted" 
cp tools/grief/test_pairs.grief tools/grief/test_pairs.txt;
cd tools/;
./generate.sh >/dev/null;
cd ..;
