#pragma once

#include <vector>
#include <string>

// A structure to hold a vector of recorded values, along with their label (as
// read from the IMU file):
struct IMU {
  std::string label;
  std::vector<double> values;
};

// Concevience type alias for a set of recordings read from file:
using IMUSet = std::vector<IMU>;

// load IMU recordings from the file specified, and return the data as an
// IMUSet:
IMUSet load_IMU (const std::string& filename);

// convenience function to get a recording from an IMUSet given its label:
const std::vector<double>& get_recording_of (const IMUSet& imu, const std::string& label);
