  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#ifndef LORA_UTILS_H
#define LORA_UTILS_H

#include "ns3/nstime.h"
#include "ns3/uinteger.h"

namespace ns3 {

/**
 * Convert from dBm to Watts.
 *
 * \param dbm the power in dBm
 *
 * \return the equivalent Watts for the given dBm
 */
double DbmToW (double dbm);
/**
 * Convert from dB to ratio.
 *
 * \param db
 *
 * \return ratio
 */
double DbToRatio (double db);
/**
 * Convert from Watts to dBm.
 *
 * \param w the power in Watts
 *
 * \return the equivalent dBm for the given Watts
 */
double WToDbm (double w);
/**
 * Convert from ratio to dB.
 *
 * \param ratio
 *
 * \return dB
 */
double RatioToDb (double ratio);

} // namespace ns3

#endif /* LORA_UTILS_H */
