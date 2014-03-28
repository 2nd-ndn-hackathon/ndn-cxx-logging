/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2013, Regents of the University of California
 *
 * BSD license, See the LICENSE file for more information
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

#ifndef NDN_TLV_NFD_HPP
#define NDN_TLV_NFD_HPP

#include "../common.hpp"

namespace ndn {
namespace tlv {
namespace nfd {

enum {
  // ControlParameters
  ControlParameters   = 104,
  FaceId              = 105,
  Uri                 = 114,
  LocalControlFeature = 110,
  Cost                = 106,
  Strategy            = 107,

  // (deprecated)
  FibManagementOptions  = ControlParameters,
  FaceManagementOptions = ControlParameters,
  StrategyChoiceOptions = ControlParameters,

  // ControlResponse
  ControlResponse = 101,
  StatusCode      = 102,
  StatusText      = 103,

  // FIB Enumeration Protocol
  FibEntry        = 128,
  NextHopRecord   = 129,

  // Face Status Protocol
  FaceStatus                   = 128,
  TotalIncomingInterestCounter = 145,
  TotalIncomingDataCounter     = 144,
  TotalOutgoingInterestCounter = 146,
  TotalOutgoingDataCounter     = 147,
  FaceEventNotification        = 192,
  FaceEventKind                = 193,
  FaceFlags                    = 194,

  // ForwarderStatus
  NfdVersion           = 128,
  StartTimestamp       = 129,
  CurrentTimestamp     = 130,
  NNameTreeEntries     = 131,
  NFibEntries          = 132,
  NPitEntries          = 133,
  NMeasurementsEntries = 134,
  NCsEntries           = 135,
  NInInterests         = 144,
  NInDatas             = 145,
  NOutInterests        = 146,
  NOutDatas            = 147
};

enum {
  // Local Control Header
  LocalControlHeader = 80,
  IncomingFaceId     = 81,
  NextHopFaceId      = 82
};

} // namespace nfd
} // namespace tlv

namespace nfd {

const uint64_t INVALID_FACE_ID = std::numeric_limits<uint64_t>::max();

} // namespace nfd

} // namespace ndn

#endif // NDN_TLV_NFD_HPP