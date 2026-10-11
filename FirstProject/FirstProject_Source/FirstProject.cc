#include <iostream>

//including the required headers
#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "QBBC.hh"
#include "G4VModularPhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
// #include "Randomize.hh"

using namespace FirstProject;


int main(int argc, char** argv)
{
  // Detecting interactive mode and defining the UI session
  G4UIExecutive* ui = nullptr;
  if (argc == 1) {
    ui = new G4UIExecutive(argc, argv);
  }

  // use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list - includes physics for optical photons
  G4VModularPhysicsList* physicsList = new QBBC;
  G4EmStandardPhysics_option4* opticalPhysics = new G4EmStandardPhysics_option4();
  physicsList->RegisterPhysics(opticalPhysics);
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization with the default graphics system
  auto visManager = new G4VisExecutive(argc, argv);
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  auto UImanager = G4UImanager::GetUIpointer();

  // Process macro or start UI session
  if (!ui) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  delete visManager;
  delete runManager;
}
