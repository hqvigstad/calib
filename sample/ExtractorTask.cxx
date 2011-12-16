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
#include "AliPHOSRecoParam.h"
#include "AliPHOS.h"
#include "AliESDEvent.h"

#include "TTree.h"
#include "TClass.h"
#include "TGeoManager.h"
#include "TList.h"



ExtractorTask::ExtractorTask(const char* name)
: AliAnalysisTaskSE(name),
  fParameters(NULL),
  fSampleTree(NULL),
  fSample(NULL),
  fHistList(NULL)
{
  
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
  // *** Initiallize parameters ***
  fParameters = new SampleParameters();
  SetParameters(fParameters);
  Int_t nGoodCells = fParameters->GetNGood();

  // *** Initiallize Sample Tree """
  fSample = new Sample(nGoodCells);
  fSampleTree = new TTree("fSampleTree", "Sample Tree");
  fSampleTree->Branch("samples", "Sample", &fSample);

  // *** Histograms ***
  fHistList = new TList;

  //DefineOutput(fParameters, 1);
  //DefineOutput(fSampleTree, 2);
  //DefineInput(fHistList, 3);
}


Bool_t ExtractorTask::UserNotify()
{
  return AliAnalysisTaskSE::UserNotify();
}

void ExtractorTask::UserExec ( Option_t* )
{
  AliESDEvent* esdEvent = dynamic_cast<AliESDEvent*>(InputEvent());

  // Get Clusters
  TRefArray* clusters = new TRefArray( esdEvent->GetNumberOfCaloClusters() /4 );
  esdEvent->GetPHOSClusters(clusters);
  
  
}

void ExtractorTask::Terminate ( Option_t* )
{
  return; //TODO: write ExtractorTask::Terminate
}



void ExtractorTask::SetParameters ( SampleParameters* params )
{
  AliCDBEntry* entryEmc = AliCDBManager::Instance()->Get("PHOS/Calib/RecoParam",-1);
  if( ! entryEmc )
    Error("SetParameters", "ocdb entry (PHOS/Calib/RecoParam) not found");
  const TObjArray* recoArray = dynamic_cast<TObjArray*> (entryEmc->GetObject());
  if( ! recoArray ) 
    Error("SetParameters", "OCDB Entry not instance of ObjArray");
  const AliPHOSRecoParam* recoParam = dynamic_cast<AliPHOSRecoParam*> (recoArray->At(1)); // "LowMult2009"
  if( ! recoParam )
    Error("SetParameters","recoParam not found");

  const AliPHOSGeometry* geometry = AliPHOSGeometry::GetInstance("IHEP");
  const AliPHOSEMCAGeometry * emcGeometry = geometry->GetEMCAGeometry();
  
  const AliPHOSCalibData* calibData = new AliPHOSCalibData();
  const Int_t nGood = kNMod*kNRowX*kNColZ - calibData->GetNumOfEmcBadChannels();
  
  // Number of Good
  params->SetNGood(nGood);
  
  Int_t nGoodFound = 0;
  for(UInt_t mod = 0; mod < kNMod; ++mod) {
    for(UInt_t xrow = 0; xrow < kNRowX; ++xrow) {
      for(UInt_t zcol = 0; zcol < kNColZ; ++zcol) {
	if( ! calibData->IsBadChannelEmc(mod, zcol, xrow) ) { // if good
	  // ID
	  Int_t tmpID = 0;
	  geometry->RelPosToAbsId(mod+1, xrow+1, zcol+1, tmpID);
	  params->SetID(nGoodFound, tmpID /*id*/);
	  // CC
	  params->SetCC(nGoodFound, calibData->GetADCchannelEmc(mod, zcol, xrow) );
	  // Local Positions (x^L))
	  Int_t relId[4];
	  Float_t x, z;
	  geometry->AbsToRelNumbering(tmpID, relId); // real numbering stored to @param relID
	  geometry->RelPosInModule(relId, x, z); // local position
	  //TVector3* localPos = new TVector3(x, 0., z);
	  params->SetLocalPos( nGoodFound, TVector3(x, 0, z) );
	  
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
  params->SetCS( crystalShift );

  params->SetLogWeight(recoParam->GetEMCLogWeight());

  if( recoParam->GetNonlinearityCorrectionVersion() == TString("Henrik2010") )
    params->SetNonLinearParams(TArrayF(7, recoParam->GetNonlinearityParams()));
  params->SetNonLinearCorrectionVersion(recoParam->GetNonlinearityCorrectionVersion());;
  
  for(Int_t mod = 0; mod < 5; mod++){
    //If GeoManager exists, take matrixes from it
    if(gGeoManager){
      char path[255] ;
      snprintf(path,255,"/ALIC_1/PHOS_%d/PEMC_1/PCOL_1/PTIO_1/PCOR_1/PAGA_1/PTII_1",mod) ;
      if (!gGeoManager->cd(path)){
	Error("SetParameters", "Geo manager can not find path");
      }
      params->SetT(mod, *gGeoManager->GetCurrentMatrix() );
    } // end if 
  } // end for mod
}
