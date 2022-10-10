#pragma once

#include <map>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <algorithm>

using BITTuple = std::tuple<uint8_t, uint8_t>;  /// First element represents the byte, second element represents bit within byte
using BITMap = std::map<BITTuple, std::string>;

inline void fixMipString(char* str, const size_t str_len)
{
  // Trim the spaces from the start of the string
  std::string cpp_str = std::string(str, str_len);
  cpp_str.erase(cpp_str.begin(), std::find_if(cpp_str.begin(), cpp_str.end(), [](unsigned char c)
  {
    return !std::isspace(c);
  }));

  // If the size of the resulting string is the same as the initial string, we can't trim it efficiently
  if (cpp_str.size() >= str_len)
    return;

  // Set the string to the same string but without the padding spaces and null terminate it
  const std::string& cpp_str_null_terminated = std::string(cpp_str.c_str());
  memset(str, 0, str_len);
  memcpy(str, cpp_str_null_terminated.c_str(), cpp_str_null_terminated.size());
  str[cpp_str_null_terminated.size()] = 0;
}

// Helper function to print BIT status
inline void printBITInfo(const std::string& name, const BITMap& mappings, const uint8_t* bytes, const size_t bytes_len)
{
    // Print the raw values first
    std::ios_base::fmtflags old_flags(std::cout.flags());
    std::cout << name << ": ";
    for (size_t i = 0; i < bytes_len; i++)
    {
        const uint8_t byte = bytes[i];
        std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<uint32_t>(byte);
    }
    std::cout << std::endl;
    std::cout.flags(old_flags);

    // Print the parsed flags
    for (const auto& mapping : mappings)
    {
        const std::string& name = mapping.second;
        const uint8_t byte_in_payload = std::get<0>(mapping.first);
        const uint8_t bit_in_byte = std::get<1>(mapping.first);
        const uint32_t bit_in_payload = (byte_in_payload * 8) + bit_in_byte;
        const bool test_value = (bytes[byte_in_payload] >> bit_in_byte) & 0x01;

        std::cout << "  " << name << ": " << (test_value ? "True " : "False") << " (bit = " << bit_in_payload << ")" << std::endl;
    }
}