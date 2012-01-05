/*
    Library for the Outlier Robust Lest Squares based estimation/calibration
    of PHOS calibration coefficiants.

    Copyright (C) 2011  Henrik Qvigstad <henrik.qvigstad@cern.com>,
		       Joseph Young <>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "ExtractorTask.h"

ClassImp(ExtractorTask)

#include <AliPHOSCalibData.h>
#include "AliPHOSGeometry.h"
#include "AliCDBManager.h"
#include "AliCDBEntry.h"
#include "AliCDBPath.h"
#include "AliESDVertex.h"
#include "AliPHOSRecoParam.h"
#include "AliPHOS.h"
#include "AliESDEvent.h"

#include "TTree.h"
#include "TClass.h"
#include "TGeoManager.h"
#include "TList.h"
#include "SampleCandidate.h"
#include <TH2I.h>



ExtractorTask::ExtractorTask(const char* name)
: AliAnalysisTaskSE(name),
  fParameters(NULL),
  fSampleTree(NULL),
  fSample(NULL),
  fHistList(NULL),
  fCandidatePtMass(NULL),
  fSelectedPtMass(NULL)
{
  DefineOutput(1, SampleParameters::Class());
  DefineOutput(2, TTree::Class());
  DefineOutput(3, TList::Class());
}


ExtractorTask::~ExtractorTask()
{
  delete fSampleTree;
  delete fSample;
  delete fParameters;
  delete fHistList;
}


void ExtractorTask::UserCreateOutputObjects()
{
  // input
  fBranchNames = "ESD:AliESDRun.,AliESDHeader.,Tracks AOD:header,tracks";

  
  // *** Initiallize parameters ***
//   fParameters = new SampleParameters();
//   AliESDEvent* esdEvent = dynamic_cast<AliESDEvent*>(InputEvent());
//   if( ! esdEvent )
//     Error("UserExec", "InputEvent did not cast to esd");
//   SetParameters(*fParameters, *esdEvent);
//   Int_t nGoodCells = fParameters->GetNGood();


  
  // *** Initiallize Sample Tree """
  //fSample = new Sample(nGoodCells);
  fSampleTree = new TTree("fSampleTree", "Sample Tree");
  fSampleTree->Branch("samples", "Sample", &fSample);

  // *** Histograms ***
  fHistList = new TList;
  fHistList->SetOwner(); //TODO: check if list of hists should be owner.

  fCandidatePtMass = new TH2I("fCandidatePtMass", "fCandidatePtMass", 100, 0, 10,   100, 0, 0.5);
  fHistList->Add(fCandidatePtMass);
  fSelectedPtMass = new TH2I("fSelectedPtMass", "fSelectedPtMass", 100, 0, 10,   100, 0, 0.5);
  fHistList->Add(fSelectedPtMass);

  PostData(1, fParameters);
  PostData(2, fSampleTree);
  PostData(3, fHistList);
}


Bool_t ExtractorTask::UserNotify()
{
  return AliAnalysisTaskSE::UserNotify();
}

void ExtractorTask::UserExec ( Option_t* )
{
  
  AliESDEvent* esdEvent = dynamic_cast<AliESDEvent*>(InputEvent());
  if( ! esdEvent )
    Error("UserExec", "InputEvent did not cast to esd");


  // *** Initiallize parameters ***
  if( fParameters ) {
    fParameters = new SampleParameters();
    SetParameters(*fParameters, *esdEvent);
    Int_t nGoodCells = fParameters->GetNGood();
    fSample = new Sample(nGoodCells);
  }


  AliESDVertex* vertex = (AliESDVertex*) esdEvent->GetPrimaryVertex();
  AliESDCaloCells* phosCells = esdEvent->GetPHOSCells();
    
  // Get Clusters
  TRefArray* phosClusterArray = new TRefArray( esdEvent->GetNumberOfCaloClusters() /4 );
  esdEvent->GetPHOSClusters(phosClusterArray);
  std::vector<AliESDCaloCluster*> selectedClusters = SelectClusters(*phosClusterArray);
  std::vector<SampleCandidate> candidates = ExtractCandidates(selectedClusters, vertex);
  std::vector<SampleCandidate> selected = SelectCandidates(candidates);


  // Fill Tree
  for(UInt_t idx = 0; idx < selected.size(); ++idx) {
    CandidateToSample(*fSample, selected[idx], *phosCells, *vertex, *fParameters);
    fSampleTree->Fill(); // fill fSample to sampletree
  }

  PostData(1, fParameters);
  PostData(2, fSampleTree);
  PostData(3, fHistList);
}


void ExtractorTask::Terminate ( Option_t* )
{
  new TCanvas;
  fCandidatePtMass->DrawCopy();
  new TCanvas;
  fSelectedPtMass->DrawCopy();
  return; //TODO: write ExtractorTask::Terminate
}



std::vector< AliESDCaloCluster* > ExtractorTask::SelectClusters ( const TRefArray& clusters )
{
  std::vector<AliESDCaloCluster*> selected;
  for(int idx = 0; idx < clusters.GetEntriesFast(); ++idx){
    AliESDCaloCluster* cluster = dynamic_cast<AliESDCaloCluster*> (clusters.At(idx));
    if(! cluster) {
      Printf(" ERROR: ExtractorTask::SelectClusters, clusters argument in function had entry which did not cast to AliESDCaloCluster");
      continue;
    }
    if( cluster->E() < 0.5 )
      continue;
    if( cluster->GetNCells() < 5 )
      continue;
    if( cluster->GetNExMax() > 1 )
      continue;

    selected.push_back(cluster);
  }
  return selected;
}


std::vector< SampleCandidate > ExtractorTask::ExtractCandidates
( const std::vector< AliESDCaloCluster* >& clusters, AliESDVertex* vtx )
{
  std::vector< SampleCandidate > candidates;
  for(unsigned int idx1 = 0; idx1 < clusters.size(); ++idx1) {
    for(unsigned int idx2 = idx1; idx2 < clusters.size(); ++idx2) {
      SampleCandidate candidate = SampleCandidate(clusters[idx1], clusters[idx2], vtx);
      candidates.push_back(candidate);
    }
  }
  return candidates;
}


std::vector< SampleCandidate > ExtractorTask::SelectCandidates ( const std::vector< SampleCandidate >& candidates )
{
  std::vector< SampleCandidate > selected;
  for(unsigned int idx = 0; idx < candidates.size(); ++idx) {
    selected.push_back(candidates[idx]);
  }
  return selected;
}


void ExtractorTask::CandidateToSample ( Sample& toSample, const SampleCandidate& candidate, const AliESDCaloCells& phosCells, const AliESDVertex& vertex, const SampleParameters& params)
{
  // Set Vertex
  Double_t vtxarr[3];
  vertex.GetXYZ(vtxarr);
  const TVector3 vtxvec(vtxarr[0], vtxarr[1], vtxarr[2]);
  toSample.SetVertex(vtxvec);


  // Set Cluster 1 paramters
  toSample.SetEnergy1(candidate.GetMomentum().E());
  toSample.SetMass(candidate.GetMomentum().M());

  const AliESDCaloCluster* cluster1 = candidate.GetCluster1();
  const Float_t pos1[3] = {0};
  candidate.GetCluster1()->GetPosition((Float_t*) pos1);;
  const TVector3 posv1((Float_t*) pos1);
  toSample.SetPostion1( posv1 );

  toSample.Amplitude1().Reset();
  const UInt_t nCells1 = cluster1->GetNCells();
  for (unsigned int idx=0; idx<nCells1; ++idx) {
    const UInt_t phosID = cluster1->GetCellAbsId(idx);
    const Double32_t fraction = cluster1->GetCellAmplitudeFraction(idx);
    const Double32_t amplitude =  phosCells.GetAmplitude(phosID) * fraction;
    const UInt_t index = params.FindIndex(phosID); // index of sample coordinate system.
    toSample.Amplitude1()[index] = amplitude;
  }


  // Set Cluster 2 paramters
  toSample.SetEnergy2(candidate.GetMomentum().E());

  const AliESDCaloCluster* cluster2 = candidate.GetCluster2();
  const Float_t pos2[3] = {0};
  candidate.GetCluster2()->GetPosition((Float_t*) pos2);
  const TVector3 posv2((Float_t*) pos2);
  toSample.SetPostion2( posv2 );

  toSample.Amplitude2().Reset();
  const UInt_t nCells2 = cluster2->GetNCells();
  for (unsigned int idx=0; idx<nCells2; ++idx) {
    const UInt_t phosID = cluster2->GetCellAbsId(idx);
    const Double32_t fraction = cluster2->GetCellAmplitudeFraction(idx);
    const Double32_t amplitude =  phosCells.GetAmplitude(phosID) * fraction;
    const UInt_t index = params.FindIndex(phosID); // index of sample coordinate system.
    toSample.Amplitude2()[index] = amplitude;
  }
}


void ExtractorTask::SetParameters ( SampleParameters& params, const AliESDEvent& esdEvent )
{

  AliCDBEntry* entryEmc = AliCDBManager::Instance()->Get("PHOS/Calib/RecoParam");
  if( ! entryEmc )
    Error("SetParameters", "ocdb entry (PHOS/Calib/RecoParam) not found");
  const TObjArray* recoArray = dynamic_cast<TObjArray*> (entryEmc->GetObject());
  if( ! recoArray ) 
    Error("SetParameters", "OCDB Entry not instance of ObjArray");
  const AliPHOSRecoParam* recoParam = dynamic_cast<AliPHOSRecoParam*> (recoArray->At(1)); // "LowMult2009"
  if( ! recoParam )
    Error("SetParameters","recoParam not found");

  AliPHOSGeometry* geometry = AliPHOSGeometry::GetInstance("IHEP");
  const AliPHOSEMCAGeometry * emcGeometry = geometry->GetEMCAGeometry();
  
  const AliPHOSCalibData* calibData = new AliPHOSCalibData();
  const Int_t nGood = kNMod*kNRowX*kNColZ - calibData->GetNumOfEmcBadChannels();
  
  // Number of Good
  params.SetNGood(nGood);
  
  Int_t nGoodFound = 0;
  for(UInt_t mod = 0; mod < kNMod; ++mod) {
    for(UInt_t xrow = 0; xrow < kNRowX; ++xrow) {
      for(UInt_t zcol = 0; zcol < kNColZ; ++zcol) {
	if( ! calibData->IsBadChannelEmc(mod, zcol, xrow) ) { // if good
	  // ID
	  Int_t tmpID = 0;
	  geometry->RelPosToAbsId(mod+1, xrow+1, zcol+1, tmpID);
	  params.SetID(nGoodFound, tmpID /*id*/);
	  // CC
	  params.SetCC(nGoodFound, calibData->GetADCchannelEmc(mod, zcol, xrow) );
	  // Local Positions (x^L))
	  Int_t relId[4];
	  Float_t x, z;
	  geometry->AbsToRelNumbering(tmpID, relId); // real numbering stored to @param relID
	  geometry->RelPosInModule(relId, x, z); // local position
	  //TVector3* localPos = new TVector3(x, 0., z);
	  params.SetLocalPos( nGoodFound, TVector3(x, 0, z) );
	  
	  ++nGoodFound;
	} // end if
      } // end col
    } // end row
  } // end mod
  if( nGood == nGoodFound )
    Error("SetParameters", "Discrepancy between number of bad, and number of good found!");


  //calculate offset to crystal surface
  // Taken from AliPHOSGeometry::Init, aliroot-v5-02-09-AN
  const Float_t * inthermo = emcGeometry->GetInnerThermoHalfSize() ;
  const Float_t * strip = emcGeometry->GetStripHalfSize() ;
  const Float_t * splate = emcGeometry->GetSupportPlateHalfSize();
  const Float_t * crystal = emcGeometry->GetCrystalHalfSize() ;
  const Float_t * pin = emcGeometry->GetAPDHalfSize() ;
  const Float_t * preamp = emcGeometry->GetPreampHalfSize() ;
  Float_t crystalShift = -inthermo[1]+strip[1]+splate[1]+crystal[1]-emcGeometry->GetAirGapLed()/2.+pin[1]+preamp[1] ;
  params.SetCS( crystalShift );

  params.SetLogWeight(recoParam->GetEMCLogWeight());

  if( recoParam->GetNonlinearityCorrectionVersion() == TString("Henrik2010") )
    params.SetNonLinearParams(TArrayF(7, recoParam->GetNonlinearityParams()));
  params.SetNonLinearCorrectionVersion(recoParam->GetNonlinearityCorrectionVersion());;
  
  for(Int_t mod = 0; mod < 5; mod++){
    //If GeoManager exists, take matrixes from it
    if(gGeoManager){
      char path[255] ;
      snprintf(path,255,"/ALIC_1/PHOS_%d/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1",mod) ;
      if (!gGeoManager->cd(path)){
	Error("SetParameters", "Geo manager can not find path");
      }
      params.SetT(mod, *gGeoManager->GetCurrentMatrix() );
    } // end if 
  } // end for mod
}
