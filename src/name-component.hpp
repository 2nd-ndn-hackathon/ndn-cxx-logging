/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * @author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 * @author: Zhenkai Zhu <zhenkai@cs.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#ifndef NDN_NAME_COMPONENT_HPP
#define NDN_NAME_COMPONENT_HPP

#include "common.hpp"
#include "encoding/block.hpp"
#include "encoding/encoding-buffer.hpp"
#include "util/string-helper.hpp"

namespace ndn {
namespace name {

/**
 * A Name::Component holds a read-only name component value.
 */
class Component : public Block
{
public:
  /// @brief Error that can be thrown from the block
  struct Error : public std::runtime_error {
    Error(const std::string& what) : std::runtime_error(what) {}
  };

  /**
   * Create a new Name::Component with a null value.
   */
  Component();

  /**
   * @brief Directly create component from wire block
   *
   * @throws Error if wire.type() is not Tlv::Component
   */
  Component(const Block& wire);

  /**
   * Create a new Name::Component, taking another pointer to the Blob value.
   * @param value A blob with a pointer to an immutable array.  The pointer is copied.
   */
  Component(const ConstBufferPtr& buffer);

  /**
   * Create a new Name::Component, copying the given value.
   * @param value The value byte array.
   */
  Component(const Buffer& value);

  /**
   * Create a new Name::Component, copying the given value.
   * @param value Pointer to the value byte array.
   * @param valueLen Length of value.
   */
  Component(const uint8_t *value, size_t valueLen);

  template<class InputIterator>
  Component(InputIterator begin, InputIterator end);

  explicit
  Component(const char *str);

  explicit
  Component(const std::string& str);

  /**
   * @brief Fast encoding or block size estimation
   */
  template<bool T>
  size_t
  wireEncode(EncodingImpl<T>& block) const;

  /**
   * Make a Blob value by decoding the escapedString between beginOffset and endOffset according to the NDN URI Scheme.
   * If the escaped string is "", "." or ".." then return a Blob with a null pointer,
   * which means the component should be skipped in a URI name.
   * @param escapedString The escaped string.  It does not need to be null-terminated because we only scan to endOffset.
   * @param beginOffset The offset in escapedString of the beginning of the portion to decode.
   * @param endOffset The offset in escapedString of the end of the portion to decode.
   * @return The Blob value. If the escapedString is not a valid escaped component, then the Blob is a null pointer.
   */
  static Component
  fromEscapedString(const char *escapedString, size_t beginOffset, size_t endOffset);

  /**
   * Make a Blob value by decoding the escapedString according to the NDN URI Scheme.
   * If the escaped string is "", "." or ".." then return a Blob with a null pointer,
   * which means the component should be skipped in a URI name.
   * @param escapedString The null-terminated escaped string.
   * @return The Blob value. If the escapedString is not a valid escaped component, then the Blob is a null pointer.
   */
  static Component
  fromEscapedString(const char *escapedString)
  {
    return fromEscapedString(escapedString, 0, ::strlen(escapedString));
  }

  /**
   * Make a Blob value by decoding the escapedString according to the NDN URI Scheme.
   * If the escaped string is "", "." or ".." then return a Blob with a null pointer,
   * which means the component should be skipped in a URI name.
   * @param escapedString The escaped string.
   * @return The Blob value. If the escapedString is not a valid escaped component, then the Blob is a null pointer.
   */
  static Component
  fromEscapedString(const std::string& escapedString)
  {
    return fromEscapedString(escapedString.c_str());
  }

  /**
   * Write the value to result, escaping characters according to the NDN URI Scheme.
   * This also adds "..." to a value with zero or more ".".
   * @param value the buffer with the value to escape
   * @param result the string stream to write to.
   */
  void
  toEscapedString(std::ostream& result) const;

  /**
   * Convert the value by escaping characters according to the NDN URI Scheme.
   * This also adds "..." to a value with zero or more ".".
   * @param value the buffer with the value to escape
   * @return The escaped string.
   */
  inline std::string
  toEscapedString() const
  {
    std::ostringstream result;
    toEscapedString(result);
    return result.str();
  }

