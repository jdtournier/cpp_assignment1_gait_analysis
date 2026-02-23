#pragma once

#include <vector>

#include "imu.h"


// calculate the knee flexion angle as the difference between the thing and
// shin angles, as supplied in the IMU set:
std::vector<double> compute_knee_flexion (const IMUSet& imu);

// find the indices of the heel strikes from the vertical acceleration data in
// the IMU set, by identifying samples with vertical acceleration greater than
// vaccel_threshold, ensuring at least min_interval samples between
// heel_strikes:
std::vector<int> locate_heel_strikes (const IMUSet& imu, double vaccel_threshold, int min_interval);



// compute the peak knee flexion within each interval between heel strikes:
std::vector<double> get_peak_knee_flexion_per_cycle (const std::vector<double>& knee_flexion, const std::vector<int>& heel_strikes);



// write final report to file specified, to summarise the information provided
// via the other arguments:
void write_report (const std::string& report_filename, const std::string& input_filename,
    const std::vector<int>& heel_strikes, const std::vector<double>& peak_knee_flexion,
    double mean_peak_knee_flexion);
