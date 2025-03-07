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
// $Maintainer: George Rosenberger $
// $Authors: George Rosenberger $
// --------------------------------------------------------------------------
#include <OpenMS/CONCEPT/ClassTest.h>
#include <OpenMS/test_config.h>

///////////////////////////
#include <OpenMS/ANALYSIS/OPENSWATH/MRMAssay.h>
#include <OpenMS/FORMAT/TraMLFile.h>
///////////////////////////

using namespace OpenMS;
using namespace std;

START_TEST(MRMAssay, "$Id$")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

MRMAssay * ptr = 0;
MRMAssay* nullPointer = 0;

class MRMAssay_test : public MRMAssay
{
  public:
    bool isUIS_test(const double fragment_ion, std::vector<double> ions, const double mz_threshold)
    {
      return isUIS_(fragment_ion, ions, mz_threshold);
    }
    void isSiteUIS_test(const AASequence sequence, ReactionMonitoringTransition & tr)
    {
      isSiteUIS_(sequence, tr);
    }
    int getSwath_test(const std::vector<std::pair<double, double> > swathes, const double precursor_mz)
    {
      return getSwath_(swathes, precursor_mz);
    }
    bool isInSwath_test(const std::vector<std::pair<double, double> > swathes, const double precursor_mz, const double product_mz)
    {
      return isInSwath_(swathes, precursor_mz, product_mz);
    }
    std::vector<std::vector<size_t> > nchoosekcombinations_test(std::vector<size_t> n, size_t k)
    {
      return nchoosekcombinations_(n, k);
    }
    std::vector<OpenMS::AASequence> addModificationsSequences_test(std::vector<OpenMS::AASequence> sequences, std::vector<std::vector<size_t> > mods_combs, OpenMS::String modification)
    {
      return addModificationsSequences_(sequences, mods_combs, modification);
    }
    std::vector<OpenMS::AASequence> combineModifications_test(OpenMS::AASequence sequence)
    {
      return combineModifications_(sequence);
    }
};

START_SECTION(MRMAssay())
{
  ptr = new MRMAssay();
  TEST_NOT_EQUAL(ptr, nullPointer)
}
END_SECTION

START_SECTION(~MRMAssay())
{
  delete ptr;
}
END_SECTION

START_SECTION(bool MRMAssay::isUIS_(const double fragment_ion, std::vector<double> ions, const double mz_threshold); )
{
  MRMAssay_test mrma;

  std::vector<double> ions;
  ions.push_back(100.00);
  ions.push_back(100.01);
  ions.push_back(100.12);
  ions.push_back(101.11);

  TEST_EQUAL(mrma.isUIS_test(100.065, ions, 0.05), true);
  TEST_EQUAL(mrma.isUIS_test(100.06, ions, 0.06), false);
}
END_SECTION

START_SECTION(void MRMAssay::isSiteUIS_(const AASequence sequence, ReactionMonitoringTransition & tr))
{
  MRMAssay_test mrma;
  MRMIonSeries mrmis;
  AASequence sequence = AASequence::fromString("PEPT(Phospho)ID(Oxidation)EK");
  ReactionMonitoringTransition tr1, tr2, tr3, tr4, btr1, btr2, btr3, btr4;

  mrmis.annotateTransitionCV(tr1, "y5^3");
  mrma.isSiteUIS_test(sequence, tr1);

  TEST_EQUAL(tr1.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr1.getMetaValue("site_identifying_class"), "revdiag")

  mrmis.annotateTransitionCV(tr2, "y4^3");
  mrma.isSiteUIS_test(sequence, tr2);

  TEST_EQUAL(tr2.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr2.getMetaValue("site_identifying_class"), "revnext")

  mrmis.annotateTransitionCV(tr3, "b4^3");
  mrma.isSiteUIS_test(sequence, tr3);

  TEST_EQUAL(tr3.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr3.getMetaValue("site_identifying_class"), "fwddiag")

  mrmis.annotateTransitionCV(tr4, "b3^3");
  mrma.isSiteUIS_test(sequence, tr4);

  TEST_EQUAL(tr4.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr4.getMetaValue("site_identifying_class"), "fwdnext")
}

