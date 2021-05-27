
//_____________________________________________________________________________
//
// main detector construction,
// detector definition is here
//_____________________________________________________________________________

//C++
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <fstream>

//Geant
#include "G4GenericMessenger.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

//local classes
#include "DetectorConstruction.h"
#include "RootOut.h"
#include "MCEvent.h"
#include "BeamMagnetDipole.h"
#include "BeamMagnetQuadrupole.h"
#include "BeamMagnetQuadrupole_cone.h"
#include "electronDet.h"
#include "photonDet.h"
#include "calBox.h"
#include "genericDet.h"

using namespace std;

int worldVol_x = 20*meter;
int worldVol_y = 20*meter;
int worldVol_z = 200*meter;

//_____________________________________________________________________________
DetectorConstruction::DetectorConstruction() : G4VUserDetectorConstruction(), fDet(0), fOut(0), fMsg(0), 
					       fIncMagnet(0), fIncB2eR(0),fIncQF1(0) {

  G4cout << "DetectorConstruction::DetectorConstruction" << G4endl;

  //output file and tree
  fOut = new RootOut();

  //all detectors and their parts
  fDet = new std::vector<Detector*>;

  //MC event, also inherits from Detector
  fMC = new MCEvent();
  AddDetector(fMC);

  magFile = "polarimeter_magnet.dat.5GeV";
  
  //messenger for detectors and components
  fMsg = new G4GenericMessenger(this, "/EPol/construct/");
  fMsg->DeclareProperty("B2eR", fIncB2eR);
  fMsg->DeclareProperty("QF1", fIncQF1);
  fMsg->DeclareProperty("magFile",magFile);
  fMsg->DeclarePropertyWithUnit("worldVol_x", "length", worldVol_x);
  fMsg->DeclarePropertyWithUnit("worldVol_y", "length", worldVol_y);
  fMsg->DeclarePropertyWithUnit("worldVol_z", "length", worldVol_z);
  
}//DetectorConstruction

//_____________________________________________________________________________
DetectorConstruction::~DetectorConstruction() {

  //write the tree and close output file
  fOut->Close();

  delete fDet;

}//~DetectorConstruction

//_____________________________________________________________________________
G4VPhysicalVolume* DetectorConstruction::Construct() {

  G4cout << G4endl << "DetectorConstruction::Construct" << G4endl;

  //vacuum top material
  G4Material* top_m = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //top world volume
  G4Box *top_s = new G4Box("top_s", worldVol_x, worldVol_y, worldVol_z);
  //G4Box *top_s = new G4Box("top_s", 3*m, 3*m, 3*m);
  G4LogicalVolume *top_l = new G4LogicalVolume(top_s, top_m, "top_l");
  top_l->SetVisAttributes( G4VisAttributes::GetInvisible() );
  G4VPhysicalVolume *top_p = new G4PVPlacement(0, G4ThreeVector(), top_l, "top_p", 0, false, 0);

  //detectors

  //beamline magnet
  //if(fIncB2eR) new BeamMagnet(-12.254*meter, top_l);
  ifstream magfile;
  magfile.open(magFile.data());

  string line;


  if(magfile.fail()) {
    G4cout << "eee ****************"<<endl;
    string description = "Can't open mag file";
    G4Exception("EventReader::OpenInput", "InputNotOpen01", FatalException, description.c_str());
  }

  string cc1,cc2;

  G4double xpos,ypos,zpos,length,r1,r2,dout,angle,bfield,gradient,rot,xysize,zsize,col1=-7,col2,col3,shiftx,rx,ry,conerot;

  int number = 0;

  while(!magfile.eof()){
    getline(magfile,line);
    //if(number>12)continue;
    if(line=="")continue;
    number++;

    stringstream ss(line);
    ss>>cc1;
    G4cout << cc1 << endl;
    if(cc1=="CAL"){    
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>rot;
        ss>>xysize;
        ss>>zsize;
        AddDetector(new calBox(cc2,xpos*meter,ypos*meter,zpos*meter,rot,xysize*mm,zsize*mm,top_l)); //add Calorimeter
    }
    if(cc1=="DET"){
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>rot;
        ss>>xysize;
        ss>>zsize;
        AddDetector(new genericDet(cc2,xpos*meter,ypos*meter,zpos*meter,rot,xysize*mm,zsize*mm,top_l)); //add Generic Detector
    }
    if(cc1=="DETe"){
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>rot;
        AddDetector(new electronDet(cc2,xpos*meter,ypos*meter,zpos*meter,rot,top_l)); //add Electron Detector
    }
    if(cc1=="DB"){
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>r1;
        ss>>r2;
        ss>>dout;
        ss>>length;
        ss>>angle;
        ss>>bfield;
        ss>>gradient;
        AddDetector(new BeamMagnetDipole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,bfield*tesla, top_l));//add Dipole
    }
    if(cc1=="QF"){
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>r1;
        ss>>r2;
        ss>>dout;
        ss>>length;
        ss>>angle;
        ss>>bfield;
        ss>>gradient;
        ss>>col1;
        if(col1 == 7){
            AddDetector(new BeamMagnetQuadrupole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,1,0,0,top_l));
        }
        else{
            ss>>col2;
            ss>>col3;
            AddDetector(new BeamMagnetQuadrupole(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,col1,col2,col3,top_l));
         //add Quadrupole
        }
    }
    if(cc1=="QFc"){
        ss>>cc2;
        ss>>xpos;
        ss>>ypos;
        ss>>zpos;
        ss>>r1;
        ss>>r2;
        ss>>dout;
        ss>>length;
        ss>>angle;
        ss>>bfield;
        ss>>gradient;
        ss>>shiftx;
        ss>>rx;
        ss>>ry;
        ss>>conerot;
        AddDetector(new BeamMagnetQuadrupole_cone(cc2,xpos*meter,ypos*meter,zpos*meter,length*meter,r1*meter,r2*meter,dout*meter,angle,gradient*tesla/meter,shiftx*mm,rx*mm,ry*mm,conerot,top_l));//add Quadrupole cone  //0.014029944 shift 0.0042092594
    }
  }

  //photon det
