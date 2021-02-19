
#ifndef ActionInitialization_h
#define ActionInitialization_h

//standard action initialization

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.h"

class ActionInitialization : public G4VUserActionInitialization {

 public:
 ActionInitialization(DetectorConstruction *dc) : G4VUserActionInitialization()
  {fDetConst = dc;}

  virtual ~ActionInitialization() {}
  
  virtual void Build() const;
 private:
  DetectorConstruction *fDetConst;

};

#endif

