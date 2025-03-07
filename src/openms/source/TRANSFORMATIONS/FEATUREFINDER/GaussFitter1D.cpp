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

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/GaussFitter1D.h>

#include <OpenMS/DATASTRUCTURES/ListUtils.h>
#include <OpenMS/CONCEPT/Factory.h>

#include <boost/math/special_functions/fpclassify.hpp>

namespace OpenMS
{
  GaussFitter1D::GaussFitter1D() :
    MaxLikeliFitter1D()
  {
    setName(getProductName());

    defaults_.setValue("statistics:variance", 1.0, "Variance of the model.", ListUtils::create<String>("advanced"));
    defaults_.setValue("statistics:mean", 1.0, "Mean value of the model.", ListUtils::create<String>("advanced"));
    defaultsToParam_();
  }

  GaussFitter1D::GaussFitter1D(const GaussFitter1D& source) :
    MaxLikeliFitter1D(source)
  {
    updateMembers_();
  }

  GaussFitter1D::~GaussFitter1D()
  {
  }

  GaussFitter1D& GaussFitter1D::operator=(const GaussFitter1D& source)
  {
    if (&source == this)
      return *this;

    MaxLikeliFitter1D::operator=(source);
    updateMembers_();

    return *this;
  }

  GaussFitter1D::QualityType GaussFitter1D::fit1d(const RawDataArrayType& set, InterpolationModel*& model)
  {
    // Calculate bounding box
    CoordinateType min_bb = set[0].getPos(), max_bb = set[0].getPos();
    for (UInt pos = 1; pos < set.size(); ++pos)
    {
      CoordinateType tmp = set[pos].getPos();
      if (min_bb > tmp)
        min_bb = tmp;
      if (max_bb < tmp)
        max_bb = tmp;
    }

    // Enlarge the bounding box by a few multiples of the standard deviation
    const CoordinateType stdev = sqrt(statistics_.variance()) * tolerance_stdev_box_;
    min_bb -= stdev;
    max_bb += stdev;


    // build model
    model = static_cast<InterpolationModel*>(Factory<BaseModel<1> >::create("GaussModel"));
    model->setInterpolationStep(interpolation_step_);

    Param tmp;
    tmp.setValue("bounding_box:min", min_bb);
    tmp.setValue("bounding_box:max", max_bb);
    tmp.setValue("statistics:mean", statistics_.mean());
    tmp.setValue("statistics:variance", statistics_.variance());
    model->setParameters(tmp);

    // fit offset
    QualityType quality;
    quality = fitOffset_(model, set, stdev, stdev, interpolation_step_);
    if (boost::math::isnan(quality))
      quality = -1.0;

    return quality;
  }

  void GaussFitter1D::updateMembers_()
  {
    MaxLikeliFitter1D::updateMembers_();
    statistics_.setMean(param_.getValue("statistics:mean"));
    statistics_.setVariance(param_.getValue("statistics:variance"));
  }

}
