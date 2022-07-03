  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef FORWARDER_H
#define FORWARDER_H

#include "ns3/application.h"
#include "ns3/lora-net-device.h"
#include "ns3/point-to-point-net-device.h"
#include "ns3/nstime.h"
#include "ns3/attribute.h"

namespace ns3 {

/**
 * This application forwards packets between NetDevices:
 * LoraNetDevice -> PointToPointNetDevice and vice versa.
 */
class Forwarder : public Application
{
public:

  Forwarder ();
  ~Forwarder ();

  static TypeId GetTypeId (void);

  /**
   * Sets the device to use to communicate with the EDs.
   *
   * \param loraNetDevice The LoraNetDevice on this node.
   */
  void SetLoraNetDevice (Ptr<LoraNetDevice> loraNetDevice);

  /**
   * Sets the P2P device to use to communicate with the NS.
   *
   * \param pointToPointNetDevice The P2PNetDevice on this node.
   */
  void SetPointToPointNetDevice (Ptr<PointToPointNetDevice> pointToPointNetDevice);

  /**
   * Receive a packet from the LoraNetDevice.
   *
   * \param loraNetDevice The LoraNetDevice we received the packet from.
   * \param packet The packet we received.
   * \param protocol The protocol number associated to this packet.
   * \param sender The address of the sender.
   * \returns True if we can handle the packet, false otherwise.
   */
  bool ReceiveFromLora (Ptr<NetDevice> loraNetDevice, Ptr<const Packet> packet,
                        uint16_t protocol, const Address& sender);

  /**
   * Receive a packet from the PointToPointNetDevice
   */
  bool ReceiveFromPointToPoint (Ptr<NetDevice> pointToPointNetDevice,
                                Ptr<const Packet> packet, uint16_t protocol,
                                const Address& sender);

  /**
   * Start the application
   */
  void StartApplication (void);

  /**
   * Stop the application
   */
  void StopApplication (void);

private:
  Ptr<LoraNetDevice> m_loraNetDevice; //!< Pointer to the node's LoraNetDevice

  Ptr<PointToPointNetDevice> m_pointToPointNetDevice; //!< Pointer to the
                                                      //!P2PNetDevice we use to
                                                      //!communicate with the NS
};

} //namespace ns3

#endif /* FORWARDER */
