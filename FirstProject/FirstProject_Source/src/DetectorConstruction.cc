#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Trd.hh"
//#include "G4OpticalMaterialProperties.hh"

#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"

namespace FirstProject

{


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  G4String name, symbol;             // a=mass of a mole;
  G4double a, z, density;            // z=mean number of protons;
  G4int iz, n;                       // iz=nb of protons  in an isotope;
                                   // n=nb of nucleons in an isotope;
  G4int ncomponents, natoms;
  G4double abundance, fractionmass;
  G4double temperature, pressure;

  G4UnitDefinition::BuildUnitsTable();

  // Envelope parameters
  G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // World parameters
  G4double world_sizeXY = 1.2 * env_sizeXY;
  G4double world_sizeZ = 1.2 * env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  //initialising the world
  auto solidWorld =
    new G4Box("World",  // its name
              0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                        world_mat,  // its material
                                        "World");  // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                     G4ThreeVector(),  // at (0,0,0)
                                     logicWorld,  // its logical volume
                                     "World",  // its name
                                     nullptr,  // its mother  volume
                                     false,  // no boolean operation
                                     0,  // copy number
                                     checkOverlaps);  // overlaps checking

  // initalising the envelope
  auto solidEnv = new G4Box("Envelope",  // its name
                            0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

  auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                      env_mat,  // its material
                                      "Envelope");  // its name

  new G4PVPlacement(nullptr,  // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    logicEnv,  // its logical volume
                    "Envelope",  // its name
                    logicWorld,  // its mother  volume
                    false,  // no boolean operation
                    0,  // copy number
                    checkOverlaps);  // overlaps checking

  //Rectangle: gives a Si box
  
  //material setup
  //G4Material* Si1_mat = new G4Material(name = "Silicon", z=14., a = 28.09*g/mole, density = 2.329*g/cm3);
  
  //position setup
  //G4ThreeVector posSi = G4ThreeVector(0*cm, 0*cm, 2.5*cm);
  
  //size setup
  //G4double Si_X = 4*cm, Si_Y = 4*cm, Si_Z = 1*cm;
  
  //auto solidSi = new G4Box("ElementalSi", 0.5*Si_X, 0.5*Si_Y, 0.5*Si_Z); //a box with name "ElementalSi"
  
  //auto logicSi = new G4LogicalVolume(solidSi, Si1_mat, "ElemetalSi");
  
  //new G4PVPlacement(nullptr, posSi, logicSi, "ElementalSi", logicEnv, false, 0, checkOverlaps);
  
  
  //Rectangle: gives a CsI(Tl) box
  
  //material setup
  
  //setting up Tl and its isotopes
  G4Isotope* Tl205 = new G4Isotope(name="Tl205", iz=81, n=205, a=201.9723446*g/mole);
  G4Isotope* Tl203 = new G4Isotope(name="Tl203", iz=81, n=203, a=204.9744278*g/mole);

  G4Element* Tl  = new G4Element(name="Thallium", symbol="Tl", ncomponents=2);
  Tl->AddIsotope(Tl205, abundance= 0.7048*perCent);
  Tl->AddIsotope(Tl203, abundance= 0.2952*perCent);
  
  //setting up Cs and I
  G4Element* Cs = new G4Element(name="Ceasium",symbol="Cs", z=55 , a = 132.90545196*g/mole);
  G4Element* I = new G4Element(name="Iodine",symbol="I", z=53 , a = 126.9044719*g/mole);
  
  //creating the CsI(Tl)
  G4Material* CsI_Tl = new G4Material(name = "CsI(Tl)", density = 4.51*g/cm3,3);
  CsI_Tl-> AddElement(Cs, 0.510114053035733);
  CsI_Tl-> AddElement(I, 0.488156344412965);
  CsI_Tl-> AddElement(Tl, 0.001729602551302);
  
  //REQUIRES two+ values here to scintillate
  
  //setting up energy dependent values
  const G4int nEntries = 6;
  
  G4double CsI_PhotonE[nEntries] = {1.907*eV, 1.99*eV, 2.254*eV, 2.431*eV, 2.61*eV, 3.1*eV};//photon energies
  G4double CsI_RInd[nEntries] = {1.79, 1.79, 1.79, 1.79, 1.79, 1.79};//refractive indexes
  G4double CsI_AbsL[nEntries] = {40.*cm, 38.1*cm, 34.8*cm, 32.3*cm, 30.4*cm, 24.7*cm};//absorption length
  G4double CsI_Scint[nEntries] = {0.289, 0.722, 0.977, 0.961, 0.594, 0.225};//relative intensities
  
  
  //defining the material properites table
  G4MaterialPropertiesTable*CsI_MPT = new G4MaterialPropertiesTable();
  
  //scintillation components
  CsI_MPT->AddProperty("SCINTILLATIONCOMPONENT1", CsI_PhotonE, CsI_Scint, nEntries);
  CsI_MPT->AddProperty("SCINTILLATIONCOMPONENT2", CsI_PhotonE, CsI_Scint, nEntries);
  
  //refractive indexes and absorption lengths
  CsI_MPT->AddProperty("RINDEX", CsI_PhotonE, CsI_RInd, nEntries);
  CsI_MPT->AddProperty("ABSLENGTH", CsI_PhotonE, CsI_AbsL, nEntries);
  
  //scintillation
  CsI_MPT->AddConstProperty("SCINTILLATIONYIELD", 54000./MeV);
  CsI_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  
  CsI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 600.*ns);
  CsI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 3500.*ns);
  
  CsI_MPT->AddConstProperty("SCINTILLATIONYIELD1", 0.637);
  CsI_MPT->AddConstProperty("SCINTILLATIONYIELD2", 0.361);
  
  //resolution scale
  CsI_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  
  CsI_Tl->SetMaterialPropertiesTable(CsI_MPT);
  
  //position setup
  G4ThreeVector posCsI_Tl = G4ThreeVector(0*cm, 0*cm, 2.5*cm);
  
  //size setup
  G4double CsI_Tl_X = 4*cm, CsI_Tl_Y = 4*cm, CsI_Tl_Z = 1*cm;
  
  auto solidCsI_Tl = new G4Box("CsI(Tl)", 0.5*CsI_Tl_X, 0.5*CsI_Tl_Y, 0.5*CsI_Tl_Z); //a box with name "ElementalSi"
  
  auto logicCsI_Tl = new G4LogicalVolume(solidCsI_Tl, CsI_Tl, "CsI(Tl)");
  
  new G4PVPlacement(nullptr, posCsI_Tl, logicCsI_Tl, "CsI(Tl)", logicEnv, false, 0, checkOverlaps);
  
  
  
  //~ //
  //~ // Shape 1
  //~ //
  //~ G4Material* shape1_mat = nist->FindOrBuildMaterial("G4_A-150_TISSUE");
  //~ G4ThreeVector pos1 = G4ThreeVector(0, 2 * cm, -7 * cm);

  //~ // Conical section shape
  //~ G4double shape1_rmina = 0. * cm, shape1_rmaxa = 2. * cm;
  //~ G4double shape1_rminb = 0. * cm, shape1_rmaxb = 4. * cm;
  //~ G4double shape1_hz = 3. * cm;
  //~ G4double shape1_phimin = 0. * deg, shape1_phimax = 360. * deg;
  //~ auto solidShape1 = new G4Cons("Shape1", shape1_rmina, shape1_rmaxa, shape1_rminb, shape1_rmaxb,
                                //~ shape1_hz, shape1_phimin, shape1_phimax);

  //~ auto logicShape1 = new G4LogicalVolume(solidShape1,  // its solid
                                         //~ shape1_mat,  // its material
                                         //~ "Shape1");  // its name

  //~ new G4PVPlacement(nullptr,  // no rotation
                    //~ pos1,  // at position
                    //~ logicShape1,  // its logical volume
                    //~ "Shape1",  // its name
                    //~ logicEnv,  // its mother  volume
                    //~ false,  // no boolean operation
                    //~ 0,  // copy number
                    //~ checkOverlaps);  // overlaps checking

  //~ //
  //~ // Shape 2
  //~ //
  //~ G4Material* shape2_mat = nist->FindOrBuildMaterial("G4_BONE_COMPACT_ICRU");
  //~ G4ThreeVector pos2 = G4ThreeVector(0, -1 * cm, 7 * cm);

  //~ // Trapezoid shape
  //~ G4double shape2_dxa = 12 * cm, shape2_dxb = 12 * cm;
  //~ G4double shape2_dya = 10 * cm, shape2_dyb = 16 * cm;
  //~ G4double shape2_dz = 6 * cm;
  //~ auto solidShape2 =
    //~ new G4Trd("Shape2",  // its name
              //~ 0.5 * shape2_dxa, 0.5 * shape2_dxb, 0.5 * shape2_dya, 0.5 * shape2_dyb,
              //~ 0.5 * shape2_dz);  // its size

  //~ auto logicShape2 = new G4LogicalVolume(solidShape2,  // its solid
                                         //~ shape2_mat,  // its material
                                         //~ "Shape2");  // its name

  //~ new G4PVPlacement(nullptr,  // no rotation
                    //~ pos2,  // at position
                    //~ logicShape2,  // its logical volume
                    //~ "Shape2",  // its name
                    //~ logicEnv,  // its mother  volume
                    //~ false,  // no boolean operation
                    //~ 0,  // copy number
                    //~ checkOverlaps);  // overlaps checking

  // Set CsI(Tl) block as scoring volume
  //
  fScoringVolume = logicCsI_Tl;

  //
  // always return the physical World
  //
  return physWorld;
}

} 
