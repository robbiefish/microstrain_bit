#include <cstring>
#include <iostream>
#include <algorithm>

#include <mscl/mscl.h>

using BITTuple = std::tuple<uint8_t, uint8_t>;  /// First element represents the byte, second element represents bit within byte
using BITMap = std::map<BITTuple, std::string>;

// Mapping between names of tests and commanded BIT and bytes (key is the tuple so that the order makes sense)
static const BITMap commanded_mappings = {
    {{0, 0}, "General Hardware Fault"},
    {{0, 1}, "General Firmware Fault"},
    {{0, 2}, "Timing Overrun        "},
    {{0, 3}, "Buffer Overrun        "},
    {{1, 0}, "IMU IPC Fault         "},
    {{1, 1}, "Nav IPC Fault         "},
    {{1, 2}, "GNSS IPC Fault        "},
    {{1, 3}, "Comms Fault           "},
    {{1, 4}, "IMU Accel Fault       "},
    {{1, 5}, "IMU Gyro Fault        "},
    {{1, 6}, "IMU Mag Fault         "},
    {{1, 7}, "IMU Press Fault       "},
    {{2, 2}, "IMU Calibration Error "},
    {{2, 3}, "IMU General Fault     "},
    {{2, 6}, "Filter Solution Fault "},
    {{2, 7}, "Filter General Fault  "},
    {{3, 0}, "GNSS Receiver 1 Fault "},
    {{3, 1}, "GNSS Antenna 1 Fault  "},
    {{3, 2}, "GNSS Receiver 2 Fault "},
    {{3, 3}, "GNSS Antenna 2 Fault  "},
    {{3, 4}, "GNSS RTCM Failure     "},
    {{3, 5}, "GNSS RTK Dongle Fault "},
    {{3, 6}, "GNSS Solution Fault   "},
    {{3, 7}, "GNSS General Fault    "},
};

// Mapping between names of tests and continuous BIT and bytes (key is the tuple so that the order makes sense)
static const BITMap continuous_mappings = {
    {{0, 0}, "Clock Failure                          "},
    {{0, 1}, "Power Fault                            "},
    {{0, 2}, "RTC Fault                              "},
    {{0, 4}, "Comms Firmware Fault                   "},
    {{0, 5}, "Comms Timing Overrun                   "},
    {{0, 6}, "Comms Buffer Overrun                   "},
    {{2, 0}, "IMU Control Fault                      "},
    {{2, 1}, "IMU Command Fault                      "},
    {{2, 2}, "IMU Desync                             "},
    {{2, 3}, "IMU Data Frame Loss                    "},
    {{2, 4}, "Nav Control Fault                      "},
    {{2, 5}, "Nav Command Fault                      "},
    {{2, 6}, "Nav Desync                             "},
    {{2, 7}, "Nav Data Frame Loss                    "},
    {{3, 0}, "GNSS Control Fault                     "},
    {{3, 1}, "GNSS Command Fault                     "},
    {{3, 2}, "GNSS Desync                            "},
    {{3, 3}, "GNSS Data Frame Loss                   "},
    {{4, 0}, "IMU Clock Fault                        "},
    {{4, 1}, "IMU Communications Fault               "},
    {{4, 2}, "IMU timing Fault                       "},
    {{4, 3}, "IMU Mems Power Fault                   "},
    {{4, 4}, "IMU Calibration Table Invalid (Accel)  "},
    {{4, 5}, "IMU Calibration Table Invalid (Gyro)   "},
    {{4, 6}, "IMU Calibration Table Invalid (Mag)    "},
    {{5, 0}, "IMU Accel Error                        "},
    {{5, 1}, "IMU Accel Overrange                    "},
    {{5, 2}, "IMU Accel Previous Commanded BIT Result"},
    {{5, 4}, "IMU Gyro Error                         "},
    {{5, 5}, "IMU Gyro Overrange                     "},
    {{5, 6}, "IMU Gyro Previous Commanded BIT Result "},
    {{6, 0}, "IMU Mag Error                          "},
    {{6, 1}, "IMU Mag Overrange                      "},
    {{6, 2}, "IMU Mag Previous Commanded BIT Result  "},
    {{6, 4}, "IMU Press Error                        "},
    {{6, 5}, "IMU Press Overrange                    "},
    {{6, 6}, "IMU Press Previous Commanded BIT Result"},
    {{8, 0}, "Nav Clock Fault                        "},
    {{8, 1}, "Nav Hardware Fault                     "},
    {{8, 2}, "Nav Minor Cycle Overrun                "},
    {{8, 3}, "Nav Major Cycle Overrun                "},
    {{8, 4}, "Nav-IMU SPI Fault                      "},
    {{8, 5}, "Nav-GNSS SPI Fault                     "},
    {{8, 6}, "Nav-Comms SPI Fault                    "},
    {{8, 7}, "Nav-Comms UART Fault                   "},
    {{12, 0}, "GNSS Clock Fault                       "},
    {{12, 1}, "GNSS Hardware Fault                    "},
    {{12, 2}, "GNSS-Comms UART Fault                  "},
    {{12, 3}, "GNSS-Comms SPI Fault                   "},
    {{12, 4}, "GNSS-Nav SPI Fault                     "},
    {{12, 5}, "GNSS GPS Time Invalid                  "},
    {{12, 6}, "GNSS Minor Cycle Overrun               "},
    {{13, 0}, "GNSS 1 Comm Fault                      "},
    {{13, 1}, "GNSS 1 Antenna Short                   "},
    {{13, 2}, "GNSS 1 Antenna Open                    "},
    {{13, 3}, "GNSS 1 Solution Fault                  "},
    {{13, 4}, "GNSS 2 Comm Fault                      "},
    {{13, 5}, "GNSS 2 Antenna Short                   "},
    {{13, 6}, "GNSS 2 Antenna Open                    "},
    {{13, 7}, "GNSS 2 Solution Fault                  "},
    {{14, 0}, "GNSS RTCM Comm Fault                   "},
    {{14, 1}, "GNSS RTK Dongle Fault                  "},
};

// Helper function to print BIT status
void printBITInfo(const std::string& name, const BITMap& mappings, const mscl::Bytes& bytes)
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

int main(int argc, char** argv)
{
    // Accept some parameters
    std::string port = "/dev/ttyACM0";
    uint32_t baud = 115200;
    if (argc >= 2)
        port = argv[1];
    if (argc >= 3)
        baud = std::atoi(argv[2]);

    // Initialize the node
    mscl::InertialNode node(mscl::Connection::Serial(port, baud));

    // Force the node to idle    
    try { node.setToIdle(); } catch (mscl::Error& e) { sleep(1); }
    node.setToIdle();

    // Set a higher timeout
    node.timeout(10000);

    // Empty params for both commands
    mscl::ByteStream param_data;

    // Run the commanded BIT first
    mscl::Bytes commanded_res = node.doCommand(0x01, 0x05, param_data.data(), true, true, 0x83);

    // Run the continuous BIT now that the commanded BIT has updated the state
    mscl::Bytes continuous_res = node.doCommand(0x01, 0x08, param_data.data(), true, true, 0x88);

    // Swap the order of the commanded bytes to fit the way the map is setup, and look closer to how the documentation documents it
    std::reverse(commanded_res.begin(), commanded_res.end());

    // Use the maps to print the status of the commanded bits
    printBITInfo("Commanded BIT", commanded_mappings, commanded_res);
    std::cout << std::endl;
    printBITInfo("Continuous BIT", continuous_mappings, continuous_res);
}