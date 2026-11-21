
#include "EventAction.hh"
#include "G4AnalysisManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4RunManager.hh"

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"

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
  fEdepComp=0.;
  fEdepPhotElec=0;
  fScintPhotonEdep = 0.;
  fAbsorpedPhotonsScint = 0.;
  //glass
  fEdepGlassTot = 0.;
  fGlassPhotonEdep = 0.;
  fAbsorpedPhotonsGlass = 0.;
  //ITO
  fEdepITOTot = 0.;
  fITOPhotonEdep = 0.;
  fAbsorpedPhotonsITO = 0.;
  //silicon
  fSiPhotonEdep = 0.;
  fEdepSiTot = 0.;
  fAbsorpedPhotons = 0.;
  //Al Back
  fEdepAlBackTot = 0.;
}


void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in run action
  //intial particle energy
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>( 
  G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
  G4double particleEnergy = particleGun->GetParticleEnergy();
  //scintillator
  fRunAction->AddEdep(fEdep);
  fRunAction->AddScintillationPhoton(fScintillationPhotons);
  fRunAction->AddEdepComp(fEdepComp);
  fRunAction->AddEdepPhotElec(fEdepPhotElec);
  fRunAction->AddScintPhotonEdep(fScintPhotonEdep);
  fRunAction->AddAbsorpedPhotonScint(fAbsorpedPhotonsScint);
  //glass
  fRunAction->AddEdepGlassTotal(fEdepGlassTot);
  fRunAction->AddGlassPhotonEdep(fGlassPhotonEdep);
  fRunAction->AddAbsorpedPhotonGlass(fAbsorpedPhotonsGlass);
  //ITO
  fRunAction->AddEdepITOTotal(fEdepITOTot);
  fRunAction->AddITOPhotonEdep(fITOPhotonEdep);
  fRunAction->AddAbsorpedPhotonITO(fAbsorpedPhotonsITO);
  //silicon
  fRunAction->AddSiPhotonEdep(fSiPhotonEdep);
  fRunAction->AddEdepSiliconTotal(fEdepSiTot);
  fRunAction->AddAbsorpedPhoton(fAbsorpedPhotons);
  //AlBack
  fRunAction->AddEdepAlBackTotal(fEdepAlBackTot);
  
  G4int eventID = event->GetEventID();

//appending data to tuples
//overallenergy deposition
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0,0, eventID);
  analysisManager->FillNtupleDColumn(0,1, particleEnergy);
  analysisManager->FillNtupleDColumn(0,2, fEdep);
  analysisManager->FillNtupleDColumn(0,3, fEdepComp);
  analysisManager->FillNtupleDColumn(0,4, fEdepPhotElec);
  analysisManager->FillNtupleDColumn(0,5, fScintPhotonEdep);
  analysisManager->FillNtupleDColumn(0,6, fEdepGlassTot);
  analysisManager->FillNtupleDColumn(0,7, fGlassPhotonEdep);
  analysisManager->FillNtupleDColumn(0,8, fEdepITOTot);
  analysisManager->FillNtupleDColumn(0,9, fITOPhotonEdep);
  analysisManager->FillNtupleDColumn(0,10, fEdepSiTot);
  analysisManager->FillNtupleDColumn(0,11, fSiPhotonEdep);
  analysisManager->FillNtupleDColumn(0,12, fEdepAlBackTot);
  analysisManager->FillNtupleIColumn(0,13, fScintillationPhotons);
  analysisManager->FillNtupleIColumn(0,14, fAbsorpedPhotonsScint);
  analysisManager->FillNtupleIColumn(0,15, fAbsorpedPhotonsGlass);
  analysisManager->FillNtupleIColumn(0,16, fAbsorpedPhotonsITO);
  analysisManager->FillNtupleIColumn(0,17, fAbsorpedPhotons);
  analysisManager->AddNtupleRow(0);
  //What this does: Each event ends with one new row in the CSV containing the total energy deposited in the scoring volume during that event.
}

} 
