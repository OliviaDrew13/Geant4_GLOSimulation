
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
	//zeroing the values at the beginning of the event
	//REQUIRED!!
	
  //scintillator
  fEdep = 0.;
  fScintillationPhotons = 0.;
  //glass
  fEdepGlassTot = 0.;
  //ITO
  fEdepITOTot = 0.;
  //silicon
  fSiPhotonEdep = 0.;
  fEdepSiTot = 0.;
  fAbsorpedPhotons = 0.;
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  //scintillator
  fRunAction->AddEdep(fEdep);
  fRunAction->AddScintillationPhoton(fScintillationPhotons);
  //glass
  fRunAction->AddEdepGlassTotal(fEdepGlassTot);
  //ITO
  fRunAction->AddEdepITOTotal(fEdepITOTot);
  //silicon
  fRunAction->AddSiPhotonEdep(fSiPhotonEdep);
  fRunAction->AddEdepSiliconTotal(fEdepSiTot);
  fRunAction->AddAbsorpedPhoton(fAbsorpedPhotons);
  G4int eventID = event->GetEventID();

//appending data to tuples
//overallenergy deposition
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0,0, eventID);
  analysisManager->FillNtupleDColumn(0,1, fEdep);
  analysisManager->FillNtupleDColumn(0,2, fSiPhotonEdep);
  analysisManager->FillNtupleDColumn(0,3, fEdepSiTot);
  analysisManager->FillNtupleDColumn(0,4, fEdepITOTot);
  analysisManager->FillNtupleDColumn(0,5, fEdepGlassTot);
  analysisManager->FillNtupleIColumn(0,6, fScintillationPhotons);
  analysisManager->FillNtupleIColumn(0,7, fAbsorpedPhotons);
  analysisManager->AddNtupleRow(0);
  //What this does: Each event ends with one new row in the CSV containing the total energy deposited in the scoring volume during that event.
}

} 
