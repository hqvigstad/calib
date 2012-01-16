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

#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <TObject.h>
#include <Sample.h>
#include <SampleParameters.h>
#include <vector>


class Calibrator : public TObject
{
public:
  Calibrator();
  Calibrator ( const TObject& object );
  virtual ~Calibrator();
  const Calibrator& operator= ( const TObject& object );

  static Double_t M(Sample s, SampleParameters p);
  static std::vector<Double_t> M_p(Sample s, SampleParameters p);
  

  virtual SampleParameters Calibrate(std::vector<Sample> & samples, SampleParameters& initalParams) = 0;

};

#endif // CALIBRATOR_H
