
#ifndef BeamMagnetQuadrupole_cone_h
#define BeamMagnetQuadrupole_cone_h

// beamline dipole magnet V2

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class BeamMagnetQuadrupole_cone : public Detector, public G4VSensitiveDetector {

  public:

    BeamMagnetQuadrupole_cone(G4String nam, G4double xpos,G4double ypos,G4double zpos,G4double length, G4double r1, G4double r2, G4double dout, G4double angle, G4double gradient,G4double shiftX,G4double rx,G4double ry,G4double conerotation, G4LogicalVolume*);
    virtual ~BeamMagnetQuadrupole_cone() {}

    //Detector
    virtual const G4String& GetName() const {return fNam;}

    //G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory*);

  private:

    G4String fNam; // magnet name

};

#endif

