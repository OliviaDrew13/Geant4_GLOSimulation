
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

  if (!fScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);

  //trying to get energies of photons being absorbed by the silicon
  
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
    }
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



}
