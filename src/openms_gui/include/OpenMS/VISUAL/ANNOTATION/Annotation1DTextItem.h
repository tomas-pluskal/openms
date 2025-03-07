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
// $Maintainer: Timo Sachsenberg $
// $Authors: Johannes Junker, Timo Sachsenberg $
// --------------------------------------------------------------------------

#ifndef OPENMS_VISUAL_ANNOTATION_ANNOTATION1DTEXTITEM_H
#define OPENMS_VISUAL_ANNOTATION_ANNOTATION1DTEXTITEM_H

#include <OpenMS/VISUAL/ANNOTATION/Annotation1DItem.h>

namespace OpenMS
{
  /** @brief An annotation item which represents an arbitrary text on the canvas.
          @see Annotation1DItem
  */
  class Annotation1DTextItem :
    public Annotation1DItem
  {
public:

    /// Constructor
    Annotation1DTextItem(const PointType & position, const QString & text, const int flags = Qt::AlignCenter);

    /// Copy constructor
    Annotation1DTextItem(const Annotation1DTextItem & rhs);

    /// Destructor
    virtual ~Annotation1DTextItem();

    // Docu in base class
    virtual void ensureWithinDataRange(Spectrum1DCanvas * const canvas);

    // Docu in base class
    virtual void draw(Spectrum1DCanvas * const canvas, QPainter & painter, bool flipped = false);

    // Docu in base class
    virtual void move(const PointType & delta);

    /// Sets the position of the item (in MZ / intensity coordinates)
    void setPosition(const PointType & position);

    /// Returns the position of the item (in MZ / intensity coordinates)
    const PointType & getPosition() const;

    /// Set Qt flags (default: Qt::AlignCenter)
    void setFlags(int flags);

    /// Get Qt flags
    int getFlags() const;

protected:

    /// The position of the item (in MZ / intensity coordinates)
    PointType position_;

    int flags_;
  };
} // namespace OpenMS

#endif
