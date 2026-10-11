
#include "EventAction.hh"

#include "RunAction.hh"

namespace FirstProject
// Taken from example B1
{


EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}


void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
}


void EventAction::EndOfEventAction(const G4Event*)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
}


} 
