#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <libalglib/statistics.h>

using namespace alglib;


int maina(int argc, char **argv)
{
	printf("%.3f\n",1.0-alglib::studenttdistribution(atoi(argv[1]),atoi(argv[2])));
	return 0;
}

float a,b;
double xA[100000];
double xB[100000];
int numA = 0;
int numB = 0;

int main(int argc, char **argv)
{
	float thr = 0.05;
	FILE* fileA = fopen(argv[1],"r");
	if (fileA == NULL){perror("Error: ");printf("First file erroneous.\n");return -1;}
	FILE* fileB = fopen(argv[2],"r");
	if (fileB == NULL){perror("Error: ");printf("Second file erroneous.\n");return -1;}
	if (argc > 3) thr = atof(argv[3]);

	while(feof(fileA)==0){
		fscanf(fileA,"%f\n",&a);
		xA[numA++]=a;
	}

	while(feof(fileB)==0){
		fscanf(fileB,"%f\n",&b);
		xB[numB++]=b;
	}

	if (numB != numA) {printf("First and second file have different lengths.\n");return -1;}
	for (int i = 0;i<numA;i++){xA[i]=xA[i]-xB[i];}

	double both,left,right;
	real_1d_array x;
        x.setcontent(numA,xA);	
	alglib::studentttest1(x,numA,0,both,left,right);
	if (right < thr) printf("First value is higher (p(higher)=%.3f)\n",1-right);
	else if (left < thr) printf("First value is smaller (p(smaller)=%.3f)\n",1-left);
	else printf("First value is similar (p(similar)=%.3f,%.3f,%.3f)\n",both,right,left);
	return 0;
}
