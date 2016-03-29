##GRIEF - trainable feature descriptor based on BRIEF 
======

This project adresses visual-based navigation of mobile robots in outdoor environments.
In particular, we adress the robustness of image features to seasonal changes in outdoor environments.
We provide an evolutionary algorithm that allows to train the BRIEF features to be robust to environmental changee.
We call this feature GRIEF (Generated BRIEF).
Moreover, we provide a simple framework that allows to benchmark the feature extractors.
The feature and its evaluation was first presented at European Conference on Mobile robotics [[1](#references)].
A new, improved version that also allows to combine different detectors and descriptors, is described in a paper that is currently reviewed in <i>Journal of Robotics and Autonomous Systems</i>. 

##Dependencies

The project itself depends on <i>openCV</i> and it uses the <i>openCV non-free</i> packages.
To install the openCV-nonfree, type this in terminal:

- sudo add-apt-repository --yes ppa:xqms/opencv-nonfree
- sudo apt-get update 
- sudo apt-get install libopencv-nonfree-dev
- sudo install libopencv-dev

Moreover, it uses the <i>gnuplot</i> and <i>transfig</i> packages to draw the results.
You can install those by:

- sudo apt-get install gnuplot xfig transfig 

##Datasets

The datasets we used for evaluation are available for download at my [google drive](https://drive.google.com/open?id=0B7TY_9FitfdlNkV6eW94NU5GVU0) and at [L-CAS owncloud](https://lcas.lincoln.ac.uk/owncloud/shared/datasets/).

##GRIEF training

1. running <b>./scripts/evolveGrief.sh DATASET NUMBER</b> will evolve a NUMBER of GRIEF generations on DATATASET,
1. training will be speeded-up if you restrict the number of images by creating a smaller dataset just for training,
1. to reset the GRIEF to be the same as BRIEF <b>./scripts/resetGrief.sh</b>
1. to switch to the GRIEF that was used in [[1](#references)], run <b>./scripts/restoreGrief.sh</b>
1. to switch to an arbitrary GRIEF that was generated during the training, run <b>./scripts/restoreGrief.sh [grief_file]</b>. The <i>grief_files</i>  are in <i>grief_history</i> directory, which contains comparisons for the individual GRIEF generations and their fitness.

##Feature evaluation

###Testing the main program 

1. Go to <i>tools</i> and compile the <i>match_all</i> utilily: <b>cd tools;make;cd ..</b>,
1. Run <b>./tools/match_all DETECTOR DESCRIPTOR DATASET</b> to perform the evaluation of a single detector/descriptor combination (e.g. <b>./tools/match_all star brief GRIEF-dataset/michigan</b>),
1. After the tests finishes, have a look in the <i>dataset/results/</i> directory for a <i>detector_descriptor.histogram</i> file (e.g. <i>GRIEF-datasets/michigan/results/up-surf_brief.histogram</i>),
1. Run the benchmark on the provided dataset: <b>./scripts/match_all.sh DATASET</b>.

###Running benchmarks 

1. The first lines of the <i>detectors</i> and <i>descriptors</i> files in the <i>settings</i> folder contain the detectors and descriptors that will be used for the benchmark. You can select the detectors and descriptors for the benchmark by editing these files. 
1. To run a benchmark of all detector/descriptor combinations  : <b>./scripts/match_all.sh dataset</b>.
1. To run a benchmark that will test the detector/descriptor pairs in a successive way: <b>./scripts/match.sh dataset</b> (see the script for reference).

###Evaluation of results
1. The scripts, which evaluate the results obtained by runnong the benchmarks, evaluate the detectors and descriptors from the first lines of the files in the <i>settings</i> folder. 
1. Running <i>./scripts/benchmark_evolution.sh DATASET</i> evaluates every interation of the GRIEF algorithm stored in the <i>grief_history</i> on a given DATASET. 
1. Running <i>./scripts/benchmark_speeds.sh</i>  creates a latex-formatted table that summarizes the time needed for detection, description and matching.
1. Running <i>./scripts/precision.sh</i>  creates a latex-formatted table that contains the error rates of the detector/descriptor combinations. 
1. Running <i>./scripts/draw.sh</i> draws (in xfig and pdf format) the dependence of the heading estimation error on the number of features extracted. 


##References
======
1. T.Krajnik, P.Cristoforis, M.Nitsche, K. Kusumam, T.Duckett: <b>[Image features and seasons revisited.](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_ECMR_2015.pdf)</b> ECMR 2015. <i>[bibtex](https://github.com/gestom/GRIEF/blob/master/papers/GRIEF_ECMR_2015.bib)</i>.
