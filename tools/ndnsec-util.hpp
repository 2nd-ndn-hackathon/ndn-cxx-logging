/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (c) 2013-2014,  Regents of the University of California.
 * All rights reserved.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 *
 * This file licensed under New BSD License.  See COPYING for detailed information about
 * ndn-cxx library copyright, permissions, and redistribution restrictions.
 *
 * @author Yingdi Yu <http://irl.cs.ucla.edu/~yingdi/>
 */

#ifndef NDNSEC_UTIL_HPP
#define NDNSEC_UTIL_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>
#include <boost/asio.hpp>
#include <boost/exception/all.hpp>


#include "security/cryptopp.hpp"

#include "security/key-chain.hpp"
#include "util/io.hpp"

bool
getPassword(std::string& password, const std::string& prompt)
{
  bool isReady = false;

  char* pw0 = 0;

  pw0 = getpass(prompt.c_str());
  if (!pw0)
    return false;
  std::string password1 = pw0;
  memset(pw0, 0, strlen(pw0));

  pw0 = getpass("Confirm:");
  if (!pw0)
    {
      char* pw1 = const_cast<char*>(password1.c_str());
      memset(pw1, 0, password1.size());
      return false;
    }

  if (!password1.compare(pw0))
    {
      isReady = true;
      password.swap(password1);
    }

  char* pw1 = const_cast<char*>(password1.c_str());
  memset(pw1, 0, password1.size());
  memset(pw0, 0, strlen(pw0));

  if (password.empty())
    return false;

  return isReady;
}

ndn::shared_ptr<ndn::IdentityCertificate>
getIdentityCertificate(const std::string& fileName)
{

  if (fileName == "-")
    return ndn::io::load<ndn::IdentityCertificate>(std::cin);
  else
    return ndn::io::load<ndn::IdentityCertificate>(fileName);
}

#endif //NDNSEC_UTIL_HPP