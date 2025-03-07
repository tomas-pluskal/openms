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
// $Maintainer: Clemens Groepl $
// $Authors: $
// --------------------------------------------------------------------------


#ifndef OPENMS_TRANSFORMATIONS_FEATUREFINDER_EMGMODEL_H
#define OPENMS_TRANSFORMATIONS_FEATUREFINDER_EMGMODEL_H

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/InterpolationModel.h>
#include <OpenMS/MATH/STATISTICS/BasicStatistics.h>


namespace OpenMS
{
  /**
      @brief Exponentially modified gaussian distribution model for elution profiles.

  @htmlinclude OpenMS_EmgModel.parameters
  */
  class OPENMS_DLLAPI EmgModel :
    public InterpolationModel
  {

public:
    typedef InterpolationModel::CoordinateType CoordinateType;
    typedef Math::BasicStatistics<CoordinateType> BasicStatistics;
    typedef LinearInterpolation::container_type ContainerType;

    /// Default constructor
    EmgModel();

    /// copy constructor
    EmgModel(const EmgModel & source);

    /// destructor
    virtual ~EmgModel();

    /// assignment operator
    virtual EmgModel & operator=(const EmgModel & source);

    /// create new EmgModel object (needed by Factory)
    static BaseModel<1> * create()
    {
      return new EmgModel();
    }

    /// name of the model (needed by Factory)
    static const String getProductName()
    {
      return "EmgModel";
    }

    /// set offset without being computing all over and without any discrepancy
    void setOffset(CoordinateType offset);

    /// set sample/supporting points of interpolation
    void setSamples();

    /// get the center of the Gaussian model i.e. the position of the maximum
    CoordinateType getCenter() const;

protected:
    CoordinateType  min_;
    CoordinateType  max_;
    BasicStatistics statistics_;
    CoordinateType height_;
    CoordinateType width_;
    CoordinateType symmetry_;
    CoordinateType retention_;

    void updateMembers_();
  };
}

#endif // OPENMS_TRANSFORMATIONS_FEATUREFINDER_EMGMODEL_H
