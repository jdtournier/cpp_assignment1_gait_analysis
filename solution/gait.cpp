#include <vector>
#include <stdexcept>
#include <fstream>

#include <termviz.h>

#include "debug.h"
#include "imu.h"
#include "gait.h"


std::vector<double> compute_knee_flexion (const IMUSet& imu)
{
  const auto& thigh_angle = get_recording_of (imu, "THIGH_ANG");
  const auto& shin_angle = get_recording_of (imu, "SHIN_ANG");

  std::vector<double> knee_flexion (thigh_angle.size());
  for (int n = 0; n < knee_flexion.size(); n++)
    knee_flexion[n] = thigh_angle[n] - shin_angle[n];

  // output useful information if running in debug mode:
  if (debug::verbose_mode) {
    debug::log ("knee flexion:");
    termviz::figure (768, 256)
      .plot (knee_flexion);
  }

  return knee_flexion;
}



std::vector<int> locate_heel_strikes (const IMUSet& imu, double vaccel_threshold, int min_interval)
{
  const auto& vaccel = get_recording_of (imu, "VERT_ACCEL");

  std::vector<int> heel_strikes;
  for (int n = 0; n < vaccel.size(); n++)
    if (vaccel[n] > vaccel_threshold)
      if (heel_strikes.empty() || n > heel_strikes.back()+min_interval)
        heel_strikes.push_back (n);

  // output useful information if running in debug mode:
  if (debug::verbose_mode) {
    debug::log ("vertical acceleration:");
    termviz::figure (768, 256)
      .plot (vaccel);

    std::string times;
    for (const auto t : heel_strikes)
      times += std::to_string (t) + " ";
    debug::log ("found heel strikes at indices: " + times);
  }

  return heel_strikes;
}


std::vector<double> get_peak_knee_flexion_per_cycle (const std::vector<double>& knee_flexion, const std::vector<int>& heel_strikes)
{
  std::vector<double> peak_knee_flexion;

  for (int n = 1; n < heel_strikes.size(); n++) {
    double peak_value = 0.0;
    for (int i = heel_strikes[n-1]; i < heel_strikes[n]; i++)
      peak_value = std::max (peak_value, knee_flexion[i]);
    peak_knee_flexion.push_back (peak_value);
  }

  // output useful information if running in debug mode:
  if (debug::verbose_mode) {
    std::string peaks;
    for (const auto p : peak_knee_flexion)
      peaks += std::to_string (p) + " ";
    debug::log ("peak knee flexion per cycle: " + peaks);
  }

  return peak_knee_flexion;
}



void write_report (const std::string& report_filename, const std::string& input_filename,
    const std::vector<int>& heel_strikes, const std::vector<double>& peak_knee_flexion,
    double mean_peak_knee_flexion)
{
  std::ofstream outfile (report_filename);
  if (!outfile)
    throw std::runtime_error ("failed to open output report file \"" + report_filename + "\"");

  outfile
    << "IMU file: " << input_filename << "\n"
    << "Heel strikes detected at sample index: ";
  for (auto t : heel_strikes)
    outfile << t << " ";
  outfile << "\nCycle durations: ";
  for (int n = 1; n < heel_strikes.size(); n++)
    outfile << heel_strikes[n]-heel_strikes[n-1] << " ";
  outfile << "\nPeak knee flexion per cycle: ";
  for (auto p : peak_knee_flexion)
    outfile << p << " ";
  outfile << "\nAverage peak knee flexion: " << mean_peak_knee_flexion
    << "\nRecovery status: ";
  if (mean_peak_knee_flexion < 60.0)
    outfile << "poor";
  else
    outfile << "good";
  outfile << "\n";
}


