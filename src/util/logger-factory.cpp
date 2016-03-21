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
#include <boost/log/expressions.hpp>
#include <fstream>

namespace ndn {
namespace util {

LoggerFactory&
LoggerFactory::get()
{
  // Initialization of block-scope variables with static storage duration is thread-safe.
  // See ISO C++ standard [stmt.dcl]/4
  static LoggerFactory globalLoggerFactory;
  return globalLoggerFactory;
}

LoggerFactory::LoggerFactory()
{
  static std::ofstream nullOutputStream;
  this->setDestinationImpl(nullOutputStream);

  const char* environ = std::getenv("NDN_CXX_LOG");
  if (environ != nullptr) {
    this->setSeverityLevelsImpl(environ);
  }
}

void
LoggerFactory::addLogger(const std::string& moduleName, Logger* logger)
{
  LoggerFactory& lf = get();
  std::lock_guard<std::mutex> lock(lf.m_mutex);
  lf.m_loggers.insert({moduleName, logger});

  auto levelIt = lf.m_enabledLevel.find(moduleName);
  if (levelIt == lf.m_enabledLevel.end()) {
    levelIt = lf.m_enabledLevel.find("*");
  }
  LogLevel level = levelIt == lf.m_enabledLevel.end() ? LogLevel::INFO : levelIt->second;
  logger->setLevel(level);
}

void
LoggerFactory::setSeverityLevel(const std::string& moduleName, LogLevel level)
{
  get().setSeverityLevel(moduleName, level);
}

void
LoggerFactory::setSeverityLevelImpl(const std::string& moduleName, LogLevel level)
{
  std::lock_guard<std::mutex> lock(m_mutex);
  if (moduleName == "*") {
    m_enabledLevel.clear();
    m_enabledLevel["*"] = level;

    for (auto i = m_loggers.begin(); i != m_loggers.end(); ++i) {
      i->second->setLevel(level);
    }
    return;
  }

  m_enabledLevel[moduleName] = level;
  auto range = m_loggers.equal_range(moduleName);
  for (auto i = range.first; i != range.second; ++i) {
    i->second->setLevel(level);
  }
}

void
LoggerFactory::setSeverityLevels(const std::string& config)
{
  get().setSeverityLevelsImpl(config);
}

void
LoggerFactory::setSeverityLevelsImpl(const std::string& config)
{
  std::stringstream ss(config);
  std::string configModule;
  while (std::getline(ss, configModule, ':')) {
    size_t ind = configModule.find('=');
    if (ind == std::string::npos)
      BOOST_THROW_EXCEPTION(std::invalid_argument("wrong log configuration"));

    std::string moduleName = configModule.substr(0, ind);
    LogLevel level = parseLevel(configModule.substr(ind+1));

    this->setSeverityLevelImpl(moduleName, level);
  }
}

LogLevel
LoggerFactory::parseLevel(const std::string& levelStr)
{
  if (levelStr == "FATAL")
    return LogLevel::FATAL;
  else if (levelStr == "NONE")
    return LogLevel::NONE;
  else if (levelStr == "ERROR")
    return LogLevel::ERROR;
  else if (levelStr == "WARN")
    return LogLevel::WARN;
  else if (levelStr == "INFO")
    return LogLevel::INFO;
  else if (levelStr == "DEBUG")
    return LogLevel::DEBUG;
  else if (levelStr == "TRACE")
    return LogLevel::TRACE;
  else if (levelStr == "ALL")
    return LogLevel::ALL;

  BOOST_THROW_EXCEPTION(std::invalid_argument("Unrecognized log level '" + levelStr + "'"));
}

void
LoggerFactory::setDestination(std::ostream& os)
{
  get().setDestinationImpl(os);
}

void
LoggerFactory::setDestinationImpl(std::ostream& os)
{
  std::lock_guard<std::mutex> lock(m_mutex);

  auto backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
  backend->auto_flush(true);
  backend->add_stream(boost::shared_ptr<std::ostream>(&os, bind([]{})));

  m_sink = boost::make_shared<Sink>(backend);
  m_sink->set_formatter(boost::log::expressions::stream << boost::log::expressions::message);
  boost::log::core::get()->add_sink(m_sink);
}

} // namespace util
} // namespace ndn
