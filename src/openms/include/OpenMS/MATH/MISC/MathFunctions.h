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
// $Maintainer: Stephan Aiche$
// $Authors: Marc Sturm $
// --------------------------------------------------------------------------

#ifndef OPENMS_MATH_MISC_MATHFUNCTIONS_H
#define OPENMS_MATH_MISC_MATHFUNCTIONS_H

#include <OpenMS/CONCEPT/Types.h>

#include <cmath>

namespace OpenMS
{
  /**
    @brief %Math namespace.

    Contains mathematical auxiliary functions.

    @ingroup Concept
  */
  namespace Math
  {
    /**
      @brief rounds @p x up to the next decimal power 10 ^ @p decPow

      @verbatim
      e.g.: (123.0 , 1)  => 130
            (123.0 , 2)  => 200
                (0.123 ,-2)  => 0.13 ( 10^-2 = 0.01 )
      @endverbatim

      @ingroup MathFunctionsMisc
    */
    inline static double ceilDecimal(double x, int decPow)
    {
      return (ceil(x / pow(10.0, decPow))) * pow(10.0, decPow); // decimal shift right, ceiling, decimal shift left
    }

    /**
        @brief rounds @p x to the next decimal power 10 ^ @p decPow

        @verbatim
        e.g.: (123.0 , 1)  => 120
              (123.0 , 2)  => 100
        @endverbatim

        @ingroup MathFunctionsMisc
    */
    inline static double roundDecimal(double x, int decPow)
    {
      if (x > 0)
        return (floor(0.5 + x / pow(10.0, decPow))) * pow(10.0, decPow);

      return -((floor(0.5 + fabs(x) / pow(10.0, decPow))) * pow(10.0, decPow));
    }

    /**
        @brief transforms point @p x of interval [left1,right1] into interval [left2,right2]

        @ingroup MathFunctionsMisc
    */
    inline static double intervalTransformation(double x, double left1, double right1, double left2, double right2)
    {
      return left2 + (x - left1) * (right2 - left2) / (right1 - left1);
    }

    /**
        @brief Transforms a number from linear to log10 scale. Avoids negative logarithms by adding 1.

        @param x The number to transform

        @ingroup MathFunctionsMisc
    */
    inline double linear2log(double x)
    {
      return log10(x + 1);     //+1 to avoid negative logarithms
    }

    /**
        @brief Transforms a number from log10 to to linear scale. Subtracts the 1 added by linear2log(double)

        @param x The number to transform

        @ingroup MathFunctionsMisc
    */
    inline double log2linear(double x)
    {
      return pow(10, x) - 1;
    }

    /**
        @brief Returns true if the given integer is odd

        @ingroup MathFunctionsMisc
    */
    inline bool isOdd(UInt x)
    {
      return (x & 1) != 0;
    }

    /**
        @brief Rounds the value

        @ingroup MathFunctionsMisc
    */
    template <typename T>
    T round(T x)
    {
      if (x >= T(0))
      {
        return T(floor(x + T(0.5)));
      }
      else
      {
        return T(ceil(x - T(0.5)));
      }
    }

    /**
        @brief Returns if @p a is approximately equal @p b , allowing a tolerance of @p tol

        @ingroup MathFunctionsMisc
    */
    inline static bool approximatelyEqual(double a, double b, double tol)
    {
      return std::fabs(a - b) <= tol;
    }

    /**
      @brief Returns the greatest common divisor (gcd) of two numbers by applying the Euclidean algorithm.
      @param a A number.
      @param b A number.
      @return The greatest common divisor.
      @see gcd(T a, T b, T& a1, T& b1)
      @ingroup MathFunctionsMisc
     */
    template <typename T>
    T gcd(T a, T b)
    {
      T c;
      while (b != 0)
      {
        c = a % b;
        a = b;
        b = c;
      }
      return a;
    }

    /**
     @brief Returns the greatest common divisor by applying the extended Euclidean algorithm (Knuth TAoCP vol. 2, p342).
     Calculates u1, u2 and u3 (which is returned) so that a * u1 + b * u2 = u3 = gcd(a, b, u1, u2)

     @param a A number.
     @param b A number.
     @param u1 A reference to the number to be returned (see the above formula).
     @param u2 A reference to the number to be returned (see the above formula).
     @return The greatest common divisor.
     @see gcd(T, T)
     @ingroup MathFunctionsMisc
     */
    template <typename T>
    T gcd(T a, T b, T & u1, T & u2)
    {
      u1 = 1;
      u2 = 0;
      T u3 = a;

      T v1 = 0;
      T v2 = 1;
      T v3 = b;

      while (v3 != 0)
      {
        T q = u3 / v3;
        T t1 = u1 - v1 * q;
        T t2 = u2 - v2 * q;
        T t3 = u3 - v3 * q;

        u1 = v1;
        u2 = v2;
        u3 = v3;

        v1 = t1;
        v2 = t2;
        v3 = t3;
      }

      return u3;
    }

  }   // namespace Math
} // namespace OpenMS

#endif // OPENMS_MATH_MISC_MATHFUNCTIONS_H
