
#ifndef EventReader_h
#define EventReader_h

// generator reader 

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include <fstream>

class G4Event;
class G4GenericMessenger;

class EventReader : public G4VUserPrimaryGeneratorAction {

  public:

    EventReader();
    virtual ~EventReader() {}

    virtual void GeneratePrimaries(G4Event *evt);

  private:

    void OpenInput(); // function to open COMRAD output

    //vertex coordinates and number of tracks
    void ReadVertex(const std::string& line, G4double& vx, G4double& vy, G4double& vz, int& ntrk);

    std::ifstream fIn; //  input file
    G4String fInputName; // name of input file

    G4GenericMessenger *fMsg; // messenger for name of input file

    int fIev; // event number for progress printout

};

#endif

