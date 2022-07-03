  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#ifndef LORA_NET_DEVICE_H
#define LORA_NET_DEVICE_H

#include "ns3/net-device.h"
#include "ns3/lora-channel.h"
#include "ns3/lora-phy.h"
#include "ns3/lora-mac.h"

namespace ns3 {

class LoraChannel;
class LoraPhy;
class LoraMac;

/**
 * Hold together all LoRa related objects.
 *
 * This class holds together pointers to LoraChannel, LoraPhy and LoraMac,
 * exposing methods through which Application instances can send packets. The
 * Application only needs to craft its packets, the NetDevice will take care of
 * calling the LoraMac's Send method with the appropriate parameters.
 */
class LoraNetDevice : public NetDevice
{
public:
  static TypeId GetTypeId (void);

  // Constructor and destructor
  LoraNetDevice ();
  virtual ~LoraNetDevice ();

  /**
   * Set which LoraMac instance is linked to this device.
   *
   * \param mac the mac layer to use.
   */
  void SetMac (Ptr<LoraMac> mac);

  /**
   * Set which LoraPhy instance is linked to this device.
   *
   * \param phy the phy layer to use.
   */
  void SetPhy (Ptr<LoraPhy> phy);

  /**
   * Get the LoraMac instance that is linked to this NetDevice.
   *
   * \return the mac we are currently using.
   */
  Ptr<LoraMac> GetMac (void) const;

  /**
   * Get the LoraPhy instance that is linked to this NetDevice.
   *
   * \return the phy we are currently using.
   */
  Ptr<LoraPhy> GetPhy (void) const;

  /**
   * Send a packet through the LoRaWAN stack.
   *
   * \param packet The packet to send.
   */
  void Send (Ptr<Packet> packet);

  /**
   * Callback the Mac layer calls whenever a packet arrives and needs to be
   * forwarded up the stack.
   *
   * \param packet The packet that was received.
   */
  void Receive (Ptr<Packet> packet);

  // From class NetDevice. Some of these have little meaning for a LoRaWAN
  // network device (since, for instance, IP is not used in the standard)
  virtual void SetReceiveCallback (NetDevice::ReceiveCallback cb);
  virtual Ptr<Channel> GetChannel (void) const;
  virtual void SetNode (Ptr<Node> node);
  virtual Ptr<Node> GetNode (void) const;

  virtual void SetIfIndex (const uint32_t index);
  virtual uint32_t GetIfIndex (void) const;
  virtual void SetAddress (Address address);
  virtual Address GetAddress (void) const;
  virtual bool SetMtu (const uint16_t mtu);
  virtual uint16_t GetMtu (void) const;
  virtual bool IsLinkUp (void) const;
  virtual void AddLinkChangeCallback (Callback<void> callback);
  virtual bool IsBroadcast (void) const;
  virtual Address GetBroadcast (void) const;
  virtual bool IsMulticast (void) const;
  virtual Address GetMulticast (Ipv4Address multicastGroup) const;
  virtual Address GetMulticast (Ipv6Address addr) const;
  virtual bool IsBridge (void) const;
  virtual bool IsPointToPoint (void) const;
  virtual bool Send (Ptr<Packet> packet, const Address& dest, uint16_t protocolNumber);
  virtual bool SendFrom (Ptr<Packet> packet, const Address& source, const Address& dest, uint16_t protocolNumber);
  virtual bool NeedsArp (void) const;
  virtual void SetPromiscReceiveCallback (PromiscReceiveCallback cb);
  virtual bool SupportsSendFrom (void) const;

protected:

  /**
   * Receive a packet from the lower layer and pass the
   * packet up the stack.
   *
   * \param packet The packet we need to forward.
   * \param from The from address.
   * \param to The to address.
   */
  void ForwardUp (Ptr<Packet> packet, Mac48Address from, Mac48Address to);

private:

  /**
   * Return the LoraChannel this device is connected to.
   */
  Ptr<LoraChannel> DoGetChannel (void) const;

  /**
   * Complete the configuration of this LoRa device by connecting all lower
   * components (PHY, MAC, Channel) together.
   */
  void CompleteConfig (void);

  // Member variables
  Ptr<Node> m_node; //!< The Node this NetDevice is connected to.
  Ptr<LoraPhy> m_phy; //!< The LoraPhy this NetDevice is connected to.
  Ptr<LoraMac> m_mac; //!< The LoraMac this NetDevice is connected to.
  bool m_configComplete; //!< Whether the configuration was already completed.

  /**
   * Upper layer callback used for notification of new data packet arrivals.
   */
  NetDevice::ReceiveCallback m_receiveCallback;
};

} //namespace ns3

#endif /* LORA_NET_DEVICE_H */
