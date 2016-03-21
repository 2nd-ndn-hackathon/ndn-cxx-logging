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
#include "name.hpp"

#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/channel_feature.hpp>
#include <boost/log/sources/channel_logger.hpp>
#include <boost/log/support/date_time.hpp>

namespace ndn {
namespace util {

#ifdef NDN_CXX_ENABLE_LOGGING

/** \brief indicates the severity level of a log message
 */
enum class LogLevel {
  FATAL   = -1,   ///< fatal (will be logged unconditionally)
  NONE    = 0,    ///< no messages
  ERROR   = 1,    ///< serious error messages
  WARN    = 2,    ///< warning messages
  INFO    = 3,    ///< informational messages
  DEBUG   = 4,    ///< debug messages
  TRACE   = 5,    ///< trace messages (most verbose)
  ALL     = 255   ///< all messages
};

class Logger
{
public:
  Logger(const std::string& name, LogLevel level);

  const std::string&
  getName() const
  {
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

/** \brief a tag that writes a timestamp upon stream output
 *  \example std::clog << LoggerTimestamp()
 */
struct LoggerTimestamp
{
};

/** \brief write a timestamp to \p os
 *  \note This function is thread-safe.
 */
std::ostream&
operator<<(std::ostream& os, const boost::posix_time::ptime& timestamp);

static
::boost::log::sources::channel_logger_mt< > g_channelLogger;

BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)

#define NDN_CXX_LOG(level, msg, expression)                                 \
  do {                                                                      \
    BOOST_LOG(::ndn::util::g_channelLogger) << " "#msg": " << expression;   \
  } while (false)

/** \brief declare a log module
 */
#define NDN_CXX_LOG_INIT(name) class Logger_##name {}

/** \brief log at TRACE level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_TRACE(expression) NDN_CXX_LOG(TRACE, TRACE, expression)

/** \brief log at DEBUG level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_DEBUG(expression) NDN_CXX_LOG(DEBUG, DEBUG, expression)

/** \brief log at INFO level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_INFO(expression) NDN_CXX_LOG(INFO, INFO, expression)

/** \brief log at WARN level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_WARN(expression) NDN_CXX_LOG(WARN, WARNING, expression)

/** \brief log at ERROR level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_ERROR(expression) NDN_CXX_LOG(ERROR, ERROR, expression)

/** \brief log at FATAL level
 *  \pre A log module must be declared in the same translation unit.
 */
#define NDN_CXX_LOG_FATAL(expression) NDN_CXX_LOG(FATAL, FATAL, expression)

#else // NDN_CXX_ENABLE_LOGGING

#define NDN_CXX_LOG_INIT(name)

#define NDN_CXX_LOG_TRACE(expression) do { } while (false)
#define NDN_CXX_LOG_DEBUG(expression) do { } while (false)
#define NDN_CXX_LOG_INFO(expression) do { } while (false)
#define NDN_CXX_LOG_WARN(expression) do { } while (false)
#define NDN_CXX_LOG_ERROR(expression) do { } while (false)
#define NDN_CXX_LOG_FATAL(expression) do { } while (false)

#endif // NDN_CXX_ENABLE_LOGGING

} // namespace util
} // namespace ndn

#endif // NDN_UTIL_LOGGER_HPP
