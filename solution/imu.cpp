#include <fstream>
#include <sstream>
#include <stdexcept>
#include <format>

#include <termviz.h>

#include "imu.h"
#include "debug.h"

IMUSet load_IMU (const std::string& filename)
{
  debug::log ("reading IMU data from file \"" + filename + "\"");

  std::ifstream infile (filename);
  if (!infile)
    throw std::runtime_error ("failed to open input file \"" + filename + "\"");


  std::string line;
  std::getline (infile, line);
  std::istringstream line_stream (line);

  IMUSet data;
  std::string label;
  while (line_stream >> label)
    data.push_back ({ label });

  if (debug::verbose_mode) {
    std::string labels;
    for (const auto& x : data)
      labels += x.label + " ";
    debug::log ("file \"" + filename + "\" contains labels: " + labels);
  }

  while (std::getline (infile, line)) {
    std::istringstream line_stream (line);
    int n = 0;
    double value;
    while (line_stream >> value) {
      if (n >= data.size())
        throw std::runtime_error ("malformed data in file \"" + filename +
            "\": too many values for number of labels");
      data[n].values.push_back (value);
      n++;
    }

  }

  debug::log (std::format ("found {} recordings of length {} in file \"{}\"",
        data.size(), data[0].values.size(), filename));

  if (debug::verbose_mode) {
    auto fig = termviz::figure (768, 256);
    for (const auto& v : data)
      fig.plot (v.values);
  }

  return data;
}




const std::vector<double>& get_recording_of (const IMUSet& imu, const std::string& label)
{
  for (const auto& rec : imu)
    if (rec.label == label)
      return rec.values;
  throw std::runtime_error ("missing recording for \"" + label + "\"");
}

