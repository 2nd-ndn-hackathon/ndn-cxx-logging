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

#include "logger-factory.hpp"

namespace ndn {
namespace util {

void
LoggerFactory::addLogger(const std::string& moduleName, const LogSource& logger)
{
  std::lock_guard<std::mutex> lock(get().m_mutex);
  get().m_loggers[moduleName] = logger;
}

void
LoggerFactory::setSeverityLevels(const std::string& config)
{
}

void
LoggerFactory::setSeverityLevel(const std::string& moduleName, LogLevel level)
{
}

void
LoggerFactory::setDestination(std::ostream& os)
{
}

LoggerFactory&
LoggerFactory::get()
{
  // Initialization of block-scope variables with static storage duration is thread-safe.
  // See ISO C++ standard [stmt.dcl]/4
  static LoggerFactory globalLoggerFactory;
  return globalLoggerFactory;
}

LogLevel
LoggerFactory::parseLevel(const std::string& levelStr)
{
  return LogLevel::NONE;
}

} // namespace util
} // namespace ndn
