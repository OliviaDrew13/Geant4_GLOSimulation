
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
  
  //if an optical photon is generated from scintillation, the number of scintillation photons produced is incremented by one
  const auto secondaries = step->GetSecondaryInCurrentStep();
  
  for (const auto sec : *secondaries){
    if (sec->GetDefinition() ->GetParticleName() == "opticalphoton"){
      auto creatorProcess=sec->GetCreatorProcess()->GetProcessName();
      if (creatorProcess == "Scintillation"){
        fEventAction->AddScintillationPhoton();
    }
  }
}

//if this is the first step and we don't have our scoring volumes defined, we define them here
//as we define them at the same time, we can just check for one
//also a good check to make sure we have a scintillator
  if (!scintScoringVolume) {
    const auto detConstruction = static_cast<const DetectorConstruction*>(
      G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    scintScoringVolume = detConstruction->GetScintScoringVolume();
    SiScoringVolume = detConstruction->GetSiScoringVolume();
    GlassScoringVolume = detConstruction->GetGlassScoringVolume();
    ITOScoringVolume = detConstruction->GetITOScoringVolume();
  }

  // get volume of the current step
  G4LogicalVolume* volume =
    step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // check if we are in any scoring volume
  if (volume != scintScoringVolume && volume != SiScoringVolume && volume != GlassScoringVolume && volume != ITOScoringVolume) {return;}

  // collect total energy deposited the CsI in this step, from gamma or otherwise
  if (volume == scintScoringVolume){
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);
}
//glass
  if (volume==GlassScoringVolume){
    G4double edepGlassTotStep=step->GetTotalEnergyDeposit();
    fEventAction-> AddEdepGlassTotal(edepGlassTotStep);
}

//ito
  if (volume==ITOScoringVolume){
    G4double edepITOTotStep=step->GetTotalEnergyDeposit();
    fEventAction-> AddEdepITOTotal(edepITOTotStep);
}



  //working in only Si scoring volume
  if (volume == SiScoringVolume){
//TOTAL ENERGY//
//calculating total energy deposited in the silicon from all sources
  G4double edepSiTotStep=step->GetTotalEnergyDeposit();
  fEventAction->AddEdepSiliconTotal(edepSiTotStep);      // adding the total energy deposition of that step

//OPTICAL PHOTONS//

  //defining the trackID
  G4Track* track= step->GetTrack();
  const G4ParticleDefinition* particle = track->GetDefinition();

//checking for interactions with ONLY optical photons within the Si
  if (particle->GetParticleName() == "opticalphoton") {
    G4String processName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    //checking for absorbed optical photons
    if (processName == "OpAbsorption") {
		//getting their KE
      G4double photonEnergy=track->GetKineticEnergy();

      G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
		//filling in the second csv with every photon and their respective energy
		//this allows us to look at a histogram of the energy emission spectra
      auto analysisManager = G4AnalysisManager::Instance();
      analysisManager->FillNtupleIColumn(1,0, eventID);
      analysisManager->FillNtupleIColumn(1,1, track->GetTrackID());
      analysisManager->FillNtupleDColumn(1,2, photonEnergy);
      analysisManager->AddNtupleRow(1);
      //incrementing the absorbed photon counter
      fEventAction->AddAbsorpedPhoton();
      //adding the photon energy to the total photon energy deposition
      fEventAction->AddSiPhotonEdep(photonEnergy);
    }
  }
}

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



}
