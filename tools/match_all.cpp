#include <sys/time.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include "grief/grief.h"
#include <dirent.h>


#define CROSSCHECK true 
#define VERTICAL_LIMIT 100 
bool norm2 =true;
bool upright =true;

bool update=false;
bool save=false;
bool draw=false; 
bool drawAll=false; 
bool normalizeSift=false; 
using namespace std;
using namespace cv;
unsigned int n;
float distance_factor = 1.0;
const int granularity = 20;
const int width = 500;
const int height = 100;
int histogram2D[width*2+granularity][height*2+granularity];
bool hist2D = false;
int difference = 0;
FILE *output = NULL;
int maxFeatures=1600;
int minFeatures=0;
int numFeatures=maxFeatures;


int numFails[1600/100+1];
char season[1000][1000]; 
int seasons = 0;
char dataset[1000];
char descriptorName[1000];
char detectorName[1000];

int time0,time1,time2,time3,time4,time5,time6;

FeatureDetector     *detector = NULL;
DescriptorExtractor *descriptor = NULL;

/*for benchmarking purposes*/
int getTime()
{
  struct  timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec*1000 + currentTime.tv_usec/1000;
}

/*to select most responsive features*/
bool compare_response(KeyPoint first, KeyPoint second)
{
	  if (first.response > second.response) return true; else return false;
}

