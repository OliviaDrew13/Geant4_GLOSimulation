
#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

namespace CsI
{

class RunAction;

/// Event action class

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

	//defining the functions to calculate the collected data
	//scintillator
    void AddEdep(G4double edep) { fEdep += edep; }
    void AddScintillationPhoton() { fScintillationPhotons += 1; } 
    
    //glass
    void AddEdepGlassTotal(G4double edepGlassTot) { fEdepGlassTot += edepGlassTot; }
    
    //ITO
    void AddEdepITOTotal(G4double edepITOTot) { fEdepITOTot += edepITOTot; }
    
    //silicon
    void AddSiPhotonEdep(G4double SiPhotonEdep) {fSiPhotonEdep += SiPhotonEdep;}
    void AddEdepSiliconTotal(G4double edepSiTot) { fEdepSiTot += edepSiTot; } 
    void AddAbsorpedPhoton(){fAbsorpedPhotons += 1;}

  private:
    RunAction* fRunAction = nullptr;
    //starting values for the above functions
    
    //scintillator
    G4double fEdep = 0.;
    G4double fScintillationPhotons = 0.;
    
    //glass
    G4double fEdepGlassTot = 0.;

    //ITO
    G4double fEdepITOTot = 0.;
    
    //silicon
    G4double fSiPhotonEdep = 0.;
    G4double fEdepSiTot = 0.;
    G4double fAbsorpedPhotons = 0;
};

}  // namespace B1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
