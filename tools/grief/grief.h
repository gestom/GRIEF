#include <sys/time.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include "opencv2/core.hpp"
#ifdef HAVE_OPENCV_XFEATURES2D
#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"
#include <stdio.h>
#include <fstream>
#include <opencv2/flann/miniflann.hpp>

using namespace std;
 
namespace cv{

class CV_EXPORTS GriefDescriptorExtractor : public DescriptorExtractor
{
public:
    static const int PATCH_SIZE = 48;
    static const int KERNEL_SIZE = 9;

    // bytes is a length of descriptor in bytes. It can be equal 16, 32 or 64 bytes.
    GriefDescriptorExtractor( int bytes = 32 );
    //CV_WRAP static Ptr<GriefDescriptorExtractor> create( int bytes = 32);

    virtual void read( const FileNode& );
    virtual void write( FileStorage& ) const;

    virtual int descriptorSize() const;
    virtual int descriptorType() const;

    virtual void computeImpl(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) const;


    /// @todo read and write for brief

//    AlgorithmInfo* info() const;

protected:

    typedef void(*PixelTestFn)(const Mat&, const vector<KeyPoint>&, Mat&);

    int bytes_;
    PixelTestFn test_fn_;
};
}

#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif
