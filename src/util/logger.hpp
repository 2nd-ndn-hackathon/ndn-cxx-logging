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

#ifndef NDN_UTIL_LOGGER_HPP
#define NDN_UTIL_LOGGER_HPP

#include "common.hpp"

#ifdef NDN_CXX_ENABLE_LOGGING

namespace ndn {
namespace util {

/** \brief indicates a log level
 */
enum class LogLevel {
  LOG_FATAL          = -1, // fatal (will be logged unconditionally)
  LOG_NONE           = 0, // no messages
  LOG_ERROR          = 1, // serious error messages
  LOG_WARN           = 2, // warning messages
  LOG_INFO           = 3, // informational messages
  LOG_DEBUG          = 4, // debug messages
  LOG_TRACE          = 5, // trace messages (most verbose)
  LOG_ALL            = 255 // all messages
};

class Logger {
public:
  Logger(const std::string& name, LogLevel level);

  const std::string&
  getName() const {
    return m_moduleName;
  }

  void
  setName(const std::string& name)
  {
    m_moduleName = name;
  }

  void
  setLogLevel(LogLevel level)
  {
    m_enabledLogLevel = level;
  }

private:
  LogLevel m_enabledLogLevel;
  std::string m_moduleName;
};

inline std::ostream&
operator<<(std::ostream& output, const Logger& logger)
{
  output << logger.getName();
  return output;
}


#define NDN_CXX_LOG_INIT(name) class Logger_##name {}

#define NDN_CXX_LOG_TRACE(expression) do { } while (false)
#define NDN_CXX_LOG_DEBUG(expression) do { } while (false)
#define NDN_CXX_LOG_INFO(expression) do { } while (false)
#define NDN_CXX_LOG_WARN(expression) do { } while (false)
#define NDN_CXX_LOG_ERROR(expression) do { } while (false)
#define NDN_CXX_LOG_FATAL(expression) do { } while (false)

} // namespace util
} // namespace ndn

#else

#define NDN_CXX_LOG_INIT(name) class Logger_##name {}

#define NDN_CXX_LOG_TRACE(expression) do { } while (false)
#define NDN_CXX_LOG_DEBUG(expression) do { } while (false)
#define NDN_CXX_LOG_INFO(expression) do { } while (false)
#define NDN_CXX_LOG_WARN(expression) do { } while (false)
#define NDN_CXX_LOG_ERROR(expression) do { } while (false)
#define NDN_CXX_LOG_FATAL(expression) do { } while (false)


#endif // ENABLE_CXX_LOGGING

#endif // NDN_UTIL_LOGGER_HPP
