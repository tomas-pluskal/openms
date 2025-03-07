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
#include <OpenMS/config.h>

#include <OpenMS/FORMAT/MzMLFile.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/FILTERING/SMOOTHING/GaussFilter.h>
#include <OpenMS/APPLICATIONS/TOPPBase.h>
#include <OpenMS/FORMAT/PeakTypeEstimator.h>
#include <OpenMS/DATASTRUCTURES/StringListUtils.h>

#include <OpenMS/FORMAT/DATAACCESS/MSDataWritingConsumer.h>

using namespace OpenMS;
using namespace std;

//-------------------------------------------------------------
//Doxygen docu
//-------------------------------------------------------------

/**
  @page TOPP_NoiseFilterGaussian NoiseFilterGaussian

  @brief  Executes a Gaussian filter to reduce the noise in an MS experiment.
 
  <center>
  <table>
  <tr>
  <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. predecessor tools </td>
  <td VALIGN="middle" ROWSPAN=4> \f$ \longrightarrow \f$ NoiseFilterGaussian \f$ \longrightarrow \f$</td>
  <td ALIGN = "center" BGCOLOR="#EBEBEB"> pot. successor tools </td>
  </tr>
  <tr>
  <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_FileConverter </td>
  <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_PeakPickerWavelet</td>
  </tr>
  <tr>
  <td VALIGN="middle" ALIGN = "center" ROWSPAN=2> @ref TOPP_Resampler </td>
  <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_PeakPickerHiRes</td>
  </tr>
  <tr>
  <td VALIGN="middle" ALIGN = "center" ROWSPAN=1> @ref TOPP_BaselineFilter</td>
  </tr>
  </table>
  </center>
 
  The Gaussian filter is a peak area preserving low-pass filter and is characterized by narrow bandwidths,
  sharp cutoffs, and low passband ripple.

  @note The Gaussian filter works for uniform as well as for non-uniform data.

  <B>The command line parameters of this tool are:</B>
  @verbinclude TOPP_NoiseFilterGaussian.cli
  <B>INI file documentation of this tool:</B>
  @htmlinclude TOPP_NoiseFilterGaussian.html
*/

// We do not want this class to show up in the docu:
/// @cond TOPPCLASSES


class TOPPNoiseFilterGaussian :
  public TOPPBase
{
public:
  TOPPNoiseFilterGaussian() :
    TOPPBase("NoiseFilterGaussian", "Removes noise from profile spectra by using Gaussian filter (on uniform as well as non-uniform data).")
  {
  }

  /**
    @brief Helper class for the Low Memory Noise filtering
  */
  class NFGaussMzMLConsumer :
    public MSDataWritingConsumer 
  {

  public:

    NFGaussMzMLConsumer(const String& filename, const GaussFilter& gf) :
      MSDataWritingConsumer(filename) 
    {
      gf_ = gf;
    }

    void processSpectrum_(MapType::SpectrumType& s)
    {
      gf_.filter(s);
    }

    void processChromatogram_(MapType::ChromatogramType& c) 
    {
      gf_.filter(c);
    }

  private:
    GaussFilter gf_;
  };

  void registerOptionsAndFlags_()
  {
    registerInputFile_("in", "<file>", "", "input raw data file ");
    setValidFormats_("in", ListUtils::create<String>("mzML"));
    registerOutputFile_("out", "<file>", "", "output raw data file ");
    setValidFormats_("out", ListUtils::create<String>("mzML"));

    registerStringOption_("processOption", "<name>", "inmemory", "Whether to load all data and process them in-memory or whether to process the data on the fly (lowmemory) without loading the whole file into memory first", false, true);
    setValidStrings_("processOption", ListUtils::create<String>("inmemory,lowmemory"));

    registerSubsection_("algorithm", "Algorithm parameters section");
  }

  Param getSubsectionDefaults_(const String & /*section*/) const
  {
    return GaussFilter().getDefaults();
  }

  ExitCodes doLowMemAlgorithm(const GaussFilter& gauss)
  {
    ///////////////////////////////////
    // Create the consumer object, add data processing
    ///////////////////////////////////
    NFGaussMzMLConsumer gaussConsumer(out, gauss);
    gaussConsumer.addDataProcessing(getProcessingInfo_(DataProcessing::SMOOTHING));

    ///////////////////////////////////
    // Create new MSDataReader and set our consumer
    ///////////////////////////////////
    MzMLFile mz_data_file;
    mz_data_file.setLogType(log_type_);
    mz_data_file.transform(in, &gaussConsumer);

    return EXECUTION_OK;
  }

  ExitCodes main_(int, const char **)
  {
    //-------------------------------------------------------------
    // parameter handling
    //-------------------------------------------------------------
    in = getStringOption_("in");
    out = getStringOption_("out");
    String process_option = getStringOption_("processOption");

    Param filter_param = getParam_().copy("algorithm:", true);
    writeDebug_("Parameters passed to filter", filter_param, 3);

    GaussFilter gauss;
    gauss.setLogType(log_type_);
    gauss.setParameters(filter_param);

    if (process_option == "lowmemory")
    {
      return doLowMemAlgorithm(gauss);
    }

    //-------------------------------------------------------------
    // loading input
    //-------------------------------------------------------------
    MzMLFile mz_data_file;
    mz_data_file.setLogType(log_type_);
    MSExperiment<Peak1D> exp;
    mz_data_file.load(in, exp);

    if (exp.empty() && exp.getChromatograms().size() == 0)
    {
      LOG_WARN << "The given file does not contain any conventional peak data, but might"
                  " contain chromatograms. This tool currently cannot handle them, sorry.";
      return INCOMPATIBLE_INPUT_DATA;
    }
    //check for peak type (profile data required)
    if (!exp.empty() && PeakTypeEstimator().estimateType(exp[0].begin(), exp[0].end()) == SpectrumSettings::PEAKS)
    {
      writeLog_("Warning: OpenMS peak type estimation indicates that this is not profile data!");
    }

    //check if spectra are sorted
    for (Size i = 0; i < exp.size(); ++i)
    {
      if (!exp[i].isSorted())
      {
        writeLog_("Error: Not all spectra are sorted according to peak m/z positions. Use FileFilter to sort the input!");
        return INCOMPATIBLE_INPUT_DATA;
      }
    }

    //check if chromatograms are sorted
    for (Size i = 0; i < exp.getChromatograms().size(); ++i)
    {
      if (!exp.getChromatogram(i).isSorted())
      {
        writeLog_("Error: Not all chromatograms are sorted according to peak m/z positions. Use FileFilter to sort the input!");
        return INCOMPATIBLE_INPUT_DATA;
      }
    }

    //-------------------------------------------------------------
    // calculations
    //-------------------------------------------------------------
    try
    {
      gauss.filterExperiment(exp);
    }
    catch (Exception::IllegalArgument & e)
    {
      writeLog_(String("Error: ") + e.getMessage());
      return INCOMPATIBLE_INPUT_DATA;
    }

    //-------------------------------------------------------------
    // writing output
    //-------------------------------------------------------------

    //annotate output with data processing info
    addDataProcessing_(exp, getProcessingInfo_(DataProcessing::SMOOTHING));

    mz_data_file.store(out, exp);

    return EXECUTION_OK;
  }

  String in;
  String out;
};


int main(int argc, const char ** argv)
{
  TOPPNoiseFilterGaussian tool;
  return tool.main(argc, argv);
}

/// @endcond
