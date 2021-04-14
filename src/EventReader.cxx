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
#include "MCEvent.h"
#include "G4ParticleGun.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"

using namespace std;

//_____________________________________________________________________________
EventReader::EventReader(DetectorConstruction *dc) : 
  G4VUserPrimaryGeneratorAction(), fIev(0), 
  fUseBeam(0), fBeamE(5*GeV), fvertexRotY(0.0103371112*rad)
  ,fvertexPosX(0), fvertexPosY(0), fvertexPosZ(0){

  //default input name
  fInputName = "events.dat";
  //command for name of input file
  fMsg = new G4GenericMessenger(this, "/EPol/input/");
  fMsg->DeclareProperty("name", fInputName);

  fDetConst = dc;

  //set use beam flag
  fMsg->DeclareProperty("useBeam", fUseBeam);
  fMsg->DeclarePropertyWithUnit("beamEnergy","GeV",fBeamE,"beam energy for particle gun");
  fMsg->DeclarePropertyWithUnit("vertexPosX", "cm",fvertexPosX,"initial vertex position x");
  fMsg->DeclarePropertyWithUnit("vertexPosY", "cm",fvertexPosY,"initial vertex position y");
  fMsg->DeclarePropertyWithUnit("vertexPosZ", "cm",fvertexPosZ,"initial vertex position z");
  fMsg->DeclarePropertyWithUnit("vertexRotY", "rad",fvertexRotY,"momentum direction rotation");

}//EventReader

//_____________________________________________________________________________
void EventReader::GeneratePrimaries(G4Event *evt) {

  //increment event count for progress printout
  fIev++;
  if( fIev%100000 == 0 ) {
    G4cout << "EventReader::GeneratePrimaries, event number: " << fIev << G4endl;
  }

  if(fUseBeam){

    G4PrimaryVertex *vtx = new G4PrimaryVertex(fvertexPosX, fvertexPosY, fvertexPosZ, 0);
    G4ThreeVector momDir(0,0,1);
    momDir.rotateY(fvertexRotY);

    G4PrimaryParticle *part=new G4PrimaryParticle(11);
    part->SetMomentumDirection(momDir);
    part->SetTotalEnergy(fBeamE);
    vtx->SetPrimary(part);
    evt->AddPrimaryVertex(vtx);

  }else{

    //open COMRAD input
    if(!fIn.is_open()) OpenInput();
    if( !fIn.good() ) {
      G4cout << "EventReader::GeneratePrimaries: no more events" << G4endl;
      return;
    }
    string line;
    getline(fIn, line);
    stringstream ss(line);

    double partMomX[2],partMomY[2],partMomZ[2],partE[2];
    G4double uXsec(0),pXsec(0),uXsecAlphaCorr(0),pXsecAlphaCorr(0);
    ss>>partMomX[0]>>partMomY[0]>>partMomZ[0]>>partE[0]
      >>uXsec>>pXsec>>uXsecAlphaCorr>>pXsecAlphaCorr
      >>partMomX[1]>>partMomY[1]>>partMomZ[1]>>partE[1];

    fDetConst->getMCEvent()->SetPolXsec(pXsec);
    fDetConst->getMCEvent()->SetUnpolXsec(uXsec);

    G4PrimaryVertex *vtx = new G4PrimaryVertex(fvertexPosX,fvertexPosY,fvertexPosZ, 0);
    const int partID[2]={22,11};
    for(int i=0;i<2;i++){
      G4ThreeVector mom(partMomX[i]*GeV,partMomY[i]*GeV,partMomZ[i]*GeV);
      mom.rotateY(fvertexRotY);

      G4PrimaryParticle *part = new G4PrimaryParticle(partID[i],mom.x(),mom.y(),mom.z(),partE[i]*GeV);
      vtx->SetPrimary(part);
    }

    evt->AddPrimaryVertex(vtx);
  }
}//GeneratePrimaries

/*
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
*/

//_____________________________________________________________________________
void EventReader::OpenInput() {

  //open the input file

  G4cout<< __PRETTY_FUNCTION__<<" "<<__LINE__<<G4endl;
  G4cout << "EventReader::OpenInput: " << fInputName << G4endl;
  fIn.open(fInputName);

  if(fIn.fail()) {
    string description = "Can't open input: " + fInputName;
    G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
  }
  
  string line;
  G4cout<< " Parameters from ComRad file: "<< G4endl;
  for(int i=0;i<3;i++){
    getline(fIn, line);
    G4cout<<line<<G4endl;
  }


}//OpenInput


















