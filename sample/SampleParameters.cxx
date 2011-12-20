/*
    Library for the Outlier Robust Lest Squares based estimation/calibration
    of PHOS calibration coefficiants for the ALICE Experiment.

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

#include "assert.h"
#include "SampleParameters.h"
#include "TArrayI.h"
#include "TH2.h"
#include "AliPHOSGeometry.h"
#include <TString.h>
#include <TMath.h>
#include <bits/mathdef.h>




ClassImp(SampleParameters)


SampleParameters::SampleParameters(UInt_t nGood)
: fNGood(nGood),
  fIDArray(nGood),
  fCCArray(nGood),
  fCS(0),
  fLocalPosArray(nGood),
  fLogWeight(4.5), // from trunk rev.53223, AliPHOSRecoParam::AliPHOSRecoParam
  fNonLinearParams(0),
  fNonLinearCorrectionVersion(""),
  fT(5),
  fIncidentVector({0,0,0}), // from trunk rev.53223, AliPHOSClusterizerv1::WriteRecPoints
  fParA(0.925), // from trunk rev.53223, AliPHOSEmcRecPoint::EvalLocalPosition
  fParB(6.25)   // from trunk rev.53223, AliPHOSEmcRecPoint::EvalLocalPosition
{
  fLocalPosArray.SetOwner();
  fT.SetOwner();
}


// SampleParameters::SampleParameters(const SampleParameters& other)
// : fNGood(other.fNGood),
//   fIDArray(other.fIDArray),
//   fCCArray(other.fCCArray),
//   fCS(other.fCS),
//   fLocalPosArray((TObjArray*) other.fLocalPosArray->Clone()),
//   fLogWeight(other.fLogWeight), 
//   fNonLinearParams(other.fNonLinearParams),
//   fNonLinearCorrectionVersion(other.fNonLinearCorrectionVersion),
//   fT({NULL}),
//   fIncidentVector(other.fIncidentVector), // from trunk rev.53223, AliPHOSClusterizerv1::WriteRecPoints
//   fParA(other.fParA), 
//   fParB(other.fParB)   
// {
//   for(int mod = 0; mod <5; ++mod)
//     if( other.fT[mod] )
//       fT[mod] = new TGeoHMatrix( * other.fT[mod]);
// }
// 
// 
// SampleParameters& SampleParameters::operator= (const SampleParameters& other )
// {
//  SetNGood( other.fNGood );
//  fIDArray = other.fIDArray;
//  fCCArray = other.fCCArray;
//  fCS = other.fCS;
//  for(unsigned int idx = 0; idx < fNGood; fNGood++){
//    if( fLocalPosArray->operator[](idx) )
//      fLocalPosArray->RemoveAt(idx);
//    if( other.fLocalPosArray->operator[](idx) )
//    fLocalPosArray->Add(new TVector3( * (TVector3*) other.fLocalPosArray->operator[](idx)));
//  }
//  
//  return *this;
// }


SampleParameters::~SampleParameters()
{
}


TCanvas* SampleParameters::DrawBadChannelMap()
{
  TH2I* hists[5] = {0};
  for(unsigned int idx = 0; idx < fNGood; ++fNGood) {
    const Int_t absID = GetIDArray()[idx];
    Int_t realpos[3];
    AliPHOSGeometry::GetInstance()->AbsToRelNumbering(absID, realpos);
    const int mod = realpos[0];
    const int x = realpos[2];
    const int z = realpos[3];
    
    if( ! hists[mod] ) { // Create Histograms
      char name[256];
      char title[256];
      sprintf(name, "badmap_%d", mod);
      sprintf(title, "Bad channel map, mod:%d (bad=0 & good=1)", mod);
      hists[mod] = new TH2I(name, title, 64, 0, 64, 56, 0, 56);
    }
    
    hists[mod]->Fill(x,z);
  }
  
  TCanvas* canv = new TCanvas;
  int nmod = 0;
  for(int mod=0; mod<5; ++mod) 
    if( hists[mod] )
      ++nmod;
  canv->Divide(nmod, 1);
  nmod = 0;
  for(int mod=0; mod<5; ++mod) 
    if( hists[mod] ){
      canv->cd(++nmod);
      hists[mod]->Draw("colz");
    }
 
 return canv; 
}

bool SampleParameters::Equal ( const SampleParameters& other)
{
  if( fNGood != other.fNGood )
    return false;
  for(unsigned int idx=0; idx<fNGood; ++fNGood)
  {
    if(fIDArray[idx] != other.fIDArray[idx] )
      return false;
    if(fCCArray[idx] != other.fCCArray[idx]) // should be machine exact equal
      return false;
    //TODO: complare local array
  }
  //TODO; finish SampleParameters::Equal
 
  return true;
}



void SampleParameters::Print ( Option_t* /*option*/ ) const
{
  // TString opt = option;
  
  printf("SampleParameters.Print, ");
  printf("   NGood: %d\n", fNGood);   
  // TODO: add more.
}


const Int_t SampleParameters::FindIndex ( const UInt_t phosID ) const
{
  // returns the index of phosID, if found!
  // if not found, returns -1.
  // cost: linear (with N, number of entries).
  
  for(int idx = 0; idx < fIDArray.GetSize(); ++idx)
    if( phosID == (UInt_t) fIDArray[idx] ) // IDs in array should be u.s.
      return idx;
  return -1;
}


const TVector3* SampleParameters::GetLocalPos ( UInt_t index ) const
{
  return dynamic_cast<TVector3*>(fLocalPosArray.At(index));
}


const TGeoHMatrix* SampleParameters::GetT ( UInt_t module ) const
{
  // gets Transformation Matrix (PHOS Local -> Global ) for module
  
  if( module < 5 )
    return dynamic_cast<TGeoHMatrix*>( fT.At(module));
  else {
    Error("GetT", "module variable must be in range [0,4]");
    return NULL;
  }
}



void SampleParameters::SetNGood ( UInt_t newNGood )
{
  if( newNGood == fNGood )
    return;
  
  fIDArray.Set( newNGood );
  fCCArray.Set( newNGood );

  if( newNGood < fNGood )
    while( newNGood < (UInt_t) fLocalPosArray.GetEntriesFast())
      fLocalPosArray.RemoveLast();
  fLocalPosArray.Expand(newNGood);
  
  fNGood = newNGood;
}


void SampleParameters::SetID ( UInt_t index, Int_t phosID )
{
  if( index < fNGood )
    fIDArray[index] = phosID;
  else
    Error("SetID","index out of bounds");
}


void SampleParameters::SetCC ( UInt_t index, Int_t cc )
{
  if( index < fNGood )
    fCCArray[index] = cc;
  else
    Error("SetCC", "index out of bounds");
}


void SampleParameters::SetLocalPos ( UInt_t index, const TVector3& localPos )
{
  if( fNGood <= index )
    Error("SetLocalPos", "index out of bounds");
  
  delete fLocalPosArray.At(index); // becouse AddAt does not clean, TODO: verify!
  fLocalPosArray.AddAt(new TVector3(localPos), index);
  fLocalPosArray.SetOwner(); // for peace of mind
}

void SampleParameters::SetT ( UInt_t module, const TGeoHMatrix& T )
{
  if( 5 <= module );
    Error("SetT", "module out of bounds");

  delete fT[module]; // becouse AddAt does not clean, TODO: verify!
  fT.AddAt(new TGeoHMatrix(T), module);
  fT.SetOwner(); // for peace of mind
}


