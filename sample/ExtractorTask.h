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
#include "Sample.h"
#include "TRefArray.h"

#include <vector>
#include <TH1F.h>
#include <AliESDCaloCluster.h>
#include <TRef.h>
#include "SampleCandidate.h"

class AliESDVertex;
class TH2I;

class AliESDEvent;
class TList;


class ExtractorTask : public AliAnalysisTaskSE
{
public:
    ExtractorTask(const char* name = "ExtractorTask");
    virtual ~ExtractorTask();
    
    virtual void UserCreateOutputObjects();
    virtual Bool_t UserNotify();
    virtual void UserExec(Option_t * );
    virtual void Terminate(Option_t * );    

    static std::vector<AliESDCaloCluster*> SelectClusters(const TRefArray& clusters);
    static std::vector<SampleCandidate> ExtractCandidates(const std::vector<AliESDCaloCluster*>& selectedClusters,
							  AliESDVertex* vtx );
    static std::vector<SampleCandidate> SelectCandidates(const std::vector<SampleCandidate>& candidates);
				       
    
    
private:
    ExtractorTask(const ExtractorTask & );// Not Implemented
    ExtractorTask& operator= (const ExtractorTask & );// Not Implemented

    // constants
    const static UInt_t kNMod = 5;
    const static UInt_t kNRowX = 64;
    const static UInt_t kNColZ = 56;

    
    void SetParameters(SampleParameters* params);

    // member variables
    SampleParameters* fParameters;
    TTree* fSampleTree;
    Sample* fSample;
    TList* fHistList;

    TH2I* fCandidatePtMass;
    
    ClassDef(ExtractorTask, 1);
};

#endif // EXTRACTOR_H
