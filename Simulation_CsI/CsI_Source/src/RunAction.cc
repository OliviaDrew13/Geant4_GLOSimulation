

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
  const G4double milligray = 1.e-3 * gray;
  const G4double microgray = 1.e-6 * gray;
  const G4double nanogray = 1.e-9 * gray;
  const G4double picogray = 1.e-12 * gray;

  new G4UnitDefinition("milligray", "milliGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "microGy", "Dose", microgray);
  new G4UnitDefinition("nanogray", "nanoGy", "Dose", nanogray);
  new G4UnitDefinition("picogray", "picoGy", "Dose", picogray);

  // Register accumulables to the accumulable manager, this allows us to manage and reset them when required
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  //scintillator
  accumulableManager->Register(fEdep);
  accumulableManager->Register(fEdep2);
  accumulableManager->Register(fEdepComp);
  accumulableManager->Register(fEdepPhotElec);
  //glass
  accumulableManager->Register(fEdepGlassTot);
  //ITO
  accumulableManager->Register(fEdepITOTot);
  //silicon
  accumulableManager->Register(fSiPhotonEdep);
  accumulableManager->Register(fEdepSiTot);
  //Al Back
  accumulableManager->Register(fEdepAlBackTot);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{
	
  // analysis manager to create a CSV file with the required data
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");
  //this prevents us from overwriting the previous run of data collection (within the same UI or macro run)
  std::string runnumber = std::to_string(run->GetRunID());
  analysisManager->OpenFile("/home/samuel/Documents/Dissertation/Geant4_GLOSimulation/Simulation_CsI/CsI_Data_" + runnumber);

// setting up the data to save to csv
  // general data
  analysisManager->CreateNtuple("EventData", "CsI(Tl)");   //tuple 0 // the first option of the tuple is appended after run number in the CSV name
  analysisManager->CreateNtupleIColumn("Event"); //column0
  analysisManager->CreateNtupleDColumn("Energy Deposited in CsI by single Gamma (MeV)");  // column 1
  analysisManager->CreateNtupleDColumn("Energy Deposited in CsI by single Gamma - Compton (MeV)");  // column 2
  analysisManager->CreateNtupleDColumn("Energy Deposited in CsI by single Gamma - Photoelectric (MeV)");  // column 3
  analysisManager->CreateNtupleDColumn("Energy Deposited in Si by Optical Photons (MeV)");  // column 4
  analysisManager->CreateNtupleDColumn("Total Energy Deposited in Si (MeV)");  // column 5
  analysisManager->CreateNtupleDColumn("Total Energy Deposited in Glass (MeV)");  // column 6
  analysisManager->CreateNtupleDColumn("Total Energy Deposited in ITO (MeV)");  // column 7
  analysisManager->CreateNtupleDColumn("Total Energy Deposited in Al Back (MeV)");  // column 8
  analysisManager->CreateNtupleIColumn("Number of Produced Scintillation Photons"); //column 9
  analysisManager->CreateNtupleIColumn("Number of Photons absorped by Si"); //column 10
  analysisManager->FinishNtuple();

  // photon absorbtion data
  //silicon
  analysisManager->CreateNtuple("PhotonTracks", "Si");   //tuple 1
  analysisManager->CreateNtupleIColumn("Event");  // column 0
  analysisManager->CreateNtupleIColumn("Photon Track num");  // column 1
  analysisManager->CreateNtupleDColumn("Energy Deposited in Si by single photon (MeV)");  // column 2
  analysisManager->FinishNtuple();
  
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

//required for each type of accumulation function
//scintillator
void RunAction::AddEdep(G4double edep)
{
  fEdep += edep;
  fEdep2 += edep * edep;
}

void RunAction::AddScintillationPhoton(G4double photons)
{
  fScintillationPhotons += photons;
}

void RunAction::AddEdepComp(G4double edepComp)
{
  fEdepComp += edepComp;
}

void RunAction::AddEdepPhotElec(G4double edepPhotElec)
{
  fEdepPhotElec  += edepPhotElec;
}


//glass
void RunAction::AddEdepGlassTotal(G4double edepGlassTot)
{
  fEdepGlassTot += edepGlassTot;
}

//ITO
void RunAction::AddEdepITOTotal(G4double edepITOTot)
{
  fEdepITOTot += edepITOTot;
}


//silicon
void RunAction::AddSiPhotonEdep(G4double SiPhotonEdep)
{
  fSiPhotonEdep += SiPhotonEdep;
}

void RunAction::AddEdepSiliconTotal(G4double edepSiTot)
{
  fEdepSiTot += edepSiTot;
}

void RunAction::AddAbsorpedPhoton(G4double abphotons)
{
  fAbsorpedPhotons += abphotons;
}

//Al Back
void RunAction::AddEdepAlBackTotal(G4double edepAlBackTot)
{
  fEdepAlBackTot += edepAlBackTot;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

}  // namespace B1
