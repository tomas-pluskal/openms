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
// $Maintainer: $
// $Authors: Hendrik Weisser $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONMODEL_H
#define OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONMODEL_H

#include <OpenMS/DATASTRUCTURES/Param.h>
#include <boost/noncopyable.hpp> // to prevent copy/assignment

namespace OpenMS
{
  /**
    @brief Base class for transformation models

    Implements the identity (no transformation). Parameters and data are ignored.

    Note that this class and its derived classes do not allow copying/assignment, due to the need for internal memory management associated with some of the transformation models.

    @ingroup MapAlignment
  */
  class OPENMS_DLLAPI TransformationModel : boost::noncopyable
  {
  public:
    /// Coordinate pair
    typedef std::pair<double, double> DataPoint;
    /// Vector of coordinate pairs
    typedef std::vector<DataPoint> DataPoints;

    /// Constructor
    TransformationModel() {}

    /// Alternative constructor (derived classes should implement this one!)
    TransformationModel(const TransformationModel::DataPoints&, const Param&);

    /// Destructor
    virtual ~TransformationModel();

    /// Evaluates the model at the given value
    virtual double evaluate(double value) const;

    /// Gets the (actual) parameters
    const Param& getParameters() const;

    /// Gets the default parameters
    static void getDefaultParameters(Param& params);

  protected:
    /// Parameters
    Param params_;
  };

} // end of namespace OpenMS

#endif // OPENMS_ANALYSIS_MAPMATCHING_TRANSFORMATIONMODEL_H
