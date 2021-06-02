
#ifndef DetectorConstruction_h
#define DetectorConstruction_h

// main detector construction

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4Step;
class G4Event;
class G4GenericMessenger;

#include "Detector.h"

class RootOut;
class vector;
class MCEvent;

class DetectorConstruction : public G4VUserDetectorConstruction {

  public:

    DetectorConstruction();
    virtual ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

    void BeginEvent(const G4Event *evt) const;
    void FinishEvent() const;

    void CreateOutput() const;
    void ConstructSDandField();

    MCEvent *getMCEvent(){return fMC;}

  private:

    void AddDetector(Detector *det); // add detector to all detectors

    std::vector<Detector*> *fDet; //all detectors

    RootOut *fOut; // output to ROOT TTree

    MCEvent *fMC; // generated particles

  G4String magFile;
    G4GenericMessenger *fMsg; // messenger for detectors and components
    G4bool fIncMagnet; // flag for spectrometer magnet
    G4bool fIncB2eR; // beamline dipole magnet
    G4bool fIncQF1;//beamline Quadrupole 
    G4int worldVol_x = 20000; G4int worldVol_y = 20000; G4int worldVol_z = 200000; //x, y, z world volume elements, default EIC volume

};

#endif













