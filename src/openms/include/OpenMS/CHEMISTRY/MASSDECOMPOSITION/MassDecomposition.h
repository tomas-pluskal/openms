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
// $Maintainer: Sandro Andreotti $
// $Authors: Andreas Bertsch $
// --------------------------------------------------------------------------

#ifndef OPENMS_CHEMISTRY_MASSDECOMPOSITION_MASSDECOMPOSITION_H
#define OPENMS_CHEMISTRY_MASSDECOMPOSITION_MASSDECOMPOSITION_H

#include <OpenMS/config.h>
#include <OpenMS/CONCEPT/Types.h>

#include <map>

namespace OpenMS
{
  class String;

  /**
    @brief Class represents a decomposition of a mass into amino acids

    This class represents a mass decomposition into amino acids. A
    decomposition are amino acids given with frequencies which add
    up to a specific mass.
  */
  class OPENMS_DLLAPI MassDecomposition
  {
public:

    /**
      @name Constructors and destructors
    */
    //@{
    /// default constructor
    MassDecomposition();

    /// copy constructor
    MassDecomposition(const MassDecomposition& deco);

    /// constructor with String as parameter
    explicit MassDecomposition(const String& deco);
    //@}

    /**
      @name Operators and accessors
    */
    //@{
    /// assignment operator
    MassDecomposition& operator=(const MassDecomposition& rhs);

    /// adds the mass decomposition d to this object
    MassDecomposition& operator+=(const MassDecomposition& d);

    /// returns the decomposition as a string
    String toString() const;

    /// returns the decomposition as a string; instead of frequencies the amino acids are repeated
    String toExpandedString() const;

    /// adds this decomposition and the decomposition given and returns a new composition
    MassDecomposition operator+(const MassDecomposition& rhs) const;

    /// returns the max frequency of this composition
    Size getNumberOfMaxAA() const;
    //@}

    /**
      @name Predicates
    */
    //@{
    /// less than predicate
    bool operator<(const MassDecomposition& rhs) const;

    /// equality operator
    bool operator==(const String& deco) const;

    /// returns true if tag is contained in the mass decomposition
    bool containsTag(const String& tag) const;

    /// returns true if the mass decomposition if contained in this instance
    bool compatible(const MassDecomposition& deco) const;
    //@}

protected:
    std::map<char, Size> decomp_;
    Size number_of_max_aa_;
  };
} // namespace OpenMS

#endif // OPENMS_CHEMISTRY_MASSDECOMPOSITION_MASSDECOMPOSITION_H
