#pragma once

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <printcalogeom/printCaloGeom.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libprintCaloGeom.so)

void Fun4All_printCaloGeom(
                     int nEvents = 1,
                     const char *filelist1 = "dst_calo_cluster.list")
{
  // this convenience library knows all our i/o objects so you don't
  // have to figure out what is in each dst type
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);  // set it to 1 if you want event printouts

  Fun4AllInputManager *inCluster = new Fun4AllDstInputManager("DSTCaloCluster");
  std::cout << "Adding file list " << filelist1 << std::endl;
  inCluster->AddListFile(filelist1,1);
  se->registerInputManager(inCluster);

  printCaloGeom *eval = new printCaloGeom("printCaloGeom");
  eval -> setGenEvent(1);
  se -> registerSubsystem(eval);
  
  se->run(nEvents);
  se->End();
  
  delete se;
  cout << "Analysis Completed" << endl;
  
  gSystem->Exit(0);
}
