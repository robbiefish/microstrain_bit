#include <chrono>
#include <cstring>
#include <iostream>
#include <algorithm>

#include <mip/mip_all.hpp>
#include <mip/platform/serial_connection.hpp>

#include "bit_utils.hpp"
#include "gq7_bit.hpp"

static constexpr auto GQ7_MODEL_NAME = "3DM-GQ7";

mip::Timestamp getCurrentTimestamp()
{
  using namespace std::chrono;
  return duration_cast<milliseconds>( steady_clock::now().time_since_epoch() ).count();
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

    // Initialize the device handle
    uint8_t buffer[1024];
    mip::platform::SerialConnection connection(port, baud);
    mip::DeviceInterface device(&connection, buffer, sizeof(buffer), mip::C::mip_timeout_from_baudrate(baud), 10000);

    // Force the device to idle
    if (!mip::commands_base::setIdle(device))
    {
        sleep(1);
        mip::commands_base::setIdle(device);
    }

    // Get some information on the type of device
    mip::commands_base::BaseDeviceInfo device_info;
    mip::commands_base::getDeviceInfo(device, &device_info);
    fixMipString(device_info.model_name, sizeof(device_info.model_name));
    fixMipString(device_info.serial_number, sizeof(device_info.serial_number));

    // Run the commanded BIT first
    uint32_t commanded_bit_raw;
    mip::commands_base::builtInTest(device, &commanded_bit_raw);

    sleep(1);

    // Run the continuous BIT now that the commanded BIT has updated the state
    uint8_t continuous_bit[16];
    mip::commands_base::continuousBit(device, continuous_bit);

    // Convert the commanded BIT to a uint8 array, and reverse the order
    uint8_t commanded_bit[sizeof(commanded_bit_raw)];
    commanded_bit[3] = (commanded_bit_raw >> 24) & 0xFF;
    commanded_bit[2] = (commanded_bit_raw >> 16) & 0xFF;
    commanded_bit[1] = (commanded_bit_raw >> 8) & 0xFF;
    commanded_bit[0] = (commanded_bit_raw) & 0xFF;

    // Print some info about the device
    std::cout << "#########################" << std::endl
              << "Model Name:    " << device_info.model_name << std::endl
              << "Serial Number: " << device_info.serial_number << std::endl
              << "#########################" << std::endl << std::endl;

    // Depending on the devive, use different mappings
    BITMap commanded_mappings;
    BITMap continuous_mappings;
    if (std::string(device_info.model_name) == GQ7_MODEL_NAME)
    {
        commanded_mappings = gq7::commanded_mappings;
        continuous_mappings = gq7::continuous_mappings;
    }
    else
    {
        std::cout << "!!! Unsupported device, only printing raw hex values" << std::endl << std::endl;
    }

    // Use the maps to print the status of the commanded bits
    printBITInfo("Commanded BIT", commanded_mappings, commanded_bit, sizeof(commanded_bit));
    std::cout << std::endl;
    printBITInfo("Continuous BIT", continuous_mappings, continuous_bit, sizeof(continuous_bit));
}