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

#ifndef SAMPLECANDIDATE_H
#define SAMPLECANDIDATE_H

#include "TObject.h"
#include <AliESD.h>

class AliESDVertex;
class AliVertex;
class TLorentzVector;
class AliESDCaloCluster;

class SampleCandidate : TObject {
public:
  SampleCandidate();
  SampleCandidate (AliESDCaloCluster* clu1, AliESDCaloCluster* clu2, AliESDVertex* vtx);
  SampleCandidate(const SampleCandidate & other);
  SampleCandidate & operator= (const SampleCandidate& other); 
  ~SampleCandidate();

  const TLorentzVector& GetMomentum() const { return fMoment; }
  AliESDCaloCluster* GetCluster1() const { return fClu1; }
  AliESDCaloCluster* GetCluster2() const { return fClu2; }
  AliESDVertex* GetVertex() const { return fVertex; }
  

private:
  TLorentzVector fMoment;
  AliESDCaloCluster* fClu1;
  AliESDCaloCluster* fClu2;
  AliESDVertex* fVertex;
};

#endif // SAMPLECANDIDATE_H
