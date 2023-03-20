
# Run
To run the example:

```shell
source /cvmfs/sft.cern.ch/lcg/views/dev4/latest/x86_64-centos7-gcc11-opt/setup.sh
cmake -B build -S . -D CMAKE_INSTALL_PREFIX=install
cmake --build build -j 10 -- install
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
python3 simple.py
```
# To check the ouptut

open the root file like `root sim.root ` and scan the position of the hits, if any
```cpp
EVENT->Scan("MYDETECTOR_HITS.position.Y():MYDETECTOR_HITS.position.X()")
```

If simulation was ok, a table of aroudn 20 photon hits should be displayed. Otherwise, empty table or 1 hit (the charged particle) will be shown.

# Error msg from DD4hep

The output is saved into `dd4hep_error_msg.txt` using this command
```
python3 simple.py --runType batch > dd4hep_error_msg.txt 2>&1
grep -n 'Error' dd4hep_error_msg.txt
```

The folling error appears when the process `OpBoundary` is called

```
SimpleDetectorHandler ERROR +++++ WRONG!!! Extract. How did we get here?
```

# Notes:

1. example DD4hep(from G4), opnovice: 
https://github.com/AIDASoft/DD4hep/blob/9bbe12b080ce52615251facc6f6ff5dd58a60e69/examples/OpticalSurfaces/src/OpNovice_geo.cpp
https://github.com/AIDASoft/DD4hep/blob/9bbe12b080ce52615251facc6f6ff5dd58a60e69/examples/OpticalSurfaces/scripts/OpNovice.py


2. G4 optical surface https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/physicsProcess.html?highlight=optics#specifying-the-surface

