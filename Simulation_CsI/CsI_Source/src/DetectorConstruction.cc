#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
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

namespace CsI

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
  
  //ELEMENTS
  
  //setting up Tl
  G4Isotope* Tl205 = new G4Isotope(name="Tl205", iz=81, n=205, a=201.9723446*g/mole);
  G4Isotope* Tl203 = new G4Isotope(name="Tl203", iz=81, n=203, a=204.9744278*g/mole);

  G4Element* Tl  = new G4Element(name="Thallium", symbol="Tl", ncomponents=2);
  Tl->AddIsotope(Tl205, abundance= 70.48*perCent);
  Tl->AddIsotope(Tl203, abundance= 29.52*perCent);
  
  //setting up Cs
  G4Element* Cs = new G4Element(name="Ceasium",symbol="Cs", z=55 , a = 132.90545196*g/mole);
  
  //setting up I
  G4Element* I = new G4Element(name="Iodine",symbol="I", z=53 , a = 126.9044719*g/mole);

	//setting up Si
  G4Isotope* Si28 = new G4Isotope(name="Si28", iz = 14, n = 28, a = 27.97692653465*g/mole);
  G4Isotope* Si29 = new G4Isotope(name="Si29", iz = 14, n = 29, a = 28.97649466490*g/mole);
  G4Isotope* Si30 = new G4Isotope(name="Si30", iz = 14, n = 30, a = 28.97649466490*g/mole);
  
  G4Element* Si = new G4Element(name = "Silicon", symbol = "Si", ncomponents = 3);
  Si->AddIsotope(Si28, abundance = 92.223*perCent);
  Si->AddIsotope(Si29, abundance = 4.685*perCent);
  Si->AddIsotope(Si30, abundance = 3.092*perCent);
  
  //setting up O
  G4Isotope* O16 = new G4Isotope(name="O16", iz = 8, n = 16, a = 15.99491461957 *g/mole);
  G4Isotope* O17 = new G4Isotope(name="O17", iz = 8, n = 17, a = 16.99913175650*g/mole);
  G4Isotope* O18 = new G4Isotope(name="O18", iz = 8, n = 18, a = 17.99915961286*g/mole);
  
  G4Element* O = new G4Element(name = "Oxygen", symbol = "O", ncomponents = 3);
  O->AddIsotope(O16, abundance = 99.757*perCent);
  O->AddIsotope(O17, abundance = 0.038*perCent);
  O->AddIsotope(O18, abundance = 0.205*perCent);
  
  //setting up Sn
  
  G4Element* Sn = new G4Element(name="Tin",symbol="Sn", z=50 , a =  118.710*g/mole);
  //G4Element* Sn = nist->FindOrBuildElement("Sn"); //WORKS?????? but G4_Sn doesn't
  
  //setting up In
  
  G4Element* In = new G4Element(name="Indium",symbol="In", z=49 , a =  114.818*g/mole);
  
  //setting up B
  G4Element* B = new G4Element(name="Boron",symbol="B", z=5 , a =  10.821*g/mole);
  
  //setting up P
  G4Element* P = new G4Element(name="Phosophorous",symbol="P", z=15 , a = 30.973761998*g/mole);
  
  //setting up Al 
  G4Element* Al = new G4Element(name = "Aluminium", symbol = "Al", z = 14, a = 26.9815385*g/mole);

  
  // MATERIALS
  
  const G4int nEntries = 6;
  
  //material setup: CsI(Tl)
  
  //creating the CsI(Tl)
  G4Material* CsI_Tl = new G4Material(name = "CsI(Tl)", density = 4.51*g/cm3,3);
  CsI_Tl-> AddElement(Cs, 0.510114053035733);
  CsI_Tl-> AddElement(I, 0.488156344412965);
  CsI_Tl-> AddElement(Tl, 0.001729602551302);
  
  //REQUIRES two+ values here to scintillate
  
  //setting up energy dependent values
  
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
  
  CsI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 600.*ns);
  CsI_MPT->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 3500.*ns);
  
  CsI_MPT->AddConstProperty("SCINTILLATIONYIELD1", 0.637);
  CsI_MPT->AddConstProperty("SCINTILLATIONYIELD2", 0.361);
  
  //resolution scale
  CsI_MPT->AddConstProperty("RESOLUTIONSCALE", 1.0);
  
  CsI_Tl->SetMaterialPropertiesTable(CsI_MPT);
    
  //Object Definition
  
  //material setup: Glass
  
  G4Material* SiO2 = new G4Material(name = "Glass", density = 4.51*g/cm3,2);
  SiO2-> AddElement(Si, natoms = 1);
  SiO2-> AddElement(O, natoms = 2);
  
  //setting up energy dependent values
  
  G4double Glass_PhotonE[nEntries] = {1.907*eV, 1.99*eV, 2.254*eV, 2.431*eV, 2.61*eV, 3.1*eV};//photon energies
  G4double Glass_RInd[nEntries] = {1.4565, 1.4574, 1.4599, 1.4618, 1.4638, 1.4701};//refractive indexes
  G4double Glass_AbsL[nEntries] = {10000000000.*cm, 10000000000.*cm, 10000000000.*cm, 10000000000.*cm, 10000000000.*cm, 10000000000.*cm};//absorption length
  
  //setting up material properties table
  G4MaterialPropertiesTable *Glass_MPT = new G4MaterialPropertiesTable();
  
  //refractive index
  Glass_MPT->AddProperty("RINDEX", Glass_PhotonE, Glass_RInd, nEntries);
  //absorbtion length
  Glass_MPT->AddProperty("ABSLENGTH", Glass_PhotonE, Glass_AbsL, nEntries);
  
  SiO2->SetMaterialPropertiesTable(Glass_MPT);
  
  //material setup: ITO
  
  G4Material* In2O3 = new G4Material(name = "In2O3", a = 7.18*g/cm3, 2);
  In2O3->AddElement(In, natoms = 2);
  In2O3->AddElement(O, natoms = 3);
  
  G4Material* SnO2 = new G4Material(name = "SnO2", a = 6.95*g/cm3, 2);
  SnO2->AddElement(Sn, natoms = 1);
  SnO2->AddElement(O, natoms = 2);
  
  G4Material* ITO = new G4Material(name = "ITO", a = 7.14*g/cm3, 2);
  ITO->AddMaterial(In2O3, fractionmass = 90*perCent);
  ITO->AddMaterial(SnO2, fractionmass = 10*perCent);
  
  //setting up energy dependent values
  G4double ITO_PhotonE[nEntries] = {1.907*eV, 1.99*eV, 2.254*eV, 2.431*eV, 2.61*eV, 3.1*eV};//photon energies
  G4double ITO_RInd[nEntries] = {1.7653, 1.7952, 1.8636, 1.9034, 1.9401, 2.0355};//refractive indexes
  G4double ITO_AbsL[nEntries] = {0.001534*cm, 0.001551807*cm, 0.00135638*cm, 0.0010825088*cm, 0.000797257*cm, 0.0002930041*cm};//absorption length
  
  //setting up material properties table
  G4MaterialPropertiesTable *ITO_MPT = new G4MaterialPropertiesTable();
  
  //refractive index
  ITO_MPT->AddProperty("RINDEX", ITO_PhotonE, ITO_RInd, nEntries);
  //absorbtion length
  ITO_MPT->AddProperty("ABSLENGTH", ITO_PhotonE, ITO_AbsL, nEntries);
  
  ITO->SetMaterialPropertiesTable(ITO_MPT);
  
  //material setup: Silicons
  
  G4Material* pSi = new G4Material("pSi", a = 2.3296*g/cm3, 2);
  pSi->AddElement(B, 0.002001934666*perCent);
  pSi->AddElement(Si, 99.9979980653*perCent);
  
  G4Material* nSi = new G4Material("nSi", a = 2.3296*g/cm3, 2);
  nSi->AddElement(P,  0.2001934666*perCent);
  nSi->AddElement(Si, 99.7998065334*perCent);
  
  G4Material* iSi = new G4Material("iSi", a = 2.3296*g/cm3, 1);
  iSi->AddElement(Si, 1);
  
  // optical properties, silicons
  
  //setting up energy dependent values
  G4double Si_PhotonE[nEntries] = {1.907*eV, 1.99*eV, 2.254*eV, 2.431*eV, 2.61*eV, 3.1*eV};//photon energies
  G4double Si_RInd[nEntries] = {4.5231, 4.6171, 4.8521, 4.9897, 5.1028, 5.1858};//refractive indexes
  G4double Si_AbsL[nEntries] = {2.76e-5*cm, 1.9e-6*cm, 7.83e-6*cm, 5.02e-6*cm, 3.44e-6*cm, 1.5e-6*cm};//absorption length
  
  //setting up material properties table
  G4MaterialPropertiesTable *Si_MPT = new G4MaterialPropertiesTable();
  
  //refractive index
  Si_MPT->AddProperty("RINDEX", Si_PhotonE, Si_RInd, nEntries);
  //absorbtion length
  Si_MPT->AddProperty("ABSLENGTH", Si_PhotonE, Si_AbsL, nEntries);
  
  pSi->SetMaterialPropertiesTable(Si_MPT);
  nSi->SetMaterialPropertiesTable(Si_MPT);
  iSi->SetMaterialPropertiesTable(Si_MPT);

  //Material setup: Al
  G4Material* mAl = new G4Material("Al", a = 2.7*g/cm3, 1);
  mAl-> AddElement(Al, 1);
  
  //POSITION DEFINITION
  //CsI(Tl)
  //position vector
  G4ThreeVector posCsI_Tl = G4ThreeVector(0*cm, 0*cm, -0.5*cm); //This is the position of the MIDPOINT
  //cylinder parameters
  G4double innerRadius_CsI = 0.*cm;
  G4double outerRadius_CsI = 2.5*cm;
  G4double hz_CsI = 1*cm; //G4 needs 1/2 width for this value, full width entered here and halved at definition for ease of use
  G4double startAngle_CsI = 0.*deg;
  G4double spanningAngle_CsI = 360.*deg;
  //defining the cylinder
  G4Tubs* solidCsI_Tl = new G4Tubs("CsI(Tl)", innerRadius_CsI, outerRadius_CsI, hz_CsI/2, startAngle_CsI, spanningAngle_CsI);
  //defining logical volume            
  auto logicCsI_Tl = new G4LogicalVolume(solidCsI_Tl, CsI_Tl, "CsI(Tl)");
  //placing the CsI
  new G4PVPlacement(nullptr, posCsI_Tl, logicCsI_Tl, "CsI(Tl)", logicEnv, false, 0, checkOverlaps);
  
  //Glass
  //position vector
  G4ThreeVector posGlass = G4ThreeVector(0*cm, 0*cm, 0.55 *mm); //glass thickness is 1.1mm 
  //box parameters
  G4double Glass_X = 56.7*mm, Glass_Y = 96.7*mm, Glass_Z = 1.1*mm;
  //defining the box
  auto solid_Glass = new G4Box("Glass", 0.5*Glass_X, 0.5*Glass_Y, 0.5*Glass_Z);
  //defining logical volume
  auto logicGlass = new G4LogicalVolume(solid_Glass, SiO2, "Glass");
  //placing the glass
  new G4PVPlacement(nullptr, posGlass, logicGlass, "Glass", logicEnv, false, 0, checkOverlaps);
  
  //ITO
  //position vector
  G4ThreeVector posITO = G4ThreeVector(0*cm, 0*cm, 1100045*nm); //ITO thickness is 90nm
  //box parameters
  G4double ITO_X = 56.7*mm, ITO_Y = 96.7*mm, ITO_Z = 90*nm;
  //defining the box
  auto solid_ITO = new G4Box("ITO", 0.5*ITO_X, 0.5*ITO_Y, 0.5*ITO_Z);
  //defining logical volume
  auto logicITO = new G4LogicalVolume(solid_ITO, ITO, "ITO");
  //placing the ITO
  new G4PVPlacement(nullptr, posITO, logicITO, "ITO", logicEnv, false, 0, checkOverlaps);
  
  //Silicon - all undoped
  //position vector
  G4ThreeVector posSi = G4ThreeVector(0*cm, 0*cm, 1100210*nm); //Si thickness is 240nm
  //box parameters
  G4double Si_X = 53.3*mm, Si_Y = 94.7*mm, Si_Z = 240*nm;
  //defining the box
  auto solid_Si = new G4Box("Si", 0.5*Si_X, 0.5*Si_Y, 0.5*Si_Z);
  //defining logical volume
  auto logicSi = new G4LogicalVolume(solid_Si, iSi, "Silicon");
  //placing the Silicon
  new G4PVPlacement(nullptr, posSi, logicSi, "Si", logicEnv, false, 0, checkOverlaps);
  
  //Al
  //position vector
  G4ThreeVector posAl = G4ThreeVector(0*cm, 0*cm, 1100380*nm); //Al thickness is 100nm
  //box parameters
  G4double Al_X = 56.7*mm, Al_Y = 96.7*mm, Al_Z = 100*nm;
  //defining the box
  auto solid_Al = new G4Box("Al", 0.5*Al_X, 0.5*Al_Y, 0.5*Al_Z);
  //defining logical volume
  auto logicAl = new G4LogicalVolume(solid_Al, mAl, "Aluminium");
  //placing the Al
  new G4PVPlacement(nullptr, posAl, logicAl, "Al", logicEnv, false, 0, checkOverlaps);
  

  // Seting scoring volumes
  //
  scintScoringVolume = logicCsI_Tl;
  SiScoringVolume = logicSi;
  

  //
  // always return the physical World
  //
  return physWorld;
}

} 
