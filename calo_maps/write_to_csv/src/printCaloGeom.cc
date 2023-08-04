
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
#include <calobase/RawTowerDefs.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfoContainerv1.h>

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
  /* RawTowerContainer *towers = findNode::getClass<RawTowerContainer>(topNode, "TOWER_RAW_CEMC"); */
  /* if (!towers) */
  /*   { */
  /*     std::cout << PHWHERE << "printCaloGeom::process_event Could not find node TOWER_RAW_CEMC" << std::endl; */
  /*     return Fun4AllReturnCodes::ABORTEVENT; */
  /*   } */

  TowerInfoContainer *towerinfos = findNode::getClass<TowerInfoContainerv1>(topNode, "TOWERINFO_CALIB_CEMC");
  if (!towerinfos)
    {
      std::cout << PHWHERE << "printCaloGeom::process_event Could not find node TOWERINFO_CALIB_CEMC" << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }

  int size = towerinfos->size();
  /* RawTowerGeomContainer::ConstRange tower_range = towergeom->get_tower_geometries(); */
  /* RawTowerGeomContainer::ConstIterator tower_it = tower_range.first; */
  /* if (size > 100) size = 100; */
  int n_towers = 0;
  std::ofstream cemc;
  cemc.open("emcal_tower_mapping.csv");
  cemc << "iphi,phi min,phi max,ieta,eta min,eta max" << std::endl;
  /* cemc << "iphi,phi min,phi max,ieta,eta min,eta max,eta center" << std::endl; */
  for (int channel=0; channel<size; channel++) {
    /* n_towers++; */
    /* TowerInfo *tow = towerinfos->get_tower_at_channel(channel); */
    unsigned int towerkey = towerinfos->encode_key(channel);
    int iphi = towerinfos->getTowerPhiBin(towerkey);
    int ieta = towerinfos->getTowerEtaBin(towerkey);
    std::pair<double, double> bounds;
    float phimin, phimax, etamin, etamax;
    bounds = towergeom->get_phibounds(iphi);
    phimin = bounds.first;
    phimax = bounds.second;
    bounds = towergeom->get_etabounds(ieta);
    etamin = bounds.first;
    etamax = bounds.second;
    cemc << Form("%i,%f,%f,%i,%f,%f", iphi, phimin, phimax, ieta, etamin, etamax) << std::endl;
    /* float eta_center = towergeom->get_etacenter(ieta); */
    /* cemc << Form("%i,%f,%f,%i,%f,%f,%f", iphi, phimin, phimax, ieta, etamin, etamax, eta_center) << std::endl; */
    if (ieta == 0) {
      n_towers++;
      /* const RawTowerGeom *tower = tower_it->second; */
      RawTowerDefs::keytype key = RawTowerDefs::encode_towerid(towergeom->get_calorimeter_id(), ieta, iphi);
      RawTowerGeom *tower = towergeom->get_tower_geometry(key);
      std::cout << Form("ieta=%i, iphi=%i; ", ieta, iphi);
      /* std::cout << "Greg info: tower=" << tower << "\n"; */
      std::cout << Form("RawTowerGeom phibin=%i, phi=%f; RawTowerGeomContainer phimin=%f, phimax=%f\n", tower->get_binphi(), tower->get_phi(), phimin, phimax);
    }
  }
  std::cout << "n_towers = " << n_towers << std::endl;
  cemc.close();

  /* std::pair<double, double> bounds; */
  /* float phimin, phimax, etamin, etamax; */
  /* for (int i=0; i<towergeom->get_phibins(); i++) { */
  /*   for (int j=0; j<towergeom->get_etabins(); j++) { */
  /*     bounds = towergeom->get_phibounds(i); */
  /*     phimin = bounds.first; */
  /*     phimax = bounds.second; */
  /*     bounds = towergeom->get_etabounds(j); */
  /*     etamin = bounds.first; */
  /*     etamax = bounds.second; */
  /*     cemc << Form("%i,%f,%f,%i,%f,%f", i, phimin, phimax, j, etamin, etamax) << std::endl; */
  /*   } */
  /* } */
  /* cemc.close(); */

  /* RawTowerGeomContainer::ConstRange tower_range = towergeom->get_tower_geometries(); */
  /* RawTowerGeomContainer::ConstIterator tower_it; */
  /* int n_towers = 0; */
  /* for (tower_it = tower_range.first; tower_it != tower_range.second; tower_it++) { */
  /*   const RawTowerGeom *tower = tower_it->second; */
  /*   std::cout << Form("tower phi bin=%i, phi=%f", tower->get_binphi(), tower->get_phi()) << std::endl; */
  /*   /1* std::cout << "tower_it->first=" << tower_it->first << "; tower=" << tower << "; tower->get_id()=" << tower->get_id() << "; tower->get_bineta()=" << tower->get_bineta() << "; tower->get_binphi()=" << tower->get_binphi() << "\n"; *1/ */
  /* } */
  /*   int ieta, iphi; */
  /*   /1* float phimin, phimax, etamin, etamax; *1/ */
  /*   ieta = tower->get_bineta(); */
  /*   iphi = tower->get_binphi(); */
  /*   bounds = towergeom->get_phibounds(iphi); */
  /*   phimin = bounds.first; */
  /*   phimax = bounds.second; */
  /*   bounds = towergeom->get_etabounds(ieta); */
  /*   etamin = bounds.first; */
  /*   etamax = bounds.second; */
  /*   n_towers++; */
  /* } */
  /* std::cout << "n_towers = " << n_towers << "\n"; */

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

