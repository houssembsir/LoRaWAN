  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/network-server.h"
#include "ns3/simulator.h"
#include "ns3/end-device-lora-mac_B.h"
#include "ns3/lora-mac-header.h"
#include "ns3/lora-frame-header.h"
#include "ns3/end-device-lora-mac_B.h"
#include "ns3/lora-tag.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SimpleNetworkServer");

NS_OBJECT_ENSURE_REGISTERED (SimpleNetworkServer);

TypeId
SimpleNetworkServer::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SimpleNetworkServer")
    .SetParent<Application> ()
    .AddConstructor<SimpleNetworkServer> ()
    .SetGroupName ("lorawan");
  return tid;

}

SimpleNetworkServer::SimpleNetworkServer ()
{
  NS_LOG_FUNCTION_NOARGS ();


}

SimpleNetworkServer::~SimpleNetworkServer ()
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SimpleNetworkServer::StartApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}

void
SimpleNetworkServer::StopApplication (void)
{
  NS_LOG_FUNCTION_NOARGS ();
}
















void
SimpleNetworkServer::AddGateway (Ptr<Node> gateway, Ptr<NetDevice> netDevice)
{
  NS_LOG_FUNCTION (this << gateway);

  // Get the PointToPointNetDevice
  Ptr<PointToPointNetDevice> p2pNetDevice;
  for (uint32_t i = 0; i < gateway->GetNDevices (); i++)
    {
      p2pNetDevice = gateway->GetDevice (i)->GetObject<PointToPointNetDevice> ();
      if (p2pNetDevice != 0)
        {
          // We found a p2pNetDevice on the gateway
          break;
        }
    }

  // Get the gateway's LoRa MAC layer (assumes gateway's MAC is configured as first device)
  Ptr<GatewayLoraMac> gwMac = gateway->GetDevice (0)->GetObject<LoraNetDevice> ()->
    GetMac ()->GetObject<GatewayLoraMac> ();
  NS_ASSERT (gwMac != 0);

  // Get the Address
  Address gatewayAddress = p2pNetDevice->GetAddress ();

  // Check whether this device already exists
  if (m_gatewayStatuses.find (gatewayAddress) == m_gatewayStatuses.end ())
    {
      // The device doesn't exist

      // Create new gatewayStatus
      GatewayStatus gwStatus = GatewayStatus (gatewayAddress, netDevice, gwMac);
      // Add it to the map
      m_gatewayStatuses.insert (std::pair<Address, GatewayStatus>
                                  (gatewayAddress, gwStatus));
      NS_LOG_DEBUG ("Added a gateway to the list");
    }
}
bool init_beacon = true ;

int indexx ;
 LoraDeviceAddress addresstochek ;
 LoraDeviceAddress addressstorege [100];
int  SimpleNetworkServer::tessst( LoraDeviceAddress addr) {
  addresstochek = addr ;
//  NS_LOG_INFO(addresstochek) ; NS_LOG_INFO(addr) ;
  for (size_t j = 0; j < 100; j++) {
    /* code */


    /* code */
  if (addressstorege[j] !=addresstochek) {
//    NS_LOG_INFO("zidzid") ;

  }
  else {

indexx= j ;
//NS_LOG_INFO("index de terminal") ;
NS_LOG_INFO(indexx) ;
  }}
// int indexx ;
  // NS_LOG_INFO("t3adetttttttttt") ;
  indexx=indexx+1 ;
return indexx ;
 }



void
SimpleNetworkServer::AddNodes (NodeContainer nodes)
{
  NS_LOG_FUNCTION_NOARGS ();

  // For each node in the container, call the function to add that single node
  NodeContainer::Iterator it;
  for (it = nodes.Begin (); it != nodes.End (); it++)
    {
      AddNode (*it);
    }
}

int i=0 ;


void
SimpleNetworkServer::AddNode (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);

  // Get the LoraNetDevice
  Ptr<LoraNetDevice> loraNetDevice;
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      loraNetDevice = node->GetDevice (i)->GetObject<LoraNetDevice> ();
      if (loraNetDevice != 0)
        {
          // We found a LoraNetDevice on the node
          break;
        }
    }
  // Get the MAC
  Ptr<EndDeviceLoraMac> edLoraMac = loraNetDevice->GetMac ()->GetObject<EndDeviceLoraMac> ();


  LoraDeviceAddress deviceAddress = edLoraMac->GetDeviceAddress ();
  while(addressstorege[i] != 0 )
  {
  i=i+1 ;
    }
  addressstorege[i] = deviceAddress ;
  for (size_t j = 0; j < 20000; j++) {
    /* code */

    /* code */
//    LoraDeviceAddress showadress = addressstorege[j] ;
  //    NS_LOG_INFO("adress is ") ;  NS_LOG_INFO(showadress) ;


  // Get the Address
  LoraDeviceAddress deviceAddress = edLoraMac->GetDeviceAddress ();
  // Check whether this device already exists
  if (m_deviceStatuses.find (deviceAddress) == m_deviceStatuses.end ())
    {
      // The device doesn't exist

      // Create new DeviceStatus
      DeviceStatus devStatus = DeviceStatus (edLoraMac);
      // Add it to the map
      m_deviceStatuses.insert (std::pair<LoraDeviceAddress, DeviceStatus>
                                 (deviceAddress, devStatus));
      NS_LOG_DEBUG ("Added to the list a device with address " <<
                    deviceAddress.Print ());
    }
} }

