

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4AccumulableManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

namespace CsI
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
{
  // add new units for dose
  //
  const G4double milligray = 1.e-3 * gray;
  const G4double microgray = 1.e-6 * gray;
  const G4double nanogray = 1.e-9 * gray;
  const G4double picogray = 1.e-12 * gray;

  new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
  new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
  new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

  // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Register(fEdep);
  accumulableManager->Register(fEdep2);
  accumulableManager->Register(fSiPhotonEdep);
  accumulableManager->Register(fSiPhotonEdep2);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{
	
  // analysis manager to create a CSV file with the required data
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");
  std::string runnumber = std::to_string(run->GetRunID());
  analysisManager->OpenFile("/home/samuel/Documents/Dissertation/Geant4_GLOSimulation/Simulation_CsI/CsI_Data_" + runnumber);

  // gamma radiation deposition into CsI
  analysisManager->CreateNtuple("EventData", "CsI(Tl)");   //tuple 0
  analysisManager->CreateNtupleIColumn("Event"); //column0
  analysisManager->CreateNtupleDColumn("Energy Deposited in CsI by single Gamma (MeV)");  // column 1
  analysisManager->CreateNtupleDColumn("Energy Deposited in Si by All Photons (MeV)");  // column 2
  analysisManager->CreateNtupleIColumn("Number of Produced Scintillation Photons"); //column 3
  analysisManager->CreateNtupleIColumn("Number of Photons absorped by Si"); //column 4
  analysisManager->FinishNtuple();

  // photon absorbtion data
  analysisManager->CreateNtuple("PhotonTracks", "Si");   //tuple 1
  analysisManager->CreateNtupleIColumn("Event");  // column 0
  analysisManager->CreateNtupleIColumn("Photon Track num");  // column 1
  analysisManager->CreateNtupleDColumn("Energy Deposited in Si by single photon (MeV)");  // column 2
  analysisManager->FinishNtuple();

  // number of scintillation photons 
  //analysisManager->CreateNtuple("N Scintillation Photons", "Produced by CsI");   //tuple 2
  //analysisManager->CreateNtupleIColumn("Event");  // column 0
  //analysisManager->CreateNtupleIColumn("Scintillation Photons");  // column 1
  //analysisManager->FinishNtuple();
  
  
  // inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // reset accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Compute dose = total energy deposit in a run and its variance
  //
  G4double edep = fEdep.GetValue();
  G4double edep2 = fEdep2.GetValue();

  G4double rms = edep2 - edep * edep / nofEvents;
  if (rms > 0.)
    rms = std::sqrt(rms);
  else
    rms = 0.;
//Getting the energy deposition stats for the scintillator
  const auto detConstruction = static_cast<const DetectorConstruction*>(
    G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  G4double mass = detConstruction->GetScintScoringVolume()->GetMass();
  G4double dose = edep / mass;
  G4double rmsDose = rms / mass;

  // Run conditions
  //  note: There is no primary generator action object for "master"
  //        run manager for multi-threaded mode.
  const auto generatorAction = static_cast<const PrimaryGeneratorAction*>(
    G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction) {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy, "Energy");
  }

  // Print
  //
  if (IsMaster()) {
    G4cout << G4endl << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout << G4endl << "--------------------End of Local Run------------------------";
  }

  G4cout << G4endl << " The run is " << nofEvents << " " << runCondition << G4endl << G4endl;
  G4cout << "  --> cumulated edep per run in scoring volume = " << G4BestUnit(edep, "Energy") 
         << " = " << edep/joule << " joule" << G4endl;  
  G4cout << "  --> mass of scoring volume = " << G4BestUnit(mass, "Mass") << G4endl << G4endl; 
  G4cout << " Absorbed dose per run in scoring volume = edep/mass = " << G4BestUnit(dose, "Dose")
         << "; rms = " << G4BestUnit(rmsDose, "Dose") << G4endl
         << "------------------------------------------------------------" << G4endl << G4endl;
  
  //writing to and closing the file
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::AddEdep(G4double edep)
{
  fEdep += edep;
  fEdep2 += edep * edep;
}

void RunAction::AddSiPhotonEdep(G4double SiPhotonEdep)
{
  fSiPhotonEdep += SiPhotonEdep;
  fSiPhotonEdep2 += SiPhotonEdep * SiPhotonEdep;
}


void RunAction::AddScintillationPhoton(G4double photons)
{
  fScintillationPhotons += photons;
}

void RunAction::AddAbsorpedPhoton(G4double abphotons)
{
  fAbsorpedPhotons += abphotons;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
