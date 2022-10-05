#include <cstring>
#include <iostream>
#include <algorithm>

#include <mscl/mscl.h>

#include "bit_utils.hpp"
#include "gq7_bit.hpp"

static constexpr auto GQ7_MODEL_NAME = "3DM-GQ7";

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

    // Print some info about the device
    const std::string& model_name = node.modelName();
    const std::string& serial_number = node.serialNumber();
    std::cout << "#########################" << std::endl
              << "Model Name:    " << model_name << std::endl
              << "Serial Number: " << serial_number << std::endl
              << "#########################" << std::endl << std::endl;

    // Depending on the devive, use different mappings
    BITMap commanded_mappings;
    BITMap continuous_mappings;
    if (model_name == GQ7_MODEL_NAME)
    {
        commanded_mappings = gq7::commanded_mappings;
        continuous_mappings = gq7::continuous_mappings;
    }
    else
    {
        std::cout << "!!! Unsupported device, only printing raw hex values" << std::endl << std::endl;
    }

    // Use the maps to print the status of the commanded bits
    printBITInfo("Commanded BIT", commanded_mappings, commanded_res);
    std::cout << std::endl;
    printBITInfo("Continuous BIT", continuous_mappings, continuous_res);
}