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
// $Maintainer: Alexandra Zerck $
// $Authors: Eva Lange $
// --------------------------------------------------------------------------

#ifndef OPENMS_FILTERING_SMOOTHING_GAUSSFILTER_H
#define OPENMS_FILTERING_SMOOTHING_GAUSSFILTER_H

#include <OpenMS/DATASTRUCTURES/DefaultParamHandler.h>
#include <OpenMS/CONCEPT/ProgressLogger.h>
#include <OpenMS/CONCEPT/Constants.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/FILTERING/SMOOTHING/GaussFilterAlgorithm.h>

#include <cmath>

namespace OpenMS
{
  /**
    @brief This class represents a Gaussian lowpass-filter which works on uniform as well as on non-uniform profile data.

    Gaussian filters are important in many signal processing,
    image processing, and communication applications. These filters are characterized by narrow bandwidths,
    sharp cutoffs, and low passband ripple. A key feature of Gaussian filters is that the Fourier transform of a
    Gaussian is also a Gaussian, so the filter has the same response shape in both the time and frequency domains.
    The coefficients \f$ \emph{coeffs} \f$ of the Gaussian-window with length \f$ \emph{frameSize} \f$ are calculated
    from the gaussian distribution
    \f[ \emph{coeff}(x) = \frac{1}{\sigma \sqrt{2\pi}} e^{\frac{-x^2}{2\sigma^2}} \f]
    where \f$ x=[-\frac{frameSize}{2},...,\frac{frameSize}{2}] \f$ represents the window area and \f$ \sigma \f$
    is the standard derivation.

    @note The wider the kernel width the smoother the signal (the more detail information get lost!).
          Use a gaussian filter kernel which has approximately the same width as your mass peaks,
          whereas the gaussian peak width corresponds approximately to 8*sigma.

        @note The data must be sorted according to ascending m/z!

        @htmlinclude OpenMS_GaussFilter.parameters

    @ingroup SignalProcessing
  */
//#define DEBUG_FILTERING

  class OPENMS_DLLAPI GaussFilter :
    public ProgressLogger,
    public DefaultParamHandler
  {
public:
    /// Constructor
    GaussFilter();

    /// Destructor
    virtual ~GaussFilter();

    /**
      @brief Smoothes an MSSpectrum containing profile data.

      Convolutes the filter and the profile data and writes the result back to the spectrum.

        @exception Exception::IllegalArgument is thrown, if the @em gaussian_width parameter is too small.
    */
    template <typename PeakType>
    void filter(MSSpectrum<PeakType> & spectrum)
    {
      typedef std::vector<double> ContainerT;

      // make sure the right data type is set
      spectrum.setType(SpectrumSettings::RAWDATA);
      bool found_signal = false;
      Size data_size = spectrum.size();
      ContainerT mz_in(data_size), int_in(data_size), mz_out(data_size), int_out(data_size);

      // copy spectrum to container
      for (Size p = 0; p < spectrum.size(); ++p)
      {
        mz_in[p] = spectrum[p].getMZ();
        int_in[p] = spectrum[p].getIntensity();
      }

      // apply filter
      ContainerT::iterator mz_out_it = mz_out.begin();
      ContainerT::iterator int_out_it = int_out.begin();
      found_signal = gauss_algo_.filter(mz_in.begin(), mz_in.end(), int_in.begin(), mz_out_it, int_out_it);

      // If all intensities are zero in the scan and the scan has a reasonable size, throw an exception.
      // This is the case if the gaussian filter is smaller than the spacing of raw data
      if (!found_signal && spectrum.size() >= 3)
      {
        String error_message = "Found no signal. The gaussian width is probably smaller than the spacing in your profile data. Try to use a bigger width.";
        if (spectrum.getRT() > 0.0)
        {
          error_message += String(" The error occured in the spectrum with retention time ") + spectrum.getRT() + ".\n";
        }
        std::cerr << error_message;
      }
      else
      {
        // copy the new data into the spectrum
        ContainerT::iterator mz_it = mz_out.begin();
        ContainerT::iterator int_it = int_out.begin();
        for (Size p = 0; mz_it != mz_out.end(); mz_it++, int_it++, p++)
        {
          spectrum[p].setIntensity(*int_it);
          spectrum[p].setMZ(*mz_it);
        }
      }
    }

    template <typename PeakType>
    void filter(MSChromatogram<PeakType> & chromatogram)
    {

      if (param_.getValue("use_ppm_tolerance").toBool())
      {
        throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__, 
          "GaussFilter: Cannot use ppm tolerance on chromatograms");
      }

      MSSpectrum<PeakType> filter_spectra;
      for (typename MSChromatogram<PeakType>::const_iterator it = chromatogram.begin(); it != chromatogram.end(); ++it)
      {
        filter_spectra.push_back(*it);
      }
      filter(filter_spectra);
      chromatogram.clear(false);
      for (typename MSSpectrum<PeakType>::const_iterator it = filter_spectra.begin(); it != filter_spectra.end(); ++it)
      {
        chromatogram.push_back(*it);
      }

    }

    /**
      @brief Smoothes an MSExperiment containing profile data.

        @exception Exception::IllegalArgument is thrown, if the @em gaussian_width parameter is too small.
          */
    template <typename PeakType>
    void filterExperiment(MSExperiment<PeakType> & map)
    {
      Size progress = 0;
      startProgress(0, map.size() + map.getChromatograms().size(), "smoothing data");
      for (Size i = 0; i < map.size(); ++i)
      {
        filter(map[i]);
        setProgress(++progress);
      }
      for (Size i = 0; i < map.getChromatograms().size(); ++i)
      {
        filter(map.getChromatogram(i));
        setProgress(++progress);
      }
      endProgress();
    }

protected:

    GaussFilterAlgorithm gauss_algo_;

    /// The spacing of the pre-tabulated kernel coefficients
    double spacing_;

    // Docu in base class
    virtual void updateMembers_();
  };

} // namespace OpenMS
#endif
