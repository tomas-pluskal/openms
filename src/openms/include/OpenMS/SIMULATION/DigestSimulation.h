// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Chris Bielow$
// $Authors: Stephan Aiche, Chris Bielow$
// --------------------------------------------------------------------------

#ifndef OPENMS_SIMULATION_DIGESTSIMULATION_H
#define OPENMS_SIMULATION_DIGESTSIMULATION_H

#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>
#include <OpenMS/SIMULATION/SimTypes.h>

namespace OpenMS
{

  /**
    @brief Simulates protein digestion

    Supports all enzymes supported by EnzymaticDigestion.h
    and additionally incorporates abundance values, which
    are distributed evenly among digestion products of each
    protein.

    @htmlinclude OpenMS_DigestSimulation.parameters

    @ingroup Simulation
  */
  class OPENMS_DLLAPI DigestSimulation :
    public DefaultParamHandler
  {

public:
    /** @name Constructors and Destructors
      */
    //@{
    /// Default constructor
    DigestSimulation();

    /// Copy constructor
    DigestSimulation(const DigestSimulation& source);

    /// Destructor
    virtual ~DigestSimulation();
    //@}

    /// Assignment operator
    DigestSimulation& operator=(const DigestSimulation& source);


    /**
        @brief Digest a set of proteins into peptides

        Digest proteins to peptides, with protein abundance distributes equally among
        created sibling peptides (this also applies for peptides with missed cleavages).
        Should a peptide be non-unique the abundances of its instances from proteins are summed up.

        @param feature_map Input FeatureMap containing the proteins that should be digested as ProteinIdentification
    **/
    void digest(SimTypes::FeatureMapSim& feature_map);

private:
    /// set defaults
    void setDefaultParams_();

  };

}

#endif
