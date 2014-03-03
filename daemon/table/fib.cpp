/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (C) 2014 Named Data Networking Project
 * See COPYING for copyright and distribution information.
 */

#include "fib.hpp"
#include "pit-entry.hpp"
#include "measurements-entry.hpp"

namespace nfd {

const shared_ptr<fib::Entry> Fib::m_emptyEntry = make_shared<fib::Entry>(Name());

Fib::Fib(NameTree& nameTree)
  : m_nameTree(nameTree)
  , m_nItems(0)
{
}

Fib::~Fib()
{
}

static inline bool
predicate_NameTreeEntry_hasFibEntry(const name_tree::Entry& entry)
{
  return static_cast<bool>(entry.getFibEntry());
}

std::pair<shared_ptr<fib::Entry>, bool>
Fib::insert(const Name& prefix)
{
  shared_ptr<name_tree::Entry> nameTreeEntry = m_nameTree.lookup(prefix);
  shared_ptr<fib::Entry> entry = nameTreeEntry->getFibEntry();
  if (static_cast<bool>(entry))
    return std::make_pair(entry, false);
  entry = make_shared<fib::Entry>(prefix);
  nameTreeEntry->setFibEntry(entry);
  m_nItems++;
  return std::make_pair(entry, true);
}

shared_ptr<fib::Entry>
Fib::findLongestPrefixMatch(const Name& prefix) const
{
  shared_ptr<name_tree::Entry> nameTreeEntry =
    m_nameTree.findLongestPrefixMatch(prefix, &predicate_NameTreeEntry_hasFibEntry);
  if (static_cast<bool>(nameTreeEntry)) {
    return nameTreeEntry->getFibEntry();
  }
  return m_emptyEntry;
}

shared_ptr<fib::Entry>
Fib::findLongestPrefixMatch(const pit::Entry& pitEntry) const
{
  return this->findLongestPrefixMatch(pitEntry.getName());
}

shared_ptr<fib::Entry>
Fib::findLongestPrefixMatch(const measurements::Entry& measurementsEntry) const
{
  return this->findLongestPrefixMatch(measurementsEntry.getName());
}

shared_ptr<fib::Entry>
Fib::findExactMatch(const Name& prefix) const
{
  shared_ptr<name_tree::Entry> nameTreeEntry = m_nameTree.findExactMatch(prefix);
  if (static_cast<bool>(nameTreeEntry))
    return nameTreeEntry->getFibEntry();
  return shared_ptr<fib::Entry>();
}

void
Fib::erase(const Name& prefix)
{
  shared_ptr<name_tree::Entry> nameTreeEntry = m_nameTree.findExactMatch(prefix);
  if (static_cast<bool>(nameTreeEntry))
  {
    nameTreeEntry->eraseFibEntry(nameTreeEntry->getFibEntry());
    m_nItems--;
  }
}

void
Fib::removeNextHopFromAllEntries(shared_ptr<Face> face)
{
  shared_ptr<std::vector<shared_ptr<name_tree::Entry > > > nameTreeEntries =
    m_nameTree.fullEnumerate(&predicate_NameTreeEntry_hasFibEntry);
  for (size_t i = 0; i < nameTreeEntries->size(); ++i) {
    shared_ptr<fib::Entry> entry = nameTreeEntries->at(i)->getFibEntry();
    entry->removeNextHop(face);
  }
}


} // namespace nfd