END_SECTION
START_SECTION(void MRMAssay::isSiteUIS_(const AASequence sequence, ReactionMonitoringTransition & tr))
{
  MRMAssay_test mrma;
  MRMIonSeries mrmis;
  AASequence sequence = AASequence::fromString("PEPT(Phospho)D(Oxidation)IEK");
  ReactionMonitoringTransition tr1, tr2, tr3, tr4, btr1, btr2, btr3, btr4;

  mrmis.annotateTransitionCV(tr1, "y5^3");
  mrma.isSiteUIS_test(sequence, tr1);

  TEST_EQUAL(tr1.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr1.getMetaValue("site_identifying_class"), "revdiag")

  mrmis.annotateTransitionCV(tr2, "y4^3");
  mrma.isSiteUIS_test(sequence, tr2);

  TEST_EQUAL(tr2.getMetaValue("site_identifying_transition"), "4,5")
  TEST_EQUAL(tr2.getMetaValue("site_identifying_class"), "revnext,revdiag")

  mrmis.annotateTransitionCV(tr3, "b4^3");
  mrma.isSiteUIS_test(sequence, tr3);

  TEST_EQUAL(tr3.getMetaValue("site_identifying_transition"), "4,5")
  TEST_EQUAL(tr3.getMetaValue("site_identifying_class"), "fwddiag,fwdnext")

  mrmis.annotateTransitionCV(tr4, "b3^3");
  mrma.isSiteUIS_test(sequence, tr4);

  TEST_EQUAL(tr4.getMetaValue("site_identifying_transition"), "4")
  TEST_EQUAL(tr4.getMetaValue("site_identifying_class"), "fwdnext")
}
END_SECTION

START_SECTION(int MRMAssay::getSwath_(const std::vector<std::pair<double, double> > swathes, const double precursor_mz))
{
  MRMAssay_test mrma;
  std::vector<std::pair<double, double> > swathes;
  swathes.push_back(std::make_pair(400, 425));
  swathes.push_back(std::make_pair(424, 450));
  swathes.push_back(std::make_pair(449, 475));
  swathes.push_back(std::make_pair(474, 500));
  swathes.push_back(std::make_pair(499, 525));
  swathes.push_back(std::make_pair(524, 550));
  swathes.push_back(std::make_pair(549, 575));
  swathes.push_back(std::make_pair(574, 600));
  swathes.push_back(std::make_pair(599, 625));
  swathes.push_back(std::make_pair(624, 650));
  swathes.push_back(std::make_pair(649, 675));
  swathes.push_back(std::make_pair(674, 700));
  swathes.push_back(std::make_pair(699, 725));
  swathes.push_back(std::make_pair(724, 750));
  swathes.push_back(std::make_pair(749, 775));
  swathes.push_back(std::make_pair(774, 800));
  swathes.push_back(std::make_pair(799, 825));
  swathes.push_back(std::make_pair(824, 850));
  swathes.push_back(std::make_pair(849, 875));
  swathes.push_back(std::make_pair(874, 900));
  swathes.push_back(std::make_pair(899, 925));
  swathes.push_back(std::make_pair(924, 950));
  swathes.push_back(std::make_pair(949, 975));
  swathes.push_back(std::make_pair(974, 1000));
  swathes.push_back(std::make_pair(999, 1025));
  swathes.push_back(std::make_pair(1024, 1050));
  swathes.push_back(std::make_pair(1049, 1075));
  swathes.push_back(std::make_pair(1074, 1100));
  swathes.push_back(std::make_pair(1099, 1125));
  swathes.push_back(std::make_pair(1124, 1150));
  swathes.push_back(std::make_pair(1149, 1175));
  swathes.push_back(std::make_pair(1174, 1200));

  TEST_EQUAL(mrma.getSwath_test(swathes, 427.229959), 1)
  TEST_EQUAL(mrma.getSwath_test(swathes, 685.8547721), 11)
  TEST_EQUAL(mrma.getSwath_test(swathes, 1685.8547721), -1)
  TEST_EQUAL(mrma.getSwath_test(swathes, -41.1), -1)
}
END_SECTION

