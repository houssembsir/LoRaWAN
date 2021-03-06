  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#ifndef LORA_TAG_H
#define LORA_TAG_H

#include "ns3/tag.h"

namespace ns3 {

/**
 * Tag used to save various data about a packet, like its Spreading Factor and
 * data about interference.
 */
class LoraTag : public Tag
{
public:
  static TypeId GetTypeId (void);
  virtual TypeId GetInstanceTypeId (void) const;

  /**
   * Create a LoraTag with a given spreading factor and collision.
   *
   * \param sf The Spreading Factor.
   * \param destroyedBy The SF this tag's packet was destroyed by.
   */
  LoraTag (uint8_t sf = 0, uint8_t destroyedBy = 0);

  virtual ~LoraTag ();

  virtual void Serialize (TagBuffer i) const;
  virtual void Deserialize (TagBuffer i);
  virtual uint32_t GetSerializedSize () const;
  virtual void Print (std::ostream &os) const;

  /**
   * Read which Spreading Factor this packet was transmitted with.
   *
   * \return This tag's packet's SF.
   */
  uint8_t GetSpreadingFactor () const;

  /**
   * Read which Spreading Factor this packet was destroyed by.
   *
   * \return The SF this packet was destroyed by.
   */
  uint8_t GetDestroyedBy () const;

  /**
   * Read the power this packet arrived with.
   *
   * \return This tag's packet received power.
   */
  double GetReceivePower () const;

  /**
   * Set which Spreading Factor this packet was transmitted with.
   *
   * \param sf The Spreading Factor.
   */
  void SetSpreadingFactor (uint8_t sf);

  /**
   * Set which Spreading Factor this packet was destroyed by.
   *
   * \param sf The Spreading Factor.
   */
  void SetDestroyedBy (uint8_t sf);

  /**
   * Set the power this packet was received with.
   *
   * \param receivePower The power, in dBm.
   */
  void SetReceivePower (double receivePower);

  /**
   * Set the frequency of the packet.
   *
   * This value works in two ways:
   * - It is used by the GW to signal to the NS the frequency of the uplink
       packet
   * - It is used by the NS to signal to the GW the freqeuncy of a downlink
       packet
   */
  void SetFrequency (double frequency);

  /**
   * Get the frequency of the packet.
   */
  double GetFrequency (void);

  /**
   * Get the data rate for this packet.
   *
   * \return The data rate that needs to be employed for this packet.
   */
  uint8_t GetDataRate (void);

  /**
   * Set the data rate for this packet.
   *
   * \param dataRate The data rate.
   */
  void SetDataRate (uint8_t dataRate);

private:
  uint8_t m_sf; //!< The Spreading Factor used by the packet.
  uint8_t m_destroyedBy; //!< The Spreading Factor that destroyed the packet.
  double m_receivePower; //!< The reception power of this packet.
  uint8_t m_dataRate; //!< The Data Rate that needs to be used to send this
                      //!packet.
  double m_frequency; //!< The frequency of this packet
};
} // namespace ns3
#endif
