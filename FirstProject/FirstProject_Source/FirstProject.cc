#include <iostream>

//including the required headers
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4SteppingVerbose.hh"

#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"

//main
int main (int argc, char** argv){
	
	G4UIExecutive* ui = nullptr;
	if (argc == 1) {
		ui = new G4UIExecutive(argc, argv);
	}
	
	// use G4SteppingVerboseWithUnits
	G4int precision = 4;
	G4SteppingVerbose::UseBestUnit(precision);
	
	//if Geant4 is installed in multi vs single threaded mode, a different run manager is required
	// this handles it for us
	#ifdef G4MULTITHREADED
		G4MTRunManager* runManager = new G4MTRunManager;
	#else 
		G4RunManager* runManager = new G4RunManager;
	#endif
	
	
	G4VisManager* visManager = new G4VisExecutive(argc, argv);
	visManager->Initialise();
	
	//setting up the UI
	G4UImanager* UImanager = G4UImanager::GetUIpointer();
	
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
	
	//Job Termination
	delete visManager;
	delete runManager;
	return 0;
}
