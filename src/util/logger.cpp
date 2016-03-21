/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2016 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include "logger.hpp"
#include "logger-factory.hpp"
#include "time.hpp"

#include <cinttypes>
#include <stdio.h>
#include <type_traits>

namespace ndn {
namespace util {

Logger::Logger(const std::string& name)
  : boost::log::sources::channel_logger_mt<>(name)
  , m_currentLogLevel(LogLevel::NONE)
{
}

Logger
makeLogger(const std::string& name)
{
  Logger logger(name);
  LoggerFactory::addLogger(name, logger);
  return std::move(logger);
}

std::ostream&
operator<<(std::ostream& os, const LoggerTimestamp&)
{
  using namespace ndn::time;

  static const microseconds::rep ONE_SECOND = 1000000;
  microseconds::rep microsecondsSinceEpoch = duration_cast<microseconds>(
                                             system_clock::now().time_since_epoch()).count();

  // 10 (whole seconds) + '.' + 6 (fraction) + '\0'
  char buffer[10 + 1 + 6 + 1];
  BOOST_ASSERT_MSG(microsecondsSinceEpoch / ONE_SECOND <= 9999999999L,
                   "whole seconds cannot fit in 10 characters");

  static_assert(std::is_same<microseconds::rep, int_least64_t>::value,
                "PRIdLEAST64 is incompatible with microseconds::rep");
  // - std::snprintf not found in some environments
  //   http://redmine.named-data.net/issues/2299 for more information
  snprintf(buffer, sizeof(buffer), "%" PRIdLEAST64 ".%06" PRIdLEAST64,
           microsecondsSinceEpoch / ONE_SECOND,
           microsecondsSinceEpoch % ONE_SECOND);

  return os << buffer;
}

} // namespace util
} // namespace ndn
