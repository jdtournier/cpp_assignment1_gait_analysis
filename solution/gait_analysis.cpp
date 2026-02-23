#include <iostream>
#include <stdexcept>

#include "debug.h"
#include "imu.h"
#include "gait.h"
#include "utils.h"


int main (int argc, char* argv[])
{
  debug::set_verbose_mode (argc, argv);

  if (argc < 3) {
    std::cerr
      << "ERROR: expected at least 2 arguments: input file and output file\n\n"
      << "  usage: gait_analysis infile outfile [ vaccel_threshold [ vaccel_min_samples_between_cycles ] ]\n\n";
    return 1;
  }

  double vaccel_threshold = 1.5;
  int vaccel_min_samples_between_cycles = 30;

  // use optional parameters if provided on command-line:
  if (argc > 3) {
    vaccel_threshold = std::stod (argv[3]);
    if (argc > 4)
      vaccel_min_samples_between_cycles = std::stoi (argv[4]);
  }

  try {
    auto imu = load_IMU (argv[1]);

    const auto knee_flexion = compute_knee_flexion (imu);
    const auto heel_strikes = locate_heel_strikes (imu, vaccel_threshold, vaccel_min_samples_between_cycles);
    const auto peak_knee_flexion = get_peak_knee_flexion_per_cycle (knee_flexion, heel_strikes);
    const auto mean_peak_knee_flexion = compute_mean (peak_knee_flexion);

    write_report (argv[2], argv[1], heel_strikes, peak_knee_flexion, mean_peak_knee_flexion);

  }
  catch (std::exception& err) {
    std::cerr << "ERROR: " << err.what() << " - aborting\n";
    return 1;
  }
  catch (...) {
    std::cerr << "ERROR: exception of unknown type was thrown - aborting\n";
    return 1;

  }

  return 0;
}
