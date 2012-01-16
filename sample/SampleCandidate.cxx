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

#include "AliESDCaloCluster.h"
#include "TLorentzVector.h"

#include "SampleCandidate.h"
#include <TVector3.h>
#include <AliESDVertex.h>

ClassImp(SampleCandidate)

SampleCandidate::SampleCandidate()
: TObject(),
  fMoment(),
  fClu1(NULL),
  fClu2(NULL),
  fVertex(NULL)
{
}

SampleCandidate::SampleCandidate ( AliESDCaloCluster* clu1, AliESDCaloCluster* clu2, AliESDVertex* vtx )
: TObject(),
  fMoment(),
  fClu1(clu1),
  fClu2(clu2),
  fVertex(vtx)
{
  if( !fClu1 || !fClu2 || !fVertex)
    Error("SampleCandidate", "some argument in SampleCandidate specific constructor is null");

  Double_t vtxarr[3];
  fVertex->GetXYZ(vtxarr);
  TLorentzVector vec2;
  fClu1->GetMomentum(fMoment, vtxarr);
  fClu2->GetMomentum(vec2, vtxarr);
  fMoment += vec2 ;
}

SampleCandidate::SampleCandidate ( const SampleCandidate& other )
: TObject ( other ),
  fMoment( other.GetMomentum() ),
  fClu1(other.fClu1),
  fClu2(other.fClu2),
  fVertex(other.fVertex)
{
}

SampleCandidate& SampleCandidate::operator= ( const SampleCandidate& other )
{
  TObject::operator=(other);

  fMoment = other.fMoment;

  fClu1 = other.fClu1;
  fClu2 = other.fClu2;
  fVertex = other.fVertex;

  return *this;
}


SampleCandidate::~SampleCandidate()
{
}



