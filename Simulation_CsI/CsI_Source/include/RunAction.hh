#ifndef B1RunAction_h
#define B1RunAction_h 1

#include "G4UserRunAction.hh"

#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

namespace CsI
{

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;
	
	//defining the intake types for the functions for the accumulateables
	
	//scintilator
    void AddEdep(G4double edep);
    void AddScintillationPhoton(G4double photons);
    void AddEdepComp(G4double edepComp);
    void AddEdepPhotElec(G4double edepPhotElec);
    void AddScintPhotonEdep(G4double ScintPhotonEdep);
    void AddAbsorpedPhotonScint(G4double abphotonsScint);
    
    
    //glass
    void AddEdepGlassTotal(G4double edepGlassTot);
    void AddGlassPhotonEdep(G4double GlassPhotonEdep);
    void AddAbsorpedPhotonGlass(G4double abphotonsGlass);
    
    //ITO
    void AddEdepITOTotal(G4double edepITOTot);
    void AddITOPhotonEdep(G4double ITOPhotonEdep);
    void AddAbsorpedPhotonITO(G4double abphotonsITO);
    
    //silicon
    void AddSiPhotonEdep(G4double SiPhotonEdep);
    void AddEdepSiliconTotal(G4double edepSiTot);
    void AddAbsorpedPhoton(G4double abphotons);

	//Al Back
	void AddEdepAlBackTotal(G4double edepAlBackTot);
	
  private:
    //Starting values for the accumulateables
    //scintillator
    G4Accumulable<G4double> fEdep = 0.;
    G4Accumulable<G4double> fEdep2 = 0.;
    G4Accumulable<G4int> fScintillationPhotons = 0.;
    G4Accumulable<G4double> fEdepComp = 0.;
    G4Accumulable<G4double> fEdepPhotElec = 0.;
    G4Accumulable<G4double> fScintPhotonEdep = 0.;
    G4Accumulable<G4double> fAbsorpedPhotonsScint = 0.;
    
    
    //Glass
    G4Accumulable<G4double> fEdepGlassTot = 0.;
    G4Accumulable<G4double> fGlassPhotonEdep = 0.;
    G4Accumulable<G4double> fAbsorpedPhotonsGlass = 0.;
    
    //ITO
    G4Accumulable<G4double> fEdepITOTot = 0.;
    G4Accumulable<G4double> fITOPhotonEdep = 0.;
    G4Accumulable<G4double> fAbsorpedPhotonsITO = 0.;
    
    //silicon
    G4Accumulable<G4double> fSiPhotonEdep = 0.;
    G4Accumulable<G4double> fEdepSiTot = 0.;
    G4Accumulable<G4int> fAbsorpedPhotons = 0.;
    
    //Al Backplate
    G4Accumulable<G4double> fEdepAlBackTot = 0.;
};

}  // namespace B1

#endif
