
#ifndef B1DetectorConstruction_h
#define B1DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace CsI
{

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction() = default;
    ~DetectorConstruction() override = default;

    G4VPhysicalVolume* Construct() override;
	
	// defining the scoring volumes from the logical volumes
    G4LogicalVolume* GetScintScoringVolume() const { return scintScoringVolume; }//scintillator scoring volume
    G4LogicalVolume* GetSiScoringVolume() const { return SiScoringVolume; }

  protected:
  //nullptrs
    G4LogicalVolume* scintScoringVolume = nullptr;
    G4LogicalVolume* SiScoringVolume = nullptr;
};

}  // namespace B1

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
