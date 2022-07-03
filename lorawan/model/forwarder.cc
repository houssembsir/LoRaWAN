  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/forwarder.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("Forwarder");

NS_OBJECT_ENSURE_REGISTERED (Forwarder);

TypeId
Forwarder::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Forwarder")
    .SetParent<Application> ()
    .AddConstructor<Forwarder> ()
    .SetGroupName ("lorawan");
  return tid;
}

Forwarder::Forwarder ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

Forwarder::~Forwarder ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
Forwarder::SetPointToPointNetDevice (Ptr<PointToPointNetDevice>
                                     pointToPointNetDevice)
{
  NS_LOG_FUNCTION (this << pointToPointNetDevice);

  m_pointToPointNetDevice = pointToPointNetDevice;
}

void
Forwarder::SetLoraNetDevice (Ptr<LoraNetDevice> loraNetDevice)
{
  NS_LOG_FUNCTION (this << loraNetDevice);

  m_loraNetDevice = loraNetDevice;
}

bool
Forwarder::ReceiveFromLora (Ptr<NetDevice> loraNetDevice, Ptr<const Packet>
                            packet, uint16_t protocol, const Address& sender)
{
  NS_LOG_FUNCTION (this << packet << protocol << sender);

  Ptr<Packet> packetCopy = packet->Copy ();

  m_pointToPointNetDevice->Send (packetCopy,
                                 m_pointToPointNetDevice->GetBroadcast (),
                                 0x800);

  return true;
}

bool
Forwarder::ReceiveFromPointToPoint (Ptr<NetDevice> pointToPointNetDevice,
                                    Ptr<const Packet> packet, uint16_t protocol,
                                    const Address& sender)
{
  NS_LOG_FUNCTION (this << packet << protocol << sender);

  Ptr<Packet> packetCopy = packet->Copy ();

  m_loraNetDevice->Send (packetCopy);

  return true;
}

void
Forwarder::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  // TODO Make sure we are connected to both needed devices
}

void
Forwarder::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // TODO Get rid of callbacks
}

}
