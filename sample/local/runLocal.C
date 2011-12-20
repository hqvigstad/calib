void runLocal()
{
  gSystem->Load("libTree.so");
  gSystem->Load("libGeom.so");
  gSystem->Load("libVMC.so");
  gSystem->Load("libSTEERBase.so");
  gSystem->Load("libESD.so");
  gSystem->Load("libAOD.so"); 
  
  // load analysis framework
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");

  // load PHOS libs.
  gSystem->Load("libSTRUCT");
  gSystem->Load("libPHOSUtils");
  gSystem->Load("libPHOSbase");
  gSystem->Load("libPHOSsim");
  gSystem->Load("libPHOSrec");
  
  gROOT->LoadMacro("$ALICE_ROOT/PWG0/CreateESDChain.C");
  TChain* chain = CreateESDChain("files.txt", 3);

  // for includes use either global setting in $HOME/.rootrc
  // ACLiC.IncludePaths: -I$(ALICE_ROOT)/include
  // or in each macro
  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_ROOT/PHOS");

  // Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("mgrAnalysis");

  //AliCDBManager::Instance()->SetDefaultStorage("local://$ALICE_ROOT/OCDB");
  AliCDBManager::Instance()->SetDefaultStorage("raw://");
  AliCDBManager::Instance()->SetRun(119161);


  AliESDInputHandler* esdH = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdH);
  
  // Create task
  gROOT->LoadMacro("../Sample.cxx+g");
  gROOT->LoadMacro("../SampleCandidate.cxx+g");
  gROOT->LoadMacro("../SampleParameters.cxx+g");
  gROOT->LoadMacro("../ExtractorTask.cxx+g");
  ExtractorTask *task = new ExtractorTask("extractorTask");

  // Add task(s)
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer("parameters", SampleParameters::Class(), AliAnalysisManager::kOutputContainer, "samples.root");
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer("sampleTree", TTree::Class(), AliAnalysisManager::kOutputContainer, "samples.root");
  AliAnalysisDataContainer *coutput3 = mgr->CreateContainer("histList", TList::Class(), AliAnalysisManager::kOutputContainer, "samples.root");

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput1);
  mgr->ConnectOutput(task, 2, coutput2);
  mgr->ConnectOutput(task, 3, coutput3);


  // Enable debug printouts
  mgr->SetDebugLevel(2);

  if (!mgr->InitAnalysis()) return;
  mgr->PrintStatus();
  mgr->StartAnalysis("local", chain);
}
