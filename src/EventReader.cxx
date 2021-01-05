
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

using namespace std;

//_____________________________________________________________________________
EventReader::EventReader() : G4VUserPrimaryGeneratorAction(), fIev(0) {

  //default input name
  fInputName = "../events.dat";

  //command for name of input file
  fMsg = new G4GenericMessenger(this, "/EPol/input/");
  fMsg->DeclareProperty("name", fInputName);

}//EventReader

//_____________________________________________________________________________
void EventReader::GeneratePrimaries(G4Event *evt) {

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

  //increment event count for progress printout
  fIev++;
  if( fIev%100000 == 0 ) {
    G4cout << "EventReader::GeneratePrimaries, event number: " << fIev << G4endl;
  }

  //get vertex coordinates, cm, and number of particles
  //getline(fIn, line);
  G4double vx, vy, vz; // cm
  int ntrk; // number of particles
  ReadVertex(line, vx, vy, vz, ntrk); // read from the vertex line

  //G4cout << line << G4endl;
  //G4cout << "EventReader::GeneratePrimaries " << vx << " " << vy << " " << vz << " " << ntrk << G4endl;

  //make the primary vertex
  G4PrimaryVertex *vtx = new G4PrimaryVertex(vx*cm, vy*cm, vz*cm, 0);
//  G4cout << "EventReader::GeneratePrimaries " << vx << " " << vy << " " << vz << " " << ntrk << G4endl;

  //tracks in event
  vector<ParticleReader> tracks;

  //particle loop
  for(int itrk=0; itrk<ntrk; itrk++) {

    getline(fIn, line);
    tracks.push_back( ParticleReader(line) );
  }//particle loop

  //put tracks to map according to pdg
  map<G4int, ParticleReader*> tmap;
  for(vector<ParticleReader>::iterator i = tracks.begin(); i != tracks.end(); i++) {

    //put the electron
   if( (*i).GetPdg() == 11 ) tmap.insert( make_pair(11, &(*i)) );

    //photon
    if( (*i).GetPdg() == 22 ) tmap.insert( make_pair(22, &(*i)) );//qiao

    //G4cout << "EventReader::GeneratePrimaries: " << (*i).GetPdg() << G4endl;
  }

  //G4cout << "EventReader::GeneratePrimaries: " << tmap[11]->GetPdg() << " " << tmap[22]->GetPdg() << G4endl;

  //generate the photon
  tmap[22]->GenerateToVertex(vtx); //qiao

  //scattered electron
 tmap[11]->GenerateToVertex(vtx);

  //put vertex to the event
  evt->AddPrimaryVertex(vtx);

}//GeneratePrimaries

//_____________________________________________________________________________
void EventReader::ReadVertex(const std::string& line, G4double& vx, G4double& vy, G4double& vz, int& ntrk) {

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


