  inline void
  toUri(std::ostream& result) const
  {
    return toEscapedString(result);
  }

  inline std::string
  toUri() const
  {
    return toEscapedString();
  }

  /**
   * @brief Interpret this name component as nonNegativeInteger
   *
   * @see http://named-data.net/doc/ndn-tlv/tlv.html#non-negative-integer-encoding
   *
   * @return The integer number.
   */
  uint64_t
  toNumber() const;

  /**
   * @brief An alias for toNumber()
   */
  uint64_t
  toVersion() const;

  /**
   * @brief An alias for toNumber()
   */
  uint64_t
  toSegment() const;

  /**
   * @brief Create a component encoded as nonNegativeInteger
   *
   * @see http://named-data.net/doc/ndn-tlv/tlv.html#non-negative-integer-encoding
   *
   * @param number The non-negative number
   * @return The component value.
   */
  static Component
  fromNumber(uint64_t number);

  /**
   * Check if this is the same component as other.
   * @param other The other Component to compare with.
   * @return true if the components are equal, otherwise false.
   */
  bool
  equals(const Component& other) const
  {
    if (value_size() != other.value_size())
      return false;
    if (value_size() == 0 /* == other.value_size()*/)
      return true;

    // somehow, behavior is wrong on OSX 10.9 when component is empty
    // (probably some bug in STL...)
    return std::equal(value_begin(), value_end(), other.value_begin());
  }

  bool
  empty() const
  {
    return !hasValue();
  }

  /**
   * Check if this is the same component as other.
   * @param other The other Component to compare with.
   * @return true if the components are equal, otherwise false.
   */
  bool
  operator==(const Component& other) const { return equals(other); }

  /**
   * Check if this is not the same component as other.
   * @param other The other Component to compare with.
   * @return true if the components are not equal, otherwise false.
   */
  bool
  operator!=(const Component& other) const { return !equals(other); }

  /**
   * Compare this to the other Component using NDN canonical ordering.
   * @param other The other Component to compare with.
   * @return 0 If they compare equal, -1 if *this comes before other in the canonical ordering, or
   * 1 if *this comes after other in the canonical ordering.
   *
   * @see http://named-data.net/doc/ndn-tlv/name.html#canonical-order
   */
  int
  compare(const Component& other) const;

  /**
   * Return true if this is less than or equal to the other Component in the NDN canonical ordering.
   * @param other The other Component to compare with.
   *
   * @see http://named-data.net/doc/ndn-tlv/name.html#canonical-order
   */
  bool
  operator<=(const Component& other) const { return compare(other) <= 0; }

  /**
   * Return true if this is less than the other Component in the NDN canonical ordering.
   * @param other The other Component to compare with.
   *
   * @see http://named-data.net/doc/ndn-tlv/name.html#canonical-order
   */
  bool
  operator<(const Component& other) const { return compare(other) < 0; }

  /**
   * Return true if this is less than or equal to the other Component in the NDN canonical ordering.
   * @param other The other Component to compare with.
   *
   * @see http://named-data.net/doc/ndn-tlv/name.html#canonical-order
   */
  bool
  operator>=(const Component& other) const { return compare(other) >= 0; }

  /**
   * Return true if this is greater than the other Component in the NDN canonical ordering.
   * @param other The other Component to compare with.
   *
   * @see http://named-data.net/doc/ndn-tlv/name.html#canonical-order
   */
  bool
  operator>(const Component& other) const { return compare(other) > 0; }

