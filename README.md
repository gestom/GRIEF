##GRIEF - trainable feature descriptor based on BRIEF 
======

This project adresses visual-based navigation of mobile robots in outdoor environments.
In particular, we adress the robustness of image features to seasonal changes in outdoor environments.
We provide an evolutionary algorithm that allows to train the BRIEF features to be robust to environmental changee.
We call this feature GRIEF (Generated BRIEF).
Moreover, we provide a simple framework that allows to benchmark the feature extractors.
The feature and its evaluation is described in detail at European Conference on Mobile robotics [[1](#references)]. 

##GRIEF training

1) running <b>./scripts/evolveGrief.sh NUMBER</b> will evolve a NUMBER generations of GRIEF 
2) you can create your own training dataset with 12 images to train the GRIEF - see <i>training_data</i> for a reference,
3) to reset the GRIEF to be the same as BRIEF <b>./scripts/resetGrief.sh</b>
4) to switch to the GRIEF that was used in [[1](#references)], run <b>./scripts/restoreGrief.sh</b>

Note, that the <i>grief_history</i> directory contains comparisons for the individual GRIEF generations and their fitness.

##Feature evaluation

###A quick test
1) Go to <i>tools</i> and compile the <i>test_all</i> util: <b>cd tools/test;make test_all;cd ../..</b>,
2) run the benchmark on the provided dataset: <b>./scripts/match.sh testing_data</b>.
3) Show the results: <b>./scripts/draw.sh; evince rates.pdf</b>

###To use on own data

1) prepare a dataset directory, see the <i>testing_data</i> folder for reference,
2) select detectors and descriptors to use by editing the <i>detectors</i> and <i>descriptors</i> files in the settings directory,
3) run <i>./scripts/match.sh NAME</i> - this performs the benchmark and populates the <i>NAME/results</i> directory
4) run <i>./scripts/draw.sh</i>  - creates a graph with the results

##Directory structure of our project

###settings:
- the first lines of the <i>detectors</i> and <i>descriptors</i> contain the detectors and descriptors that will be used for the benchmark

###testing_data
- the first 100 pictures of the stromovka-path long-term dataset including manual, hand-made ground-truth in the <i>displacements.txt</i> files
	- <i>results</i> subfolder contains benchmarking results

###tools:
- binaries for testing and GRIEF evolution

###scripts:
- match.sh DATASET 		- performs the benchmark and fills the DATASET/results 
- evolveGrief.sh NUMBER		- evolves GRIEF for NUMBER generations on the 12 images provided in the <i>training_data</i> 
- match-all.sh DATASET 		- performs the benchmark, fills the DATASET/results, combines every detector with every descriptor from the first lines of the setting/detector and setting/descriptors files
- draw.sh 			- generates a graph <i>rates.pdf</i> from the <i>DATASET/results</i> directory


##References
======
1. T.Krajnik, P.Cristoforis, M.Nitsche, K. Kusumam, T.Duckett: <b>[Image features and seasons revisited.](http:/github.com/gestom/GRIEF/papers/GRIEF_ECMR_2015.pdf)</b> ECMR 2015.

