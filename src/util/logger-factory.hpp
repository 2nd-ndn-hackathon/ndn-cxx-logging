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

#ifndef NDN_UTIL_LOGGER_FACTORY_HPP
#define NDN_UTIL_LOGGER_FACTORY_HPP

#include "common.hpp"
#include "logger.hpp"
#include <boost/log/sources/channel_logger.hpp>
#include <map>
#include <mutex>

enum class LogLevel { NONE };

namespace ndn {
namespace util {

class LoggerFactory : noncopyable
{
public:

  class Error : public std::runtime_error
  {
  public:
    explicit
    Error(const std::string& error)
      : std::runtime_error(error)
    {
    }
  };

  static void
  addLogger(const std::string& moduleName, boost::shared_ptr<boost::log::sources::channel_logger_mt<>>& loggerPtr);

  /** @example *=INFO:Face=DEBUG:Controller=WARN
   */
  static void
  setSeverityLevels(const std::string& config);

  static void
  setSeverityLevel(const std::string& moduleName, const std::string& level);

  static void
  setDestination(std::ostream& os);

private:
  LoggerFactory();

  static LoggerFactory&
  get();

  LogLevel
  parseLevel(const std::string& level);

private:
  typedef std::map<std::string, LogLevel> LevelMap;
  LevelMap m_enabledLevel;

  typedef std::map<std::string, boost::log::sources::channel_logger_mt<>> LoggerMap;
  LoggerMap m_loggers;

  std::mutex m_lock;
};

} // namespace util
} // namespace ndn

#endif // NDN_UTIL_LOGGER_FACTORY_HPP
