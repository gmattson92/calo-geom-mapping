//____________________________________________________________________________..
//
// This is a template for a Fun4All SubsysReco module with all methods from the
// $OFFLINE_MAIN/include/fun4all/SubsysReco.h baseclass
// You do not have to implement all of them, you can just remove unused methods
// here and in CaloGeomMapping.h.
//
// CaloGeomMapping(const std::string &name = "CaloGeomMapping")
// everything is keyed to CaloGeomMapping, duplicate names do work but it makes
// e.g. finding culprits in logs difficult or getting a pointer to the module
// from the command line
//
// CaloGeomMapping::~CaloGeomMapping()
// this is called when the Fun4AllServer is deleted at the end of running. Be
// mindful what you delete - you do loose ownership of object you put on the node tree
//
// int CaloGeomMapping::Init(PHCompositeNode *topNode)
// This method is called when the module is registered with the Fun4AllServer. You
// can create historgrams here or put objects on the node tree but be aware that
// modules which haven't been registered yet did not put antyhing on the node tree
//
// int CaloGeomMapping::InitRun(PHCompositeNode *topNode)
// This method is called when the first event is read (or generated). At
// this point the run number is known (which is mainly interesting for raw data
// processing). Also all objects are on the node tree in case your module's action
// depends on what else is around. Last chance to put nodes under the DST Node
// We mix events during readback if branches are added after the first event
//
// int CaloGeomMapping::process_event(PHCompositeNode *topNode)
// called for every event. Return codes trigger actions, you find them in
// $OFFLINE_MAIN/include/fun4all/Fun4AllReturnCodes.h
//   everything is good:
//     return Fun4AllReturnCodes::EVENT_OK
//   abort event reconstruction, clear everything and process next event:
//     return Fun4AllReturnCodes::ABORT_EVENT; 
//   proceed but do not save this event in output (needs output manager setting):
//     return Fun4AllReturnCodes::DISCARD_EVENT; 
//   abort processing:
//     return Fun4AllReturnCodes::ABORT_RUN
// all other integers will lead to an error and abort of processing
//
// int CaloGeomMapping::ResetEvent(PHCompositeNode *topNode)
// If you have internal data structures (arrays, stl containers) which needs clearing
// after each event, this is the place to do that. The nodes under the DST node are cleared
// by the framework
//
// int CaloGeomMapping::EndRun(const int runnumber)
// This method is called at the end of a run when an event from a new run is
// encountered. Useful when analyzing multiple runs (raw data). Also called at
// the end of processing (before the End() method)
//
// int CaloGeomMapping::End(PHCompositeNode *topNode)
// This is called at the end of processing. It needs to be called by the macro
// by Fun4AllServer::End(), so do not forget this in your macro
//
// int CaloGeomMapping::Reset(PHCompositeNode *topNode)
// not really used - it is called before the dtor is called
//
// void CaloGeomMapping::Print(const std::string &what) const
// Called from the command line - useful to print information when you need it
//
//____________________________________________________________________________..

#include "CaloGeomMapping.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <calobase/RawTowerDefs.h>           // for encode_towerid
#include <calobase/RawTowerGeom.h>           // for RawTowerGeom
#include <calobase/RawTowerGeomContainer.h>  // for RawTowerGeomC...
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerGeomv1.h>

//____________________________________________________________________________..
CaloGeomMapping::CaloGeomMapping(const std::string &name):
 SubsysReco(name)
{
  std::cout << "CaloGeomMapping::CaloGeomMapping(const std::string &name) Calling ctor" << std::endl;
}

//____________________________________________________________________________..
CaloGeomMapping::~CaloGeomMapping()
{
  std::cout << "CaloGeomMapping::~CaloGeomMapping() Calling dtor" << std::endl;
}

