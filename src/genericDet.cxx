#include "TTree.h"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"

//local classes
#include "genericDet.h"
#include "DetUtils.h"

//_____________________________________________________________________________
genericDet::genericDet(const G4String& nam, 
		       G4double x, G4double y, G4double z, 
		       G4double rotation, G4double xysize, G4double zsize,
		       G4LogicalVolume *top): 
  Detector(), G4VSensitiveDetector(nam), fNam(nam) {

  G4cout << "  genericDet: " << fNam << G4endl;

  G4Box *shape = new G4Box(fNam, xysize/2., xysize/2., zsize/2.);

  //SILICON_DIOXIDE material
  G4Material *mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //logical volume
  G4LogicalVolume *vol = new G4LogicalVolume(shape, mat, fNam);

  //visibility
  G4VisAttributes *vis = new G4VisAttributes();
  vis->SetColor(1.0, 0., 1.0); // blue
  vis->SetLineWidth(2.0);
  vol->SetVisAttributes(vis);

  //rotation in x-z plane by rotation along y
  G4double rot_y = rotation;
  G4RotationMatrix rot(G4ThreeVector(0, 1, 0), rot_y*rad); //is typedef to CLHEP::HepRotation

  //placement with rotation at a given position in x, y and z
  G4double xpos = x*mm; // center position in x, mm
  G4double ypos = y*mm; // position in y, mm
  //use y position as the edge closer to the beam axis
  G4double ymid = 0;
  if(ypos > 0.1) {
    ymid = xysize/2. + ypos;
  } else if(ypos < -0.1) {
    ymid = -1*xysize/2. + ypos;
  }

  G4double zpos = z*mm; // position of the front face along z
  G4ThreeVector pos(xpos*mm, ymid*mm, zpos-zsize/2);
  G4Transform3D transform(rot, pos); // is HepGeom::Transform3D

  //put to the top volume
  new G4PVPlacement(transform, vol, fNam, top, false, 0);

  //clear all event variables
  ClearEvent();
}//genericDet

//_____________________________________________________________________________
G4bool genericDet::ProcessHits(G4Step *step, G4TouchableHistory*) {

  //remove the track
  G4Track *track = step->GetTrack();

  genHit thisHit;
  thisHit.e= track->GetTotalEnergy();
  thisHit.ke= track->GetKineticEnergy();
  thisHit.trackID = track->GetTrackID();
  thisHit.parentID = track->GetParentID();

  G4ParticleDefinition* particle = track->GetDefinition();
  thisHit.pID = particle->GetPDGEncoding();

  //hit position
  const G4ThreeVector hp = step->GetPostStepPoint()->GetPosition();
  thisHit.x = hp.x();
  thisHit.y = hp.y();
  thisHit.z = hp.z();

  const G4ThreeVector hmom = step->GetPostStepPoint()->GetMomentum();
  thisHit.px = hmom.x();
  thisHit.py = hmom.y();
  thisHit.pz = hmom.z();

  fHits.push_back(thisHit);

  return true;

}//ProcessHits

//_____________________________________________________________________________
void genericDet::CreateOutput(TTree *tree) {
  // G4cout<<"creating bracnh for generic"<<G4endl;
  // std::cin.ignore();//FIXME
  tree->Branch(Form("%s_hits",fNam.data()), &fHits);
}//CreateOutput

//_____________________________________________________________________________
void genericDet::ClearEvent() {
  fHits.clear();
}//ClearEvent

//_____________________________________________________________________________
void genericDet::FinishEvent() {
  //G4cout<<"finishding "<<fNam<<" "<<fHits.size()<<G4endl;
}














