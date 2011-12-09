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


#ifndef EXTRACTORTASK_H
#define EXTRACTORTASK_H

#include "AliAnalysisTaskSE.h"
#include "SampleParameters.h"



class ExtractorTask : public AliAnalysisTaskSE
{
public:
    ExtractorTask(const char* name = "ExtractorTask");
    virtual ~ExtractorTask();
    
    virtual void UserCreateOutputObjects();
    virtual Bool_t UserNotify();
    virtual void UserExec(Option_t * );
    virtual void Terminate(Option_t * );    

    
    struct ClusterParams {
      ClusterParams(Int_t nAmps=0) :energy(0), pos({0}), amps(0) { amps = new Float_t[nAmps]; amps = {0}; }
      ~ClusterParams() { delete amps; }
      Float_t energy;
      Float_t pos[3];
      Float_t* amps;
    private:
      ClusterParams(const ClusterParams& other); // Not implemted, declared for suppression of warnings
      ClusterParams& operator= (const ClusterParams& other); // Not implemted, declared for suppression of warnings
    };
      
    const static UInt_t kNMod = 5;
    const static UInt_t kNRowX = 64;
    const static UInt_t kNColZ = 56;
    
private:
    ExtractorTask(const ExtractorTask & );// Not Implemented
    ExtractorTask operator= (const ExtractorTask & );// Not Implemented
    
    void SetParameters(SampleParameters* params);

    // member variables
    TTree* fSampleTree;
    TLorentzVector* fMomentum;
    Double32_t fVertex[3];
    ClusterParams* fClu1;
    ClusterParams* fClu2;
    SampleParameters* fParameters;
    
    ClassDef(ExtractorTask, 1);
};

#endif // EXTRACTOR_H