  //
  // !!! MUST NOT INCLUDE ANY DATA HERE !!!
  //
  // This class is just a helper and is directly reinterpret_cast'ed from Block
};

inline std::ostream&
operator << (std::ostream& os, const Component& component)
{
  component.toEscapedString(os);
  return os;
}

inline
Component::Component()
  : Block(Tlv::NameComponent)
{
}

inline
Component::Component(const Block& wire)
  : Block(wire)
{
  if (type() != Tlv::NameComponent)
    throw Error("Constructing name component from non name component TLV wire block");
}

inline
Component::Component(const ConstBufferPtr& buffer)
  : Block (Tlv::NameComponent, buffer)
{
}

inline
Component::Component(const Buffer& value)
  : Block (Tlv::NameComponent, ConstBufferPtr(new Buffer(value)))
{
}

inline
Component::Component(const uint8_t *value, size_t valueLen)
  : Block (Tlv::NameComponent, ConstBufferPtr(new Buffer(value, valueLen)))
{
}

template<class InputIterator>
inline
Component::Component(InputIterator begin, InputIterator end)
  : Block (Tlv::NameComponent, ConstBufferPtr(new Buffer(begin, end)))
{
}

inline
Component::Component(const char *str)
  : Block (Tlv::NameComponent, ConstBufferPtr(new Buffer(str, ::strlen(str))))
{
}

inline
Component::Component(const std::string& str)
  : Block (Tlv::NameComponent, ConstBufferPtr(new Buffer(str.begin(), str.end())))
{
}


inline Component
Component::fromEscapedString(const char* escapedString, size_t beginOffset, size_t endOffset)
{
  std::string trimmedString(escapedString + beginOffset, escapedString + endOffset);
  trim(trimmedString);
  std::string value = unescape(trimmedString);

  if (value.find_first_not_of(".") == std::string::npos) {
    // Special case for component of only periods.
    if (value.size() <= 2)
      // Zero, one or two periods is illegal.  Ignore this component.
      return Component();
    else
      // Remove 3 periods.
      return Component((const uint8_t *)&value[3], value.size() - 3);
  }
  else
    return Component((const uint8_t *)&value[0], value.size());
}


inline void
Component::toEscapedString(std::ostream& result) const
{
  const uint8_t *valuePtr = value();
  size_t valueSize = value_size();

  bool gotNonDot = false;
  for (unsigned i = 0; i < valueSize; ++i) {
    if (valuePtr[i] != 0x2e) {
      gotNonDot = true;
      break;
    }
  }
  if (!gotNonDot) {
    // Special case for component of zero or more periods.  Add 3 periods.
    result << "...";
    for (size_t i = 0; i < valueSize; ++i)
      result << '.';
  }
  else {
    // In case we need to escape, set to upper case hex and save the previous flags.
    std::ios::fmtflags saveFlags = result.flags(std::ios::hex | std::ios::uppercase);

    for (size_t i = 0; i < valueSize; ++i) {
      uint8_t x = valuePtr[i];
      // Check for 0-9, A-Z, a-z, (+), (-), (.), (_)
      if ((x >= 0x30 && x <= 0x39) || (x >= 0x41 && x <= 0x5a) ||
          (x >= 0x61 && x <= 0x7a) || x == 0x2b || x == 0x2d ||
          x == 0x2e || x == 0x5f)
        result << x;
      else {
        result << '%';
        if (x < 16)
          result << '0';
        result << (unsigned int)x;
      }
    }

    // Restore.
    result.flags(saveFlags);
  }
}


inline Component
Component::fromNumber(uint64_t number)
{
  /// \todo Change to Tlv::NumberComponent
  return nonNegativeIntegerBlock(Tlv::NameComponent, number);
}


inline uint64_t
Component::toNumber() const
{
  /// \todo Check if Component is of Tlv::NumberComponent type
  return readNonNegativeInteger(static_cast<const Block&>(*this));
}


inline uint64_t
Component::toVersion() const
{
  return toNumber();
}

inline uint64_t
Component::toSegment() const
{
  return toNumber();
}

inline int
Component::compare(const Component& other) const
{
  // Imitate ndn_Exclude_compareComponents.
  if (value_size() < other.value_size())
    return -1;
  if (value_size() > other.value_size())
    return 1;

  if (value_size() == 0)
    return 0;

  // The components are equal length.  Just do a byte compare.
  return std::memcmp(value(), other.value(), value_size());
}


template<bool T>
inline size_t
Component::wireEncode(EncodingImpl<T>& block) const
{
  size_t total_len = 0;
  if (value_size() > 0)
    total_len += block.prependByteArray (value(), value_size());
  total_len += block.prependVarNumber (value_size());
  total_len += block.prependVarNumber (Tlv::NameComponent);
  return total_len;
}

} // namespace name
} // namespace ndn

#endif // NDN_NAME_COMPONENT_HPP