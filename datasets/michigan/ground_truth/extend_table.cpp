#include <stdio.h>
const int len  = 12;
int reference[len];

int main(int argc,char* argv[])
{
	for (int i = 0;i<len;i++){
		fscanf(stdin,"%i ",&reference[i]);
	}
	for (int i = 0;i<len*len;i++){
		fprintf(stdout,"%i\t",reference[i%len]-reference[i/len]);
		if (i%len == (len-1)) fprintf(stdout,"\n");
	}
}
