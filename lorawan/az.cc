

 #include "ns3/point-to-point-module.h"

  #include "ns3/pointer.h"

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
 LoraDeviceAddress addressstorege [1000]; // 1000 devices
int  SimpleNetworkServer::tessst( LoraDeviceAddress addr) {
  addresstochek = addr ;
//  NS_LOG_INFO(addresstochek) ; NS_LOG_INFO(addr) ;
  for (size_t j = 0; j < 1000; j++) {
    /* code */


    /* code */
  if (addressstorege[j] !=addresstochek) {
//    NS_LOG_INFO("zidzid") ;

  }
  else {

indexx= j ;
NS_LOG_INFO("index de terminal") ;
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
  for (size_t j = 0; j < 2000; j++) {
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

int m_numberOfBeacons =0 ;

bool
SimpleNetworkServer::Receive (Ptr<NetDevice> device, Ptr<const Packet> packet,
                              uint16_t protocol, const Address& address)
{
  NS_LOG_FUNCTION (this << packet << protocol << address);

  // Create a copy of the packet
  Ptr<Packet> myPacket = packet->Copy ();

  // Extract the headers
  LoraMacHeader macHdr;
//   NS_LOG_INFO ("mac header houssem  " <<  macHdr);
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

/////////////////////
//////////// AES 128
// ////////////////////////////
//
//
//
//
  Time timestamp = Simulator::Now();
  Time nextBeacon = Seconds (128);

   m_numberOfBeacons++;

  uint64_t secs = timestamp.GetMilliSeconds() / 1000; //note: not using GetSeconds as it returns a double.
  uint64_t t = secs & 0x00000000FFFFFFFF; // t should be the 4 LSB bytes of secs TODO: there must be a nicer way of doing this.

  NS_LOG_DEBUG("Current time is " << timestamp);
  NS_LOG_DEBUG("Next beacon will be sent at " << nextBeacon + timestamp);
//
//   // build a beacon frame
//   // unlike for Class A, where the gateway is just a relay, in Class B the beacons must be modified in the gateway, as some parameters are gateway dependent
//
//   // build the majority of the beacon on the NS layer
  uint8_t beacon[17] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//
//   // the beacon payload is in this format (EU868 only):
//   // 2   4    2   7          2
//   // RFU Time CRC GwSpecific CRC
//
//   // RFU is 0,0
  beacon[0] = 0x00;
  beacon[1] = 0x00;

  // Time is seconds in GPS
  // place into beacon frame
  beacon[2] = (t >> 0)  & 0xFF;
  beacon[3] = (t >> 8)  & 0xFF;
  beacon[4] = (t >> 16) & 0xFF;
  beacon[5] = (t >> 24) & 0xFF;

  uint32_t time = (uint32_t)((beacon[5] << 24) | (beacon[4] << 16) | (beacon[3] << 8) | (beacon[2] << 0));
  NS_LOG_DEBUG("time put into beacon: " << time);

  // CRC is defined in IEEE 802.15.4-2003 section 7.2.1.8, and is calculated on the bytes in the order they are sent over the air
  // e.g. so if the GPS time was 3422683136, then the hex of that is CC020000
  // and so the two byte CRC would be calculated on [00 00 00 00 02 CC]
  // we're not actually implementing the CRC check, but if we were this is where it would be done. TODO: perform the calculation
  beacon[6] = 0x00;
  beacon[7] = 0x00;
//
  Ptr<Packet> p =  Create<Packet> (beacon, 17);

  //add the tags to the packet
  //note that there is no LoRa PHY or MAC headers in beacons

  for (int j=0; j<100 ; j++)
    {NS_LOG_INFO("addressstorege[j]") ; NS_LOG_INFO( addressstorege[j]) ;


    /*
    period  = (2^32)/slots
    R = AES_128_enc(16x 0x00, Time | DevAddr | pad16)
    O = (R[0] + R[1] * 256) % period

    then timings = {O + x*period | x < slots, x element of N}

    schedule the NS to call a sendDownlink function at each of those times, with the devAddr as parameter.
    then the DL works similarly to the current DL function here, but with a set DR and channel. Closest GW is chosen.
    Calculate all slots first, then schedule them - that way conflicting slots (same time and gw) can be handled.
    */
//
//
//
//
      uint64_t period = std::pow(2.0, 12) / 6;
      uint8_t key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // crypto not implemented across layers yet, assuming key of all 0s.
      uint8_t buf[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

      double secs = timestamp.GetSeconds(); // convert to a 4 byte buffer
      uint32_t secsTruncated = (uint32_t) secs;

      uint8_t *sp = (uint8_t *)&secsTruncated;
      buf[0] = sp[0];
      buf[1] = sp[1];
      buf[2] = sp[2];
      buf[3] = sp[3];

      LoraDeviceAddress devAddr = addressstorege[j] ;

      uint8_t addr[4];
      devAddr.Serialize(addr);
      buf[4] = addr[0];
      buf[5] = addr[1];
      buf[6] = addr[2];
      buf[7] = addr[3];

      //pad16 is to ensure the buffer is 16 bytes long
      //the rest of the buffer (the other 8 bytes) is just 0s.
      for(uint i=8; i<16;i++){
        buf[i] = 0;
      }

      AES aes;
      aes.SetKey(key, 16);
      aes.Encrypt(buf, 16);
// NS_LOG_DEBUG("fffffffffffffffffffffffffffffffffff") ;
//
      uint64_t O = (buf[0] + buf[1]*256) % period;
     uint64_t xx = (buf[0] + buf[1]*256) % period;
////
///// c out ping slots
     // std::cout << xx ;
     // std::cout << "xx" << "\t" << std::endl ;
     //
          LoraDeviceAddress deviceAddr = addressstorege[j] ;
      uint32_t beacon_reserved = 2120; // ms
      uint32_t slotLength = 30; // ms
      uint32_t dAddr = (uint32_t) deviceAddr.Get ();


 int m_ClassBPingPeriodicity = 6 ;
 int m_ClassBPingSlots = std::pow(2.0, 7 - m_ClassBPingPeriodicity);

      //calculate and schedule ping slots for this device
      NS_LOG_DEBUG("Scheduling ping slots for device:  " << deviceAddr.Get ());
  //    NS_LOG_DEBUG("Scheduling ping slots for device" << deviceAddr.Get ());

for(uint i=0;i<100; i++){
  uint64_t pingTime = beacon_reserved + (O + period*i) * slotLength; // Ping slot time is beacon_reserved + (pingOffset + N*pingPeriod) * slotLength

  Time ping = MilliSeconds(pingTime);
  Time time_ping = Simulator::Now() + ping ;
 NS_LOG_DEBUG("gw : ping slot for device ") ;

    NS_LOG_DEBUG( dAddr) ;

 NS_LOG_DEBUG("at time :  ") ;

       NS_LOG_DEBUG(pingTime ) ;

  Simulator::Schedule ( time_ping , &SimpleNetworkServer::sendonpingslot, this,
                       deviceAddr);

//  Simulator::Schedule (ping, &LoRaWANNetworkServer::ClassBPingSlot, this, dAddr, O + period*i);


}

//
}









////////////////////////////////
/////////////////////////////////
/////////////////////////////////

      LoraDeviceAddress adresstemp = frameHdr.GetAddress () ;
           for (size_t j = 0; j < 100; j++) {
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
/////////////////////////////
///////////////////////////
////////// beacons
//////////////////////////
//////////////////////////////
//////////////////////////////
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


// prep_inf() ;

    }

  else if (macHdr.GetMType () == LoraMacHeader::CONFIRMED_DATA_UP
           && m_deviceStatuses.at (frameHdr.GetAddress ()).HasReply ())
    {
      NS_LOG_DEBUG ("There is already a reply for this device. Scheduling it and update frequency");

      m_deviceStatuses.at (frameHdr.GetAddress ()).SetFirstReceiveWindowFrequency (tag.GetFrequency ());
      LoraDeviceAddress adresstemp = frameHdr.GetAddress () ;

      Simulator::Schedule (Seconds (1), &SimpleNetworkServer::SendOnFirstWindow,
                           this, frameHdr.GetAddress ());
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
counter_down =counter_down +1  ;
 NS_LOG_INFO(counter_down) ;
NS_LOG_INFO("counter_downlink") ;
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
 size_t counter_droped_from_gateway;
int count_total_pck_from_NS =0 ;
void
SimpleNetworkServer::sendonpingslot (LoraDeviceAddress address)
{
  NS_LOG_FUNCTION (this << address);

  double slotWindowFrequency = m_deviceStatuses.at
      (address).GetslotWindowFrequency ();

  // Decide on which gateway we'll transmit our reply
  Address gatewayForReply = GetGatewayForReply (address, slotWindowFrequency);
count_total_pck_from_NS++ ;
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
      counter_droped_from_gateway= counter_droped_from_gateway +1 ;
      // Schedule a reply on the second receive window
      NS_LOG_INFO ("Giving up on this reply, no GW available for RX window");
NS_LOG_INFO("counter_droped_from_gateway") ; NS_LOG_INFO(counter_droped_from_gateway) ;
    }
}


int SimpleNetworkServer::return_count_total_pck_from_NS (){
 return count_total_pck_from_NS ;
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
static int matrice_of_slot [1000][4096] ; // 1000 devices
///////////
///////
////////////
void  exist() {
  bool exiist ;
  for (int i = 0; i < 1000; i++) {
    /* code */
    int y =matrice_of_slot[i][0] ;
if (y!=0) {
//  NS_LOG_INFO("EEEEEEEEEEEEEEEEEEE") ;
  exiist= false ;
}else
{//NS_LOG_INFO("AAAAAAAAAAAAAAAAAAA") ;
  exiist = true ;

}

}
//

if(exiist!=true){
int z=0 ;
NS_LOG_INFO("dont exist");
  int y =matrice_of_slot[z][0] ;
  while (y!=0){
      z++ ;
y =matrice_of_slot[z][0] ;

//    NS_LOG_INFO("Z is") ; NS_LOG_INFO(z) ;
  }
  matrice_of_slot[z][0] = 0 ;
}}
///////////
/////////
///////////
///////////


int get_random_slot_B (int a) {
int i =2;

  while (matrice_of_slot[a][i]==0) {
    i++ ;
  }
  return i ;
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
