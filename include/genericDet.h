#ifndef genericDet_h
#define genericDet_h

#include "Detector.h"
#include "G4VSensitiveDetector.hh"

class G4VPhysicalVolume;
class G4Step;
struct genHit{
  G4int pID;
  G4int trackID;
  G4int parentID;
  G4double x,y,z;
  G4double px,py,pz;
  G4double e,ke;
};

class genericDet : public Detector, public G4VSensitiveDetector {

  public:

    genericDet(const G4String&, 
	       G4double, G4double, G4double,
	       G4double, G4double, G4double, G4LogicalVolume*);
    virtual ~genericDet() {}

    //called via G4VSensitiveDetector
    virtual G4bool ProcessHits(G4Step *step, G4TouchableHistory*);

    //called via Detector
    virtual const G4String& GetName() const {return fNam;}
    virtual void CreateOutput(TTree *tree);
    virtual void ClearEvent();
    void FinishEvent();

  private:
    G4String fNam; // detector name
    std::vector<genHit> fHits;

};

#endif



