START_SECTION(bool MRMAssay::isInSwath_(const std::vector<std::pair<double, double> > swathes, const double precursor_mz, const double product_mz))
{
  MRMAssay_test mrma;
  std::vector<std::pair<double, double> > swathes;
  swathes.push_back(std::make_pair(400, 425));
  swathes.push_back(std::make_pair(424, 450));
  swathes.push_back(std::make_pair(449, 475));
  swathes.push_back(std::make_pair(474, 500));
  swathes.push_back(std::make_pair(499, 525));
  swathes.push_back(std::make_pair(524, 550));
  swathes.push_back(std::make_pair(549, 575));
  swathes.push_back(std::make_pair(574, 600));
  swathes.push_back(std::make_pair(599, 625));
  swathes.push_back(std::make_pair(624, 650));
  swathes.push_back(std::make_pair(649, 675));
  swathes.push_back(std::make_pair(674, 700));
  swathes.push_back(std::make_pair(699, 725));
  swathes.push_back(std::make_pair(724, 750));
  swathes.push_back(std::make_pair(749, 775));
  swathes.push_back(std::make_pair(774, 800));
  swathes.push_back(std::make_pair(799, 825));
  swathes.push_back(std::make_pair(824, 850));
  swathes.push_back(std::make_pair(849, 875));
  swathes.push_back(std::make_pair(874, 900));
  swathes.push_back(std::make_pair(899, 925));
  swathes.push_back(std::make_pair(924, 950));
  swathes.push_back(std::make_pair(949, 975));
  swathes.push_back(std::make_pair(974, 1000));
  swathes.push_back(std::make_pair(999, 1025));
  swathes.push_back(std::make_pair(1024, 1050));
  swathes.push_back(std::make_pair(1049, 1075));
  swathes.push_back(std::make_pair(1074, 1100));
  swathes.push_back(std::make_pair(1099, 1125));
  swathes.push_back(std::make_pair(1124, 1150));
  swathes.push_back(std::make_pair(1149, 1175));
  swathes.push_back(std::make_pair(1174, 1200));

  TEST_EQUAL(mrma.isInSwath_test(swathes, 685.8547721, 427.229959), false)
  TEST_EQUAL(mrma.isInSwath_test(swathes, 685.8547721, 689), true)
}
END_SECTION

START_SECTION(std::vector<std::vector<size_t> > MRMAssay::nchoosekcombinations_(std::vector<size_t> n, size_t k))
{
  MRMAssay_test mrma;
  std::vector<size_t> n;
  size_t k = 5;

  for (size_t i = 1; i <= 16; i++)
  {
    n.push_back(i);
  }

  TEST_EQUAL(mrma.nchoosekcombinations_test(n, k).size(), 4368)
  TEST_EQUAL(mrma.nchoosekcombinations_test(n, k)[0].size(), 5)
}
END_SECTION

START_SECTION(std::vector<std::vector<size_t> > MRMAssay::nchoosekcombinations_(std::vector<size_t> n, size_t k))
{
  MRMAssay_test mrma;
  std::vector<size_t> n;
  size_t k = 3;

  for (size_t i = 1; i <= 5; i++)
  {
    n.push_back(i);
  }

  std::vector<std::vector<size_t> > res = mrma.nchoosekcombinations_test(n, k);

  TEST_EQUAL(res.size(), 10)
  TEST_EQUAL(res[0].size(), 3)

  TEST_EQUAL(res[0][0], 1)
  TEST_EQUAL(res[0][1], 2)
  TEST_EQUAL(res[0][2], 3)

  TEST_EQUAL(res[1][0], 1)
  TEST_EQUAL(res[1][1], 2)
  TEST_EQUAL(res[1][2], 4)

  TEST_EQUAL(res[2][0], 1)
  TEST_EQUAL(res[2][1], 2)
  TEST_EQUAL(res[2][2], 5)

  TEST_EQUAL(res[3][0], 1)
  TEST_EQUAL(res[3][1], 3)
  TEST_EQUAL(res[3][2], 4)

  TEST_EQUAL(res[4][0], 1)
  TEST_EQUAL(res[4][1], 3)
  TEST_EQUAL(res[4][2], 5)

  TEST_EQUAL(res[5][0], 1)
  TEST_EQUAL(res[5][1], 4)
  TEST_EQUAL(res[5][2], 5)

  TEST_EQUAL(res[6][0], 2)
  TEST_EQUAL(res[6][1], 3)
  TEST_EQUAL(res[6][2], 4)

  TEST_EQUAL(res[7][0], 2)
  TEST_EQUAL(res[7][1], 3)
  TEST_EQUAL(res[7][2], 5)

  TEST_EQUAL(res[8][0], 2)
  TEST_EQUAL(res[8][1], 4)
  TEST_EQUAL(res[8][2], 5)

  TEST_EQUAL(res[9][0], 3)
  TEST_EQUAL(res[9][1], 4)
  TEST_EQUAL(res[9][2], 5)
}
END_SECTION

