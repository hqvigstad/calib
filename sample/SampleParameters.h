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

#ifndef SAMPLEPARAMETERS_H
#define SAMPLEPARAMETERS_H

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayF.h>
#include <TCanvas.h>
#include <TObjArray.h>
#include <TVector3.h>
#include <TGeoMatrix.h>

class SampleParameters : public TObject
{
public:
  SampleParameters(UInt_t nGood = 0);
  SampleParameters(const SampleParameters& other );
  SampleParameters& operator= (const SampleParameters& other );
  ~SampleParameters();
  
  TCanvas* DrawBadChannelMap();

  bool Equal(SampleParameters* other);
  
  void Print(Option_t* option = "") const;
  

  // *** Getters ***
  const Int_t GetNGood() const { return fNGood; }
  const TArrayI& GetIDArray() const { return fIDArray; }

  const TArrayF& GetCCArray() const { return fCCArray; }
  const Float_t GetCS() const { return fCS; }
  const TVector3* GetLocalPos(UInt_t index) const;
  const Float_t GetLogWeight() const { return fLogWeight; }
  const TArrayF& GetNonLinearParams() const { return fNonLinearParams; }
  const TString& GetNonLinearCorrectionVersion() const { return fNonLinearCorrectionVersion; }
  const TGeoHMatrix* GetT(UInt_t module) const;
  const TVector3& GetIncidentVector() const { return fIncidentVector; }
  const Float_t GetPara() const {return fParA; }
  const Float_t GetParb() const {return fParB; }
    
  
  // *** Setters ***
  void SetNGood(UInt_t nGood);
  void SetID(UInt_t index, Int_t phosID);
  
  void SetCC(UInt_t index, Int_t cc);
  void SetCS(Int_t cs) { fCS = cs; }
  void SetLocalPos(UInt_t index, const TVector3& localPos);
  void SetLogWeight(Float_t logWeight) { fLogWeight = logWeight; }
  void SetNonLinearParams(const TArrayF& paramArray) {fNonLinearParams = paramArray; }
  void SetNonLinearCorrectionVersion(const TString& name) { fNonLinearCorrectionVersion = name; }
  void SetT(UInt_t module, const TGeoHMatrix& T);
  void SetIncidentVector(const TVector3& incVector) { fIncidentVector = incVector; }
  void SetParA(Float_t para) { fParA = para; }
  void SetParB(Float_t parb) { fParB = parb; }
  
  
private:
  UInt_t fNGood;
  TArrayI fIDArray; // [fNGood] PHOS ID of Good Channels
    
  TArrayF fCCArray; // [fNGood] 
  Float_t fCS; // AliPHOSGeometry::fCrystalShift, Distance from crystal center to front surface
  TObjArray* fLocalPosArray; // [fNGood] Position of Cell
  Float_t fLogWeight; // AliPHOSClusterizerv1::fW0, recoParam->GetEMCLogWeight()
  TArrayF fNonLinearParams; // [6] if fNonLinearCorrectionVersion=="Henrik2010"
  TString fNonLinearCorrectionVersion; // "Henrik2010", or other
  TGeoHMatrix* fT[5]; // [nModules] Transformation Matrixes (g=T*l, were l is local fram and g is global)
  TVector3 fIncidentVector;
  Float_t fParA; // depth correction AliPHOSEmcRecPoint::EvalLocalPosition::para, 0.925
  Float_t fParB;  // depth correction AliPHOSEmcRecPoint::EvalLocalPosition::parb, 0.925
  
  //TObjArray* fPositions;
  
  ClassDef(SampleParameters, 1);
};

#endif // SAMPLEPARAMETERS_H
