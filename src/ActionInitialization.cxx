
//_____________________________________________________________________________
//
// standard action initialization,
// selects the event generator
//_____________________________________________________________________________

//local headers
#include "ActionInitialization.h"
#include "GeneratorAction.h"
#include "EventAction.h"
#include "EventReader.h"
#include "RunAction.h"

//_____________________________________________________________________________
void ActionInitialization::Build() const {

  //select the generator
  //SetUserAction(new GeneratorAction);
  SetUserAction(new EventReader);
  SetUserAction(new EventAction);
  SetUserAction(new RunAction);

}//build