START_SECTION(std::vector<OpenMS::AASequence> MRMAssay::addModificationsSequences_(std::vector<OpenMS::AASequence> sequences, std::vector<std::vector<size_t> > mods_combs, OpenMS::String modification))
{
  MRMAssay_test mrma;

  AASequence sequence = AASequence::fromString("PEPTDIEK");
  std::vector<OpenMS::AASequence> sequences;
  sequences.push_back(sequence);

  std::vector<size_t> no;
  no.push_back(0);
  no.push_back(2);
  no.push_back(4);
  no.push_back(7);

  std::vector<std::vector<size_t> > mods_combs_o = mrma.nchoosekcombinations_test(no, 1);

  sequences = mrma.addModificationsSequences_test(sequences, mods_combs_o, String("Oxidation"));

  std::vector<size_t> np;
  np.push_back(3);
  np.push_back(4);
  np.push_back(7);

  std::vector<std::vector<size_t> > mods_combs_p = mrma.nchoosekcombinations_test(np, 1);

  sequences = mrma.addModificationsSequences_test(sequences, mods_combs_p, String("Phospho"));

  TEST_EQUAL(sequences.size(), 10)
  TEST_EQUAL(sequences[0].toString(), String("P(Oxidation)EPT(Phospho)DIEK"));
  TEST_EQUAL(sequences[1].toString(), String("P(Oxidation)EPTD(Phospho)IEK"));
  TEST_EQUAL(sequences[2].toString(), String("P(Oxidation)EPTDIEK(Phospho)"));
  TEST_EQUAL(sequences[3].toString(), String("PEP(Oxidation)T(Phospho)DIEK"));
  TEST_EQUAL(sequences[4].toString(), String("PEP(Oxidation)TD(Phospho)IEK"));
  TEST_EQUAL(sequences[5].toString(), String("PEP(Oxidation)TDIEK(Phospho)"));
  TEST_EQUAL(sequences[6].toString(), String("PEPT(Phospho)D(Oxidation)IEK"));
  TEST_EQUAL(sequences[7].toString(), String("PEPTD(Oxidation)IEK(Phospho)"));
  TEST_EQUAL(sequences[8].toString(), String("PEPT(Phospho)DIEK(Oxidation)"));
  TEST_EQUAL(sequences[9].toString(), String("PEPTD(Phospho)IEK(Oxidation)"));
}
END_SECTION

START_SECTION(std::vector<OpenMS::AASequence> MRMAssay::combineModifications_(OpenMS::AASequence sequence))
{
  MRMAssay_test mrma;

  std::vector<AASequence> sequences = mrma.combineModifications_test(AASequence::fromString("PEPT(Phospho)DIEK"));

  TEST_EQUAL(sequences.size(), 3)
  TEST_EQUAL(sequences[0], AASequence::fromString("PEPT(Phospho)DIEK"));
  TEST_EQUAL(sequences[1], AASequence::fromString("PEPTD(Phospho)IEK"));
  TEST_EQUAL(sequences[2], AASequence::fromString("PEPTDIEK(Phospho)"));
}
END_SECTION