bool
SimpleNetworkServer::Receive (Ptr<NetDevice> device, Ptr<const Packet> packet,
                              uint16_t protocol, const Address& address)
{
  NS_LOG_FUNCTION (this << packet << protocol << address);
// each reveive packet

prep_inf() ;


  // Create a copy of the packet
  Ptr<Packet> myPacket = packet->Copy ();

  // Extract the headers
  LoraMacHeader macHdr;
   NS_LOG_INFO ("mac header houssem  " <<  macHdr);
  myPacket->RemoveHeader (macHdr);
  LoraFrameHeader frameHdr;
  frameHdr.SetAsUplink ();
  myPacket->RemoveHeader (frameHdr);
  LoraTag tag;
  myPacket->RemovePacketTag (tag);

  // Register which gateway this packet came from
  double rcvPower = tag.GetReceivePower ();
  m_deviceStatuses.at (frameHdr.GetAddress ()).UpdateGatewayData (address,
                                                                  rcvPower);



      LoraDeviceAddress adresstemp = frameHdr.GetAddress () ;
           for (size_t j = 0; j < 20000; j++) {
             /* code */

             /* code */
           if (addressstorege[j] !=adresstemp) {
    //         NS_LOG_INFO("zidzid") ;

           }
           else {
    size_t indxx= j ;
  //  NS_LOG_INFO("mawjoud") ;
    //  NS_LOG_INFO(indxx) ;
           }}
  // Determine whether the packet requires a reply
  if (macHdr.GetMType () == LoraMacHeader::UNCONFIRMED_DATA_UP
      )
    {
      NS_LOG_DEBUG ("Scheduling a reply for this device");

      DeviceStatus::Reply reply;
      reply.hasReply = true;

      LoraMacHeader replyMacHdr = LoraMacHeader ();
      replyMacHdr.SetMajor (0);
      replyMacHdr.SetMType (LoraMacHeader::UNCONFIRMED_DATA_DOWN);
      reply.macHeader = replyMacHdr;

      LoraFrameHeader replyFrameHdr = LoraFrameHeader ();
      replyFrameHdr.SetAsDownlink ();
      replyFrameHdr.SetAddress (frameHdr.GetAddress ());
      replyFrameHdr.SetAck (true);
      reply.frameHeader = replyFrameHdr;

      // The downlink packet carries 0 bytes of payload
      Ptr<Packet> replyPacket = Create<Packet> (0);
      reply.packet = replyPacket;

      m_deviceStatuses.at (frameHdr.GetAddress ()).SetReply (reply);
      m_deviceStatuses.at (frameHdr.GetAddress ()).SetFirstReceiveWindowFrequency (tag.GetFrequency ());
LoraDeviceAddress address_to_send = frameHdr.GetAddress () ;
  /* code */

  LoraDeviceAddress adresstemp = frameHdr.GetAddress () ;
           for (size_t j = 0; j < 20000; j++) {
             /* code */

             /* code */
           if (addressstorege[j] !=adresstemp) {
        //     NS_LOG_INFO("zidzid") ;

           }
           else {
  size_t indxx= j ;
  //  NS_LOG_INFO("mawjoud") ;
    //  NS_LOG_INFO(indxx) ;
     }}

     int x= 128;
           if (init_beacon == true ) {
             for (size_t i = 0; i < 400; i++) { // 338 beacon ( 12h / 128s )
               /* code */

           Simulator::Schedule (Seconds (x), &SimpleNetworkServer::sendonpingslot, this,
                                address_to_send);
     x= x +128 ;
     NS_LOG_INFO("sending beacon time ") ; NS_LOG_INFO(x) ;
                              }
     init_beacon = false ;
     NS_LOG_INFO("it works") ;
     }
  Simulator::Schedule (Seconds (1), &SimpleNetworkServer::SendOnFirstWindow,
                       this, address_to_send);

      //Schedule a reply on the first receive window
  //     Simulator::Schedule (Seconds (1), &SimpleNetworkServer::SendOnFirstWindow,
    //                        this, address_to_send );
      // //                      // Simulator::Schedule (Seconds (10), &SimpleNetworkServer::SendOnFirstWindow,
                           //                      this, frameHdr.GetAddress ());




    }

  else if (macHdr.GetMType () == LoraMacHeader::CONFIRMED_DATA_UP
           && m_deviceStatuses.at (frameHdr.GetAddress ()).HasReply ())
    {
      NS_LOG_DEBUG ("There is already a reply for this device. Scheduling it and update frequency");

      m_deviceStatuses.at (frameHdr.GetAddress ()).SetFirstReceiveWindowFrequency (tag.GetFrequency ());
      LoraDeviceAddress adresstemp = frameHdr.GetAddress () ;
           for (size_t j = 0; j < 20000; j++) {
             /* code */

             /* code */
           if (addressstorege[j] !=adresstemp) {
  //    NS_LOG_INFO("zidzid") ;
           }
           else {
             size_t indxx= j ;
          //     NS_LOG_INFO("mawjoud") ;
        //         NS_LOG_INFO(indxx) ;
           }}
      // Schedule a reply on the first receive window
        /* code */

      Simulator::Schedule (Seconds (1), &SimpleNetworkServer::SendOnFirstWindow,
                           this, frameHdr.GetAddress ());


                           int x= 128;
                                 if (init_beacon == true ) {
                                   for (size_t i = 0; i < 400; i++) { // 338 beacon ( 12h / 128s )
                                     /* code */

                                 Simulator::Schedule (Seconds (x), &SimpleNetworkServer::sendonpingslot, this,
                                                    frameHdr.GetAddress ());
                           x= x +128 ;
                           NS_LOG_INFO("sending beacon time ") ; NS_LOG_INFO(x) ;
                                                    }
                           init_beacon = false ;
                           NS_LOG_INFO("it works") ;
                           }
        }

  return true;
}
int counter_down = 0 ;
void
SimpleNetworkServer::SendOnFirstWindow (LoraDeviceAddress address)
{
  NS_LOG_FUNCTION (this << address);

  // Decide on which gateway we'll transmit our reply
  double firstReceiveWindowFrequency = m_deviceStatuses.at
      (address).GetFirstReceiveWindowFrequency ();

  Address gatewayForReply = GetGatewayForReply (address,
                                                firstReceiveWindowFrequency);

  if (gatewayForReply != Address ())
    {
      NS_LOG_FUNCTION ("Found a suitable GW!");

      // Get the packet to use in the reply
      Ptr<Packet> replyPacket = m_deviceStatuses.at (address).GetReplyPacket ();
    //  NS_LOG_DEBUG ("Packet size: " << replyPacket->GetSize ());

      // Tag the packet so that the Gateway sends it according to the first
      // receive window parameters
      LoraTag replyPacketTag;
      uint8_t dataRate = m_deviceStatuses.at (address).GetFirstReceiveWindowDataRate ();
      double frequency = m_deviceStatuses.at (address).GetFirstReceiveWindowFrequency ();
      replyPacketTag.SetDataRate (dataRate);
      replyPacketTag.SetFrequency (frequency);

      replyPacket->AddPacketTag (replyPacketTag);

      NS_LOG_INFO ("Sending reply through the gateway with address " << gatewayForReply << " and initialize the reply.");

      InitializeReply (address, false);

      // Inform the gateway of the transmission
      m_gatewayStatuses.find (gatewayForReply)->second.GetNetDevice ()->
      Send (replyPacket, gatewayForReply, 0x0800);
      //


      // beacon
int x= 128;
      if (init_beacon == true ) {
        for (size_t i = 0; i < 400; i++) { // 338 beacon ( 12h / 128s )
          /* code */

      Simulator::Schedule (Seconds (x), &SimpleNetworkServer::sendonpingslot, this,
                           address);
x= x +128 ;
NS_LOG_INFO("sending beacon time ") ; NS_LOG_INFO(x) ;
                         }
init_beacon = false ;
NS_LOG_INFO("it works") ;
}







int random_to_send_down = rand()%100 ;
if (random_to_send_down >20) {
int ind_in_mat = tessst(address) ;
int first_index = EndDeviceLoraMac::get_random_slot(ind_in_mat);
NS_LOG_INFO("first time slot") ; NS_LOG_INFO(first_index) ;


Simulator::Schedule (Seconds (first_index*0.03), &SimpleNetworkServer::sendonpingslot, this,
                     address);
counter_down++ ;
NS_LOG_INFO(counter_down) ; NS_LOG_INFO("counter_downlink") ;
}










  //    Simulator::Schedule (Seconds (1), &SimpleNetworkServer::sendonpingslot, this,
      //                        address);

    }
  else
    {
      NS_LOG_FUNCTION ("No suitable GW found, scheduling second window reply");

  /* code */

      // Schedule a reply on the second receive window

      Simulator::Schedule (Seconds (2), &SimpleNetworkServer::sendonpingslot, this,
                           address);
}
}
int counter_droped_from_gateway=0 ;
void
SimpleNetworkServer::sendonpingslot (LoraDeviceAddress address)
{
  NS_LOG_FUNCTION (this << address);

  double slotWindowFrequency = m_deviceStatuses.at
      (address).GetslotWindowFrequency ();

  // Decide on which gateway we'll transmit our reply
  Address gatewayForReply = GetGatewayForReply (address, slotWindowFrequency);

  if (gatewayForReply != Address ())
    {
      // Get the packet to use in the reply


      // Tag the packet so that the Gateway sends it according to the second
      // receive window parameters
        Ptr<Packet> replyPacket = Create<Packet> (20);
      LoraTag replyPacketTag;
      uint8_t dataRate = m_deviceStatuses.at (address).GetslotWindowDataRate ();
      double frequency = m_deviceStatuses.at (address).GetslotWindowFrequency ();
      replyPacketTag.SetDataRate (dataRate);
      replyPacketTag.SetFrequency (frequency);

      replyPacket->AddPacketTag (replyPacketTag);

      NS_LOG_INFO ("Sending reply through the gateway with address " <<
                   gatewayForReply << " and initialize reply.");

      InitializeReply (address, false);

      // Inform the gateway of the transmission
      m_gatewayStatuses.find (gatewayForReply)->second.GetNetDevice ()->
      Send (replyPacket, gatewayForReply, 0x0800);
    }
  else
    {
      counter_droped_from_gateway++ ;
      // Schedule a reply on the second receive window
      NS_LOG_INFO ("Giving up on this reply, no GW available for RX window");
NS_LOG_INFO("counter_droped_from_gateway") ; NS_LOG_INFO(counter_droped_from_gateway) ;
    }
}

