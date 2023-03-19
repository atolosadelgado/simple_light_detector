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

    // Define the radiator gas
    Box gasShape(19 * cm / 2.,
                 19 * cm / 2.,
                 19 * cm / 2.);
    Volume gasVol("gasvol", gasShape, desc.material("C4F10_PFRICH"));
    gasVol.setVisAttributes(desc.visAttributes("gas_vis"));

    // define the sensor, to be placed inside the gas
    Box sensorShape(9 * cm / 2.,
                    9 * cm / 2.,
                    1 * cm / 2.);
    Volume sensorVol("myworld", sensorShape, desc.material("MyAwsomeSilicon"));
    sensorVol.setVisAttributes(desc.visAttributes("sensor_vis"));
    sensorVol.setSensitiveDetector(sens);



    // place the sensor inside the gas
    auto sensorPV = gasVol.placeVolume(sensorVol, Position(0,0,8.5*cm));
    sensorPV.addPhysVolID("system", 123);
    DetElement sensorDE(det, "ARC_sensor", 127);
    sensorDE.setPlacement(sensorPV);

    // get the world volume
    Volume wVol = desc.pickMotherVolume(det);
    wVol.setVisAttributes(desc.visAttributes("no_vis"));

    // place the gas inside the world
    PlacedVolume gasPV = wVol.placeVolume(gasVol);
    gasPV.addPhysVolID("system", detID);
    det.setPlacement(gasPV);

    OpticalSurfaceManager surfMgr = desc.surfaceManager();
    auto sensorSurf = surfMgr.opticalSurface("SensorSurface_PFRICH");

    // Define a skin...
    SkinSurface sensorSkin(desc, sensorDE, "sensor_optical_surface", sensorSurf, sensorVol); // FIXME: 3rd arg needs `imod`?
    sensorSkin.isValid();

    // ... or a border, same result
    // BorderSurface sensorBorder(desc, sensorDE, "sensor_optical_surface", sensorSurf, gasPV, sensorPV);
    // sensorBorder.isValid();

    return det;
}

DECLARE_DETELEMENT(MYDETECTOR, createDetector)
