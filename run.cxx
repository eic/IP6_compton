
//C++ headers
#include <iostream>
#include <string>

//explicit instantiation to reduce warnings from old Geant 4.10.00 on EIC nodes
//template std::vector<std::basic_string<char> > boost::program_options::to_internal(const std::vector<std::basic_string<char> >&);

//Geant headers
#include "G4UIExecutive.hh"
#include "Randomize.hh"
#include "G4RunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"

//local headers
#include "DetectorConstruction.h"
#include "ActionInitialization.h"

using namespace std;

//_____________________________________________________________________________
int main(int argc, char* argv[]) {

  //command line arguments

  //visualization macro

  //UI session
  G4String macro;

  G4String vis_mac("init_vis.mac"); // default name


  if( argc == 2 ) {
    macro = argv[1];
  }

   G4UIExecutive* ui = 0;
  if ( argc == 1 ) {
    ui = new G4UIExecutive(argc, argv,"qt");
  }


  //random engine
  G4Random::setTheEngine(new CLHEP::RanecuEngine);

  //default run manager
  G4RunManager *runManager = new G4RunManager;

  //detector construction
  runManager->SetUserInitialization(new DetectorConstruction);

  //physics
  FTFP_BERT *physicsList = new FTFP_BERT;
  //G4OpticalPhysics *opt = new G4OpticalPhysics();
  //physicsList->RegisterPhysics(opt); // uncomment to turn optics on
  runManager->SetUserInitialization(physicsList);

  //action
  runManager->SetUserInitialization(new ActionInitialization);

  //visualization
  G4VisExecutive *visManager = new G4VisExecutive;
  visManager->Initialize();

  //user interface manager
  G4UImanager *UImanager = G4UImanager::GetUIpointer();

  //UI session for interactive or batch mode
  G4String command = "/control/execute ";
  if ( macro.size() > 0 ) {
    // batch mode
    UImanager->ApplyCommand(command+macro);
  } else {
    //interactive
    UImanager->ApplyCommand(command+vis_mac);
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    ui->SessionStart();
    delete ui;
  }

  //job termination
  delete visManager;
  delete runManager;

  return 0;

}//main





























