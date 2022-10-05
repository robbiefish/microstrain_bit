#pragma once

#include <cstring>
#include <iostream>
#include <algorithm>

#include <mscl/mscl.h>

using BITTuple = std::tuple<uint8_t, uint8_t>;  /// First element represents the byte, second element represents bit within byte
using BITMap = std::map<BITTuple, std::string>;

// Helper function to print BIT status
inline void printBITInfo(const std::string& name, const BITMap& mappings, const mscl::Bytes& bytes)
{
    // Print the raw values first
    std::ios_base::fmtflags old_flags(std::cout.flags());
    std::cout << name << ": ";
    for (const auto& byte : bytes)
    {
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
        const bool test_value = (bytes.at(byte_in_payload) >> bit_in_byte) & 0x01;

        std::cout << "  " << name << ": " << (test_value ? "True " : "False") << " (bit = " << bit_in_payload << ")" << std::endl;
    }
}