/*matching scheme*/
void distinctiveMatch(const Mat& descriptors1, const Mat& descriptors2, vector<DMatch>& matches, bool norm2= true, bool crossCheck=false)
{
   DescriptorMatcher *descriptorMatcher;
   vector<vector<DMatch> > allMatches1to2, allMatches2to1;

   if (norm2)
      descriptorMatcher = new BFMatcher(cv::NORM_L2,  false);
   else 
      descriptorMatcher = new BFMatcher(cv::NORM_HAMMING, false);
   descriptorMatcher->knnMatch(descriptors1, descriptors2, allMatches1to2, 2);

   if (!crossCheck)
   {
      for(unsigned int i=0; i < allMatches1to2.size(); i++)
      {
         if (allMatches1to2[i].size() == 2)
         { 
            if (allMatches1to2[i][0].distance < allMatches1to2[i][1].distance * distance_factor)
            {
               DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
               matches.push_back(match);
            }
         }         
         else if (allMatches1to2[i].size() == 1)
         {
            DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
            matches.push_back(match);
            cout << "ERROR" << endl;
         }
         
      }
   }
   else
   {
      descriptorMatcher->knnMatch(descriptors2, descriptors1, allMatches2to1, 2);
      for(unsigned int i=0; i < allMatches1to2.size(); i++)
      {
         if (allMatches1to2[i].size() == 2)
         { 
            if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 2)
            {
               if (allMatches1to2[i][0].distance < allMatches1to2[i][1].distance * distance_factor && allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance < allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance * distance_factor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
               {
                  DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
                  matches.push_back(match);
               }
            }
            else if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 1)
               if (allMatches1to2[i][0].distance  < allMatches1to2[i][1].distance * distance_factor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
               {
                  DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
                  matches.push_back(match);
                  cout << "ERROR" << endl;
               } 
         }
         else if (allMatches2to1[allMatches1to2[i][0].trainIdx].size() == 2)
         {
            if (allMatches2to1[allMatches1to2[i][0].trainIdx][0].distance < allMatches2to1[allMatches1to2[i][0].trainIdx][1].distance * distance_factor && allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
            {
                  DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
                  matches.push_back(match);
                  cout << "ERROR" << endl;
            }
         }
         else if (allMatches1to2[i][0].trainIdx == allMatches2to1[allMatches1to2[i][0].trainIdx][0].queryIdx)
         {
               DMatch match = DMatch(allMatches1to2[i][0].queryIdx, allMatches1to2[i][0].trainIdx, allMatches1to2[i][0].distance);
               matches.push_back(match);
               cout << "ERROR" << endl;
         } 

      }

   }
   delete descriptorMatcher;
}

int *seq1;
int *seq2;
int *offsetX;
int *offsetY;
int numTests = 0;

void rootSift(Mat *m)
{
	for (int r = 0;r<m->rows;r++)
	{
		float n1 = 0;	
		for (int c = 0;c<m->cols;c++) n1+=fabs(m->at<float>(r,c));
		for (int c = 0;c<m->cols;c++) m->at<float>(r,c)=sqrt(fabs(m->at<float>(r,c)/n1));
	}
}

/*initialize the dataset parameters*/
int initializeDateset()
{
	DIR *dpdf;
	struct dirent *epdf;
	FILE* file; 
	int dum1,dum2,dum3,dum4; //for reading from a file
	char filename[1000];

	/*how many seasons are in the dataset?*/
	dpdf = opendir(dataset);
	if (dpdf != NULL)
	{
		while (epdf = readdir(dpdf))
		{
			if (strncmp(epdf->d_name,"season_",7)==0 && epdf->d_type == 4)
			{
				printf("Season directory %i %s \n",seasons,epdf->d_name);
				strcpy(season[seasons++],epdf->d_name);
			}
		}
	}else{
		fprintf(stderr,"Dataset directory not found\n");
		return -1;
	}

	/*are there at least two?*/
	if (seasons < 2)
	{
		fprintf(stderr,"At least two directories with the prefix season_ have to be present in the datasets directory\n");
		return -1;	
	}
	

	
	/*check the files*/
	dum4 = 0;
	for (int i = 0;i<seasons;i++){
		sprintf(filename,"%s/%s/displacements.txt",dataset,season[i]);
		file = fopen(filename,"r");
		if (file==NULL)
		{
			fprintf(stderr,"File %s not found - look at the examples on how to fill the datasets.\n",filename);
			return -1;
		}
		dum3 = 0;
		while (feof(file)==0)
		{
			if (fscanf(file,"%i %i\n",&dum1,&dum2)!=2)
			{
				fprintf(stderr,"File %s corrupt at line %i!\n",filename,dum3);
				return -1;
			}
			dum3++;
		}
		if (i==0) dum4 = dum3;
		if (dum3 != dum4)
		{
				fprintf(stderr,"Files from %s and %s dataset do not have the same number of lines %i %i!\n",season[0],season[1],dum4,dum3);
				return -1;
		}
		fclose(file);
	}
	numTests = dum4;
	printf("Dataset seems to be OK: %i seasons and %i locations\n",seasons,numTests);
	/*allocate variables*/
	offsetX = (int*)malloc(sizeof(int)*numTests*seasons);
	offsetY = (int*)malloc(sizeof(int)*numTests*seasons);

	/*read offsets*/
	for (int i = 0;i<seasons;i++){
		sprintf(filename,"%s/%s/displacements.txt",dataset,season[i]);
		file = fopen(filename,"r");
		dum3 = 0;
		while (feof(file) == 0)
		{
			fscanf(file,"%i\t%i\n",&dum1,&dum2);
			offsetX[i*numTests+dum3] = dum1;
			offsetY[i*numTests+dum3] = dum2;
			dum3++;
		}
		fclose(file);
	}
}

/*class FASTDetector:FeatureDetector
{
public:
  FASTDetector(int thr){thre = thr;}
  ~FASTDetector();
private:
  int thre;
public:
  void detectImpl(const Mat& image, vector<KeyPoint>& keypoints, int threshold)
  {
	  FAST(image,keypoints,thre);
  }
};*/


/*initialize detector*/
int initializeDetector(char *nameI)
{
	char name[100];
	strcpy(name,nameI);
	/*modifiers*/
	if (strncmp("up-",name,3)==0)	{upright = true;strcpy(name,&nameI[3]);}
	if (strncmp("norm-",name,5)==0)	{upright = false;strcpy(name,&nameI[5]);}
	/*detectors*/
	if (strcmp("sift",  name)==0)  	detector = new SIFT(0,3,0.0,10,1.6);
	if (strcmp("surf",  name)==0)  	detector = new SURF(0);
	if (strcmp("star",  name)==0)  	detector = new StarFeatureDetector(45,0,10,8,5);
	if (strcmp("brisk", name)==0) 	detector = new BRISK(0,4);
	if (strcmp("orb",   name)==0) 	detector = new OrbFeatureDetector(maxFeatures,1.2f,8,31,0,2,0,31); 
	if (strcmp("fast",  name)==0)	detector = new FastFeatureDetector(0,true);
	if (strcmp("mser",  name)==0)	detector = new MSER(2);
}

/*initialize detector*/
int initializeDescriptor(char *nameI)
{
	char name[100];
	strcpy(name,nameI);
	/*modifiers*/
	if (strncmp("root-",name,5)==0){normalizeSift= true;strcpy(name,&nameI[5]);}
	/*descriptors*/
	if (strcmp("sift",  name)==0)  	{norm2=true;descriptor = new SIFT(0,3,0.0,10,1.6);}
	if (strcmp("surf",  name)==0)   {norm2=true;descriptor = new SURF(0);}
	if (strcmp("brisk", name)==0)   {norm2=true;descriptor = new BRISK(0,4);}
	if (strcmp("brief", name)==0)   {norm2=false;descriptor = new BriefDescriptorExtractor(32);}
	if (strcmp("grief", name)==0)   {norm2=false;descriptor = new GriefDescriptorExtractor(32);}
	if (strcmp("orb",   name)==0)   {norm2=false;descriptor = new OrbFeatureDetector(maxFeatures,1.2f,8,31,0,2,0,31);} 
}

int main(int argc, char ** argv) 
{
	srand (time(NULL));
	strcpy(dataset,argv[3]);
	strcpy(descriptorName,argv[2]);
	strcpy(detectorName,argv[1]);
	initializeDateset();
	if (argc > 4 && strcmp(argv[4],"draw")==0) drawAll = true;
	if (argc > 4 && strcmp(argv[4],"save")==0) save = true;
	if (argc > 4 && strcmp(argv[4],"update")==0) update = true;
	if (update) output = fopen("newcon.txt","w+");
	distance_factor = 1.0;
	memset(numFails,0,(maxFeatures/100+1)*sizeof(int));	

	for (int ims=0;ims<numTests;ims++)
	{
		delete detector;
		delete descriptor;
		char filename[100];
		Mat im[seasons];
		Mat img[seasons];
		Mat descriptors[seasons];
		vector<KeyPoint> keypoints[seasons];
		for (int s = 0;s<seasons;s++)
		{
			sprintf(filename,"%s/%s/%09i.bmp",dataset,season[s],ims);
			im[s] =  imread(filename, CV_LOAD_IMAGE_COLOR);
			img[s] = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
			if (img[s].empty())
			{
				printf("Can't read image %s\n",filename);
				return -1;
			}
		}
		initializeDetector(detectorName);
		initializeDescriptor(descriptorName);
		KeyPoint kp;
		Mat dp;

		for (int s = 0;s<seasons;s++)
		{
			/*extraction*/
			time0 = getTime();
			detector->detect(img[s], keypoints[s]);
			/*if (strcmp("mser", detectorName)==0)
			{
				for (int j = 0;j<keypoints[s].size();j++) keypoints[s][j].response = ;
			}*/	
			sort(keypoints[s].begin(),keypoints[s].end(),compare_response);
			if (upright)for (int j = 0;j<keypoints[s].size();j++) keypoints[s][j].angle = -1;
			keypoints[s].resize(maxFeatures);
			descriptor->compute(img[s],keypoints[s],descriptors[s]);
			if (normalizeSift) rootSift(&descriptors[s]);	
		}

		for (int nFeatures=maxFeatures;nFeatures>minFeatures;nFeatures-=100)
		{
			Mat descriptors1,descriptors2;
			vector<KeyPoint> keypoints1,keypoints2;
			descriptors1 = descriptors[0];
			descriptors2 = descriptors[1];
			keypoints1 = keypoints[0];	
			keypoints2 = keypoints[1];	
			numFeatures = nFeatures;
			int numRemove = max(0,descriptors1.rows-numFeatures); 
			descriptors1.pop_back(numRemove);
			numRemove = max(0,descriptors2.rows-numFeatures); 
			descriptors2.pop_back(numRemove);
			time2 = time1 = getTime();
			vector<DMatch> matches, inliers_matches;
			int sumDev,auxMax,histMax;
			sumDev = auxMax = histMax = 0;

			// matching descriptors
			time3 = getTime();
			matches.clear();
			inliers_matches.clear();
			if (descriptors1.rows*descriptors2.rows > 0) distinctiveMatch(descriptors1, descriptors2, matches, norm2, CROSSCHECK);
			time4 = getTime();

			if (matches.size() > 0){

				//histogram assembly

				if (hist2D){
					int iX = 0;
					int iY = 0;

					for(int i = 0; i < width*2+granularity; i++ )
					{
						for(int j = 0; j < height*2+granularity; j++ ) histogram2D[i][j]=0;
					}

					//create the histogram
					for(size_t i = 0; i < matches.size(); i++ )
					{
						int i1 = matches[i].queryIdx;
						int i2 = matches[i].trainIdx;
						int iXO = (int)(keypoints1[i1].pt.x-keypoints2[i2].pt.x + width);
						int iYO = (int)(keypoints1[i1].pt.y-keypoints2[i2].pt.y + height);
						for(int ii = 0; ii < granularity; ii++ ){
							for(int ij = 0; ij < granularity; ij++ ){
								iX = iXO+ii;
								iY = iYO+ij;
								if (iX > -1 && iX < width*2 && iY > -1 && iY < height*2) histogram2D[iX][iY]++;
							}
						}
					}

					//find histogram maximum
					int index = 0;
					histMax = 0;
					for (int i = 0;i<width*2;i++){
						for (int j = 0;j<height*2;j++){
							if (histMax < histogram2D[i][j]){
								histMax = histogram2D[i][j];
								iX = i;
								iY = j;
							}
						}
					}
					sumDev = iX-width-granularity/2;
					sumDev = sumDev*histMax;
					auxMax = 0;
				}else{
					//histogram assembly
					int numBins = 100; 
					int histogram[numBins];
					int bestHistogram[numBins];
					memset(bestHistogram,0,sizeof(int)*numBins);
					histMax = 0;
					int maxS,domDir;
					for (int s = 0;s<granularity;s++){
						memset(histogram,0,sizeof(int)*numBins);
						for( size_t i = 0; i < matches.size(); i++ )
						{
							int i1 = matches[i].queryIdx;
							int i2 = matches[i].trainIdx;
							if ((fabs(keypoints1[i1].pt.y-keypoints2[i2].pt.y))<VERTICAL_LIMIT){
								int devx = (int)(keypoints1[i1].pt.x-keypoints2[i2].pt.x + numBins/2*granularity);
								int index = (devx+s)/granularity;
								if (index > -1 && index < numBins) histogram[index]++;
							}
						}
						for (int i = 0;i<numBins;i++){
							if (histMax < histogram[i]){
								histMax = histogram[i];
								maxS = s;
								domDir = i;
								memcpy(bestHistogram,histogram,sizeof(int)*numBins);
							}
						}
					}

					auxMax=0;
					for (int i =0;i<numBins;i++){
						if (auxMax < bestHistogram[i] && bestHistogram[i] != histMax){
							auxMax = bestHistogram[i];
						}
					}

					sumDev = 0;
					for( size_t i = 0; i < matches.size(); i++ ){
						int i1 = matches[i].queryIdx;
						int i2 = matches[i].trainIdx;

						if ((int)((keypoints1[i1].pt.x-keypoints2[i2].pt.x + numBins/2*granularity+maxS)/granularity) == domDir && fabs(keypoints1[i1].pt.y-keypoints2[i2].pt.y)<VERTICAL_LIMIT)
						{
							sumDev += keypoints1[i1].pt.x-keypoints2[i2].pt.x;
							inliers_matches.push_back(matches[i]);
						}
					}
					//if (histMax > 0) printf("\nDirection histogram %i %i %i\n",-(sumDev/histMax),histMax,auxMax); else printf("\nDirection histogram 1000 0 0\n");

				}
				if (histMax > 0) difference = (sumDev/histMax)-(offsetX[ims+numTests]-offsetX[ims]); else difference = 1000;
				//if (histMax > 0) printf("\nDirection histogram %i %i %i\n",-(sumDev/histMax),histMax,auxMax); else printf("\nDirection histogram 1000 0 0\n");
				//if (histMax > 0) printf("%05i %05i %i %i %i %i %i\n",imNum1,imNum2,difference,-(sumDev/histMax),-offsetX[ims],histMax,auxMax); else printf("%05i %05i 1000 1000 %i 0 0\n",imNum1,imNum2,offsetX[ims]);
				if (drawAll==false && update) draw = (abs(difference) > 35); else draw = drawAll;
				//if (update && draw == false) fprintf(output,"Offset %i %i %i %i\n",imNum1,imNum2,offsetX[ims],offsetY[ims]);
				//if (histMax > 0) fprintf(output,"%05i %05i %i %i %i %i %i\n",imNum1,imNum2,-(sumDev/histMax)+offset[ims],-(sumDev/histMax),-offset[ims],histMax,auxMax); else fprintf(output,"%05i %05i 1000 1000 %i 0 0\n",imNum1,imNum2,offset[ims]);
			}else{
				difference = 1000;
				printf("%05i %05i 1000 1000 %i 0 0\n",ims,ims,offsetX[ims]);
				draw = update;
			}
			if (fabs(difference) > 35){
				numFails[numFeatures/100]++;
			}
			if (draw)
			{
				Mat imA,imB,img_matches,img_matches_transposed;
				vector<KeyPoint> kpA,kpB;
				KeyPoint kp;
				kpA.clear();
				kpB.clear();
				for (int s=0;s<keypoints1.size();s++){
					kp = keypoints1[s];
					kp.pt.x = keypoints1[s].pt.y;
					kp.pt.y = keypoints1[s].pt.x;
					kpA.push_back(kp);
				}
				for (int s=0;s<keypoints2.size();s++){
					kp = keypoints2[s];
					kp.pt.x = keypoints2[s].pt.y;
					kp.pt.y = keypoints2[s].pt.x;
					kpB.push_back(kp);
				}
				cv::transpose(im[0], imA);
				cv::transpose(im[1], imB);
				namedWindow("matches", 1);
				if (kpA.size() >0 && kpB.size()>0 && inliers_matches.size() >0){
					drawMatches(imA, kpA, imB, kpB, matches, img_matches, Scalar(0, 0, 255), Scalar(0, 0, 255), vector<char>(), 0);
				}else{
					kpA.push_back(kp);
					kpB.push_back(kp);
					drawMatches(imA, kpA, imB, kpB, inliers_matches, img_matches, Scalar(0, 0, 255), Scalar(0, 0, 255), vector<char>(), 0);
				}
				cv::transpose(img_matches,img_matches_transposed);
				imshow("matches", img_matches_transposed);
				waitKey(0);
			}
		}
		printf("Test %i of %i \n",ims+1,numTests);
	}
	if (update) fclose(output);

	numFails[0] = numTests;
	char report[100];
	sprintf(report,"%s/results/%s_%s.histogram",dataset,detectorName,descriptorName);
	FILE* summary = fopen(report,"w+");
	for (int n=0;n<=maxFeatures/100;n++) fprintf(summary,"%02i %.4f\n",n,100.0*numFails[n]/numTests);
	fclose(summary);
	delete seq1;
	delete seq2;
	delete offsetX;
	delete offsetY;
	return 0;
}
