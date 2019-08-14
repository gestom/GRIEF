This branch includes source files with workable detectors and descriptors (including grief) which differ from the previous version. A few changes had to be made because of the new version of openCV (openCV 3.4.7). The changes are usually applicable for multiple files.

- the include paths had to be changed because of the new arrangement of openCV:
    ie. '#include <opencv2/xfeatures2d.hpp>' instead of '#include <opencv2/nonfree/features2d.hpp>'

- some small changes were implemented due to different structure of some opencv libraries:
    ie. using Ptr instead of standard pointer
        using function static function create() as a generator

- there have been other small changes due to some compilation errors
