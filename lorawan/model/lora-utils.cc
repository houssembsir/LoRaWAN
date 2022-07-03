  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#include "lora-utils.h"
#include <cmath>

namespace ns3 {

double
DbToRatio (double dB)
{
  double ratio = std::pow (10.0, dB / 10.0);
  return ratio;
}

double
DbmToW (double dBm)
{
  double mW = std::pow (10.0, dBm / 10.0);
  return mW / 1000.0;
}

double
WToDbm (double w)
{
  return 10.0 * std::log10 (w * 1000.0);
}

double
RatioToDb (double ratio)
{
  return 10.0 * std::log10 (ratio);
}

} //namespace ns3
