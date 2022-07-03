  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/one-shot-sender.h"
#include "ns3/end-device-lora-mac_B.h"
#include "ns3/pointer.h"
#include "ns3/log.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/lora-net-device.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OneShotSender");

NS_OBJECT_ENSURE_REGISTERED (OneShotSender);

TypeId
OneShotSender::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::OneShotSender")
    .SetParent<Application> ()
    .AddConstructor<OneShotSender> ()
    .SetGroupName ("lorawan");
  return tid;
}

OneShotSender::OneShotSender ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

OneShotSender::OneShotSender (Time sendTime)
  : m_sendTime (sendTime)
{
  NS_LOG_FUNCTION_NOARGS ();
}

OneShotSender::~OneShotSender ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
OneShotSender::SetSendTime (Time sendTime)
{
  NS_LOG_FUNCTION (this << sendTime);

  m_sendTime = sendTime;
}

void
OneShotSender::SendPacket (void)
{
  NS_LOG_FUNCTION (this);

  // Create and send a new packet
  Ptr<Packet> packet = Create<Packet> (10);
  m_mac->Send (packet);
}

void
OneShotSender::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  // Make sure we have a MAC layer
  if (m_mac == 0)
    {
      // Assumes there's only one device
      Ptr<LoraNetDevice> loraNetDevice = m_node->GetDevice (0)->GetObject<LoraNetDevice> ();

      m_mac = loraNetDevice->GetMac ();
      NS_ASSERT (m_mac != 0);
    }

  // Schedule the next SendPacket event
  Simulator::Cancel (m_sendEvent);
  m_sendEvent = Simulator::Schedule (m_sendTime, &OneShotSender::SendPacket,
                                     this);
}

void
OneShotSender::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
  Simulator::Cancel (m_sendEvent);
}
}
