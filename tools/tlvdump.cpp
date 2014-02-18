/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil -*- */
/**
 * Copyright (C) 2013 Regents of the University of California.
 * @author: Jeff Thompson <jefft0@remap.ucla.edu>
 * See COPYING for copyright and distribution information.
 */

#include "face.hpp"
#include "encoding/block.hpp"

#include <iomanip>
#include <fstream>

const uint32_t TLV_DICT_SIZE = 30;

const std::string TLV_DICT[TLV_DICT_SIZE] = {
  "RESERVED", //      = 0
  "RESERVED", //      = 1
  "RESERVED", //      = 2
  "RESERVED", //      = 3
  "RESERVED", //      = 4
  "Interest", //      = 5,
  "Data", //          = 6,
  "Name", //          = 7,
  "NameComponent", // = 8,
  "Selectors", //     = 9,
  "Nonce", //         = 10,
  "Scope", //         = 11,
  "InterestLifetime", //          = 12,
  "MinSuffixComponents", //       = 13,
  "MaxSuffixComponents", //       = 14,
  "PublisherPublicKeyLocator", // = 15,
  "Exclude", //       = 16,
  "ChildSelector", // = 17,
  "MustBeFresh", //   = 18,
  "Any", //           = 19,
  "MetaInfo", //      = 20,
  "Content", //       = 21,
  "SignatureInfo", // = 22,
  "SignatureValue", // = 23,
  "ContentType", //   = 24,
  "FreshnessPeriod", // = 25,
  "FinalBlockId" // = 26
  "SignatureType", // = 27,
  "KeyLocator", //    = 28,
  "KeyLocatorDigest", // = 29
};

void
printTypeInfo(uint32_t type)
{
  std::cout << type << " (";

  if (type < TLV_DICT_SIZE) {
    std::cout << TLV_DICT[type];
  }
  else if (TLV_DICT_SIZE <= type && type < 128) {
    std::cout << "RESERVED_1";
  }
  else if (128 <= type && type < 253) {
    std::cout << "APP_TAG_1";
  }
  else if (253 <= type && type < 32767) {
    std::cout << "RESERVED_3";
  }
  else {
    std::cout << "APP_TAG_3";
  }
  std::cout << ")";
}

  
void
BlockPrinter(const ndn::Block& block, const std::string& indent="")
{
  std::cout << indent;
  printTypeInfo(block.type());
  std::cout << " (size: " << block.value_size() << ")";

  try {
    // if (block.type() != ndn::Tlv::Content && block.type() != ndn::Tlv::SignatureValue)
    block.parse();
  }
  catch(ndn::Tlv::Error &e) {
    // pass (e.g., leaf block reached)

    // @todo: Figure how to deterministically figure out that value is not recursive TLV block
  }

  if (block.elements().empty())
    {
      std::cout << " [[";
      ndn::name::Component(block.value(), block.value_size()).toEscapedString(std::cout);
      std::cout<< "]]";
    }
  std::cout << std::endl;
  
  for(ndn::Block::element_const_iterator i = block.elements_begin();
      i != block.elements_end();
      ++i)
    {
      BlockPrinter(*i, indent+"  ");
    }
}

void
HexPrinter(const ndn::Block& block, const std::string &indent="")
{
  std::cout << indent;
  for (ndn::Buffer::const_iterator i = block.begin (); i != block.value_begin(); ++i)
    {
      std::cout << "0x" << std::noshowbase << std::hex << std::setw(2) << std::setfill('0') << (int)*i;
      std::cout << ", ";
    }
  std::cout << "\n";
  
  if (block.elements_size() == 0 && block.value_size() > 0)
    {
      std::cout << indent << "    ";
      for (ndn::Buffer::const_iterator i = block.value_begin (); i != block.value_end(); ++i)
      {
        std::cout << "0x" << std::noshowbase << std::hex << std::setw(2) << std::setfill('0') << (int)*i;
        std::cout << ", ";
      }
      std::cout << "\n";
    }
  else
    {
      for(ndn::Block::element_const_iterator i = block.elements_begin();
          i != block.elements_end();
          ++i)
        {
          HexPrinter(*i, indent+"    ");
        }
    }
}

int main(int argc, const char *argv[])
{
  unsigned char buf[9000];
  std::streamsize s = 0;
  if (argc == 1 ||
      (argc == 2 && std::string(argv[1]) == "-"))
    {
      std::cin.read(reinterpret_cast<char*>(buf), 9000);
      s = std::cin.gcount();
    }
  else
    {
      std::ifstream file(argv[1]);
      file.read(reinterpret_cast<char*>(buf), 9000);
      s = file.gcount();
    }

  try {
    ndn::Block block(buf, s);
    BlockPrinter(block, "");
    // HexPrinter(block, "");
  }
  catch(std::exception &e) {
    std::cerr << "ERROR: "<< e.what() << std::endl;
  }
  
  return 0;
}