//    AddDetector(new calBox("Gdet_ecal",515.59067,0,-60870.712,-0.0103371112 ,250*mm,400*mm,top_l)); //add Ecal detector 32m from laser IP primary
  // to study the recoil electron
//   		AddDetector(new electronDet("Edet_QD9",178.78630,0,-83845.821,0.0036928331,top_l));  //recoil electron detector
// 		
//   AddDetector(new genericDet("gDet",515.59067,0,-60870-410.712,-0.0103371112 ,500,1, top_l));  //recoil electron detector
//   AddDetector(new genericDet("bQ9EF",178.78630,0,-83845        ,-0.0103371112 ,500,1, top_l));  //recoil electron detector
//   AddDetector(new genericDet("aQ9EF",178.78630,0,-82845        ,-0.0103371112 ,500,1, top_l));  //recoil electron detector
// 
// 
//   AddDetector(new genericDet("aD22EF",199,0,-90607+1600   ,-0.0103371112 ,500,1, top_l));  //recoil electron detector
//   AddDetector(new genericDet("bD22EF",199,0,-90607-1600   ,-0.0103371112 ,500,1, top_l));  //recoil electron detector
//   AddDetector(new genericDet("aQ10EF",199,0,-88345+360    ,-0.0103371112 ,500,1, top_l));  //recoil electron detector
// 



  // if(fIncB2eR) AddDetector(new BeamMagnetDipole("B2eR",0.238873*meter,0.0*meter, -6.6488*meter,4.22367*meter,0.1*meter,0.1*meter,0.5*meter,-12.699,-0.272916*tesla, top_l));
  //  if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF1", -9.5956*meter, top_l));
  // if(fIncQF1) AddDetector(new BeamMagnetQuadrupole("QF2", -19.5956*meter, top_l));


  return top_p;

}//Construct

//_____________________________________________________________________________
void DetectorConstruction::BeginEvent(const G4Event *evt) const {

  //detector loop for  ClearEvent  in each detector
  std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::ClearEvent ));

  //set MC
  fMC->BeginEvent(evt);

}//BeginEvent

//_____________________________________________________________________________
void DetectorConstruction::FinishEvent() const {
  //detector loop
  std::for_each(fDet->begin(), fDet->end(), std::mem_fun( &Detector::FinishEvent ));
  
  //fill the output tree
  fOut->FillTree();

}//WriteEvent

//_____________________________________________________________________________
void DetectorConstruction::AddDetector(Detector *det) {

  //add detector to all detectors
  det->Add(fDet);

}//AddDetector

//_____________________________________________________________________________
void DetectorConstruction::CreateOutput() const {

  //open output file
  fOut->Open();

  //detector loop to call CreateOutput
  std::vector<Detector*>::iterator i = fDet->begin();
  while(i != fDet->end()) {
    (*i++)->CreateOutput( fOut->GetTree() );
  }//detector loop

}//CreateOutput

//_____________________________________________________________________________
void DetectorConstruction::ConstructSDandField() {

  G4cout << "DetectorConstruction::ConstructSDandField" << G4endl;

  //detector loop
  std::vector<Detector*>::iterator i;
  for(i = fDet->begin(); i != fDet->end(); ++i) {
    Detector *det = *i;

    G4VSensitiveDetector *sd = dynamic_cast<G4VSensitiveDetector*>(det);
    if(!sd) continue;

    //detector inherits also from G4VSensitiveDetector, add it to Geant

    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    SetSensitiveDetector(det->GetName(), sd);

    G4cout << "  " << det->GetName() << G4endl;
  }//detector loop

}//ConstructSDandField



















