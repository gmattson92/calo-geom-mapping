#include <cdbobjects/CDBTTree.h>
#include <fstream>

R__LOAD_LIBRARY(libcdbobjects.so)

void MakeCDBTTree(const std::string &fname = "calo_geom_mapping.root")
{
  CDBTTree *cdbttree = new CDBTTree(fname);
  std::ifstream inText;
  int nbins_eta;
  int nbins_phi;

  // CEMC
  nbins_eta = 96;
  nbins_phi = 256;
  inText.open("emcal.txt");
  for (int ibin = 0; ibin < nbins_eta; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "cemc_eta_first", first);
    cdbttree->SetDoubleValue(ibin, "cemc_eta_second", second);
  }
  for (int ibin = 0; ibin < nbins_phi; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "cemc_phi_first", first);
    cdbttree->SetDoubleValue(ibin, "cemc_phi_second", second);
  }
  inText.close();

  // HCALIN
  nbins_eta = 24;
  nbins_phi = 64;
  inText.open("ihcal.txt");
  for (int ibin = 0; ibin < nbins_eta; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "hcalin_eta_first", first);
    cdbttree->SetDoubleValue(ibin, "hcalin_eta_second", second);
  }
  for (int ibin = 0; ibin < nbins_phi; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "hcalin_phi_first", first);
    cdbttree->SetDoubleValue(ibin, "hcalin_phi_second", second);
  }
  inText.close();

  // HCALOUT
  nbins_eta = 24;
  nbins_phi = 64;
  inText.open("ohcal.txt");
  for (int ibin = 0; ibin < nbins_eta; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "hcalout_eta_first", first);
    cdbttree->SetDoubleValue(ibin, "hcalout_eta_second", second);
  }
  for (int ibin = 0; ibin < nbins_phi; ibin++)
  {
    char row[256];
    inText.getline(row, 256);
    std::string rowstring = row;
    double first, second;
    size_t pos = rowstring.find_first_of(",");
    first = std::stod(rowstring.substr(0, pos));
    second = std::stod(rowstring.substr(pos+1));
    cdbttree->SetDoubleValue(ibin, "hcalout_phi_first", first);
    cdbttree->SetDoubleValue(ibin, "hcalout_phi_second", second);
  }
  inText.close();

  cdbttree->Commit();
  cdbttree->Print();
  cdbttree->WriteCDBTTree();
  delete cdbttree;
  gSystem->Exit(0);
}

void Read(const std::string &fname = "test.root")
{
  CDBTTree *cdbttree = new CDBTTree(fname);
  cdbttree->LoadCalibrations();
  cout << "Test2: " << cdbttree->GetSingleFloatValue("Test2") << endl;
  cout << "Tt2(2): " << cdbttree->GetFloatValue(2,"Tt2") << endl;
  cout << "Tt2(4): " << cdbttree->GetFloatValue(4,"Tt2") << endl;
  cout << "blar: " << cdbttree->GetIntValue(10,"blar") << endl;
  cout << "int Tt2(4): " << cdbttree->GetIntValue(4,"Tt2") << endl;
  cout << "dvar5: " << cdbttree->GetDoubleValue(23,"dvar5");
  cdbttree->Print();
  delete cdbttree;
  gSystem->Exit(0);
}
