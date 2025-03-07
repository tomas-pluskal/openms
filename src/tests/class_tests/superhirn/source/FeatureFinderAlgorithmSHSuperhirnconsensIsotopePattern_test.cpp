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
// $Maintainer: Florian Zeller$
// $Authors: Florian Zeller$
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include "test_config.h"

///////////////////////////

#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/CentroidPeak.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/MSPeak.h>
#include <OpenMS/TRANSFORMATIONS/FEATUREFINDER/SUPERHIRN/ConsensusIsotopePattern.h>

///////////////////////////

START_TEST(ConsensusIsotopePattern, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace OpenMS;
using namespace std;

ConsensusIsotopePattern* ptr = 0;
ConsensusIsotopePattern* nullPtr = 0;

START_SECTION((ConsensusIsotopePattern()))
	ptr = new ConsensusIsotopePattern();
	TEST_NOT_EQUAL(ptr,nullPtr)
END_SECTION

START_SECTION((~ConsensusIsotopePattern()))
	delete ptr;
END_SECTION

ptr = new ConsensusIsotopePattern();

START_SECTION((ConsensusIsotopePattern::operator=(const ConsensusIsotopePattern & tmp)))
	ConsensusIsotopePattern i;
	ConsensusIsotopePattern other = i;
END_SECTION

START_SECTION(TODO)
// 			// constructs the consensus pattern:
// 			void constructConsusPattern();
// 			// order an isotope trace in the correct cluster:
// 			void addIsotopeTrace(double, double);
// 			// condenses the pattern, make average peaks from the traces:
// 			void condensIsotopePattern(std::pair<std::vector<double>, std::vector<double> >*);
// 
// 			///////////////////////////////
// 			// start here all the get / set
// 			// function to access the
// 			// variables of the class
// 
// 			std::map<double, double>::iterator getConsensIsotopeIteratorStart();
// 			std::map<double, double>::iterator getConsensIsotopeIteratorEnd();
END_SECTION
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_TEST
