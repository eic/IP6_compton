
//_____________________________________________________________________________
//
// generator particle for generator readers,
// loads momentum and vertex information from a specific generator format
// and generates itself into the primary vertex
//_____________________________________________________________________________

//C++ 
#include <sstream>

//Geant 
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4PrimaryVertex.hh"
#include "G4ParticleMomentum.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryParticle.hh"

//local classes
#include "ParticleReader.h"

using namespace std;

//_____________________________________________________________________________
ParticleReader::ParticleReader(G4int pdg): fPdg(pdg), fPx(0), fPy(0), fPz(0), fVx(0), fVy(0), fVz(0) {

  // constructor for a given pdg

  fDef = G4ParticleTable::GetParticleTable()->FindParticle(pdg);

  fMass = fDef->GetPDGMass();
  fCharge = fDef->GetPDGCharge();

}//ParticleReader

//_____________________________________________________________________________
ParticleReader::ParticleReader(const std::string &txlin): fVx(0), fVy(0), fVz(0) {

  //get the momentum
  stringstream ss(txlin);
  string cc1;
  int pdg;
  ss>>cc1;
  ss>>fPdg;
  ss>>fPx;
  ss>>fPy;
  ss>>fPz;

  //definition from the pdg
  fDef = G4ParticleTable::GetParticleTable()->FindParticle(fPdg);

  //mass and charge from the definition
  fMass = fDef->GetPDGMass();
  fCharge = fDef->GetPDGCharge();

}//ParticleReader

//_____________________________________________________________________________
void ParticleReader::GenerateToVertex(G4PrimaryVertex *vtx) {

  //generate particle to the vertex

  //particle momentum and energy
  G4ParticleMomentum momentum(fPx*GeV, fPy*GeV, fPz*GeV);
  G4ParticleMomentum momentum_direction = momentum.unit();
  G4double mmag = momentum.mag();
  G4double energy = sqrt( mmag*mmag + fMass+fMass ) - fMass;

  //create the primary to put to the vertex
  G4PrimaryParticle *part = new G4PrimaryParticle(fDef);
  part->SetKineticEnergy(energy);
  part->SetMomentumDirection(momentum_direction);

  //mass and charge from the definition
  part->SetMass(fMass);
  part->SetCharge(fCharge);

  //no polarization
  part->SetPolarization(0, 0, 0);

  //put to the vertex
  vtx->SetPrimary(part);

}//GenerateToVertex

//_____________________________________________________________________________
G4PrimaryVertex* ParticleReader::MakePrimaryVertex() {

  //create primary vertex from particle position

  return new G4PrimaryVertex(fVx*mm, fVy*mm, fVz*mm, 0);

}//MakePrimaryVertex

//_____________________________________________________________________________
