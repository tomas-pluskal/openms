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
// $Maintainer: Andreas Bertsch $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#include <OpenMS/COMPARISON/SPECTRA/SpectrumAlignmentScore.h>
#include <OpenMS/COMPARISON/SPECTRA/SpectrumAlignment.h>
#include <OpenMS/DATASTRUCTURES/ListUtils.h>
#include <cmath>

#include <boost/math/special_functions/erf.hpp>

using namespace std;

namespace OpenMS
{
  SpectrumAlignmentScore::SpectrumAlignmentScore() :
    PeakSpectrumCompareFunctor()
  {
    setName(SpectrumAlignmentScore::getProductName());
    defaults_.setValue("tolerance", 0.3, "Defines the absolute (in Da) or relative (in ppm) tolerance");
    defaults_.setValue("is_relative_tolerance", "false", "if true, the tolerance value is interpreted as ppm");
    defaults_.setValidStrings("is_relative_tolerance", ListUtils::create<String>("true,false"));
    defaults_.setValue("use_linear_factor", "false", "if true, the intensities are weighted with the relative m/z difference");
    defaults_.setValidStrings("use_linear_factor", ListUtils::create<String>("true,false"));
    defaults_.setValue("use_gaussian_factor", "false", "if true, the intensities are weighted with the relative m/z difference using a gaussian");
    defaults_.setValidStrings("use_gaussian_factor", ListUtils::create<String>("true,false"));
    defaultsToParam_();
  }

  SpectrumAlignmentScore::SpectrumAlignmentScore(const SpectrumAlignmentScore & source) :
    PeakSpectrumCompareFunctor(source)
  {
  }

  SpectrumAlignmentScore::~SpectrumAlignmentScore()
  {
  }

  SpectrumAlignmentScore & SpectrumAlignmentScore::operator=(const SpectrumAlignmentScore & source)
  {
    if (this != &source)
    {
      PeakSpectrumCompareFunctor::operator=(source);
    }
    return *this;
  }

  double SpectrumAlignmentScore::operator()(const PeakSpectrum & spec) const
  {
    return operator()(spec, spec);
  }

  double SpectrumAlignmentScore::operator()(const PeakSpectrum & s1, const PeakSpectrum & s2) const
  {
    const double tolerance = (double)param_.getValue("tolerance");
    bool is_relative_tolerance = param_.getValue("is_relative_tolerance").toBool();
    bool use_linear_factor = param_.getValue("use_linear_factor").toBool();
    bool use_gaussian_factor = param_.getValue("use_gaussian_factor").toBool();

    if (use_linear_factor && use_gaussian_factor)
    {
      cerr << "Warning: SpectrumAlignmentScore, use either 'use_linear_factor' or 'use_gaussian_factor'!" << endl;
    }

    SpectrumAlignment aligner;
    Param p;
    p.setValue("tolerance", tolerance);
    p.setValue("is_relative_tolerance", (String)param_.getValue("is_relative_tolerance"));
    aligner.setParameters(p);

    vector<pair<Size, Size> > alignment;
    aligner.getSpectrumAlignment(alignment, s1, s2);

    double score(0), sum(0), sum1(0), sum2(0);
    for (PeakSpectrum::ConstIterator it1 = s1.begin(); it1 != s1.end(); ++it1)
    {
      sum1 += it1->getIntensity() * it1->getIntensity();
    }

    for (PeakSpectrum::ConstIterator it1 = s2.begin(); it1 != s2.end(); ++it1)
    {
      sum2 += it1->getIntensity() * it1->getIntensity();
    }

    for (vector<pair<Size, Size> >::const_iterator it = alignment.begin(); it != alignment.end(); ++it)
    {
      //double factor(0.0);
      //factor = (epsilon - fabs(s1[it->first].getPosition()[0] - s2[it->second].getPosition()[0])) / epsilon;
      double mz_tolerance(tolerance);

      if (is_relative_tolerance)
      {
        mz_tolerance = mz_tolerance * s1[it->first].getPosition()[0] / 1e6;
      }

      double mz_difference(fabs(s1[it->first].getPosition()[0] - s2[it->second].getPosition()[0]));
      double factor = 1.0;

      if (use_linear_factor || use_gaussian_factor)
      {
        factor = getFactor_(mz_tolerance, mz_difference, use_gaussian_factor);
      }
      sum += sqrt(s1[it->first].getIntensity() * s2[it->second].getIntensity() * factor);
    }

    score = sum / (sqrt(sum1 * sum2));

    return score;
  }

  double SpectrumAlignmentScore::getFactor_(double mz_tolerance, double mz_difference, bool is_gaussian) const
  {
    double factor(0.0);

    if (is_gaussian)
    {
      static const double denominator = mz_tolerance * 3.0 * sqrt(2.0);
      factor = boost::math::erfc(mz_difference / denominator);
      //cerr << "Factor: " << factor << " " << mz_tolerance << " " << mz_difference << endl;
    }
    else
    {
      factor = (mz_tolerance - mz_difference) / mz_tolerance;
    }
    return factor;
  }

}
