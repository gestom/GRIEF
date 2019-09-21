# Materials for ICRA 2020 submission number 1622 

This page contains supporting materials for ICRA 2020 submission number 1622, which is currently in review.
More info will be released once the review process is completed.

## Image Features and Style Transfer for Visual Teach-and-Repeat Navigation in Changing Environments

This project adresses visual-based navigation of mobile robots in outdoor environments.
In particular, we adress the robustness of image features to seasonal changes in outdoor environments.
First, we provide a simple framework that allows to benchmark the feature extractors and we also provide two datasets pre-processed with GAN-based style transfer.

## Dependencies

### Analythic tools 
 
To analyse the results of the evaluation, you need to install a few additional libraries: 

1. To do so, run the following commands: `sudo apt install gnuplot xfig transfig  libalglib-dev imagemagick`

### OpenCV 

OpenCV has a weird policy towards some of the feature extraction methodsd.
They were kept in separate and now you need to perform some additional steps to have it working.
While there are a few other ways to have openCV running with all the necessary features used in our evaluation, the one here works for me.

1. Create a folder to perform the compilation and switch to it: `mkdir ~/opencv;cd ~/opencv`
1. Download opencv: `git clone -b 3.4 --single-branch https://github.com/opencv/opencv.git`
1. Download opencv-contrib: `git clone -b 3.4 --single-branch https://github.com/opencv/opencv_contrib.git`
1. Go to opencv folder, create a build folder and switch to it: `mkdir opencv/build;cd opencv/build`
1. Tell opencv to compile with the contrib: `cmake -DOPENCV_ENABLE_NONFREE:BOOL=ON -DOPENCV_EXTRA_MODULES_PATH=~/opencv/opencv_contrib/modules ~/opencv/opencv`
1. Compile it: `make -j5`.
1. Install it: `sudo make install`

## Prepare the evaluation framework

To download and prepare the evaluation tools:
1.  Clone the evaluation framework: `git clone -b style_transfer --single-branch https://github.com/gestom/grief.git`
1.  Switch to the folder with evaluation tools: `cd grief/tools`
1.  Compile the tools: `make`
1.  Go back `cd ..`
 
## Dataset

### Download
 
The evaluation datasets can be obtained at links provided in the [dataset_link](https://raw.githubusercontent.com/gestom/GRIEF/style_transfer/dataset_link.txt)
Just open any of the links provided in a browser, download and unzip the file so that the *stromovka* and *nclt* folders reside in the *grief* folder. 
The *stromovka* and *nclt* folders should contain both original images as described in Sections IV.ab of the paper.

### Preprocessing 

To prepare the datasets for evaluation, run `./scripts/preprocess_dataset.sh`.
This will restructure the datasets according to Section IV.c of the paper.

## Feature evaluation

### Testing the main program 

1. Test the evaluation, e.g. by **./tools/match_all star brief nclt_01**, to perform the evaluation of a single detector/descriptor combination on a signe dataset
1. After the test finishes, have a look in the *nclt_01/results/* folder to see the reports.

### Running the evaluations

1. You can set the detectors and descriptors for testing in the relevant files in the *settings* folder.
1. To run the benchmarks, simply run **./scripts/compute.sh**. This will process all the datasets with the required detector/descriptor combinations.
1. To evaluate the results run **./scripts/summarise.sh**. This will run pairwise statistical t-tests and identify optimal detector/descriptor combinations.

## Acknowledgements
This research is currently supported by the Czech Science Foundation project 17-27006Y _STRoLL_.
