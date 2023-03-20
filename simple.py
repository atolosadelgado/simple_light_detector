"""
DD4hep simulation with some argument parsing
Based on M. Frank and F. Gaede runSim.py
   @author  A.Sailer
   @version 0.1

Modified with settings for RICH simulation
"""
from __future__ import absolute_import, unicode_literals
import logging
import sys
import os

from DDSim.DD4hepSimulation import DD4hepSimulation


if __name__ == "__main__":
    logging.basicConfig(
        format="%(name)-16s %(levelname)s %(message)s",
        level=logging.INFO,
        stream=sys.stdout,
        )
    logger = logging.getLogger("DDSim")

    SIM = DD4hepSimulation()

    # Ensure that Cerenkov and optical physics are always loaded
    def setupCerenkov(kernel):
        from DDG4 import PhysicsList

        seq = kernel.physicsList()
        cerenkov = PhysicsList(kernel, "Geant4CerenkovPhysics/CerenkovPhys")
        cerenkov.MaxNumPhotonsPerStep = 10
        cerenkov.MaxBetaChangePerStep = 10.0
        cerenkov.TrackSecondariesFirst = False
        cerenkov.VerboseLevel = 0
        cerenkov.enableUI()
        seq.adopt(cerenkov)

        ph = PhysicsList(kernel, "Geant4OpticalPhotonPhysics/OpticalGammaPhys")
        ph.addParticleConstructor("G4OpticalPhoton")
        ph.BoundaryInvokeSD = True
        ph.VerboseLevel = 0
        ph.enableUI()
        seq.adopt(ph)

        return None

    SIM.physics.setupUserPhysics(setupCerenkov)

    # Allow energy depositions to 0 energy in trackers (which include optical detectors)
    SIM.filter.tracker = "edep0"
    SIM.runType = "qt"
    SIM.macroFile ='vis.mac'
    # Some detectors are only sensitive to optical photons
    SIM.filter.filters["opticalphotons"] = dict(
        name="ParticleSelectFilter/OpticalPhotonSelector",
        parameter={"particle": "opticalphoton"},
        )
    # The pattern is based on the name of the detector (SimpleDetector), 
    # not the type (MYDETECTOR).
    SIM.filter.mapDetFilter["SimpleDetector"] = "opticalphotons"

    # Use the optical tracker for the PFRICH
    SIM.action.mapActions["SimpleDetector"] = "Geant4OpticalTrackerAction"

    # Disable user tracker particle handler, so hits can be associated to photons
    SIM.part.userParticleHandler = ""

    # Particle gun settings: pions with fixed energy and theta, varying phi
    SIM.numberOfEvents = 1
    SIM.enableGun = True
    SIM.gun.multiplicity = 1
    SIM.gun.energy = "5*GeV"
    SIM.gun.particle = "pi+"
    SIM.gun.direction = "0 0 1"
    SIM.gun.position = "0 0 0*cm"

    # Installed compact file, otherwise assume the user passed `--compactFile`
    SIM.compactFile = "./compact/simple_detector.xml"

    # Output file (assuming CWD)
    SIM.outputFile = "sim.root"

    # Override with user options
    SIM.parseOptions()

    # Run the simulation
    try:
        SIM.run()
        logger.info("TEST: passed")
    except NameError as e:
        logger.fatal("TEST: failed")
        if "global name" in str(e):
            globalToSet = str(e).split("'")[1]
            logger.fatal("Unknown global variable, please add\nglobal %s\nto your steeringFile" % globalToSet)
