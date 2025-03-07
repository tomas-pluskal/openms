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
// $Maintainer: Hendrik Weisser $
// $Authors: Andreas Bertsch, Marc Sturm, Sven Nahnsen, Hendrik Weisser $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_ID_CONSENSUSIDALGORITHMWORST_H
#define OPENMS_ANALYSIS_ID_CONSENSUSIDALGORITHMWORST_H

#include <OpenMS/ANALYSIS/ID/ConsensusIDAlgorithmIdentity.h>

namespace OpenMS
{
  /**
    @brief Calculates a consensus from multiple ID runs by taking the worst search score (conservative approach).

    @htmlinclude OpenMS_ConsensusIDAlgorithmWorst.parameters
    
    @ingroup Analysis_ID
  */
  class OPENMS_DLLAPI ConsensusIDAlgorithmWorst :
    public ConsensusIDAlgorithmIdentity
  {
  public:
    /// Default constructor
    ConsensusIDAlgorithmWorst();

  private:
    /// Not implemented
    ConsensusIDAlgorithmWorst(const ConsensusIDAlgorithmWorst&);

    /// Not implemented
    ConsensusIDAlgorithmWorst& operator=(const ConsensusIDAlgorithmWorst&);

    /// Aggregate peptide scores into one final score (by taking the worst score)
    virtual double getAggregateScore_(std::vector<double>& scores,
                                      bool higher_better);
  };

} // namespace OpenMS

#endif // OPENMS_ANALYSIS_ID_CONSENSUSIDALGORITHMWORST_H
