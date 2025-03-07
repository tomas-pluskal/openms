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
// $Maintainer: Clemens Groepl $
// $Authors: Hendrik Weisser $
// --------------------------------------------------------------------------

#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////

#include <OpenMS/ANALYSIS/MAPMATCHING/TransformationDescription.h>

#include <vector>

///////////////////////////

START_TEST(TransformationDescription, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace OpenMS;
using namespace std;


TransformationDescription* ptr = 0;
TransformationDescription* nullPointer = 0;
START_SECTION((TransformationDescription()))
	ptr = new TransformationDescription;
	TEST_NOT_EQUAL(ptr, nullPointer)
END_SECTION

START_SECTION((~TransformationDescription()))
	delete ptr;
END_SECTION

TransformationDescription::DataPoints data;
data.push_back(make_pair(0.0, 1.0));
data.push_back(make_pair(0.25, 1.5));
data.push_back(make_pair(0.5, 2.0));
data.push_back(make_pair(1.0, 3.0));



START_SECTION((TransformationDescription(const DataPoints& data)))
{
	ptr = new TransformationDescription(data);
  TEST_NOT_EQUAL(ptr, nullPointer)
	TEST_EQUAL(ptr->getDataPoints() == data, true);
	delete ptr;
}
END_SECTION

START_SECTION((const DataPoints& getDataPoints() const))
{
	TransformationDescription td;
	TEST_EQUAL(td.getDataPoints().empty(), true);
}
END_SECTION

START_SECTION((void setDataPoints(const DataPoints& data)))
{
	TransformationDescription td;
	td.fitModel("identity", Param());
	TEST_EQUAL(td.getModelType(), "identity");
	td.setDataPoints(data);
	// setting data points clears the model:
	TEST_EQUAL(td.getModelType(), "none");
	TEST_EQUAL(td.getDataPoints().size(), 4)
	TEST_EQUAL(td.getDataPoints() == data, true);

	TransformationDescription::DataPoints empty;
  empty.clear();
	td.setDataPoints(empty);
	TEST_EQUAL(td.getDataPoints().empty(), true)
}
END_SECTION

START_SECTION((double apply(double value) const))
{
	TransformationDescription td;
	TEST_EQUAL(td.apply(-0.5), -0.5);
	TEST_EQUAL(td.apply(1000), 1000);
	// tested further together with "fitModel"
}
END_SECTION

START_SECTION((const String& getModelType() const))
{
	TransformationDescription td;
	TEST_EQUAL(td.getModelType(), "none");
}
END_SECTION

START_SECTION((static void getModelTypes(StringList& result)))
{
	StringList result;
	TransformationDescription::getModelTypes(result);
	TEST_EQUAL(result.size(), 3);
	TEST_EQUAL(result.at(0), "linear");
	TEST_EQUAL(result.at(1), "b_spline");
	TEST_EQUAL(result.at(2), "interpolated");
}
END_SECTION


START_SECTION((void fitModel(const String& model_type, const Param& params=Param())))
{
	TransformationDescription td(data);
	Param params;
	td.fitModel("linear", params);
	TEST_EQUAL(td.getModelType(), "linear");
	TEST_REAL_SIMILAR(td.apply(0.0), 1.0);
	TEST_REAL_SIMILAR(td.apply(0.5), 2.0);
	TEST_REAL_SIMILAR(td.apply(1.0), 3.0);
	
  // special model type for reference files:
	td.fitModel("identity", Param());
	TEST_EQUAL(td.getModelType(), "identity");
	TEST_REAL_SIMILAR(td.apply(0.0), 0.0);
	TEST_REAL_SIMILAR(td.apply(0.5), 0.5);
	TEST_REAL_SIMILAR(td.apply(1.0), 1.0);
	// can't fit a different model to an "identity" transformation:
	td.fitModel("linear", params);
	TEST_EQUAL(td.getModelType(), "identity");
}
END_SECTION

START_SECTION((void getModelParameters(Param& params) const))
{
	TransformationDescription td;
	Param params = td.getModelParameters();
	TEST_EQUAL(params, Param());
	params.setValue("slope", 2.5);
	params.setValue("intercept", -100.0);
	const Param const_params = params;
	td.fitModel("linear", const_params);
	params = td.getModelParameters();
	TEST_EQUAL(params, const_params);
}
END_SECTION

START_SECTION((TransformationDescription(const TransformationDescription& rhs)))
{
	TransformationDescription td(data);
	td.fitModel("linear", Param());
	TransformationDescription td2 = td;
	TEST_EQUAL(td.getModelType(), td2.getModelType());
	TEST_EQUAL(td.getDataPoints() == td2.getDataPoints(), true);
	Param params = td.getModelParameters();
	Param params2 = td2.getModelParameters();
	TEST_EQUAL(params, params2);
}
END_SECTION

START_SECTION((TransformationDescription& operator=(const TransformationDescription& rhs)))
{
	TransformationDescription td(data);
	td.fitModel("linear", Param());
	TransformationDescription td2;
	td2 = td;
	TEST_EQUAL(td.getModelType(), td2.getModelType());
	TEST_EQUAL(td.getDataPoints() == td2.getDataPoints(), true);
  Param params = td.getModelParameters();
  Param params2 = td2.getModelParameters();
	TEST_EQUAL(params, params2);
}
END_SECTION

START_SECTION((void invert()))
{

	// test null transformation:
	TransformationDescription td;
	td.fitModel("none", Param());
	td.invert();
	TEST_EQUAL(td.getModelType(), "none");

  // test inversion of data points:
	TransformationDescription td1;
  td1.setDataPoints(data);
  td1.invert();
  td1.invert();
  TEST_EQUAL(td1.getDataPoints() == data, true);


	// test linear transformation:
  TransformationDescription td2;
  td2.setDataPoints(data);
  Param params2;
	params2.setValue("slope", 2.0);
	params2.setValue("intercept", 47.12);
	td2.fitModel("linear", params2);
	TEST_REAL_SIMILAR(params2.getValue("slope"), 2.0);
  TEST_REAL_SIMILAR(params2.getValue("intercept"), 47.12);

  TransformationDescription td3;
  td3.setDataPoints(data);
  Param params3;
  td3.fitModel("linear", params3);
	td3.invert();
	TEST_EQUAL(td3.getModelType(), "linear");
	TEST_REAL_SIMILAR(td3.apply(1.0), 0.0);//control input values
	TEST_REAL_SIMILAR(td3.apply(2.0), 0.5);//control input values
	TEST_REAL_SIMILAR(td3.apply(3.0), 1.0);//control input values
	TEST_REAL_SIMILAR(td3.apply(4.0), 1.5);//control interpolation values
	TEST_REAL_SIMILAR(td3.apply(5.0), 2.0);//control interpolation values



	// test interpolated-linear transformation:
	TransformationDescription td4;
	td4.setDataPoints(data);
  Param params4;
	params4.setValue("interpolation_type", "cspline");
	td4.fitModel("interpolated", params4);
	td4.invert();
	TEST_EQUAL(td4.getModelType(), "interpolated");
	// pairs have changed...
	TEST_EQUAL(td4.getDataPoints() != data, true);
	td4.invert();
	// ... now they're back to the original:
	TEST_EQUAL(td4.getDataPoints() == data, true);
}
END_SECTION


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
