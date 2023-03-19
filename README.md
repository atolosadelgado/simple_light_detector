
To run the example:

```shell
source /cvmfs/sft.cern.ch/lcg/views/dev4/latest/x86_64-centos7-gcc11-opt/setup.sh
cmake -B build -S . -D CMAKE_INSTALL_PREFIX=install
cmake --build build -j 10 -- install
export LD_LIBRARY_PATH=$PWD/install/lib:$LD_LIBRARY_PATH
python3 simple.py
```

Notes:
example DD4hep(from G4), opnovice: https://github.com/AIDASoft/DD4hep/blob/9bbe12b080ce52615251facc6f6ff5dd58a60e69/examples/OpticalSurfaces/src/OpNovice_geo.cpp
https://github.com/AIDASoft/DD4hep/blob/9bbe12b080ce52615251facc6f6ff5dd58a60e69/examples/OpticalSurfaces/scripts/OpNovice.py


G4 optical surface https://geant4-userdoc.web.cern.ch/UsersGuides/ForApplicationDeveloper/html/TrackingAndPhysics/physicsProcess.html?highlight=optics#specifying-the-surface