//____________________________________________________________________________..
int CaloGeomMapping::Init(PHCompositeNode *topNode)
{
  std::cout << "CaloGeomMapping::Init(PHCompositeNode *topNode) Initializing" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::InitRun(PHCompositeNode *topNode)
{
  std::cout << "CaloGeomMapping::InitRun(PHCompositeNode *topNode) Initializing for Run XXX" << std::endl;
  try
  {
    CreateGeomNode(topNode);
  }
  catch (std::exception &e)
  {
    std::cout << e.what() << std::endl;
    exit(1);
  }
  topNode->print();
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::process_event(PHCompositeNode *topNode)
{
  std::cout << "CaloGeomMapping::process_event(PHCompositeNode *topNode) Processing Event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::ResetEvent(PHCompositeNode *topNode)
{
  std::cout << "CaloGeomMapping::ResetEvent(PHCompositeNode *topNode) Resetting internal structures, prepare for next event" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::EndRun(const int runnumber)
{
  std::cout << "CaloGeomMapping::EndRun(const int runnumber) Ending Run for Run " << runnumber << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::End(PHCompositeNode *topNode)
{
  std::cout << "CaloGeomMapping::End(PHCompositeNode *topNode) This is the End..." << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
int CaloGeomMapping::Reset(PHCompositeNode *topNode)
{
 std::cout << "CaloGeomMapping::Reset(PHCompositeNode *topNode) being Reset" << std::endl;
  return Fun4AllReturnCodes::EVENT_OK;
}

//____________________________________________________________________________..
void CaloGeomMapping::Print(const std::string &what) const
{
  std::cout << "CaloGeomMapping::Print(const std::string &what) const Printing info for " << what << std::endl;
}

void CaloGeomMapping::CreateGeomNode(PHCompositeNode* topNode)
{
  PHNodeIterator nodeItr(topNode);
  // DST node
  PHCompositeNode *dst_node = dynamic_cast<PHCompositeNode *>(
      nodeItr.findFirst("PHCompositeNode", "DST"));
  if (!dst_node)
  {
    std::cout << "PHComposite node created: DST" << std::endl;
    dst_node = new PHCompositeNode("DST");
    topNode->addNode(dst_node);
  }

  m_TowerGeomNodeName = "TOWERGEOM_" + m_Detector;
  m_RawTowerGeomContainer = findNode::getClass<RawTowerGeomContainer>(topNode, m_TowerGeomNodeName);
  if (!m_RawTowerGeomContainer)
  {
    const RawTowerDefs::CalorimeterId caloid = RawTowerDefs::convert_name_to_caloid(m_Detector);
    m_RawTowerGeomContainer = new RawTowerGeomContainer_Cylinderv1(caloid);
    // add it to the node tree
    PHIODataNode<PHObject> *newNode = new PHIODataNode<PHObject>(m_RawTowerGeomContainer, m_TowerGeomNodeName, "PHObject");
    dst_node->addNode(newNode);

    // Fill in the tower geometry info here
    for (int ieta=0; ieta<m_RawTowerGeomContainer->get_etabins(); ieta++)
    {
      for (int iphi=0; iphi<m_RawTowerGeomContainer->get_phibins(); iphi++)
      {
        // build tower geom here
	const RawTowerDefs::keytype key =
            RawTowerDefs::encode_towerid(caloid, ieta, iphi);

        const double x(r * cos(m_RawTowerGeomContainer->get_phicenter(iphi)));
        const double y(r * sin(m_RawTowerGeomContainer->get_phicenter(iphi)));
        const double z(r / tan(PHG4Utils::get_theta(m_RawTowerGeomContainer->get_etacenter(ieta))));

        RawTowerGeom *tg = m_RawTowerGeomContainer->get_tower_geometry(key);
        if (tg)
        {
          if (Verbosity() > 0)
          {
            std::cout << "RawTowerBuilder::CreateNodes - Tower geometry " << key << " already exists" << std::endl;
          }

          if (fabs(tg->get_center_x() - x) > 1e-4)
          {
            std::cout << "RawTowerBuilder::CreateNodes - Fatal Error - duplicated Tower geometry " << key << " with existing x = " << tg->get_center_x() << " and expected x = " << x
                      << std::endl;

            exit(1);
          }
          if (fabs(tg->get_center_y() - y) > 1e-4)
          {
            std::cout << "RawTowerBuilder::CreateNodes - Fatal Error - duplicated Tower geometry " << key << " with existing y = " << tg->get_center_y() << " and expected y = " << y
                      << std::endl;
            exit(1);
          }
          if (fabs(tg->get_center_z() - z) > 1e-4)
          {
            std::cout << "RawTowerBuilder::CreateNodes - Fatal Error - duplicated Tower geometry " << key << " with existing z= " << tg->get_center_z() << " and expected z = " << z
                      << std::endl;
            exit(1);
          }
        }
        else
        {
          if (Verbosity() > 0)
          {
            std::cout << "RawTowerBuilder::CreateNodes - building tower geometry " << key << "" << std::endl;
          }

          tg = new RawTowerGeomv1(key);

          tg->set_center_x(x);
          tg->set_center_y(y);
          tg->set_center_z(z);
          m_RawTowerGeomContainer->add_tower_geometry(tg);
	}
      }
    }  // end loop over eta, phi bins
  }  // end of building RawTowerGeomContainer

}
