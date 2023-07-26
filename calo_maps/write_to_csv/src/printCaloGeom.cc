
//____________________________________________________________________________..

//our base code
#include "printCaloGeom.h"
#include <iostream>
#include <fstream>
#include <utility>
#include <TString.h>

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>

//Fun4all stuff
#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>
#include <phool/phool.h>
#include <ffaobjects/EventHeader.h>

//for emc clusters
#include <calobase/RawCluster.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>

//caloEvalStack for cluster to truth matching
#include <g4eval/CaloEvalStack.h>
#include <g4eval/CaloRawClusterEval.h>

#pragma GCC diagnostic push 
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic pop

//____________________________________________________________________________..
printCaloGeom::printCaloGeom(const std::string &name):
  SubsysReco(name),
  getEvent(-9999)
{
std::cout << "printCaloGeom::printCaloGeom(const std::string &name) Calling ctor" << std::endl;
}
//____________________________________________________________________________..
printCaloGeom::~printCaloGeom()
{
  std::cout << "printCaloGeom::~printCaloGeom() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int printCaloGeom::Init(PHCompositeNode *topNode)
{
  std::cout << "printCaloGeom::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::InitRun(PHCompositeNode *topNode)
{
  std::cout << "printCaloGeom::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::process_event(PHCompositeNode *topNode)
{
  /* std::cout << "printCaloGeom::process_event(PHCompositeNode *topNode) Processing event..." << std::endl; */

  //Tower geometry node for location information
  //CEMC
  RawTowerGeomContainer *towergeom = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");
  if (!towergeom)
    {
      std::cout << PHWHERE << "printCaloGeom::process_event Could not find node TOWERGEOM_CEMC"  << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
  /* std::cout << "\n\nGreg info: got CEMC tower geometry. More details:\n"; */
  /* towergeom->identify(); */
  std::ofstream cemc;
  cemc.open("emcal_tower_mapping.csv");
  cemc << "iphi,phi min,phi max,ieta,eta min,eta max" << std::endl;
  std::pair<double, double> bounds;
  float phimin, phimax, etamin, etamax;
  for (int i=0; i<towergeom->get_phibins(); i++) {
    for (int j=0; j<towergeom->get_etabins(); j++) {
      bounds = towergeom->get_phibounds(i);
      phimin = bounds.first;
      phimax = bounds.second;
      bounds = towergeom->get_etabounds(j);
      etamin = bounds.first;
      etamax = bounds.second;
      cemc << Form("%i,%f,%f,%i,%f,%f", i, phimin, phimax, j, etamin, etamax) << std::endl;
    }
  }
  cemc.close();

  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::ResetEvent(PHCompositeNode *topNode)
{
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::EndRun(const int runnumber)
{
  std::cout << "printCaloGeom::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::End(PHCompositeNode *topNode)
{
  std::cout << "printCaloGeom::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int printCaloGeom::Reset(PHCompositeNode *topNode)
{
  std::cout << "printCaloGeom::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void printCaloGeom::Print(const std::string &what) const
{
  std::cout << "printCaloGeom::Print(const std::string &what) const Printing info for " << what << std::endl;
}

