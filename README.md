# Datasets and code for ICRA 2020 submission number 1622 

This page contains supporting materials for ICRA 2020 submission number 1622, which is currently in review.
More info will be released once the review process is completed.

## Image Features and Style Transfer for Visual Teach-and-Repeat Navigation in Changing Environments

This project adresses visual-based navigation of mobile robots in outdoor environments.
In particular, we adress the robustness of image features to seasonal changes in outdoor environments.
First, we provide a simple framework that allows to benchmark the feature extractors and we also provide two datasets pre-processed with GAN-based style transfer.

## Dependencies

### Analythic tools 
 
To analyse the results of the evaluation, you need to install a few additional libraries: 

1. To do so, run the following commands: `sudo apt install gnuplot xfig transfig  libalglib-dev`

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

The evaluation datasets can be obtained at links provided in the [dataset_link](https://raw.githubusercontent.com/gestom/GRIEF/style_transfer/dataset_link.txt)
Just open any of the links provided in a browser, download and unzip the file so that the #stromovka# and #nclt# folders reside in the #grief# one 
You should obtain the same


## Datasets

The datasets we used for evaluation are available for download at my [google drive](https://drive.google.com/open?id=1EIsLjW8Z995wYPj3WJWYtURAAF9PgLvw) and at [L-CAS owncloud](https://lcas.lincoln.ac.uk/owncloud/shared/datasets/).

## Feature evaluation

### Testing the main program 

1. Go to <i>tools</i> and compile the <i>match_all</i> utilily: <b>cd tools;make;cd ..</b>,
1. Run <b>./tools/match_all DETECTOR DESCRIPTOR DATASET</b> to perform the evaluation of a single detector/descriptor combination (e.g. <b>./tools/match_all star brief GRIEF-dataset/michigan</b>),
1. After the tests finishes, have a look in the <i>dataset/results/</i> directory for a <i>detector_descriptor.histogram</i> file (e.g. <i>GRIEF-datasets/michigan/results/up-surf_brief.histogram</i>),
1. Run the benchmark on the provided dataset: <b>./scripts/match_all.sh DATASET</b>.

### Running benchmarks 

1. The first lines of the <i>detectors</i> and <i>descriptors</i> files in the <i>settings</i> folder contain the detectors and descriptors that will be used for the benchmark. You can select the detectors and descriptors for the benchmark by editing these files. Ty to modify the first line of the <i>settings/detectors</i> so that it contains <i>star up-sift</i>, and the first line of the <i>settings/descriptors</i> so that it contains <i>brief root-sift</i>.
1. To run a benchmark of all detector/descriptor combinations: <b>./scripts/match_all.sh DATASET</b>. For example, running the <b>./scripts/match_all.sh GRIEF-datasets/michigan</b> with the files set according to the previous point will test four image features: <i>star+brief</i>, <i>star+root-sift</i>, <i>up-sift+brief</i> and <i>up-sift+root-sift</i> on the <i>GRIEF-datasets/michigan</i> dataset.
1. To run a benchmark that will test the detector/descriptor pairs in a successive way, run <b>./scripts/match.sh DATASET</b>. That is, running the <b>./scripts/match.sh GRIEF-datasets/michigan</b> with the <i>settings/detectors</i> and <i>settings/descriptors</i> files set according to point 1 will test <i>star+brief</i> and <i>up-sift+root-sift</i> image features.

### Evaluation of results
1. The scripts, which evaluate the results obtained by running the benchmarks, evaluate the detectors and descriptors from the first lines of the files in the <i>settings</i> folder.
1. Running <b>./scripts/benchmark_evolution.sh DATASET</b> evaluates every iteration of the GRIEF algorithm stored in the <i>grief_history</i> on a given DATASET.
1. Running <b>./scripts/benchmark_precision.sh DATASET</b>  creates a latex-formatted table that contains the error rates of the detector/descriptor combinations. 
1. Running <b>./scripts/draw.sh DATASET</b> draws the dependence of the heading estimation error on the number of features extracted and stores the results in <i>rates.fig</i> and <i>rates.pdf</i> files.

## GRIEF training

1. To initiate the training, you need to set the initial comparisons of the GRIEF feature. Either reset the GRIEF to be the same as BRIEF by <b>./scripts/resetGrief.sh</b> or switch to the GRIEF that was used in [[1](#references)] by running <b>./scripts/restoreGrief.sh</b>.
1. Running <b>./scripts/evolveGrief.sh DATASET NUMBER</b> will evolve a NUMBER of GRIEF generations on DATATASET, e.g. <b>./scripts/evolveGrief.sh GRIEF-dataset/michigan 100</b>.
1. Training will be speeded-up if you restrict the number of images by creating a smaller dataset just for training.
1. To switch to an arbitrary GRIEF that was generated during the training, run <b>./scripts/restoreGrief.sh [grief_file]</b>. The <i>grief_files</i>  are in <i>grief_history</i> directory, which contains comparisons for the individual GRIEF generations and their fitness.

## References
1. T.Krajnik, P.Cristoforis, K. Kusumam, P.Neubert, T.Duckett: <b>[Image features for Visual Teach-and-Repeat Navigation in Changing Environments.](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_RAS_2016.pdf)</b> Journal of Robotics and Autonomous Systems, 2016 <i>[bibtex](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_RAS_2016.bib)</i>.
1. T.Krajnik, P.Cristoforis, M.Nitsche, K. Kusumam, T.Duckett: <b>[Image features and seasons revisited.](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_ECMR_2015.pdf)</b> ECMR 2015. <i>[bibtex](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_ECMR_2015.bib)</i>.

## Acknowledgements
This research is currently supported by the Czech Science Foundation project 17-27006Y _STRoLL_.
It was also funded by the EU ICT project 600623 _STRANDS_.
