//_____________________________________________________________________________
//
// generator reader for COMRAD
//_____________________________________________________________________________

//C++ headers
#include <string.h>
#include <sstream>
#include <vector>
#include <map>

//Geant headers
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"
#include "G4Event.hh"

//local headers
#include "EventReader.h"
#include "ParticleReader.h"
#include "MCEvent.h"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

using namespace std;

//_____________________________________________________________________________
EventReader::EventReader(DetectorConstruction *dc) : 
  G4VUserPrimaryGeneratorAction(), fIev(0), 
  fUseBeam(0), fBeamE(5), fBeamVX(0), fBeamVY(0), fBeamVZ(0),
  fBeamPx(0),fBeamPy(0),fBeamPz(1){

  //default input name
  fInputName = "events.dat";
  //command for name of input file
  fMsg = new G4GenericMessenger(this, "/EPol/input/");
  fMsg->DeclareProperty("name", fInputName);

  fDetConst = dc;

  //set use beam flag
  fMsg->DeclareProperty("useBeam", fUseBeam);
  fMsg->DeclarePropertyWithUnit("beamEnergy","GeV",fBeamE,"beam energy for particle gun");
  fMsg->DeclarePropertyWithUnit("beamVx", "cm",fBeamVX,"initial vertex position x");
  fMsg->DeclarePropertyWithUnit("beamVy", "cm",fBeamVY,"initial vertex position y");
  fMsg->DeclarePropertyWithUnit("beamVz", "cm",fBeamVZ,"initial vertex position z");
  fMsg->DeclareProperty("beamPx", fBeamPx);
  fMsg->DeclareProperty("beamPy", fBeamPy);
  fMsg->DeclareProperty("beamPz", fBeamPz);

}//EventReader

//_____________________________________________________________________________
void EventReader::GeneratePrimaries(G4Event *evt) {

  //increment event count for progress printout
  fIev++;
  if( fIev%100000 == 0 ) {
    G4cout << "EventReader::GeneratePrimaries, event number: " << fIev << G4endl;
  }
  if(fUseBeam){
    G4PrimaryVertex *vtx = new G4PrimaryVertex(fBeamVX, fBeamVY, fBeamVZ, 0);
    G4ThreeVector beamDir(fBeamPx,fBeamPy,fBeamPz);
    //force normalization
    beamDir /= beamDir.mag();
    const G4double eMass= 0.510998910 * MeV;
    G4double p2 = sqrt(fBeamE*fBeamE - eMass*eMass);
    beamDir *= p2/GeV;

    std::ostringstream trk;
    trk << "TRACK:  11 "<<beamDir.getX()<<"   "<<beamDir.getY()<<"   "<<beamDir.getZ();
    ParticleReader *beamElectron= new ParticleReader(trk.str());
    beamElectron->GenerateToVertex(vtx);
    evt->AddPrimaryVertex(vtx);
  }else{

    //open COMRAD input
    if(!fIn.is_open()) OpenInput();

    //load the next COMRAD event
    string line;
    while( line.find("EVENT") == string::npos ) {

      if( !fIn.good() ) {
	G4cout << "EventReader::GeneratePrimaries: no more events" << G4endl;
	return;
      }

      getline(fIn, line);
      //G4cout << line << G4endl;
    }

    //get vertex coordinates, cm, and number of particles
    //getline(fIn, line);
    G4double vx, vy, vz; // cm
    G4double uXsec(0),pXsec(0);
    int ntrk; // number of particles
    ReadVertex(line, vx, vy, vz, uXsec, pXsec, ntrk); // read from the vertex line

    fDetConst->getMCEvent()->SetPolXsec(pXsec);
    fDetConst->getMCEvent()->SetUnpolXsec(uXsec);


    //G4cout << line << G4endl;
    //G4cout << "EventReader::GeneratePrimaries " << vx << " " << vy << " " << vz << " " << ntrk << G4endl;

    //make the primary vertex
    G4PrimaryVertex *vtx = new G4PrimaryVertex(vx*cm, vy*cm, vz*cm, 0);
    //  G4cout << "EventReader::GeneratePrimaries " << vx << " " << vy << " " << vz << " " << ntrk << G4endl;

    //tracks in event
    std::vector<ParticleReader> tracks;

    //particle loop
    for(int itrk=0; itrk<ntrk; itrk++) {

      getline(fIn, line);
      tracks.push_back( ParticleReader(line) );
    }//particle loop

    //put tracks to map according to pdg
    map<G4int, ParticleReader*> tmap;
    for(std::vector<ParticleReader>::iterator i = tracks.begin(); i != tracks.end(); i++) {

      //put the electron
      if( (*i).GetPdg() == 11 ) tmap.insert( make_pair(11, &(*i)) );

      //photon
      if( (*i).GetPdg() == 22 ) tmap.insert( make_pair(22, &(*i)) );//qiao

      //G4cout << "EventReader::GeneratePrimaries: " << (*i).GetPdg() << G4endl;
    }

    //G4cout << "EventReader::GeneratePrimaries: " << tmap[11]->GetPdg() << " " << tmap[22]->GetPdg() << G4endl;

    //generate the photon
    tmap[22]->GenerateToVertex(vtx);

    //scattered electron
    tmap[11]->GenerateToVertex(vtx);

    //put vertex to the event
    evt->AddPrimaryVertex(vtx);
  }
}//GeneratePrimaries

//_____________________________________________________________________________
void EventReader::ReadVertex(const std::string& line, G4double& vx, G4double& vy, G4double& vz, 
			     G4double &uXsec, G4double &pXsec, int& ntrk) {

  //vertex coordinates and number of tracks
  stringstream ss(line);
  string cc1;
  int nevts;
  ss>>cc1;
  ss>>nevts;
  ss>>ntrk;
  ss>>vx;
  ss>>vy;
  ss>>vz;
  ss>>uXsec;
  ss>>pXsec;
}//ReadVertex

//_____________________________________________________________________________
void EventReader::OpenInput() {

  //open the input file

  G4cout << "EventReader::OpenInput: " << fInputName << G4endl;
  fIn.open(fInputName);

  //test if file exists
  if(fIn.fail()) {
    string description = "Can't open input: " + fInputName;
    G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
  }

}//OpenInput


















