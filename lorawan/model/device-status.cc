
  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#include "ns3/device-status.h"
#include "ns3/log.h"
#include <algorithm>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("DeviceStatus");

DeviceStatus::DeviceStatus ()
{
  NS_LOG_FUNCTION (this);
}

DeviceStatus::~DeviceStatus ()
{
  NS_LOG_FUNCTION (this);
}

DeviceStatus::DeviceStatus (Ptr<EndDeviceLoraMac> endDeviceMac) :
  m_mac (endDeviceMac)
{
  NS_LOG_FUNCTION (this);

  m_reply = DeviceStatus::Reply ();
}

uint8_t
DeviceStatus::GetDataRate ()
{
  NS_LOG_FUNCTION (this);

  return m_mac->GetDataRate ();
}

LoraDeviceAddress
DeviceStatus::GetAddress ()
{
  NS_LOG_FUNCTION (this);

  return m_address;
}

void
DeviceStatus::SetAddress (LoraDeviceAddress address)
{
  NS_LOG_FUNCTION (this);

  m_address = address;
}

void
DeviceStatus::UpdateGatewayData (Address gwAddress, double rcvPower)
{
  NS_LOG_FUNCTION (this << gwAddress << rcvPower);

  std::map<Address, double>::iterator it = m_gateways.find (gwAddress);
  if (it != m_gateways.end ())
    {
      // Erase the existing entry
      m_gateways.erase (it);
    }
  // Create a new entry
  m_gateways.insert (std::pair<Address, double> (gwAddress, rcvPower));
}

Address
DeviceStatus::GetBestGatewayAddress (void)
{
  NS_LOG_FUNCTION (this);

  return (*(std::max_element (m_gateways.begin (), m_gateways.end (),
                              [] (const std::pair<Address, double>&p1,
                                  const std::pair<Address, double>&p2)
                              { return p1.second > p2.second; }
                              ))).first;
}

std::list<Address>
DeviceStatus::GetSortedGatewayAddresses (void)
{
  NS_LOG_FUNCTION (this);

  // Copy the map pairs into a vector
  std::vector<std::pair<Address, double> > pairs;
  for (auto it = m_gateways.begin (); it != m_gateways.end (); ++it)
    {
      pairs.push_back (*it);
    }

  // Sort the vector
  std::sort (pairs.begin (), pairs.end (),
             [] (const std::pair<Address, double>&p1,
                 const std::pair<Address, double>&p2)
             { return p1.second > p2.second; }
             );

  // Create a new array with only the addresses
  std::list<Address> addresses;
  for (auto it = pairs.begin (); it != pairs.end (); ++it)
    {
      addresses.push_back ((*it).first);
    }

  // Return the list
  return addresses;
}

bool
DeviceStatus::HasReply (void)
{
  NS_LOG_FUNCTION (this << m_reply.hasReply);

  return m_reply.hasReply;
}

void
DeviceStatus::SetReply (struct Reply reply)
{
  NS_LOG_FUNCTION (this);

  m_reply = reply;
}

Ptr<Packet>
DeviceStatus::GetReplyPacket (void)
{
  NS_LOG_FUNCTION (this);

  // Add headers to the packet
  Ptr<Packet> replyPacket = m_reply.packet->Copy ();
  replyPacket->AddHeader (m_reply.frameHeader);
  replyPacket->AddHeader (m_reply.macHeader);

  return replyPacket;
}

void
DeviceStatus::SetFirstReceiveWindowFrequency (double frequency)
{
  NS_LOG_FUNCTION (this << frequency);

  m_firstReceiveWindowFrequency = frequency;
}

double
DeviceStatus::GetFirstReceiveWindowFrequency (void)
{
  NS_LOG_FUNCTION (this);

  return m_firstReceiveWindowFrequency;
}

double
DeviceStatus::GetslotWindowFrequency (void)
{
  NS_LOG_FUNCTION (this);

  return m_mac->GetslotWindowFrequency ();
}

uint8_t
DeviceStatus::GetFirstReceiveWindowDataRate (void)
{
  NS_LOG_FUNCTION (this);

  return m_mac->GetFirstReceiveWindowDataRate ();
}

uint8_t
DeviceStatus::GetslotWindowDataRate (void)
{
  NS_LOG_FUNCTION (this);

  return m_mac->GetslotWindowDataRate ();
}

}
