  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef SUB_BAND_H
#define SUB_BAND_H

#include "ns3/object.h"
#include "ns3/logical-lora-channel.h"
#include "ns3/nstime.h"

namespace ns3 {

class LogicalLoraChannel;

/**
 * Class representing a SubBand, i.e., a frequency band subject to some
 * regulations on duty cycle and transmission power.
 */
class SubBand : public Object
{
public:

  static TypeId GetTypeId (void);

  SubBand ();

  /**
   * Create a new SubBand by specifying all of its properties.
   *
   * \param firstFrequency The SubBand's lowest frequency.
   * \param lastFrequency The SubBand's highest frequency.
   * \param dutyCycle The duty cycle (as a fraction) allowed on this SubBand.
   * \param maxTxPowerDbm The maximum transmission power [dBm] allowed on this SubBand.
   */
  SubBand (double firstFrequency, double lastFrequency, double dutyCycle, double maxTxPowerDbm);

  virtual ~SubBand ();

  /**
   * Get the lowest frequency of the SubBand.
   *
   * \return The lowest frequency of the SubBand.
   */
  double GetFirstFrequency (void);

  /**
   * Get the last frequency of the subband.
   *
   * \return The lowest frequency of the SubBand.
   */
  // double GetLastFrequency (void);

  /**
   * Get the duty cycle of the subband.
   *
   * \return The duty cycle (as a fraction) that needs to be enforced on this
   * SubBand.
   */
  double GetDutyCycle (void);

  /**
   * Update the next transmission time.
   *
   * This function is used by LogicalLoraChannelHelper, which computes the time
   * based on the SubBand's duty cycle and on the transmission duration.
   *
   * \param nextTime The future time from which transmission should be allowed
   * again.
   */
  void SetNextTransmissionTime (Time nextTime);

  /**
   * Returns the next time from which transmission on this subband will be
   * possible.
   *
   * \return The next time at which transmission in this SubBand will be
   * allowed.
   */
  Time GetNextTransmissionTime (void);

  /**
   * Return whether or not a frequency belongs to this SubBand.
   *
   * \param frequency the frequency we want to test against the current subband
   * \return True if the frequency is between firstFrequency and lastFrequency,
   * false otherwise.
   */
  bool BelongsToSubBand (double frequency);

  /**
   * Return whether or not a channel belongs to this SubBand.
   *
   * \param channel the channel we want to test against the current subband
   * \return True if the channel's center frequency is between firstFrequency
   * and lastFrequency, false otherwise.
   */
  bool BelongsToSubBand (Ptr<LogicalLoraChannel> channel);

  /**
   * Set the maximum transmission power that is allowed on this SubBand.
   *
   * \param maxTxPowerDbm The maximum transmission power [dBm] to set.
   */
  void SetMaxTxPowerDbm (double maxTxPowerDbm);

  /**
   * Return the maximum transmission power that is allowed on this SubBand
   *
   * \return The maximum transmission power, in dBm.
   */
  double GetMaxTxPowerDbm (void);

private:

  double m_firstFrequency;   //!< Starting frequency of the subband, in MHz
  double m_lastFrequency;   //!< Ending frequency of the subband, in MHz
  double m_dutyCycle;   //!< The duty cycle that needs to be enforced on this subband
  Time m_nextTransmissionTime;   //!< The next time a transmission will be allowed in this subband
  double m_maxTxPowerDbm;   //!< The maximum transmission power that is admitted on this subband
};
} /* namespace ns3 */
#endif /* SUB_BAND_H */
