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
// $Authors: Marc Sturm $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////

#include <OpenMS/METADATA/MetaInfoRegistry.h>

///////////////////////////

START_TEST(MetaInfoRegistry, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace OpenMS;
using namespace std;

MetaInfoRegistry* test = 0;
MetaInfoRegistry* nullPointer = 0;
START_SECTION((MetaInfoRegistry()))
	test = new MetaInfoRegistry;
  TEST_NOT_EQUAL(test, nullPointer)
END_SECTION


START_SECTION((~MetaInfoRegistry()))
	delete test;
END_SECTION

MetaInfoRegistry mir;

START_SECTION((UInt registerName(const String& name, const String& description, const String& unit="") const))
	UInt testname = mir.registerName("testname","this is just a test");
	TEST_EQUAL(1024,testname)
	UInt retention_time = mir.registerName("retention time","this is just another test","sec");
	TEST_EQUAL(1025,retention_time)
	UInt another_testname = mir.registerName("another testname", "i will be set later", "so am i");
	TEST_EQUAL(1026,another_testname);
END_SECTION

START_SECTION((void setDescription(UInt index, const String& description) ))
	mir.setDescription(1026, "foo");
	TEST_EQUAL(mir.getDescription(1026), "foo")
END_SECTION

START_SECTION((void setDescription(const String& name, const String& description) ))
	mir.setDescription("another testname", "bar");
	TEST_EQUAL(mir.getDescription(1026), "bar")
END_SECTION

START_SECTION((void setUnit(UInt index, const String& unit) ))
	mir.setUnit(1026, "foo");
	TEST_EQUAL(mir.getUnit(1026), "foo")
END_SECTION

START_SECTION((void setUnit(const String& name, const String& unit) ))
	mir.setUnit("another testname", "bar");
	TEST_EQUAL(mir.getUnit(1026), "bar")
END_SECTION

START_SECTION((UInt getIndex(const String& name) const))
	UInt tmp;
	tmp = mir.getIndex ("testname");
	TEST_EQUAL(1024,tmp)
	tmp = mir.getIndex ("retention time");
	TEST_EQUAL(1025,tmp)
	tmp = mir.getIndex ("isotopic_range");
	TEST_EQUAL(1,tmp)
	tmp = mir.getIndex ("cluster_id");
	TEST_EQUAL(2,tmp)
	tmp = mir.getIndex ("unregistered name");
	TEST_EQUAL(1027,tmp)
	tmp = mir.getIndex ("another unregistered name");
	TEST_EQUAL(1028,tmp)
	tmp = mir.getIndex ("unregistered name");
	TEST_EQUAL(1027,tmp)
END_SECTION

START_SECTION((String getName(UInt index) const ))
	string tmp;
	tmp = mir.getName (1);
	TEST_EQUAL(tmp,"isotopic_range")
	tmp = mir.getName (2);
	TEST_EQUAL(tmp,"cluster_id")
	tmp = mir.getName (3);
	TEST_EQUAL(tmp,"label")
	tmp = mir.getName (4);
	TEST_EQUAL(tmp,"icon")
	tmp = mir.getName (1024);
	TEST_EQUAL(tmp,"testname")
	tmp = mir.getName (1025);
	TEST_EQUAL(tmp,"retention time")
END_SECTION

START_SECTION((String getDescription(UInt index) const ))
	string tmp;
	tmp = mir.getDescription(1024);
	TEST_EQUAL(tmp,string("this is just a test"))
	tmp = mir.getDescription(1025);
	TEST_EQUAL(tmp,string("this is just another test"))
	tmp = mir.getDescription(1);
	TEST_EQUAL(tmp,string("consecutive numbering of the peaks in an isotope pattern. 0 is the monoisotopic peak"))
	tmp = mir.getDescription(2);
	TEST_EQUAL(tmp,string("consecutive numbering of isotope clusters in a spectrum"))
END_SECTION

START_SECTION((String getDescription(const String& name) const ))
	string tmp;
	tmp = mir.getDescription("testname");
	TEST_EQUAL(tmp,string("this is just a test"))
	tmp = mir.getDescription("retention time");
	TEST_EQUAL(tmp,string("this is just another test"))
	tmp = mir.getDescription("isotopic_range");
	TEST_EQUAL(tmp,string("consecutive numbering of the peaks in an isotope pattern. 0 is the monoisotopic peak"))
	tmp = mir.getDescription("cluster_id");
	TEST_EQUAL(tmp,string("consecutive numbering of isotope clusters in a spectrum"))
END_SECTION

START_SECTION((String getUnit(UInt index) const ))
	string tmp;
	tmp = mir.getUnit(1024);
	TEST_EQUAL(tmp,string(""))
	tmp = mir.getUnit(1025);
	TEST_EQUAL(tmp,string("sec"))
	tmp = mir.getUnit(1);
	TEST_EQUAL(tmp,string(""))
	tmp = mir.getUnit(2);
	TEST_EQUAL(tmp,string(""))
END_SECTION

START_SECTION((String getUnit(const String& name) const ))
	string tmp;
	tmp = mir.getUnit("testname");
	TEST_EQUAL(tmp,string(""))
	tmp = mir.getUnit("retention time");
	TEST_EQUAL(tmp,string("sec"))
	tmp = mir.getUnit("isotopic_range");
	TEST_EQUAL(tmp,string(""))
	tmp = mir.getUnit("cluster_id");
	TEST_EQUAL(tmp,string(""))
END_SECTION

START_SECTION((MetaInfoRegistry(const MetaInfoRegistry& rhs)))
	MetaInfoRegistry mir2(mir);
	TEST_EQUAL(1024,mir2.getIndex ("testname"))
	TEST_EQUAL(1025,mir2.getIndex ("retention time"))
	TEST_EQUAL(mir2.getName(1),"isotopic_range")
	TEST_EQUAL(mir2.getName(1024),"testname")
	TEST_EQUAL(mir2.getName(1025),"retention time")
	TEST_EQUAL(mir2.getDescription(1024),string("this is just a test"))
	TEST_EQUAL(mir2.getDescription(1025),string("this is just another test"))
	TEST_EQUAL(mir2.getDescription("testname"),string("this is just a test"))
	TEST_EQUAL(mir2.getDescription("retention time"),string("this is just another test"))
	TEST_EQUAL(mir2.getUnit(1024),string(""))
	TEST_EQUAL(mir2.getUnit(1025),string("sec"))
	TEST_EQUAL(mir2.getUnit("testname"),string(""))
	TEST_EQUAL(mir2.getUnit("retention time"),string("sec"))
END_SECTION

START_SECTION((MetaInfoRegistry& operator = (const MetaInfoRegistry& rhs)))
	MetaInfoRegistry mir2;
	mir2 = mir;
	TEST_EQUAL(1024,mir2.getIndex ("testname"));
	TEST_EQUAL(1025,mir2.getIndex ("retention time"));
	TEST_EQUAL(mir2.getName(1),"isotopic_range");
	TEST_EQUAL(mir2.getName(1024),"testname");
	TEST_EQUAL(mir2.getName(1025),"retention time");
	TEST_EQUAL(mir2.getDescription(1024),string("this is just a test"))
	TEST_EQUAL(mir2.getDescription(1025),string("this is just another test"))
	TEST_EQUAL(mir2.getDescription("testname"),string("this is just a test"))
	TEST_EQUAL(mir2.getDescription("retention time"),string("this is just another test"))
	TEST_EQUAL(mir2.getUnit(1024),string(""))
	TEST_EQUAL(mir2.getUnit(1025),string("sec"))
	TEST_EQUAL(mir2.getUnit("testname"),string(""))
	TEST_EQUAL(mir2.getUnit("retention time"),string("sec"))
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
