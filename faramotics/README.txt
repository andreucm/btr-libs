
FaRaMoTics: Fast Ranging Models for roboTics

AUTHORS
Andreu Corominas Murtra ( andreu(at)beta-robots.com ), from Beta Robots ( www.beta-robots.com )

HISTORY
The first version was developed at IRI (www.iri.upc.edu) by the same author, as a research library for real time map-based localization. 
This relesase version has been completely updated specially in two senses: 
- Use of pose_stat_time library for pose3d operations.
- Use of an external library for 3d model parsing.
Moreover some minor bugs has been fixed. 

LICENSE
The current license is BSD 

PURPOSE
This library computes simulated (also called expected in filtering applucaions) range data in 3D models. 
It is specially designed for computation of simulated laser scanner or depth camera data, and it has 
ready to use functions to get simulated range data for the most popular devices such as:
- Hokuyo UTM30LX
- Hokuyo URGX
- Leuze RS4
- Kinect
- SwissRange SR4000

In order to compute simulated ranging data, the library needs to load 3D models. This is done by means of the
library XXX (www....) , which has also a BSD license. 

APPROACH
The implementation renders 3D models viewed from the device position in optimally-sized windows, which allows fast 
rendering and depth retrieval. Optimal size of the windows is based on the sensor parameters such as angular accuracy, 
angular aperture and min/max ranges. See [1] for further details.

REFERENCES
[1]
@INPROCEEDINGS{corominas_simpar10,
  AUTHOR = {{Corominas Murtra}, A. and Trulls, E. and {Mirats Tur}, J.M. and Sanfeliu, A.},
  TITLE = {{Efficient Use of 3D Environment Models for Mobile Robot Simulation and Localization}},
  BOOKTITLE = Proceedings of the Simulation, Modelling and Programming for Autonomous Robots (SIMPAR).,
  ADDRESS = Lecture Notes on Artificial Intelligence, Num. 6472. Springer ed. ISBN 978-3-642-17318-9. Darmstadt, Germany. November, 2010.,
  PAGES={461-472},
}


Tweets (to do)
FaRaMoTics, Our new robot app! Fast Ranging Models for roboTics, available at our git repository (BSD liscense)
Want to compute xxxx simulated laser scans in a second? Try FaRaMoTics, available at our git repository (BSD liscense)
Want to compute xxxx simulated depth images in a second? Try FaRaMoTics, available at our git repository (BSD liscense)
Need real-time expected range data for particle filtering? Try FaRaMoTics, available at our git repository (BSD liscense)


