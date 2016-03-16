/**
 * Copyright (C) 2016 Chalmers REVERE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 */

#ifndef PROXYCANREPLAYTESTSUITE_HPP_
#define PROXYCANREPLAYTESTSUITE_HPP_

#include "cxxtest/TestSuite.h"

#include <string>
#include <vector>

#include <opendavinci/odcore/data/Container.h>

#include "opendlvdata/GeneratedHeaders_OpenDLVData.h"

// Include local header files.
#include "../include/can/replay/canascreplay.hpp"

/**
 * This class derives from opendlv::proxy::can::replay::CANASCReplay
 * to allow access to protected methods.
 */
class CANASCReplayTestling : public opendlv::proxy::can::replay::CANASCReplay {
 private:
  CANASCReplayTestling();

 public:
  CANASCReplayTestling(const int32_t &a_argc, char **a_argv)
      : opendlv::proxy::can::replay::CANASCReplay(a_argc, a_argv)
  {
  }

  // Here, you need to add all methods which are protected in
  // sensor::camera::Camera and which are needed for the test cases.
};

/**
 * The actual testsuite starts here.
 */
class CANASCReplayTest : public CxxTest::TestSuite {
 private:
  CANASCReplayTestling *m_pt;

 public:
  CANASCReplayTest()
      : m_pt(nullptr)
  {
  }
  CANASCReplayTest(CANASCReplayTest const &) = delete;
  CANASCReplayTest &operator=(CANASCReplayTest const &) = delete;

  /**
   * This method will be called before each testXYZ-method.
   */
  void setUp()
  {
    std::string argv0("proxy-can-ascreplay");
    std::string argv1("--cid=100");
    int32_t argc = 2;
    char **argv;
    argv = new char *[2];
    argv[0] = const_cast<char *>(argv0.c_str());
    argv[1] = const_cast<char *>(argv1.c_str());

    m_pt = new CANASCReplayTestling(argc, argv);
  }

  /**
   * This method will be called after each testXYZ-method.
   */
  void tearDown()
  {
    delete m_pt;
    m_pt = nullptr;
  }

  //////////////////////////////////////////////////////////////////////////
  // Below this line the actual testcases are defined.
  //////////////////////////////////////////////////////////////////////////
  void testCANASCReplayTestlingSuccessfullyCreated()
  {
    TS_ASSERT(m_pt != nullptr);
  }

  void testDecodeInvalidPayload()
  {
    std::vector<odcore::data::Container> result = m_pt->getMessages("Hello World");
    TS_ASSERT(result.size() == 0);
  }

  void testDecodeValidPayload()
  {
    std::string payload(" 279.733972 1  104             Rx   d 6 7F 7D A7 7D 87 7D  Length = 388000 BitCount = 101");
    std::vector<odcore::data::Container> result = m_pt->getMessages(payload);
    TS_ASSERT(result.size() == 1);
    TS_ASSERT(result.at(0).getDataType() == opendlv::gcdc::fh16::VehicleDynamics::ID());

    opendlv::gcdc::fh16::VehicleDynamics vd = result.at(0).getData<opendlv::gcdc::fh16::VehicleDynamics>();
    TS_ASSERT_DELTA(vd.getAcceleration_x(), 1.17188e-05, 1e-5);
    TS_ASSERT_DELTA(vd.getAcceleration_y(), 0.019543, 1e-5);
    TS_ASSERT_DELTA(vd.getYawrate(), 0.00047, 1e-5);
  }
};

#endif
