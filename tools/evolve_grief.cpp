#include <sys/time.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <fstream>
#include "grief/grief.h"

#define CROSSCHECK true 
#define VERTICAL_LIMIT 100

bool save=false;
bool draw=false; 
using namespace std;
using namespace cv;
unsigned int n;
float distance_factor = 1.0;
int griefDescriptorLength= 256;

typedef struct{
	int id;
	int value;
}TRating;

TRating griefRating[1024];

int time0,time1,time2,time3,time4,time5,time6;

int compare(const void * a, const void * b)
{
  if ( (*(TRating*)a).value <  (*(TRating*)b).value ) return +1;
  if ( (*(TRating*)a).value >  (*(TRating*)b).value ) return -1;
  return 0;
}

int generateNew()
{
	int x1[512];
	int y1[512];
	int x2[512];
	int y2[512];
	int xWindow = 51;
	int yWindow = 51;
	FILE* file = fopen("tools/grief/test_pairs.txt","r+");
	for (int i = 0;i<512;i++){
		fscanf(file,"%i %i %i %i\n",&x1[i],&y1[i],&x2[i],&y2[i]);
	}
	fclose(file);

	file = fopen("tools/grief/pair_stats.txt","w");
	for (int i = 0;i<griefDescriptorLength;i++){
		fprintf(file,"%i %i %i %i %i\n",x1[i],y1[i],x2[i],y2[i],griefRating[i].value);
	}
	fclose(file);

	int pairs = 0;
	int sum = 0;
	qsort (griefRating,griefDescriptorLength,sizeof(TRating),compare);
	for (int i = 0;i<griefDescriptorLength;i++){
		 printf("Ratio %i %i \n",griefRating[i].id,griefRating[i].value);
		 sum+=griefRating[i].value;
	}
	sum=sum/griefDescriptorLength;

	//exchange comparions so that it conforms the window size (this is typically not used)
	for (int i = 0;i<griefDescriptorLength;i++){
		if (abs(x1[i]) > xWindow/2 || abs(y1[i]) > yWindow/2 || abs(x2[i]) > xWindow/2 || abs(y2[i]) > yWindow/2)
		{
			x1[i] = rand()%xWindow-xWindow/2;
			y1[i] = rand()%yWindow-yWindow/2;
			//x2[i] = rand()%xWindow-xWindow/2;
			//y2[i] = rand()%yWindow-yWindow/2;
			x2[i] = rand()%2;
			y2[i] = rand()%2;
			i = griefDescriptorLength;
		}
	}

	//exchange 10 comparisons by 10 random ones
	for (int i = griefDescriptorLength-10;i<griefDescriptorLength;i++){
		int id = griefRating[i].id;
		x1[id] = rand()%xWindow-xWindow/2;
		y1[id] = rand()%yWindow-yWindow/2;
		//x2[i] = rand()%xWindow-xWindow/2;
		//y2[i] = rand()%yWindow-yWindow/2;
		x2[id] = rand()%2;
		y2[id] = rand()%2;
	}

	/*
	//calculate statistics
	float avgX,avgY,devX,devY,avgDX,avgDY,devDX,devDY; 
	avgX = avgY = devX = devY = avgDX = avgDY = devDX = devDY = 0; 
	for (int i = 0;i<griefDescriptorLength;i++)
	{
		avgX+=x1[i]+x2[i];	
		avgY+=y1[i]+y2[i];	
		avgDX+=abs(x1[i]-x2[i]);	
		avgDY+=abs(y1[i]-y2[i]);	
	}
	avgX  =  avgX/(2*griefDescriptorLength);
	avgY  =  avgY/(2*griefDescriptorLength);
	avgDX = avgDX/griefDescriptorLength;
	avgDY = avgDY/griefDescriptorLength;
	for (int i = 0;i<griefDescriptorLength;i++)
	{
		devX+=sqrt((x1[i]-avgX)*(x1[i]-avgX));
		devX+=sqrt((x2[i]-avgX)*(x2[i]-avgX));
		devY+=sqrt((y1[i]-avgY)*(y1[i]-avgY));
		devY+=sqrt((y2[i]-avgY)*(y2[i]-avgY));
		devDX+=sqrt((abs(x1[i]-x2[i])-avgDX)*(abs(x1[i]-x2[i])-avgDX));
		devDY+=sqrt((abs(y1[i]-y2[i])-avgDY)*(abs(y1[i]-y2[i])-avgDY));
	}
	devX  =  devX/(2*griefDescriptorLength);
	devY  =  devY/(2*griefDescriptorLength);
	devDX = devDX/griefDescriptorLength;
	devDY = devDY/griefDescriptorLength;
	fprintf(stdout,"Positions:   X: %.2f %.2f Y: %.2f %.2f \n",avgX,devX,avgY,devY);
	fprintf(stdout,"Differences: X: %.2f %.2f Y: %.2f %.2f \n",avgDX,devDX,avgDY,devDY);
	int idevDX,idevDY,iavgDX,iavgDY,idevX,idevY;
	idevDX = (int)(devDX+1.0);
	idevDY = (int)(devDY+1.0);
	iavgDX = (int)(avgDX+1.0);
	iavgDY = (int)(avgDY+1.0);
	idevX = (int)(devX+1.0);
	idevY = (int)(devY+1.0);
	fprintf(stdout,"Rounded: %i %i X: %i %i Y: %i %i \n",idevX,idevY,iavgDX,idevDX,iavgDY,idevDY);

	for (int i = griefDescriptorLength-10;i<griefDescriptorLength;i++)
	{
		int dx = rand()%(idevDX*2+1)-idevDX;//+iavgDX;
		int dy = rand()%(idevDY*2+1)-idevDY;//+iavgDY;
		int id = griefRating[i].id;
		//x1[id] = rand()%(idevX*2+1)-idevX;
		//y1[id] = rand()%(idevY*2+1)-idevY;
		x1[id] = rand()%xWindow-xWindow/2;
		y1[id] = rand()%yWindow-yWindow/2; 
		if (rand()%2==0) x2[id] = x1[id] + dx; else x2[id] = x1[id] - dx; 
		if (rand()%2==0) y2[id] = y1[id] + dy; else y2[id] = y1[id] - dy; 
		if (abs(x1[id]) > xWindow/2 ||abs(x2[id]) > xWindow/2 ||abs(y1[id]) > yWindow/2 ||abs(y2[id]) > yWindow/2) i--;
	}
*/

	file = fopen("tools/grief/test_pairs.txt","w");
	for (int i = 0;i<512;i++){
		fprintf(file,"%i %i %i %i\n",x1[i],y1[i],x2[i],y2[i]);
	}
	printf("Population fitness: %i\n",sum);
	fclose(file);
}


