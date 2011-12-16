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

class AliVertex;
class TLorentzVector;
class AliVCluster;

class SampleCandidate : TObject {
public:
  SampleCandidate();
  SampleCandidate (AliVCluster* clu1, AliVCluster* clu2, AliVertex* vtx);
  SampleCandidate(const SampleCandidate & other);
  ~SampleCandidate();

  const TLorentzVector* GetMomentum() const { return fMoment; }
  

private:
  SampleCandidate & operator= (const SampleCandidate& other); // not implemented
  
  TLorentzVector* fMoment;
  AliVCluster* fClu1;
  AliVCluster* fClu2;
  AliVertex* fVertex;
};

#endif // SAMPLECANDIDATE_H
