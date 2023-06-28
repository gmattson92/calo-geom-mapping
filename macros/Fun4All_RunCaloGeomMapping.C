// these include guards are not really needed, but if we ever include this
// file somewhere they would be missed and we will have to refurbish all macros
#ifndef MACRO_FUN4ALLRUNCALOGEOMMAPPING_C
#define MACRO_FUN4ALLRUNCALOGEOMMAPPING_C

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <calogeommapping/CaloGeomMapping.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libCaloGeomMapping.so)

void Fun4All_RunCaloGeomMapping(
      const int nEvents = 1,
      const string &inFile = "data_without_geom.root",
      const string &outputFile = "data_with_geom.root"
			)
{
  // this convenience library knows all our i/o objects so you don't
  // have to figure out what is in each dst type
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);  // set it to 1 if you want event printouts

  Fun4AllInputManager *inCaloDst = new Fun4AllDstInputManager("DSTCalo");
  /* std::cout << "Adding file list " << inFile << std::endl; */
  /* inCaloDst -> AddListFile(inFile,1); */
  std::cout << "Adding input file " << inFile << std::endl;
  inCaloDst -> AddFile(inFile);
  se->registerInputManager(inCaloDst);  

  CaloGeomMapping *eval = new CaloGeomMapping("dummy");//, outputFile);
  se->registerSubsystem(eval);

  /* Fun4AllOutputManager *outCaloDst = new Fun4AllOutputManager("OutDSTCalo", outputFile); */
  /* se->registerOutputManager(outCaloDst); */
  /* std::cout << "\n\nGreg info: output file will be " << outCaloDst->OutFileName() << "\n\n"; */
  
  se->run(nEvents);
  se->End();
  
  delete se;
  cout << "Analysis Completed" << endl;
  
  gSystem->Exit(0);
}

#endif  //MACRO_FUN4ALLRUNCALOGEOMMAPPING_C
