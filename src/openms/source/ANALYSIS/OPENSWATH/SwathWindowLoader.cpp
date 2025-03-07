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
// $Maintainer: Hannes Roest$
// $Authors: Hannes Roest$
// --------------------------------------------------------------------------

#include <OpenMS/ANALYSIS/OPENSWATH/SwathWindowLoader.h>

#include <OpenMS/CONCEPT/Types.h>
#include <OpenMS/CONCEPT/Exception.h>

#include <iostream>
#include <fstream>
#include <sstream>

namespace OpenMS
{

  static bool SortSwathMapByLower(const OpenSwath::SwathMap & left, const OpenSwath::SwathMap & right)
  {
    return left.upper < right.upper;
  }

  void SwathWindowLoader::annotateSwathMapsFromFile(const std::string & filename,
    std::vector< OpenSwath::SwathMap >& swath_maps, bool doSort)
  {
    std::vector<double> swath_prec_lower_, swath_prec_upper_;
    readSwathWindows(filename, swath_prec_lower_, swath_prec_upper_);

    // Sort the windows by the start of the lower window
    if (doSort)
    {
      std::sort(swath_maps.begin(), swath_maps.end(), SortSwathMapByLower);
    }

    Size i = 0, j = 0;
    for (; i < swath_maps.size(); i++)
    {
      if (swath_maps[i].ms1)
      {
        // skip to next map (only increase i)
        continue;
      }
      if (j >= swath_prec_lower_.size())
      {
        std::cerr << "Trying to access annotation for SWATH map " << j <<
          " but there are only " << swath_prec_lower_.size() << " windows in the" <<
          " swath_windows_file. Please check your input." << std::endl;
        throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__,
            "The number of SWATH maps read from the raw data and from the annotation file do not match.");
      }

      std::cout << "Re-annotate from file: SWATH " <<
        swath_maps[i].lower << " / " << swath_maps[i].upper << " is annotated with " <<
        swath_prec_lower_[j] << " / " << swath_prec_upper_[j] << std::endl;

      swath_maps[i].lower = swath_prec_lower_[j];
      swath_maps[i].upper = swath_prec_upper_[j];
      j++;
    }

    if (j != swath_prec_upper_.size())
    {
      std::cerr << "The number of SWATH maps read from the raw data (" <<
        j << ") and from the annotation file (" << swath_prec_upper_.size() << ") do not match." << std::endl;
      throw Exception::IllegalArgument(__FILE__, __LINE__, __PRETTY_FUNCTION__,
          "The number of SWATH maps read from the raw data and from the annotation file do not match.");
    }
  }

  void SwathWindowLoader::readSwathWindows(const std::string & filename,
    std::vector<double> & swath_prec_lower_,
    std::vector<double> & swath_prec_upper_ )
  {
    std::ifstream data(filename.c_str());
    std::string line;
    std::getline(data, line); //skip header
    std::cout << "Read Swath window header " << line << std::endl;
    double lower, upper;
    while (std::getline(data, line))
    {
      std::stringstream lineStream(line);

      lineStream >> lower;
      lineStream >> upper;

      swath_prec_lower_.push_back(lower);
      swath_prec_upper_.push_back(upper);
    }
    assert(swath_prec_lower_.size() == swath_prec_upper_.size());
    std::cout << "Read Swath window file with " << swath_prec_lower_.size() << " SWATH windows." << std::endl;
  }

}
