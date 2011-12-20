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

#include "Sample.h"
#include "TArrayF.h"

ClassImp(Sample)

Sample::Sample ( UInt_t N )
 : TObject(),
   fN(N),
   fMass(0),
   fVertex(0,0,0),
   fEnergy1(0),
   fPosition1(0,0,0),
   fAmplitudes1(N),
   fEnergy2(0),
   fPosition2(0,0,0),
   fAmplitudes2(N)
{
}

Sample::~Sample()
{
}