int getTime()
{
  struct  timeval currentTime;
  gettimeofday(&currentTime, NULL);
  return currentTime.tv_sec*1000 + currentTime.tv_usec/1000;
}

void printResults(vector<KeyPoint> keypoints1, vector<KeyPoint> keypoints2, vector<DMatch> matches, vector<DMatch> inliers_matches)
{
//   cout << "Detected features image 1: " << keypoints1.size() << endl;
//   cout << "Detected features image 2: " << keypoints2.size() << endl;
//   cout << "Detected matches : " << matches.size() << endl;
//   cout << "Correct matches: " << inliers_matches.size() << endl;
//   cout << "Incorrect matches: " << matches.size() - inliers_matches.size() << endl;
//   cout << "Extractor time: " << time2-time0 << endl;
//   cout << "Matcher time: " << time4-time3 << endl;
//   cout << "RANSAC time: " << time6-time5 << endl;
}

//feature matching - this can combine 'ratio' and 'cross-check' 
void distinctiveMatch(const Mat& descriptors1, const Mat& descriptors2, vector<DMatch>& matches, bool norm2= true, bool crossCheck=false)
{
   Ptr<DescriptorMatcher> descriptorMatcher;
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

}


int main(int argc, char ** argv) 
{
	srand (time(NULL));
	int detectorThreshold = atoi(argv[1]);
	// process command line args
	if(argc != 3 && argc != 4){
		return -1;
	}
	if (argc > 3 && strcmp(argv[3],"draw")==0) draw = true;
	if (argc > 3 && strcmp(argv[3],"save")==0) save = true;

	distance_factor = atof(argv[2])/100.0;
	char filename[100];
	Mat im[12];
	Mat img[12];

	//we assume that there are 12 images from the same location to train
	for (int i=0;i<12;i++){
		sprintf(filename,"training_data/%04i.bmp",i);
		im[i] =  imread(filename, CV_LOAD_IMAGE_COLOR);
		img[i] = imread(filename, CV_LOAD_IMAGE_GRAYSCALE);
		if(img[i].empty())
		{
			printf("Can't read image %s\n",filename);
			return -1;
		}
	}

	// detecting keypoints and generating descriptors
	Mat descriptors[12];
	vector<KeyPoint> keypoints[12];
	KeyPoint kp;
	Mat dp;

	bool norm2 =true;
	cout << "Detecting STAR features and extracting genetically modified BRIEF descriptors"  << endl;
	StarFeatureDetector detector(45,detectorThreshold,10,8,5);
	GriefDescriptorExtractor extractor(griefDescriptorLength/8);

	time0 = getTime();
	for (int i = 0;i<12;i++){
		detector.detect(img[i], keypoints[i]);
		extractor.compute(img[i], keypoints[i], descriptors[i]);
	}
	time2 = time1 = getTime();
	norm2 = false;

	//reset the comparison ratings
	vector<DMatch> matches, inliers_matches;
	int matcher = 0;
	for (int i = 0;i<1024;i++){
		griefRating[i].value=0;
		griefRating[i].id=i;
	}

	// matching descriptors
	for (int ik = 0;ik<12;ik++){
		for (int jk = 0;jk<12;jk++){
			time3 = getTime();
			matches.clear();
			inliers_matches.clear();
			if (descriptors[ik].rows*descriptors[jk].rows > 0) distinctiveMatch(descriptors[ik], descriptors[jk], matches, norm2, CROSSCHECK);
			time4 = getTime();

			if (matches.size() > 0){
				//histogram assembly
				int numBins = 100; 
				int granularity = 20;
				int histogram[numBins];
				int bestHistogram[numBins];
				memset(bestHistogram,0,sizeof(int)*numBins);
				int histMax = 0;
				int maxS,domDir;
				for (int s = 0;s<granularity;s++){
					memset(histogram,0,sizeof(int)*numBins);
					for( size_t i = 0; i < matches.size(); i++ )
					{
						int i1 = matches[i].queryIdx;
						int i2 = matches[i].trainIdx;
						if ((fabs(keypoints[ik][i1].pt.y-keypoints[jk][i2].pt.y))<VERTICAL_LIMIT){
							int devx = (int)(keypoints[ik][i1].pt.x-keypoints[jk][i2].pt.x + numBins/2*granularity);
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

				int auxMax=0;
				for (int i =0;i<numBins;i++){
					if (auxMax < bestHistogram[i] && bestHistogram[i] != histMax){
						auxMax = bestHistogram[i];
					}
				}

				int sumDev = 0;
				for( size_t i = 0; i < matches.size(); i++ ){
					int i1 = matches[i].queryIdx;
					int i2 = matches[i].trainIdx;
					char eff = 0;
					
					if ((int)((keypoints[ik][i1].pt.x-keypoints[jk][i2].pt.x + numBins/2*granularity+maxS)/granularity) == domDir && fabs(keypoints[ik][i1].pt.y-keypoints[jk][i2].pt.y)<VERTICAL_LIMIT)
					{
						sumDev += keypoints[ik][i1].pt.x-keypoints[jk][i2].pt.x;
						inliers_matches.push_back(matches[i]);
						eff = -1;
					}else{
						eff = +1;
					}
					for (int o = 0;o<griefDescriptorLength/8;o++){
						unsigned char b = descriptors[ik].at<uchar>(i1,o)^descriptors[jk].at<uchar>(i2,o);
						unsigned char oo = 128;
						for (int p = 0;p<8;p++){
							if (oo&b)  griefRating[8*o+p].value+=eff; else griefRating[8*o+p].value-=eff;
							oo=oo/2;
						}
					}
				}
				//if (histMax > 0) printf("\nDirection histogram %i %i %i\n",-(sumDev/histMax),histMax,auxMax); else printf("\nDirection histogram 1000 0 0\n");
				if (histMax > 0) printf("%02i%02i %i %i %i\n",ik+1,jk+1,-(sumDev/histMax),histMax,auxMax); else printf("%02i%02i 1000 0 0\n",ik+1,jk+1);
			}else{
				printf("%02i%02i 1000 0 0\n",ik+1,jk+1);
			}
			printResults(keypoints[ik], keypoints[jk], matches, inliers_matches);

			//for drawing the results
			if (draw){
				Mat imA,imB,img_matches,img_matches_transposed;
				vector<KeyPoint> kpA,kpB;
				KeyPoint kp;
				kpA.clear();
				kpB.clear();
				for (int s=0;s<keypoints[ik].size();s++){
					kp = keypoints[ik][s];
					kp.pt.x = keypoints[ik][s].pt.y;
					kp.pt.y = keypoints[ik][s].pt.x;
					kpA.push_back(kp);
				}
				for (int s=0;s<keypoints[jk].size();s++){
					kp = keypoints[jk][s];
					kp.pt.x = keypoints[jk][s].pt.y;
					kp.pt.y = keypoints[jk][s].pt.x;
					kpB.push_back(kp);
				}
				cv::transpose(im[ik], imA);
				cv::transpose(im[jk], imB);
				namedWindow("matches", 1);
				drawMatches(imA, kpA, imB, kpB, inliers_matches, img_matches, Scalar(0, 0, 255), Scalar(0, 0, 255), vector<char>(), 2);
				cv::transpose(img_matches,img_matches_transposed);
				imshow("matches", img_matches_transposed);
				waitKey(0);
			}
			//end drawing
		}
	}
	generateNew();
	return 0;
}
