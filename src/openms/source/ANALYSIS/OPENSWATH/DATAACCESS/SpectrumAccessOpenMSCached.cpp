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
// $Maintainer: Hannes Roest $
// $Authors: Hannes Roest $
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/OPENSWATH/DATAACCESS/SpectrumAccessOpenMSCached.h>

#include <OpenMS/FORMAT/CachedMzML.h>

namespace OpenMS
{

  SpectrumAccessOpenMSCached::SpectrumAccessOpenMSCached(String filename)
  {
    filename_cached_ = filename + ".cached";
    filename_ = filename;

    // Create the index from the given file
    CachedmzML cache;
    cache.createMemdumpIndex(filename_cached_);
    spectra_index_ = cache.getSpectraIndex();
    chrom_index_ = cache.getChromatogramIndex();;

    // open the filestream
    ifs_.open(filename_cached_.c_str(), std::ios::binary);

    // load the meta data from disk
    MzMLFile().load(filename, meta_ms_experiment_);
  }

  SpectrumAccessOpenMSCached::~SpectrumAccessOpenMSCached()
  {
    ifs_.close();
  }

  SpectrumAccessOpenMSCached::SpectrumAccessOpenMSCached(const SpectrumAccessOpenMSCached & rhs) :
    meta_ms_experiment_(rhs.meta_ms_experiment_),
    ifs_(rhs.filename_cached_.c_str(), std::ios::binary),
    filename_(rhs.filename_),
    spectra_index_(rhs.spectra_index_),
    chrom_index_(rhs.chrom_index_)
  {
  }

  boost::shared_ptr<OpenSwath::ISpectrumAccess> SpectrumAccessOpenMSCached::lightClone() const 
  {
    return boost::shared_ptr<SpectrumAccessOpenMSCached>(new SpectrumAccessOpenMSCached(*this));
  }

  OpenSwath::SpectrumPtr SpectrumAccessOpenMSCached::getSpectrumById(int id) 
  {
    OpenSwath::BinaryDataArrayPtr mz_array(new OpenSwath::BinaryDataArray);
    OpenSwath::BinaryDataArrayPtr intensity_array(new OpenSwath::BinaryDataArray);
    int ms_level = -1;
    double rt = -1.0;

    if ( !ifs_.seekg(spectra_index_[id]) )
    {
      std::cerr << "Error while reading spectrum " << id << " - seekg created an error when trying to change position to " << spectra_index_[id] << "." << std::endl;
      std::cerr << "Maybe an invalid position was supplied to seekg, this can happen for example when reading large files (>2GB) on 32bit systems." << std::endl;
      throw Exception::ParseError(__FILE__, __LINE__, __PRETTY_FUNCTION__,
        "Error while changing position of input stream pointer.", filename_cached_);
    }

    CachedmzML::readSpectrumFast(mz_array, intensity_array, ifs_, ms_level, rt);

    OpenSwath::SpectrumPtr sptr(new OpenSwath::Spectrum);
    sptr->setMZArray(mz_array);
    sptr->setIntensityArray(intensity_array);
    return sptr;
  }

  OpenSwath::SpectrumMeta SpectrumAccessOpenMSCached::getSpectrumMetaById(int id) const
  {
    OpenSwath::SpectrumMeta meta;
    meta.RT = meta_ms_experiment_[id].getRT();
    meta.ms_level = meta_ms_experiment_[id].getMSLevel();
    return meta;
  }

  OpenSwath::ChromatogramPtr SpectrumAccessOpenMSCached::getChromatogramById(int id) 
  {
    OpenSwath::BinaryDataArrayPtr rt_array(new OpenSwath::BinaryDataArray);
    OpenSwath::BinaryDataArrayPtr intensity_array(new OpenSwath::BinaryDataArray);

    if ( !ifs_.seekg(chrom_index_[id]) )
    {
      std::cerr << "Error while reading chromatogram " << id << " - seekg created an error when trying to change position to " << chrom_index_[id] << "." << std::endl;
      std::cerr << "Maybe an invalid position was supplied to seekg, this can happen for example when reading large files (>2GB) on 32bit systems." << std::endl;
      throw Exception::ParseError(__FILE__, __LINE__, __PRETTY_FUNCTION__,
        "Error while changing position of input stream pointer.", filename_cached_);
    }

    CachedmzML::readChromatogramFast(rt_array, intensity_array, ifs_);

    OpenSwath::ChromatogramPtr cptr(new OpenSwath::Chromatogram);
    cptr->setTimeArray(rt_array);
    cptr->setIntensityArray(intensity_array);
    return cptr;
  }

  std::vector<std::size_t> SpectrumAccessOpenMSCached::getSpectraByRT(double RT, double deltaRT) const
  {
    OPENMS_PRECONDITION(deltaRT >= 0, "Delta RT needs to be a positive number");

    // we first perform a search for the spectrum that is past the
    // beginning of the RT domain. Then we add this spectrum and try to add
    // further spectra as long as they are below RT + deltaRT.
    std::vector<std::size_t> result;
    MSExperimentType::ConstIterator spectrum = meta_ms_experiment_.RTBegin(RT - deltaRT);
    result.push_back(std::distance(meta_ms_experiment_.begin(), spectrum));
    spectrum++;
    while (spectrum->getRT() < RT + deltaRT && spectrum != meta_ms_experiment_.end())
    {
      result.push_back(spectrum - meta_ms_experiment_.begin());
      spectrum++;
    }
    return result;
  }

  size_t SpectrumAccessOpenMSCached::getNrSpectra() const
  {
    return meta_ms_experiment_.size();
  }

  SpectrumSettings SpectrumAccessOpenMSCached::getSpectraMetaInfo(int id) const
  {
    return meta_ms_experiment_[id];
  }

  size_t SpectrumAccessOpenMSCached::getNrChromatograms() const
  {
    return meta_ms_experiment_.getChromatograms().size();
  }

  ChromatogramSettings SpectrumAccessOpenMSCached::getChromatogramMetaInfo(int id) const
  {
    return meta_ms_experiment_.getChromatograms()[id];
  }

  std::string SpectrumAccessOpenMSCached::getChromatogramNativeID(int id) const
  {
    return meta_ms_experiment_.getChromatograms()[id].getNativeID();
  }

} //end namespace OpenMS
