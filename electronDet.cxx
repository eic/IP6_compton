
//_____________________________________________________________________________
//
// Simple calorimeter version 2 (V2) for tests,
// absorbs every particle in a single step with no secondaries.
//
// Historical note: such calorimeter was originally invented by Jara Cimrman.
//_____________________________________________________________________________

//C++

//ROOT
#include "TTree.h"

//Geant
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

//local classes
#include "electronDet.h"
#include "DetUtils.h"

//_____________________________________________________________________________
electronDet::electronDet(const G4String& nam, G4double x, G4double y, G4double z, G4double rotation, G4LogicalVolume *top): Detector(),
  G4VSensitiveDetector(nam), fNam(nam) {

  G4cout << "  electronDet: " << fNam << G4endl;

  //detector shape
  G4double xysiz = 2000*mm;
  G4double zsiz = 0.00001*mm;
  G4Box *shape = new G4Box(fNam, xysiz/2., xysiz/2., zsiz/2.);

  //SILICON_DIOXIDE material
  G4Material *mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  //logical volume
  G4LogicalVolume *vol = new G4LogicalVolume(shape, mat, fNam);

  //visibility
  G4VisAttributes *vis = new G4VisAttributes();
  vis->SetColor(0, 0, 1); // blue
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
    ymid = xysiz/2. + ypos;
  } else if(ypos < -0.1) {
    ymid = -1*xysiz/2. + ypos;
  }

  G4double zpos = z*mm; // position of the front face along z
  G4ThreeVector pos(xpos*mm, ymid*mm, zpos-zsiz/2);
  G4Transform3D transform(rot, pos); // is HepGeom::Transform3D

  //put to the top volume
  new G4PVPlacement(transform, vol, fNam, top, false, 0);

  //load flag for primary particles in ProcessHits if defined
  fSelectPrim = true;
  //G4cout << "  electronDet, select_prim: " << fSelectPrim << G4endl;

  //clear all event variables
  ClearEvent();

}//electronDet

//_____________________________________________________________________________
G4bool electronDet::ProcessHits(G4Step *step, G4TouchableHistory*) {

  //remove the track
  G4Track *track = step->GetTrack();

  //G4cout << track->GetParentID() << G4endl;
  //G4cout << "before: " << track->GetTotalEnergy() << G4endl;
  //G4cout << "before: " << track->GetKineticEnergy() << G4endl;

  track->SetTrackStatus(fKillTrackAndSecondaries);
//  track->SetTrackStatus(fAlive);

  //G4cout << "after:  " << track->GetTotalEnergy() << G4endl;
  //G4cout << G4endl;

  fEnAll += track->GetTotalEnergy();

  //primary track only
//  if(track->GetParentID() == 22 )return true;
  //if( fSelectPrim == true && track->GetParentID() == 11 ) return true;

  //consider only first hit by the primary track
  if(fIsHit == kFALSE) {

    fIsHit = kTRUE;
  } else {
    return true;
  }

  //energy
  fEnPrim = track->GetTotalEnergy();

  //hit position
  const G4ThreeVector hp = step->GetPostStepPoint()->GetPosition();
  fHx = hp.x();
  fHy = hp.y();
  fHz = hp.z();

//  G4cout<<" Hits X: "<<fHx<<" Hits Y: "<<fHy<<" Hits Z: "<<fHz<<G4endl;
 // G4cout << "electronDet::ProcessHits: " << track->GetParentID() << G4endl;

  return true;

}//ProcessHits

//_____________________________________________________________________________
void electronDet::CreateOutput(TTree *tree) {

  //output from electronDet

  DetUtils u(fNam, tree);

  u.AddBranch("_IsHit", &fIsHit, "O");

  u.AddBranch("_EnPrim", &fEnPrim, "D");
  u.AddBranch("_en", &fEnAll, "D");

  u.AddBranch("_hx", &fHx, "D");
  u.AddBranch("_hy", &fHy, "D");
  u.AddBranch("_hz", &fHz, "D");

}//CreateOutput

//_____________________________________________________________________________
void electronDet::ClearEvent() {

  fIsHit = kFALSE;

  fEnPrim = -9999.;
  fEnAll = 0;

  fHx = 9999.;
  fHy = 9999.;
  fHz = 9999.;

}//ClearEvent














