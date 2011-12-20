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

#ifndef SAMPLE_H
#define SAMPLE_H

#include <TObject.h>
#include <TLorentzVector.h>
#include <TVector3.h>
#include <vector>
#include <TArrayF.h>


class Sample : public TObject
{

public:
  Sample(UInt_t N = 0);
  virtual ~Sample();

public:  
  const TVector3& GetVertex() const { return fVertex; }
  void SetVertex(const TVector3& vertex) {fVertex = vertex;}

  const Float_t GetMass() const { return fMass; }
  void SetMass(const Float_t mass) {fMass = mass;}


  // Cluster 1
  Float_t GetEnergy1() const { return fEnergy1; }
  void SetEnergy1(Float_t energy1) {fEnergy1 = energy1;}

  const TVector3& GetPostion1() const { return fPosition1; }
  void SetPostion1(const TVector3& postion1) {fPosition1 = postion1;}

  const TArrayF& Amplitudes1() const { return fAmplitudes1; }
  TArrayF& Amplitude1() {return fAmplitudes1; }


  // Cluster 2
  Float_t GetEnergy2() const { return fEnergy2; }
  void SetEnergy2(Float_t energy2) {fEnergy2 = energy2;}

  const TVector3& GetPostion2() const { return fPosition2; }
  void SetPostion2(const TVector3& postion2) {fPosition2 = postion2;}

  const TArrayF& Amplitudes2() const { return fAmplitudes2; }
  TArrayF& Amplitude2() {return fAmplitudes2; }


private:
  Sample(const Sample& other); // Not implemted, declared for suppression of warnings
  virtual Sample& operator=(const Sample& other); // Not implemted, declared for suppression of warnings

  UInt_t fN; // N, length of amplitudes, Immutable

  // Sample globals
  Float_t fMass;
  TVector3 fVertex;

  // Cluster 1
  Float_t fEnergy1;
  TVector3 fPosition1;
  TArrayF fAmplitudes1;

  // Cluster 2
  Float_t fEnergy2;
  TVector3 fPosition2;
  TArrayF fAmplitudes2;

  ClassDef(Sample, 1);
};



#endif // SAMPLE_H
