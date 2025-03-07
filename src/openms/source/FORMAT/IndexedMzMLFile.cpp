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

#include <OpenMS/FORMAT/IndexedMzMLFile.h>

#include <OpenMS/FORMAT/HANDLERS/IndexedMzMLDecoder.h>
#include <OpenMS/FORMAT/HANDLERS/MzMLSpectrumDecoder.h>

namespace OpenMS
{

  void IndexedMzMLFile::parseFooter_(String filename)
  {
    //-------------------------------------------------------------
    // Find offset
    //-------------------------------------------------------------

    index_offset_ = IndexedMzMLDecoder().findIndexListOffset(filename);
    int res = IndexedMzMLDecoder().parseOffsets(filename, index_offset_, spectra_offsets_, chromatograms_offsets_);

    spectra_before_chroms_ = true;
    if (!spectra_offsets_.empty() && !chromatograms_offsets_.empty())
    {
      if (spectra_offsets_[0].second < chromatograms_offsets_[0].second) spectra_before_chroms_ = true;
      else spectra_before_chroms_ = false;
    }

    if (res == 0) parsing_success_ = true;
    else parsing_success_ = false;
  }

  IndexedMzMLFile::IndexedMzMLFile(String filename) 
  {
    openFile(filename);
  }

  IndexedMzMLFile::IndexedMzMLFile(const IndexedMzMLFile& source) :
    filename_(source.filename_),
    spectra_offsets_(source.spectra_offsets_),
    chromatograms_offsets_(source.chromatograms_offsets_),
    index_offset_(source.index_offset_),
    spectra_before_chroms_(source.spectra_before_chroms_),
    // do not copy the filestream itself but open a new filestream using the same file
    filestream_(source.filename_.c_str()),
    parsing_success_(source.parsing_success_)
  {
  }

  IndexedMzMLFile::~IndexedMzMLFile()
  {
  }

  void IndexedMzMLFile::openFile(String filename) 
  {
    if (filestream_.is_open())
    {
      filestream_.close();
    }
    filename_ = filename;
    filestream_.open(filename.c_str());
    parseFooter_(filename);
  }

  bool IndexedMzMLFile::getParsingSuccess() const
  {
    return parsing_success_;
  }

  size_t IndexedMzMLFile::getNrSpectra() const
  {
    return spectra_offsets_.size();
  }

  size_t IndexedMzMLFile::getNrChromatograms() const
  {
    return chromatograms_offsets_.size();
  }

  OpenMS::Interfaces::SpectrumPtr IndexedMzMLFile::getSpectrumById(int id)
  {
    int spectrumToGet = id;

    if (!parsing_success_)
      throw Exception::ParseError(__FILE__, __LINE__, __PRETTY_FUNCTION__, 
          "Parsing was unsuccessful, cannot read file", "");
    if (spectrumToGet < 0)
      throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__, 
          String( "id needs to be positive, was " + String(id) ));
    if (spectrumToGet >= (int)getNrSpectra())
      throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__, String( 
        "id needs to be smaller than the number of spectra, was " + String(id) 
        + " maximal allowed is " + String(getNrSpectra()) ));

    std::streampos startidx = -1;
    std::streampos endidx = -1;

    if (spectrumToGet == int(getNrSpectra() - 1))
    {
      startidx = spectra_offsets_[spectrumToGet].second;
      if (chromatograms_offsets_.empty() || !spectra_before_chroms_)
      {
        // just take everything until the index starts
        endidx = index_offset_;
      }
      else
      {
        // just take everything until the chromatograms start
        endidx = chromatograms_offsets_[0].second;
      }
    }
    else
    {
      startidx = spectra_offsets_[spectrumToGet].second;
      endidx = spectra_offsets_[spectrumToGet + 1].second;
    }

    std::streampos readl = endidx - startidx;
    char* buffer = new char[readl + std::streampos(1)];
    filestream_.seekg(startidx, filestream_.beg);
    filestream_.read(buffer, readl);
    buffer[readl] = '\0';
    std::string text(buffer);
    delete[] buffer;

#ifdef DEBUG_READER
    // print the full text we just read
    std::cout << text << std::endl;
#endif

    OpenMS::Interfaces::SpectrumPtr sptr(new OpenMS::Interfaces::Spectrum);
    MzMLSpectrumDecoder d;
    d.setSkipXMLChecks(skip_xml_checks_ );
    d.domParseSpectrum(text, sptr);

#ifdef DEBUG_READER
    std::cout << sptr->getIntensityArray()->data.size() << " int and mz : " << sptr->getMZArray()->data.size() << std::endl;
#endif

    return sptr;
  }

  OpenMS::Interfaces::ChromatogramPtr IndexedMzMLFile::getChromatogramById(int id)
  {
    int chromToGet = id;

    if (!parsing_success_)
      throw Exception::ParseError(__FILE__, __LINE__, __PRETTY_FUNCTION__, 
          "Parsing was unsuccessful, cannot read file", "");
    if (chromToGet < 0)
      throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__, 
          String( "id needs to be positive, was " + String(id) ));
    if (chromToGet >= (int)getNrChromatograms())
      throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__, String( 
        "id needs to be smaller than the number of spectra, was " + String(id) 
        + " maximal allowed is " + String(getNrSpectra()) ));

    std::streampos startidx = -1;
    std::streampos endidx = -1;

    if (chromToGet == int(getNrChromatograms() - 1))
    {
      startidx = chromatograms_offsets_[chromToGet].second;
      if (spectra_offsets_.empty() || spectra_before_chroms_)
      {
        // just take everything until the index starts
        endidx = index_offset_;
      }
      else
      {
        // just take everything until the chromatograms start
        endidx = spectra_offsets_[0].second;
      }
    }
    else
    {
      startidx = chromatograms_offsets_[chromToGet].second;
      endidx = chromatograms_offsets_[chromToGet + 1].second;
    }

    std::streampos readl = endidx - startidx;
    char* buffer = new char[readl + std::streampos(1)];
    filestream_.seekg(startidx, filestream_.beg);
    filestream_.read(buffer, readl);
    buffer[readl] = '\0';
    std::string text(buffer);
    delete[] buffer;

#ifdef DEBUG_READER
    // print the full text we just read
    std::cout << text << std::endl;
#endif

    OpenMS::Interfaces::ChromatogramPtr sptr(new OpenMS::Interfaces::Chromatogram);
    MzMLSpectrumDecoder d;
    d.setSkipXMLChecks(skip_xml_checks_ );
    d.domParseChromatogram(text, sptr);

#ifdef DEBUG_READER
    std::cout << sptr->getIntensityArray()->data.size() << " int and time : " << sptr->getTimeArray()->data.size() << std::endl;
#endif

    return sptr;
  }

}