START_SECTION(void reannotateTransitions(OpenMS::TargetedExperiment& exp, double mz_threshold, std::vector<String> fragment_types, std::vector<size_t> fragment_charges, bool enable_alternative_localizations, bool enable_reannotation, bool enable_losses))
{
  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_reannotateTransitions_input.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  double product_mz_threshold1 = 0.05;
  std::vector<String> fragment_types1;
  fragment_types1.push_back(String("y"));
  std::vector<size_t> fragment_charges1;
  fragment_charges1.push_back(2);
  bool enable_alternative_localizations1 = false;
  bool enable_reannotation1 = true;
  bool enable_losses1 = false;

  String out1 = "MRMAssay_reannotateTransitions_output_1.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.reannotateTransitions(targeted_exp1, product_mz_threshold1, fragment_types1, fragment_charges1, enable_alternative_localizations1, enable_reannotation1, enable_losses1);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

  double product_mz_threshold2 = 0.05;
  std::vector<String> fragment_types2;
  fragment_types2.push_back(String("y"));
  fragment_types2.push_back(String("b"));
  std::vector<size_t> fragment_charges2;
  fragment_charges2.push_back(2);
  fragment_charges2.push_back(3);
  bool enable_alternative_localizations2 = false;
  bool enable_reannotation2 = true;
  bool enable_losses2 = true;

  String out2 = "MRMAssay_reannotateTransitions_output_2.TraML";

  TargetedExperiment targeted_exp2 = targeted_exp;

  mrma.reannotateTransitions(targeted_exp2, product_mz_threshold2, fragment_types2, fragment_charges2, enable_alternative_localizations2, enable_reannotation2, enable_losses2);

  String test2;
  NEW_TMP_FILE(test2);
  traml.store(test2, targeted_exp2);

  TEST_FILE_EQUAL(test2.c_str(), OPENMS_GET_TEST_DATA_PATH(out2))

  double product_mz_threshold3 = 0.05;
  std::vector<String> fragment_types3;
  fragment_types3.push_back(String("y"));
  std::vector<size_t> fragment_charges3;
  fragment_charges3.push_back(2);
  bool enable_alternative_localizations3 = true;
  bool enable_reannotation3 = true;
  bool enable_losses3 = false;

  String out3 = "MRMAssay_reannotateTransitions_output_3.TraML";

  TargetedExperiment targeted_exp3 = targeted_exp;

  mrma.reannotateTransitions(targeted_exp3, product_mz_threshold3, fragment_types3, fragment_charges3, enable_alternative_localizations3, enable_reannotation3, enable_losses3);

  String test3;
  NEW_TMP_FILE(test3);
  traml.store(test3, targeted_exp3);

  TEST_FILE_EQUAL(test3.c_str(), OPENMS_GET_TEST_DATA_PATH(out3))
}
END_SECTION

START_SECTION(void restrictTransitions(OpenMS::TargetedExperiment& exp, double lower_mz_limit, double upper_mz_limit, std::vector<std::pair<double, double> > swathes))
{
  std::vector<std::pair<double, double> > swathes;
  swathes.push_back(std::make_pair(400, 425));
  swathes.push_back(std::make_pair(424, 450));
  swathes.push_back(std::make_pair(449, 475));
  swathes.push_back(std::make_pair(474, 500));
  swathes.push_back(std::make_pair(499, 525));
  swathes.push_back(std::make_pair(524, 550));
  swathes.push_back(std::make_pair(549, 575));
  swathes.push_back(std::make_pair(574, 600));
  swathes.push_back(std::make_pair(599, 625));
  swathes.push_back(std::make_pair(624, 650));
  swathes.push_back(std::make_pair(649, 675));
  swathes.push_back(std::make_pair(674, 700));
  swathes.push_back(std::make_pair(699, 725));
  swathes.push_back(std::make_pair(724, 750));
  swathes.push_back(std::make_pair(749, 775));
  swathes.push_back(std::make_pair(774, 800));
  swathes.push_back(std::make_pair(799, 825));
  swathes.push_back(std::make_pair(824, 850));
  swathes.push_back(std::make_pair(849, 875));
  swathes.push_back(std::make_pair(874, 900));
  swathes.push_back(std::make_pair(899, 925));
  swathes.push_back(std::make_pair(924, 950));
  swathes.push_back(std::make_pair(949, 975));
  swathes.push_back(std::make_pair(974, 1000));
  swathes.push_back(std::make_pair(999, 1025));
  swathes.push_back(std::make_pair(1024, 1050));
  swathes.push_back(std::make_pair(1049, 1075));
  swathes.push_back(std::make_pair(1074, 1100));
  swathes.push_back(std::make_pair(1099, 1125));
  swathes.push_back(std::make_pair(1124, 1150));
  swathes.push_back(std::make_pair(1149, 1175));
  swathes.push_back(std::make_pair(1174, 1200));

  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_restrictTransitions_input.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  double lower_mz_limit = 400;
  double upper_mz_limit = 2000;

  String out1 = "MRMAssay_restrictTransitions_output.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.restrictTransitions(targeted_exp1, lower_mz_limit, upper_mz_limit, swathes);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

}
END_SECTION

START_SECTION(void detectingTransitions(OpenMS::TargetedExperiment& exp, int min_transitions, int max_transitions))
{
  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_detectingTransitions_input.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  int min_transitions = 4;
  int max_transitions = 6;

  String out1 = "MRMAssay_detectingTransitions_output.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.detectingTransitions(targeted_exp1, min_transitions, max_transitions);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

}
END_SECTION

