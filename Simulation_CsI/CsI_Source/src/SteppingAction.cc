
#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4AnalysisManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
namespace CsI
{
SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{

  const auto secondaries = step->GetSecondaryInCurrentStep();
  
  for (const auto sec : *secondaries){
    if (sec->GetDefinition() ->GetParticleName() == "opticalphoton"){
      auto creatorProcess=sec->GetCreatorProcess()->GetProcessName();
      if (creatorProcess == "Scintillation"){
        fEventAction->AddScintillationPhoton();
    }
  }
}

  if (!scintScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    scintScoringVolume = detConstruction->GetScintScoringVolume();
    SiScoringVolume = detConstruction->GetSiScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != scintScoringVolume && volume != SiScoringVolume) {return;}

  // collect energy deposited the CsI in this step
  if (volume == scintScoringVolume){
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
}
  //trying to get energies of photons being absorbed by the silicon
  
  if (volume == SiScoringVolume){
  G4Track* track= step->GetTrack();
  const G4ParticleDefinition* particle = track->GetDefinition();

  if (particle->GetParticleName() == "opticalphoton") {
    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    if (processName == "OpAbsorption") {
      G4double photonEnergy=track->GetKineticEnergy();

      G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillNtupleIColumn(1,0, eventID);
      analysisManager->FillNtupleIColumn(1,1, track->GetTrackID());
      analysisManager->FillNtupleDColumn(1,2, photonEnergy);
      analysisManager->AddNtupleRow(1);
      
      fEventAction->AddAbsorpedPhoton();
    }
  }
}

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



}
