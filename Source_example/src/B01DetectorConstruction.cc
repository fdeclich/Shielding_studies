//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file biasing/B01/src/B01DetectorConstruction.cc
/// \brief Implementation of the B01DetectorConstruction class
//
//
//

#include "B01DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Colour.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Tubs.hh"
#include "G4Types.hh"
#include "G4VisAttributes.hh"
#include "globals.hh"
//To use pre defined elements and materials
#include "G4NistManager.hh"

#include <set>
#include <sstream>

// For Primitive Scorers
#include "G4MultiFunctionalDetector.hh"
#include "G4PSNofCollision.hh"
#include "G4PSPopulation.hh"
#include "G4PSTrackCounter.hh"
#include "G4PSTrackLength.hh"
#include "G4SDManager.hh"
#include "G4SDParticleFilter.hh"

// for importance biasing
#include "G4IStore.hh"

// for weight window technique
#include "G4WeightWindowStore.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01DetectorConstruction::B01DetectorConstruction()
  : G4VUserDetectorConstruction(), fLogicalVolumeVector(), fPhysicalVolumeVector()
{
  ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B01DetectorConstruction::~B01DetectorConstruction()
{
  fLogicalVolumeVector.clear();
  fPhysicalVolumeVector.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B01DetectorConstruction::Construct()
{
  G4double pos_x;
  G4double pos_y;
  G4double pos_z;

  G4double density, pressure, temperature;
  G4double A;
  G4int Z;

  G4String name, symbol;
  G4double z;
  G4double fractionmass;
  A = 1.01 * g / mole;
  //G4Element* elH = new G4Element(name = "Hydrogen", symbol = "H", Z = 1, A);

  A = 12.01 * g / mole;
  //G4Element* elC = new G4Element(name = "Carbon", symbol = "C", Z = 6, A);

  A = 16.00 * g / mole;
  //G4Element* elO = new G4Element(name = "Oxygen", symbol = "O", Z = 8, A);

  A = 22.99 * g / mole;
  //G4Element* elNa = new G4Element(name = "Natrium", symbol = "Na", Z = 11, A);

  A = 200.59 * g / mole;
  G4Element* elHg = new G4Element(name = "Hg", symbol = "Hg", Z = 80, A);

  A = 26.98 * g / mole;
  G4Element* elAl = new G4Element(name = "Aluminium", symbol = "Al", Z = 13, A);

  A = 28.09 * g / mole;
  G4Element* elSi = new G4Element(name = "Silicon", symbol = "Si", Z = 14, A);

  A = 39.1 * g / mole;
  G4Element* elK = new G4Element(name = "K", symbol = "K", Z = 19, A);

  A = 69.72 * g / mole;
  G4Element* elCa = new G4Element(name = "Calzium", symbol = "Ca", Z = 31, A);

  A = 55.85 * g / mole;
  G4Element* elFe = new G4Element(name = "Iron", symbol = "Fe", Z = 26, A);

  //Defining materials to build the ones of the detector
  G4Element* elC = new G4Element("Carbon", "C", 6., 12.011 * g / mole);
  G4Element* elH = new G4Element("Hydrogen", "H", 1., 1.00784 * g / mole);
  G4Element* elN = new G4Element("Nitrogen", "N", 7., 14.0067 * g / mole);
  G4Element* elNa = new G4Element("Sodium", "Na", 11., 22.99 * g / mole);
  G4Element* elO = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
  G4Element* elS = new G4Element("Sulphur", "S", 16., 32.065 * g / mole);
  G4Element* elB = new G4Element("Boron", "B", 5., 10.811 * g / mole);

  //Defining "real" elements
  G4NistManager* manager = G4NistManager::Instance();
  G4Material* steel = manager->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4Material* cellulose = manager->FindOrBuildMaterial("G4_CELLULOSE_BUTYRATE");
  G4Material* alluminium = manager->FindOrBuildMaterial("G4_ALUMINUM_OXIDE");
  G4Material* polyethilene = manager->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material* water = manager->FindOrBuildMaterial("G4_WATER");

  G4Material* lignine = new G4Material("Lignine", 1.40 * g / cm3, 6);  
  lignine->AddElement(elC, 18);
  lignine->AddElement(elH, 13);
  lignine->AddElement(elN, 3);
  lignine->AddElement(elNa, 2);
  lignine->AddElement(elO, 8);
  lignine->AddElement(elS, 2);

  G4Material* plywood = new G4Material("Plywood", 0.50 * g / cm3, 2);
  plywood->AddMaterial(cellulose, 0.70);
  plywood->AddMaterial(lignine, 0.30);
  
  G4Material* polyurethane = new G4Material("Polyurethane", 90 * kg / m3, 4);
  polyurethane->AddElement(elC, 27);
  polyurethane->AddElement(elH, 36);
  polyurethane->AddElement(elN, 2);
  polyurethane->AddElement(elO, 10);

  G4Material* BoronOxide = new G4Material("BoronOxide", 2.50 * g / cm3, 2);
  BoronOxide->AddElement(elB, 2);
  BoronOxide->AddElement(elO, 3);
  const G4ElementVector* elements= BoronOxide->GetElementVector();
  const G4double* dens = BoronOxide->GetVecNbOfAtomsPerVolume();
  for (int i =0; i<BoronOxide->GetNumberOfElements(); i++) {
    G4String elname= elements->at(i)->GetName();
    G4double ndens = dens[i];
    G4cout << "Name: " << elname.data() << " ; Density: " << ndens <<G4endl;
  }
  
  G4Material* BoratedPolyethilene = new G4Material("BoratedPolyethilene", 1.0 * g / cm3, 2);
  BoratedPolyethilene->AddMaterial(polyethilene, 0.85);
  BoratedPolyethilene->AddMaterial(BoronOxide, 0.15);
  
  density = universe_mean_density;  // from PhysicalConstants.h
  pressure = 3.e-18 * pascal;
  temperature = 2.73 * kelvin;
  G4Material* Galactic = new G4Material(name = "Galactic", z = 1., A = 1.01 * g / mole, density,
                                        kStateGas, temperature, pressure);
  
  density = 2.03 * g / cm3;
  G4Material* Concrete = new G4Material("Concrete", density, 10);
  Concrete->AddElement(elH, fractionmass = 0.01);
  Concrete->AddElement(elO, fractionmass = 0.529);
  Concrete->AddElement(elNa, fractionmass = 0.016);
  Concrete->AddElement(elHg, fractionmass = 0.002);
  Concrete->AddElement(elAl, fractionmass = 0.034);
  Concrete->AddElement(elSi, fractionmass = 0.337);
  Concrete->AddElement(elK, fractionmass = 0.013);
  Concrete->AddElement(elCa, fractionmass = 0.044);
  Concrete->AddElement(elFe, fractionmass = 0.014);
  Concrete->AddElement(elC, fractionmass = 0.001);


  /////////////////////////////
  // world cylinder volume
  ////////////////////////////
  
  // world solid

  G4double innerRadiusCylinder = 0 * cm;
  G4double outerRadiusCylinder = 100 * cm;
  G4double heightCylinder = 68.635 * cm;
  G4double startAngleCylinder = 0 * deg;
  G4double spanningAngleCylinder = 360 * deg;

  G4Tubs* worldCylinder = new G4Tubs("worldCylinder", innerRadiusCylinder, outerRadiusCylinder,
                                     heightCylinder, startAngleCylinder, spanningAngleCylinder);

  // logical world

  G4LogicalVolume* worldCylinder_log =
    new G4LogicalVolume(worldCylinder, Galactic, "worldCylinder_log");
  fLogicalVolumeVector.push_back(worldCylinder_log);

  name = "shieldWorld";
  fWorldVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), worldCylinder_log, name, 0, false, 0);

  fPhysicalVolumeVector.push_back(fWorldVolume);

  //Creating a struct to collect all the data
  struct shieldLayer_t {
    G4double thickness = {};
    G4Material* material = nullptr;
    G4Colour colour = {};
    G4String vname = {};
    shieldLayer_t (G4double thick, G4Material* mat, G4Colour col, G4String nam) {thickness = thick; 
                  material = mat; colour = col; vname = nam;}
  };

  std::vector<shieldLayer_t> layers;
  layers.emplace_back(15 * cm, water, G4Colour(0, 0, 0.9), "water");
  layers.emplace_back(1.5 * cm, steel, G4Colour(0.55, 0.5, 0.65), "steel0");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood0");
  layers.emplace_back(5 * cm, BoratedPolyethilene, G4Colour(0.3, 1.0, 0.0), "borpol0");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood1");
  layers.emplace_back(19.5 * cm, polyurethane, G4Colour(1.0, 1.0, 0.0), "poly0");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood2");
  layers.emplace_back(0.75 , steel, G4Colour(0.55, 0.5, 0.65), "steel1");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood3");
  layers.emplace_back(19.5 * cm, polyurethane, G4Colour(1.0, 1.0, 0.0), "poly1");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood4");
  layers.emplace_back(5 * cm, BoratedPolyethilene, G4Colour(0.3, 1.0, 0.0), "borpol1");
  layers.emplace_back(.5 * cm, plywood, G4Colour(0.5, 0.35, 0.25), "plywood5");
  layers.emplace_back(0.6 , steel, G4Colour(0.55, 0.5, 0.65), "steel2");

  // Creating every layer of the cryostat: long process :(
  //Outer frame
  G4double innerRadiusShield = 0 * cm;
  G4double outerRadiusShield = 100 * cm;
  G4double heightShield = 68.635 * cm;
  G4double startAngleShield = 0 * deg;
  G4double spanningAngleShield = 360 * deg;

  pos_x = 0 * cm;
  pos_y = 0 * cm;
  G4double startz = -68.635 * cm;
  pos_z = startz;
  for (int i=0; i < layers.size(); i++) {
    auto &l=layers.at(i);
    G4Tubs* Solid = new G4Tubs (l.vname + "_solid", innerRadiusShield, outerRadiusShield, 
                                l.thickness, startAngleShield, spanningAngleShield);
    G4LogicalVolume* Logic = new G4LogicalVolume (Solid, l.material, l.vname + "_lv");
    Logic->SetVisAttributes(l.colour);
    fLogicalVolumeVector.push_back(Logic);
    if (i == 0) {
      pos_z = startz + l.thickness; 
    }
    else {
      pos_z= pos_z + l.thickness + layers.at(i-1).thickness;
    }
    G4VPhysicalVolume* pvol = new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), Logic,
                                                l.vname + "_PV", worldCylinder_log, false, i, true);
    fPhysicalVolumeVector.push_back(pvol);
  };
  SetSensitive();
  return fWorldVolume;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VIStore* B01DetectorConstruction::CreateImportanceStore()
{
  G4cout << " B01DetectorConstruction:: Creating Importance Store " << G4endl;
  if (!fPhysicalVolumeVector.size()) {
    G4Exception("B01DetectorConstruction::CreateImportanceStore", "exampleB01_0001",
                RunMustBeAborted, "no physical volumes created yet!");
  }

  fWorldVolume = fPhysicalVolumeVector[0];

  // creating and filling the importance store

  G4IStore* istore = G4IStore::GetInstance();

  G4int n = 0;
  G4double imp = 1;
  istore->AddImportanceGeometryCell(1, *fWorldVolume);
  for (std::vector<G4VPhysicalVolume*>::iterator it = fPhysicalVolumeVector.begin();
       it != fPhysicalVolumeVector.end() - 1; it++)
  {
    if (*it != fWorldVolume) {
      //mp = std::pow(2., n++);
      G4cout << "Going to assign importance: " << imp << ", to volume: " << (*it)->GetName()
             << G4endl;
      istore->AddImportanceGeometryCell(imp, *(*it), n);
    }
  }

  // the remaining part pf the geometry (rest) gets the same
  // importance as the last conrete cell
  //
  istore->AddImportanceGeometryCell(imp, *(fPhysicalVolumeVector[fPhysicalVolumeVector.size() - 1]),
                                    ++n);

  return istore;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VWeightWindowStore* B01DetectorConstruction::CreateWeightWindowStore()
{
  if (!fPhysicalVolumeVector.size()) {
    G4Exception("B01DetectorConstruction::CreateWeightWindowStore", "exampleB01_0002",
                RunMustBeAborted, "no physical volumes created yet!");
  }

  fWorldVolume = fPhysicalVolumeVector[0];

  // creating and filling the weight window store

  G4WeightWindowStore* wwstore = G4WeightWindowStore::GetInstance();

  // create one energy region covering the energies of the problem
  //
  std::set<G4double, std::less<G4double>> enBounds;
  enBounds.insert(1 * GeV);
  wwstore->SetGeneralUpperEnergyBounds(enBounds);

  G4int n = 0;
  G4double lowerWeight = 1;
  std::vector<G4double> lowerWeights;

  lowerWeights.push_back(1);
  G4GeometryCell gWorldCell(*fWorldVolume, 0);
  wwstore->AddLowerWeights(gWorldCell, lowerWeights);

  for (std::vector<G4VPhysicalVolume*>::iterator it = fPhysicalVolumeVector.begin();
       it != fPhysicalVolumeVector.end() - 1; it++)
  {
    if (*it != fWorldVolume) {
      lowerWeight = 1. / std::pow(2., n++);
      G4cout << "Going to assign lower weight: " << lowerWeight
             << ", to volume: " << (*it)->GetName() << G4endl;
      G4GeometryCell gCell(*(*it), n);
      lowerWeights.clear();
      lowerWeights.push_back(lowerWeight);
      wwstore->AddLowerWeights(gCell, lowerWeights);
    }
  }

  // the remaining part pf the geometry (rest) gets the same
  // lower weight bound  as the last conrete cell
  //
  G4GeometryCell gRestCell(*(fPhysicalVolumeVector[fPhysicalVolumeVector.size() - 1]), ++n);
  wwstore->AddLowerWeights(gRestCell, lowerWeights);

  return wwstore;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String B01DetectorConstruction::GetCellName(G4int i)
{
  std::ostringstream os;
  os << "cell_";
  if (i < 10) {
    os << "0";
  }
  os << i;
  G4String name = os.str();
  return name;
}

G4VPhysicalVolume* B01DetectorConstruction::GetWorldVolume()
{
  return fWorldVolume;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01DetectorConstruction::SetSensitive()
{
  //  -------------------------------------------------
  //   The collection names of defined Primitives are
  //   0       ConcreteSD/Collisions
  //   1       ConcreteSD/CollWeight
  //   2       ConcreteSD/Population
  //   3       ConcreteSD/TrackEnter
  //   4       ConcreteSD/SL
  //   5       ConcreteSD/SLW
  //   6       ConcreteSD/SLWE
  //   7       ConcreteSD/SLW_V
  //   8       ConcreteSD/SLWE_V
  //  -------------------------------------------------

  // moved to ConstructSDandField() for MT compliance
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B01DetectorConstruction::ConstructSDandField()
{
  //  Sensitive Detector Manager.
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  // Sensitive Detector Name
  G4String concreteSDname = "ConcreteSD";

  //------------------------
  // MultiFunctionalDetector
  //------------------------
  //
  // Define MultiFunctionalDetector with name.
  G4MultiFunctionalDetector* MFDet = new G4MultiFunctionalDetector(concreteSDname);
  SDman->AddNewDetector(MFDet);  // Register SD to SDManager

  G4String fltName, particleName;
  G4SDParticleFilter* neutronFilter =
    new G4SDParticleFilter(fltName = "neutronFilter", particleName = "neutron");

  MFDet->SetFilter(neutronFilter);

  for (std::vector<G4LogicalVolume*>::iterator it = fLogicalVolumeVector.begin();
       it != fLogicalVolumeVector.end(); it++)
  {
    //      (*it)->SetSensitiveDetector(MFDet);
    SetSensitiveDetector((*it)->GetName(), MFDet);
  }

  G4String psName;
  G4PSNofCollision* scorer0 = new G4PSNofCollision(psName = "Collisions");
  MFDet->RegisterPrimitive(scorer0);

  G4PSNofCollision* scorer1 = new G4PSNofCollision(psName = "CollWeight");
  scorer1->Weighted(true);
  MFDet->RegisterPrimitive(scorer1);

  G4PSPopulation* scorer2 = new G4PSPopulation(psName = "Population");
  MFDet->RegisterPrimitive(scorer2);

  G4PSTrackCounter* scorer3 = new G4PSTrackCounter(psName = "TrackEnter", fCurrent_In);
  MFDet->RegisterPrimitive(scorer3);

  G4PSTrackLength* scorer4 = new G4PSTrackLength(psName = "SL");
  MFDet->RegisterPrimitive(scorer4);

  G4PSTrackLength* scorer5 = new G4PSTrackLength(psName = "SLW");
  scorer5->Weighted(true);
  MFDet->RegisterPrimitive(scorer5);

  G4PSTrackLength* scorer6 = new G4PSTrackLength(psName = "SLWE");
  scorer6->Weighted(true);
  scorer6->MultiplyKineticEnergy(true);
  MFDet->RegisterPrimitive(scorer6);

  G4PSTrackLength* scorer7 = new G4PSTrackLength(psName = "SLW_V");
  scorer7->Weighted(true);
  scorer7->DivideByVelocity(true);
  MFDet->RegisterPrimitive(scorer7);

  G4PSTrackLength* scorer8 = new G4PSTrackLength(psName = "SLWE_V");
  scorer8->Weighted(true);
  scorer8->MultiplyKineticEnergy(true);
  scorer8->DivideByVelocity(true);
  MFDet->RegisterPrimitive(scorer8);

  G4PSTermination* scorer9 = new G4PSTermination (psName = "Termination_W");
  scorer9->Weighted(true);
  MFDet->RegisterPrimitive(scorer9);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