Address
SimpleNetworkServer::GetGatewayForReply (LoraDeviceAddress deviceAddress,
                                         double frequency)
{
  NS_LOG_FUNCTION (this);

  // Check which gateways can send this reply
  // Go in the order suggested by the DeviceStatus
  std::list<Address> addresses = m_deviceStatuses.at
      (deviceAddress).GetSortedGatewayAddresses ();

  for (auto it = addresses.begin (); it != addresses.end (); ++it)
    {
      if (m_gatewayStatuses.at (*it).IsAvailableForTransmission (frequency))
        {
          m_gatewayStatuses.at (*it).SetNextTransmissionTime (Simulator::Now ());
          return *it;
        }
    }

  return Address ();
}

void
SimpleNetworkServer::InitializeReply (LoraDeviceAddress addr, bool hasRep)
{
  DeviceStatus::Reply reply;
  reply.hasReply = hasRep;

  m_deviceStatuses.at (addr).SetReply (reply);
}



// void to prepare information
void
SimpleNetworkServer::prep_inf(){
int rand_end_device_index=rand()%100 ;
LoraDeviceAddress adress_to_send =addressstorege [rand_end_device_index];
int rand_time_to_send= rand()%10000; // 10000s = 10% * 1000s eache uplink



int first_index = EndDeviceLoraMac::get_random_slot(rand_end_device_index);
int cal_time = (first_index%4096)*128 + first_index *0.03 ;
Time time_to_send = Seconds(cal_time) ;

Simulator::Schedule (time_to_send, &SimpleNetworkServer::sch_send, this,
                     adress_to_send);



}


// void to send
void
SimpleNetworkServer::sch_send(LoraDeviceAddress address)
{


    double slotWindowFrequency = m_deviceStatuses.at
        (address).GetslotWindowFrequency ();

    // Decide on which gateway we'll transmit our reply
    Address gatewayForReply = GetGatewayForReply (address, slotWindowFrequency);

  Ptr<Packet> replyPacket = Create<Packet> (20);
LoraTag replyPacketTag;
uint8_t dataRate = m_deviceStatuses.at (address).GetslotWindowDataRate ();
double frequency = m_deviceStatuses.at (address).GetslotWindowFrequency ();
replyPacketTag.SetDataRate (dataRate);
replyPacketTag.SetFrequency (frequency);

replyPacket->AddPacketTag (replyPacketTag);

NS_LOG_INFO ("Sending reply through the gateway with address " <<
             gatewayForReply << " and initialize reply.");

InitializeReply (address, false);

// Inform the gateway of the transmission
 m_gatewayStatuses.find (gatewayForReply)->second.GetNetDevice ()->
 Send (replyPacket, gatewayForReply, 0x0800);

}








}
