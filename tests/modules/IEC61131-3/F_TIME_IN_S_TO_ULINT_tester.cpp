/*******************************************************************************
 * Copyright (c) 2019 TU Wien/ACIN
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *   Martin Melik-Merkumians - initial tests
 *******************************************************************************/
#include "../../core/fbtests/fbtestfixture.h"

#ifdef FORTE_ENABLE_GENERATED_SOURCE_CPP
#include "F_TIME_IN_S_TO_ULINT_tester_gen.cpp"
#endif

struct F_TIME_IN_S_TO_ULINT_TestFixture : public CFBTestFixtureBase {

    F_TIME_IN_S_TO_ULINT_TestFixture() :
        CFBTestFixtureBase(g_nStringIdF_TIME_IN_S_TO_ULINT) {
      SETUP_INPUTDATA(&mIn_F_TIME_IN_S_TO_ULINT);
      SETUP_OUTPUTDATA(&mOut_F_TIME_IN_S_TO_ULINT);
      CFBTestFixtureBase::setup();
    }

    CIEC_TIME mIn_F_TIME_IN_S_TO_ULINT; //DATA INPUT
    CIEC_ULINT mOut_F_TIME_IN_S_TO_ULINT; //DATA OUTPUT
};

BOOST_FIXTURE_TEST_SUITE( F_TIME_IN_S_TO_ULINT_Tests, F_TIME_IN_S_TO_ULINT_TestFixture)

  BOOST_AUTO_TEST_CASE(timeConversion) {
    mIn_F_TIME_IN_S_TO_ULINT.fromString("T#2d5h43m12s");
    /* trigger the inputevent */
    triggerEvent(0);
    BOOST_CHECK(checkForSingleOutputEventOccurence(0));
    BOOST_CHECK_EQUAL(UINT64_C(172800) + UINT64_C(18000) + UINT64_C(2580) + UINT64_C(12), mOut_F_TIME_IN_S_TO_ULINT);
  }

  BOOST_AUTO_TEST_SUITE_END()