START_SECTION(void uisTransitions(OpenMS::TargetedExperiment& exp, std::vector<String> fragment_types, std::vector<size_t> fragment_charges, bool enable_losses, bool enable_uis_scoring, bool enable_site_scoring, double mz_threshold, std::vector<std::pair<double, double> > swathes))
{
  std::vector<std::pair<double, double> > swathes;
  swathes.push_back(std::make_pair(400, 425));
  swathes.push_back(std::make_pair(424, 450));
  swathes.push_back(std::make_pair(449, 475));
  swathes.push_back(std::make_pair(474, 500));
  swathes.push_back(std::make_pair(499, 525));
  swathes.push_back(std::make_pair(524, 550));
  swathes.push_back(std::make_pair(549, 575));
  swathes.push_back(std::make_pair(574, 600));
  swathes.push_back(std::make_pair(599, 625));
  swathes.push_back(std::make_pair(624, 650));
  swathes.push_back(std::make_pair(649, 675));
  swathes.push_back(std::make_pair(674, 700));
  swathes.push_back(std::make_pair(699, 725));
  swathes.push_back(std::make_pair(724, 750));
  swathes.push_back(std::make_pair(749, 775));
  swathes.push_back(std::make_pair(774, 800));
  swathes.push_back(std::make_pair(799, 825));
  swathes.push_back(std::make_pair(824, 850));
  swathes.push_back(std::make_pair(849, 875));
  swathes.push_back(std::make_pair(874, 900));
  swathes.push_back(std::make_pair(899, 925));
  swathes.push_back(std::make_pair(924, 950));
  swathes.push_back(std::make_pair(949, 975));
  swathes.push_back(std::make_pair(974, 1000));
  swathes.push_back(std::make_pair(999, 1025));
  swathes.push_back(std::make_pair(1024, 1050));
  swathes.push_back(std::make_pair(1049, 1075));
  swathes.push_back(std::make_pair(1074, 1100));
  swathes.push_back(std::make_pair(1099, 1125));
  swathes.push_back(std::make_pair(1124, 1150));
  swathes.push_back(std::make_pair(1149, 1175));
  swathes.push_back(std::make_pair(1174, 1200));

  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_uisTransitions_input_1.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  std::vector<String> fragment_types1;
  fragment_types1.push_back(String("y"));
  std::vector<size_t> fragment_charges1;
  fragment_charges1.push_back(2);
  bool enable_losses1 = false;
  bool enable_uis_scoring1 = false;
  bool enable_site_scoring1 = true;
  double product_mz_threshold1 = 0.05;

  String out1 = "MRMAssay_uisTransitions_output_1.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.uisTransitions(targeted_exp1, fragment_types1, fragment_charges1, enable_losses1, enable_uis_scoring1, enable_site_scoring1, product_mz_threshold1, swathes);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

  std::vector<String> fragment_types2;
  fragment_types2.push_back(String("y"));
  std::vector<size_t> fragment_charges2;
  fragment_charges2.push_back(2);
  bool enable_losses2 = false;
  bool enable_uis_scoring2 = true;
  bool enable_site_scoring2 = false;
  double product_mz_threshold2 = 0.05;

  String out2 = "MRMAssay_uisTransitions_output_2.TraML";

  TargetedExperiment targeted_exp2 = targeted_exp;

  mrma.uisTransitions(targeted_exp2, fragment_types2, fragment_charges2, enable_losses2, enable_uis_scoring2, enable_site_scoring2, product_mz_threshold2, swathes);

  String test2;
  NEW_TMP_FILE(test2);
  traml.store(test2, targeted_exp2);

  TEST_FILE_EQUAL(test2.c_str(), OPENMS_GET_TEST_DATA_PATH(out2))

}
END_SECTION

START_SECTION(void uisTransitions(OpenMS::TargetedExperiment& exp, std::vector<String> fragment_types, std::vector<size_t> fragment_charges, bool enable_losses, bool enable_uis_scoring, bool enable_site_scoring, double mz_threshold, std::vector<std::pair<double, double> > swathes))
{
  std::vector<std::pair<double, double> > swathes;
  swathes.push_back(std::make_pair(400, 425));
  swathes.push_back(std::make_pair(424, 450));
  swathes.push_back(std::make_pair(449, 475));
  swathes.push_back(std::make_pair(474, 500));
  swathes.push_back(std::make_pair(499, 525));
  swathes.push_back(std::make_pair(524, 550));
  swathes.push_back(std::make_pair(549, 575));
  swathes.push_back(std::make_pair(574, 600));
  swathes.push_back(std::make_pair(599, 625));
  swathes.push_back(std::make_pair(624, 650));
  swathes.push_back(std::make_pair(649, 675));
  swathes.push_back(std::make_pair(674, 700));
  swathes.push_back(std::make_pair(699, 725));
  swathes.push_back(std::make_pair(724, 750));
  swathes.push_back(std::make_pair(749, 775));
  swathes.push_back(std::make_pair(774, 800));
  swathes.push_back(std::make_pair(799, 825));
  swathes.push_back(std::make_pair(824, 850));
  swathes.push_back(std::make_pair(849, 875));
  swathes.push_back(std::make_pair(874, 900));
  swathes.push_back(std::make_pair(899, 925));
  swathes.push_back(std::make_pair(924, 950));
  swathes.push_back(std::make_pair(949, 975));
  swathes.push_back(std::make_pair(974, 1000));
  swathes.push_back(std::make_pair(999, 1025));
  swathes.push_back(std::make_pair(1024, 1050));
  swathes.push_back(std::make_pair(1049, 1075));
  swathes.push_back(std::make_pair(1074, 1100));
  swathes.push_back(std::make_pair(1099, 1125));
  swathes.push_back(std::make_pair(1124, 1150));
  swathes.push_back(std::make_pair(1149, 1175));
  swathes.push_back(std::make_pair(1174, 1200));

  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_uisTransitions_input_3.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  std::vector<String> fragment_types1;
  fragment_types1.push_back(String("y"));
  fragment_types1.push_back(String("b"));
  std::vector<size_t> fragment_charges1;
  fragment_charges1.push_back(2);
  fragment_charges1.push_back(3);
  bool enable_losses1 = true;
  bool enable_uis_scoring1 = false;
  bool enable_site_scoring1 = true;
  double product_mz_threshold1 = 0.05;

  String out1 = "MRMAssay_uisTransitions_output_3.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.uisTransitions(targeted_exp1, fragment_types1, fragment_charges1, enable_losses1, enable_uis_scoring1, enable_site_scoring1, product_mz_threshold1, swathes);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

  std::vector<String> fragment_types2;
  fragment_types2.push_back(String("y"));
  fragment_types2.push_back(String("b"));
  std::vector<size_t> fragment_charges2;
  fragment_charges2.push_back(2);
  fragment_charges2.push_back(3);
  bool enable_losses2 = true;
  bool enable_uis_scoring2 = true;
  bool enable_site_scoring2 = false;
  double product_mz_threshold2 = 0.05;

  String out2 = "MRMAssay_uisTransitions_output_4.TraML";

  TargetedExperiment targeted_exp2 = targeted_exp;

  mrma.uisTransitions(targeted_exp2, fragment_types2, fragment_charges2, enable_losses2, enable_uis_scoring2, enable_site_scoring2, product_mz_threshold2, swathes);

  String test2;
  NEW_TMP_FILE(test2);
  traml.store(test2, targeted_exp2);

  TEST_FILE_EQUAL(test2.c_str(), OPENMS_GET_TEST_DATA_PATH(out2))

}
END_SECTION

START_SECTION(void insilicoTransitions(OpenMS::TargetedExperiment& exp, std::vector<String> fragment_types, std::vector<size_t> fragment_charges, bool enable_losses))
{
  TraMLFile traml;
  TargetedExperiment targeted_exp;
  String in = "MRMAssay_insilicoTransitions_input.TraML";
  traml.load(OPENMS_GET_TEST_DATA_PATH(in), targeted_exp);
  MRMAssay mrma;

  std::vector<String> fragment_types1;
  fragment_types1.push_back(String("y"));
  fragment_types1.push_back(String("b"));
  std::vector<size_t> fragment_charges1;
  fragment_charges1.push_back(2);
  fragment_charges1.push_back(3);
  bool enable_losses1 = true;

  String out1 = "MRMAssay_insilicoTransitions_output.TraML";

  TargetedExperiment targeted_exp1 = targeted_exp;

  mrma.insilicoTransitions(targeted_exp1, fragment_types1, fragment_charges1, enable_losses1);

  String test1;
  NEW_TMP_FILE(test1);
  traml.store(test1, targeted_exp1);

  TEST_FILE_EQUAL(test1.c_str(), OPENMS_GET_TEST_DATA_PATH(out1))

}
END_SECTION

END_TEST
