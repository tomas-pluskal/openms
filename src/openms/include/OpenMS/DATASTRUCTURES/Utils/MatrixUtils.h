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
// $Maintainer: Christian Ehrlich $
// $Authors: Christian Ehrlich $
// --------------------------------------------------------------------------

#ifndef OPENMS_DATASTRUCTURES_UTILS_MATRIXUTILS_H
#define OPENMS_DATASTRUCTURES_UTILS_MATRIXUTILS_H

#include <OpenMS/DATASTRUCTURES/Matrix.h>

#include <Eigen/Core>

#include <boost/shared_ptr.hpp>

namespace OpenMS
{
  /** Matrix utility functions.
   *
   */

  typedef boost::shared_ptr< const Eigen::MatrixXd > EigenMatrixXdPtr;
  typedef boost::shared_ptr< Eigen::MatrixXd > MutableEigenMatrixXdPtr;

  static EigenMatrixXdPtr
  convertOpenMSMatrix2EigenMatrixXd( const Matrix<double>& m )
  {
    MutableEigenMatrixXdPtr em ( new Eigen::MatrixXd(m.rows(), m.cols()) );
    for (unsigned i=0; i<m.rows(); ++i)
    {
      for (unsigned j=0; j<m.cols(); ++j)
      {
        (*em)(i,j) = m(i,j);
      }
    }
    return em;
  }

  static bool
  matrixIsIdentityMatrix(const Matrix<double>& channel_frequency)
  {
    bool is_identity = true;

    for (Matrix<double>::SizeType i = 0; i < channel_frequency.rows(); ++i)
    {
      for (Matrix<double>::SizeType j = 0; j < channel_frequency.rows(); ++j)
      {
        // check if the entries are those of a identity matrix;
        // i==j -> m(i,j) == 1.0 && i!=j -> m(i,j) == 0.0
        if ((i == j && channel_frequency(i, j) != 1.0) || channel_frequency(i, j) != 0.0)
        {
          is_identity = false;
          break;
        }
      }
      // leave outer loop if we have reached the abortion criteria
      if (!is_identity) break;
    }
    return is_identity;
  }
}//namespace

#endif /* OPENMS_DATASTRUCTURES_UTILS_MATRIXUTILS_H */
