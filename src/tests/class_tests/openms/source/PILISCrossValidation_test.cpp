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

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////
#include <OpenMS/ANALYSIS/ID/PILISCrossValidation.h>
///////////////////////////

using namespace OpenMS;
using namespace std;

START_TEST(PILISCrossValidation, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

PILISCrossValidation* ptr = 0;
PILISCrossValidation* nullPointer = 0;
START_SECTION(PILISCrossValidation())
{
	ptr = new PILISCrossValidation();
	TEST_NOT_EQUAL(ptr, nullPointer)
}
END_SECTION

START_SECTION(virtual ~PILISCrossValidation())
{
	delete ptr;
}
END_SECTION

START_SECTION((PILISCrossValidation(const PILISCrossValidation &rhs)))
{
  // TODO
}
END_SECTION

START_SECTION((PILISCrossValidation& operator=(const PILISCrossValidation &rhs)))
{
  // TODO
}
END_SECTION

START_SECTION((void setOptions(const Map<String, Option> &rhs)))
{
  // TODO
}
END_SECTION

START_SECTION((void setOption(const String &name, const Option &option)))
{
  // TODO
}
END_SECTION

START_SECTION((void apply(Param &PILIS_param, const PILISModel &base_model, const std::vector< Peptide > &peptides)))
{
  // TODO
}
END_SECTION

START_SECTION((double scoreHits(const std::vector< std::vector< std::vector< RichPeakSpectrum > > > &sim_spectra, const std::vector< std::vector< RichPeakSpectrum > > &exp_spectra)))
{
  // TODO
}
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST



