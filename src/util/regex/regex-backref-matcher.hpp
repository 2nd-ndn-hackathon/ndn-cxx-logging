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

#ifndef NDN_UTIL_REGEX_REGEX_BACKREF_MATCHER_HPP
#define NDN_UTIL_REGEX_REGEX_BACKREF_MATCHER_HPP

#include "../../common.hpp"

#include "regex-matcher.hpp"

namespace ndn {

class RegexBackrefMatcher : public RegexMatcher
{
public:
  RegexBackrefMatcher(const std::string& expr, shared_ptr<RegexBackrefManager> backRefManager);

  virtual ~RegexBackrefMatcher(){}

  void
  lateCompile()
  {
    compile();
  }

protected:
  virtual void
  compile();
};

} // namespace ndn

#include "regex-pattern-list-matcher.hpp"

namespace ndn {

inline RegexBackrefMatcher::RegexBackrefMatcher(const std::string& expr,
                                                shared_ptr<RegexBackrefManager> backRefManager)
  : RegexMatcher (expr, EXPR_BACKREF, backRefManager)
{
  // compile();
}

inline void
RegexBackrefMatcher::compile()
{
  int lastIndex = m_expr.size() - 1;
  if ('(' == m_expr[0] && ')' == m_expr[lastIndex]){
    // m_backRefManager->pushRef(this);

    shared_ptr<RegexMatcher> matcher(new RegexPatternListMatcher(m_expr.substr(1, lastIndex - 1),
                                                                 m_backrefManager));
    m_matcherList.push_back(matcher);
  }
  else
    throw RegexMatcher::Error(std::string("Error: RegexBackrefMatcher.Compile(): ")
                              + " Unrecognoized format " + m_expr);
}


} // namespace ndn

#endif // NDN_UTIL_REGEX_REGEX_BACKREF_MATCHER_HPP