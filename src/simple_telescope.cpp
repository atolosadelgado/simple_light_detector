#include "DD4hep/DetFactoryHelper.h"
#include "DD4hep/OpticalSurfaces.h"
#include "DD4hep/Printout.h"
#include "DDRec/DetectorData.h"
#include <XML/Helper.h>

using namespace dd4hep;
using namespace dd4hep::rec;
using dd4hep::SubtractionSolid;

// create the detector
static Ref_t createDetector(Detector &desc, xml::Handle_t handle, SensitiveDetector sens)
{
    xml::DetElement detElem = handle;
    std::string detName = detElem.nameStr();
    int detID = detElem.id();
    DetElement det(detName, detID);
    sens.setType("tracker");

    // We define our own mother volume (vacuum)
    // that will be inside of world (made of air)
    Box gasShape(19 * cm / 2.,
                 19 * cm / 2.,
                 19 * cm / 2.);
    Volume gasVol("gasvol", gasShape, desc.material("C4F10_PFRICH"));
    gasVol.setVisAttributes(desc.visAttributes("cooling_vis"));
//       gasVol.setSensitiveDetector(sens);

    Box sensorShape(9 * cm / 2.,
                    9 * cm / 2.,
                    1 * cm / 2.);
    Volume sensorVol("myworld", sensorShape, desc.material("MyAwsomeSilicon"));
    sensorVol.setVisAttributes(desc.visAttributes("sensor_vis"));
    sensorVol.setSensitiveDetector(sens);


    // Place our mother volume in the world
    Volume wVol = desc.pickMotherVolume(det);
    wVol.setVisAttributes(desc.visAttributes("no_vis"));
    auto sensorPV = gasVol.placeVolume(sensorVol, Position(0,0,8.5*cm));
    sensorPV.addPhysVolID("system", 123);
    // // Make sensor sensitive + define optical properties
    DetElement sensorDE(det, "ARC_sensor", 127);
    sensorDE.setPlacement(sensorPV);
    OpticalSurfaceManager surfMgr = desc.surfaceManager();

    auto sensorSurf = surfMgr.opticalSurface("SensorSurface_PFRICH");

//     SkinSurface sensorSkin(desc, sensorDE, "sensor_optical_surface", sensorSurf, sensorVol); // FIXME: 3rd arg needs `imod`?
//     sensorSkin.isValid();



    PlacedVolume mPV = wVol.placeVolume(gasVol, Position(0, 0, 0*cm));
    BorderSurface sensorBorder(desc, sensorDE, "sensor_optical_surface", sensorSurf, mPV, sensorPV);
    sensorBorder.isValid();
    // Assign the system ID to our mother volume
    mPV.addPhysVolID("system", detID);

    // Set the physical volumes of the detector element.
    det.setPlacement(mPV);
    return det;
}

DECLARE_DETELEMENT(MYTELESCOPE, createDetector)
