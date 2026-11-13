
#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"

#include "RunAction.hh"

namespace CsI
// Taken from example B1
{


EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}


void EventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
  fSiPhotonEdep = 0.;
  fScintillationPhotons = 0.;
  fAbsorpedPhotons = 0.;
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  fRunAction->AddEdep(fEdep);
  fRunAction->AddSiPhotonEdep(fSiPhotonEdep);
  fRunAction->AddScintillationPhoton(fScintillationPhotons);
  fRunAction->AddAbsorpedPhoton(fAbsorpedPhotons);

  G4int eventID = event->GetEventID();

//appending data to tuples
//overallenergy deposition
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0,0, eventID);
  analysisManager->FillNtupleDColumn(0,1, fEdep);
  analysisManager->FillNtupleDColumn(0,2, fSiPhotonEdep);
  analysisManager->FillNtupleIColumn(0,3, fScintillationPhotons);
  analysisManager->FillNtupleIColumn(0,4, fAbsorpedPhotons);
  analysisManager->AddNtupleRow(0);

  //recording number of scintillation photons
  //analysisManager->FillNtupleIColumn(2,0, eventID);
  //analysisManager->FillNtupleIColumn(2,1, fScintillationPhotons);
  //analysisManager->AddNtupleRow(2);

  //What this does: Each event ends with one new row in the CSV containing the total energy deposited in the scoring volume during that event.
}

} 
