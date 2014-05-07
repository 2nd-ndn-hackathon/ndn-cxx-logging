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
 */

#ifndef NDN_UTIL_SCHEDULER_HPP
#define NDN_UTIL_SCHEDULER_HPP

#include "../common.hpp"
#include "monotonic_deadline_timer.hpp"

namespace ndn {

struct EventIdImpl; ///< \brief Private storage of information about the event
/**
 * \brief Opaque type (shared_ptr) representing ID of the scheduled event
 */
typedef shared_ptr<EventIdImpl> EventId;

/**
 * \brief Generic scheduler
 */
class Scheduler
{
public:
  typedef function<void()> Event;

  Scheduler(boost::asio::io_service& ioService);

  /**
   * \brief Schedule one time event after the specified delay
   * \returns EventId that can be used to cancel the scheduled event
   */
  EventId
  scheduleEvent(const time::nanoseconds& after, const Event& event);

  /**
   * \brief Schedule periodic event that should be fired every specified period.
   *        First event will be fired after the specified delay.
   * \returns EventId that can be used to cancel the scheduled event
   */
  EventId
  schedulePeriodicEvent(const time::nanoseconds& after,
                        const time::nanoseconds& period,
                        const Event& event);

  /**
   * \brief Cancel scheduled event
   */
  void
  cancelEvent(const EventId& eventId);

private:
  void
  onEvent(const boost::system::error_code& code);

private:
  struct EventInfo
  {
    EventInfo(const time::nanoseconds& after,
              const time::nanoseconds& period,
              const Event& event);

    EventInfo(const time::steady_clock::TimePoint& when, const EventInfo& previousEvent);

    bool
    operator <=(const EventInfo& other) const
    {
      return this->m_scheduledTime <= other.m_scheduledTime;
    }

    bool
    operator <(const EventInfo& other) const
    {
      return this->m_scheduledTime < other.m_scheduledTime;
    }

    time::nanoseconds
    expiresFromNow() const;

    time::steady_clock::TimePoint m_scheduledTime;
    time::nanoseconds m_period;
    Event m_event;
    mutable EventId m_eventId;
  };

  typedef std::multiset<EventInfo> EventQueue;
  friend struct EventIdImpl;

  EventQueue m_events;
  EventQueue::iterator m_scheduledEvent;
  monotonic_deadline_timer m_deadlineTimer;

  bool m_isEventExecuting;
};

} // namespace ndn

#endif // NDN_UTIL_SCHEDULER_HPP