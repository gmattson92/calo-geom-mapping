#include "CaloGeomMapping.h"

#include <fun4all/Fun4AllReturnCodes.h>

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <phool/PHNode.h>
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>
#include <phool/getClass.h>
#include <phool/phool.h>  // for PHWHERE

#include <calobase/RawTowerDefs.h>           // for encode_towerid
#include <calobase/RawTowerGeom.h>           // for RawTowerGeom
#include <calobase/RawTowerGeomContainer.h>  // for RawTowerGeomC...
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/RawTowerGeomv1.h>

#include <g4detectors/PHG4CylinderCellGeom.h>
#include <g4detectors/PHG4CylinderCellGeomContainer.h>

#include <g4main/PHG4Utils.h>

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

  std::string geonodename = "CYLINDERCELLGEOM_" + m_Detector;
  PHG4CylinderCellGeomContainer *cellgeos = findNode::getClass<PHG4CylinderCellGeomContainer>(topNode, geonodename);
  if (!cellgeos)
  {
    std::cout << PHWHERE << " " << geonodename
              << " Node missing, doing nothing." << std::endl;
    throw std::runtime_error(
        "Failed to find " + geonodename + " node in RawTowerBuilder::CreateNodes");
  }
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
  PHNodeIterator iter(topNode);
  PHCompositeNode *runNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "RUN"));
  if (!runNode)
  {
      std::cout << PHWHERE << "Run Node missing, doing nothing." << std::endl;
      throw std::runtime_error("Failed to find Run node in CaloGeomMapping::CreateGeomNode");
  }

  PHNodeIterator runIter(runNode);
  PHCompositeNode *RunDetNode = dynamic_cast<PHCompositeNode *>(runIter.findFirst("PHCompositeNode", m_Detector));
  if (!RunDetNode)
  {
      RunDetNode = new PHCompositeNode(m_Detector);
      runNode->addNode(RunDetNode);
  }

  std::string geonodename = "CYLINDERCELLGEOM_" + m_Detector;
  PHG4CylinderCellGeomContainer *cellgeos = findNode::getClass<PHG4CylinderCellGeomContainer>(topNode, geonodename);
  if (!cellgeos)
  {
    std::cout << PHWHERE << " " << geonodename
              << " Node missing, doing nothing." << std::endl;
    throw std::runtime_error(
        "Failed to find " + geonodename + " node in RawTowerBuilder::CreateNodes");
  }

  const RawTowerDefs::CalorimeterId caloid = RawTowerDefs::convert_name_to_caloid(m_Detector);
  m_TowerGeomNodeName = "TOWERGEOM_" + m_Detector;
  m_RawTowerGeomContainer = findNode::getClass<RawTowerGeomContainer>(topNode, m_TowerGeomNodeName);
  if (!m_RawTowerGeomContainer)
  {
    m_RawTowerGeomContainer = new RawTowerGeomContainer_Cylinderv1(caloid);
    // add it to the node tree
    PHIODataNode<PHObject> *newNode = new PHIODataNode<PHObject>(m_RawTowerGeomContainer, m_TowerGeomNodeName, "PHObject");
    RunDetNode->addNode(newNode);
  }

  // Set the number of eta and phi bins
  m_RawTowerGeomContainer->set_radius(NAN);
  m_RawTowerGeomContainer->set_thickness(NAN);
  m_RawTowerGeomContainer->set_phibins(256); // EMCal
  //  m_RawTowerGeomContainer->set_phistep(m_PhiStep);
  //  m_RawTowerGeomContainer->set_phimin(m_PhiMin);
  m_RawTowerGeomContainer->set_etabins(96); // EMCal

  // Set the eta and phi bounds of each bin
  for (int ibin = 0; ibin < m_RawTowerGeomContainer->get_phibins(); ibin++)
  {
    /* const std::pair<double, double> range = first_cellgeo->get_phibounds(ibin); */
    const std::pair<double, double> range(0.0, 0.0);
    m_RawTowerGeomContainer->set_phibounds(ibin, range);
  }
  for (int ibin = 0; ibin < m_RawTowerGeomContainer->get_etabins(); ibin++)
  {
    /* const std::pair<double, double> range = first_cellgeo->get_etabounds(ibin); */
    const std::pair<double, double> range(0.0, 0.0);
    m_RawTowerGeomContainer->set_etabounds(ibin, range);
  }

  // Populate container with RawTowerGeom objects
  for (int ieta=0; ieta<m_RawTowerGeomContainer->get_etabins(); ieta++)
  {
    for (int iphi=0; iphi<m_RawTowerGeomContainer->get_phibins(); iphi++)
    {
      // build tower geom here
      const RawTowerDefs::keytype key =
	RawTowerDefs::encode_towerid(caloid, ieta, iphi);

      /* double r = 97.5; // need to set this to calo radius */
      /* const double x(r * cos(m_RawTowerGeomContainer->get_phicenter(iphi))); */
      /* const double y(r * sin(m_RawTowerGeomContainer->get_phicenter(iphi))); */
      /* const double z(r / tan(PHG4Utils::get_theta(m_RawTowerGeomContainer->get_etacenter(ieta)))); */
      const double x(0);
      const double y(0);
      const double z(0);

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

