
FaRaMoTics: Fast Ranging Models for roboTics

AUTHORS
Andreu Corominas Murtra, from Beta Robots ( www.beta-robots.com )
andreu (at) beta-robots.com

VERSION
2.0

LICENSE
The current license is BSD 

OVERVIEW
FaRaMoTics is a software library dedicated to fast computation of range measurement models in 3D environments. 
It is specially suited to compute expected range measurements in filtering or optimization applications where 2D/3D laser scanners or depth cameras are involved. 
It has ready to use functions to get these range data for the most popular devices such as:
      - Hokuyo UTM30LX
      - Hokuyo URGX
      - Leuze RS4
      - Kinect
      - SwissRange SR4000

APPROACH
The implementation renders 3D models viewed from the device position in optimally-sized windows, which allows fast 
rendering and depth retrieval. Optimal size of these windows is based on sensor parameters such as angular accuracy, 
angular aperture and min/max ranges. See [1] for further details.      
      
INPUT DATA 
Required inputs to run the models are:
- A 3D environment or object model, described in a format allowed by assimp lib (assimp.sourceforge.net/lib_html/index.html)
- To create rangeScan2D or rangeImage objects, it is required:
      - angular apertures (horizontal and vertical) [degrees]
      - number of range points (horizontal and vertical) [#]
      - angular resolution (horizontal and vertical) [degrees]
      - Minimum and maximum range [meters]
      
HISTORY
The first version (not publicited) was developed at IRI (www.iri.upc.edu) by the same author, as a research library 
for real time map-based particle filter localization. This project [2] used "faramotics inside", where hundreds of thousdands rays per second where computed in the correction step of the particle filter. 

RELEASE NOTE
This release version has been completely updated specially in two senses: 
      - Use of pose_state_time library for pose3d operations.
      - Use of assimp library for 3d model import.
Moreover, some minor bugs have been fixed and the whole code has been reorganized.

OPERATION SYSTEM
Tested and developed under Ubuntu 12.04. Not tested under other OS.

OPTIMAL HARDWARE
Performant graphics card improves speed, since most of computations are direct calls to openGL

DEPENDENCIES
The following packages/libraries are required to be installed on the system: 
      - g++, cmake (pkg)
      - Mesa implementation of OpenGL2.1 specification (libgl1-mesa-dev pkg)
      - freeglut (freeglut3-dev pkg)
      - assimp (model importer, assimp.sourceforge.net)
      - dlib (algebra, header only, dlib.net)
      - pose_state_time library (github.com/beta-robots/btr-libs/wiki/pose_state_time)
      
HOW TO INSTALL
 $ mkdir build
 $ cd build
 $ cmake ..
 $ make
 $ sudo make install

REFERENCES
[1]
@INPROCEEDINGS{corominas_simpar10,
  AUTHOR = {{Corominas Murtra}, A. and Trulls, E. and {Mirats Tur}, J.M. and Sanfeliu, A.},
  TITLE = {{Efficient Use of 3D Environment Models for Mobile Robot Simulation and Localization}},
  BOOKTITLE = Proceedings of the Simulation, Modelling and Programming for Autonomous Robots (SIMPAR).,
  ADDRESS = Lecture Notes on Artificial Intelligence, Num. 6472. Springer ed. ISBN 978-3-642-17318-9. Darmstadt, Germany. November, 2010.,
  PAGES={461-472},
}
[2]
@article {trulls11,
author = {Trulls, E. and Corominas Murtra, A. and P\'{e}rez-Ibarz, J. and Ferrer, G. and Vasquez, D. and Mirats-Tur, Josep M. and Sanfeliu, A.},
title = {Autonomous navigation for mobile service robots in urban pedestrian environments},
journal = {Journal of Field Robotics},
volume = {28},
number = {3},
publisher = {Wiley Subscription Services, Inc., A Wiley Company},
issn = {1556-4967},
url = {http://dx.doi.org/10.1002/rob.20386},
doi = {10.1002/rob.20386},
pages = {329-354},
year = {2011},
}

Tweets (to do)
FaRaMoTics: Fast Ranging Models for roboTics. Our new robot app available at our git repository (BSD liscense)
Want to compute hundreds of thousands of simulated laser scans per second? Try FaRaMoTics! [link]
Want to compute thousands of simulated depth images per second? Try FaRaMoTics! [link]
Need real-time expected range measurements for particle filtering or Optimization? Try FaRaMoTics! [link]


