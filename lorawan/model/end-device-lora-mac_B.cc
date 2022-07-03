
  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
 #include "ns3/end-device-lora-mac_B.h"
 #include "ns3/end-device-lora-phy.h"
 #include "ns3/simulator.h"
 #include "ns3/log.h"
 #include <algorithm>
//////////////////
#include "ns3/gateway-status.h"

#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/boolean.h"
#include "ns3/string.h"
////////////////////////////////////
 #include "ns3/end-device-lora-mac_B.h"
 #include "ns3/end-device-lora-phy.h"
 #include "ns3/simulator.h"
 #include "ns3/log.h"
 #include <algorithm>
 #include "network-server.h"
 #include <fstream>
#include "ns3/aes.h"
 #include <math.h>

 #include "ns3/end-device-lora-phy.h"
 #include "ns3/gateway-lora-phy.h"
 #include "ns3/end-device-lora-mac_B.h"
 #include "ns3/gateway-lora-mac_B.h"
 #include "ns3/simulator.h"
 #include "ns3/log.h"
 #include "ns3/constant-position-mobility-model.h"
 #include "ns3/lora-helper.h"
 #include "ns3/mobility-helper.h"
 #include "ns3/node-container.h"
 #include "ns3/position-allocator.h"
 #include "ns3/sender-helper.h"
 #include "ns3/command-line.h"
 #include <algorithm>
 #include <ctime>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("EndDeviceLoraMac");

NS_OBJECT_ENSURE_REGISTERED (EndDeviceLoraMac);

TypeId
EndDeviceLoraMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::EndDeviceLoraMac")
    .SetParent<LoraMac> ()
    .SetGroupName ("lorawan")
    .AddTraceSource ("RequiredTransmissions",
                     "Total number of transmissions required to deliver this packet",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_requiredTxCallback),
                     "ns3::TracedValueCallback::uint8_t")
    .AddTraceSource ("DataRate",
                     "Data Rate currently employed by this end device",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_dataRate),
                     "ns3::TracedValueCallback::uint8_t")
    .AddTraceSource ("TxPower",
                     "Transmission power currently employed by this end device",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_txPower),
                     "ns3::TracedValueCallback::Double")
    .AddTraceSource ("LastKnownLinkMargin",
                     "Last known demodulation margin in "
                     "communications between this end device "
                     "and a gateway",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_lastKnownLinkMargin),
                     "ns3::TracedValueCallback::Double")
    .AddTraceSource ("LastKnownGatewayCount",
                     "Last known number of gateways able to "
                     "listen to this end device",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_lastKnownGatewayCount),
                     "ns3::TracedValueCallback::Int")
    .AddTraceSource ("AggregatedDutyCycle",
                     "Aggregate duty cycle, in fraction form, "
                     "this end device must respect",
                     MakeTraceSourceAccessor
                       (&EndDeviceLoraMac::m_aggregatedDutyCycle),
                     "ns3::TracedValueCallback::Double")
    .AddConstructor<EndDeviceLoraMac> ();
  return tid;
}

EndDeviceLoraMac::EndDeviceLoraMac ()
  : m_enableDRAdapt (false),
    m_maxNumbTx (8),
    m_dataRate (0),
    m_txPower (14),
    m_codingRate (1),
    // LoraWAN default
    m_headerDisabled (0),
    // LoraWAN default
    m_receiveDelay1 (Seconds (1)),
    // LoraWAN default
    m_receiveDelay2 (Seconds (2)),
    // LoraWAN default
    m_receiveWindowDuration (Seconds (0.03)),
    m_address (LoraDeviceAddress (0)),
    m_rx1DrOffset (0),
    // LoraWAN default
    m_lastKnownLinkMargin (0),
    m_lastKnownGatewayCount (0),
    m_aggregatedDutyCycle (10),
    m_mType (LoraMacHeader::CONFIRMED_DATA_UP)

{
  NS_LOG_FUNCTION (this);

  // Initialize the random variable we'll use to decide which channel to
  // transmit on.
  m_uniformRV = CreateObject<UniformRandomVariable> ();

  // Void the two receiveWindow events
  m_closeFirstWindow = EventId ();
  m_closeFirstWindow.Cancel ();
  m_closepingslot = EventId ();
  m_closepingslot.Cancel ();
  m_pingslotwindow = EventId ();
  m_pingslotwindow.Cancel ();

  // Void the transmission event
  m_nextTx = EventId ();
  m_nextTx.Cancel ();

  // Initialize structure for retransmission parameters
  m_retxParams = EndDeviceLoraMac::LoraRetxParameters ();
  m_retxParams.retxLeft = m_maxNumbTx;
}
EndDeviceLoraMac::~EndDeviceLoraMac ()
{
  NS_LOG_FUNCTION_NOARGS ();
}
uint8_t     m_ClassBPingSlots=4069;
  uint8_t     m_missedBeaconsCounter;
  bool m_isClassB;
Time   m_timestamp = Seconds(0) ;
Time simulationTime = Seconds (0) ;
static int matrice_of_slot [100000][4096] ;
int pingcal=0 ;
////////////////////////
//  Sending methods   //
////////////////////////

//

















//
void
EndDeviceLoraMac::Send (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this << packet);

  // Check that payload length is below the allowed maximum
  if (packet->GetSize () > m_maxAppPayloadForDataRate.at (m_dataRate))
    {
      NS_LOG_WARN ("Attempting to send a packet larger than the maximum allowed"
                   << " size at this DataRate (DR" << unsigned(m_dataRate) <<
                   "). Transmission canceled.");
      return;
    }

  // If it is not possible to transmit now because of the duty cycle,
  // or because we are receiving, schedule a tx/retx later

  Time netxTxDelay = GetNextTransmissionDelay ();
  if (netxTxDelay != Seconds (0))
    {
      // Add the ACK_TIMEOUT random delay if it is a retransmission.
      if (m_retxParams.waitingAck)
        {
          double ack_timeout = m_uniformRV->GetValue (1,3);
          netxTxDelay = netxTxDelay + Seconds (ack_timeout);
        }
      postponeTransmission (netxTxDelay, packet);
    }

  // Pick a channel on which to transmit the packet
  Ptr<LogicalLoraChannel> txChannel = GetChannelForTx ();

  if (!(txChannel && m_retxParams.retxLeft > 0))
    {
      if (!txChannel)
        {
          m_cannotSendBecauseDutyCycle (packet);
        }
      else
        {
          NS_LOG_INFO ("Max number of transmission achieved: packet not transmitted.");
        }
    }
  else
  // the transmitting channel is available and we have not run out the maximum number of retransmissions
    {
      // Make sure we can transmit at the current power on this channel
      NS_ASSERT_MSG (m_txPower <= m_channelHelper.GetTxPowerForChannel (txChannel),
                     " The selected power is too hight to be supported by this channel.");
      DoSend (packet);
    }
}

void
EndDeviceLoraMac::postponeTransmission (Time netxTxDelay, Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  // Delete previously scheduled transmissions if any.
  Simulator::Cancel (m_nextTx);
  m_nextTx = Simulator::Schedule (netxTxDelay, &EndDeviceLoraMac::DoSend, this, packet);
  NS_LOG_WARN ("Attempting to send, but the aggregate duty cycle won't allow it. Scheduling a tx at a delay "
               << netxTxDelay.GetSeconds () << ".");
}


void
EndDeviceLoraMac::DoSend (Ptr<Packet> packet)
{
  NS_LOG_FUNCTION (this);
  // Checking if this is the transmission of a new packet
  if (packet != m_retxParams.packet)
    {
      NS_LOG_DEBUG ("Received a new packet from application. Resetting retransmission parameters.");
      NS_LOG_DEBUG ("APP packet: " << packet << ".");

      // Add the Lora Frame Header to the packet
      LoraFrameHeader frameHdr;
      ApplyNecessaryOptions (frameHdr);
      packet->AddHeader (frameHdr);

      NS_LOG_INFO ("Added frame header of size " << frameHdr.GetSerializedSize () <<
                   " bytes.");

      // Add the Lora Mac header to the packet
      LoraMacHeader macHdr;
      ApplyNecessaryOptions (macHdr);
      packet->AddHeader (macHdr);


      if (m_retxParams.waitingAck)
        {
          // Call the callback to notify about the failure
          uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
          m_requiredTxCallback (txs, false, m_retxParams.firstAttempt, m_retxParams.packet);
          NS_LOG_DEBUG (" Received new packet from the application layer: stopping retransmission procedure. Used " <<
                        unsigned(txs) << " transmissions out of a maximum of " << unsigned(m_maxNumbTx) << ".");
        }

      // Reset retransmission parameters
      resetRetransmissionParameters ();

      // If this is the first transmission of a confirmed packet, save parameters for the (possible) next retransmissions.
      if (m_mType == LoraMacHeader::CONFIRMED_DATA_UP)
        {
          m_retxParams.packet = packet->Copy ();
          m_retxParams.retxLeft = m_maxNumbTx;
          m_retxParams.waitingAck = true;
          m_retxParams.firstAttempt = Simulator::Now ();
          m_retxParams.retxLeft = m_retxParams.retxLeft - 1; // decreasing the number of retransmissions

          NS_LOG_DEBUG ("Message type is " << m_mType);
          NS_LOG_DEBUG ("It is a confirmed packet. Setting retransmission parameters and decreasing the number of transmissions left.");

          NS_LOG_INFO ("Added MAC header of size " << macHdr.GetSerializedSize () <<
                       " bytes.");

          // Sent a new packet
          NS_LOG_DEBUG ("Copied packet: " << m_retxParams.packet);
          m_sentNewPacket (m_retxParams.packet);

          SendToPhy (m_retxParams.packet);
        }
      else
        {
          SendToPhy (packet);
        }

    }
  // this is a retransmission
  else
    {
      if (m_retxParams.waitingAck)
        {
          m_retxParams.retxLeft = m_retxParams.retxLeft - 1; // decreasing the number of retransmissions
          NS_LOG_DEBUG ("Retransmitting an old packet.");

          SendToPhy (m_retxParams.packet);
        }
    }

}

void
EndDeviceLoraMac::SendToPhy (Ptr<Packet> packetToSend)
{
  /////////////////////////////////////////////////////////
  // Add headers, prepare TX parameters and send the packet
  /////////////////////////////////////////////////////////

  NS_LOG_DEBUG ("PacketToSend: " << packetToSend);

  // Data Rate Adaptation as in LoRaWAN specification, V1.0.2 (2016)
  if (m_enableDRAdapt && (m_dataRate > 0) && (m_retxParams.retxLeft < m_maxNumbTx) && (m_retxParams.retxLeft % 2 == 0) )
    {
      m_dataRate = m_dataRate - 1;
    }

  // Craft LoraTxParameters object
  LoraTxParameters params;
  params.sf = GetSfFromDataRate (m_dataRate);
  params.headerDisabled = m_headerDisabled;
  params.codingRate = m_codingRate;
  params.bandwidthHz = GetBandwidthFromDataRate (m_dataRate);
  params.nPreamble = m_nPreambleSymbols;
  params.crcEnabled = 1;
  params.lowDataRateOptimizationEnabled = 0;

  // Wake up PHY layer and directly send the packet

  Ptr<LogicalLoraChannel> txChannel = GetChannelForTx ();

  NS_LOG_DEBUG ("PacketToSend: " << packetToSend);
  m_phy->Send (packetToSend, params, txChannel->GetFrequency (), m_txPower);

  //////////////////////////////////////////////
  // Register packet transmission for duty cycle
  //////////////////////////////////////////////

  // Compute packet duration
  Time duration = m_phy->GetOnAirTime (packetToSend, params);

  // Register the sent packet into the DutyCycleHelper
  m_channelHelper.AddEvent (duration, txChannel);

  //////////////////////////////
  // Prepare for the downlink //
  //////////////////////////////

  // Switch the PHY to the channel so that it will listen here for downlink
  m_phy->GetObject<EndDeviceLoraPhy> ()->SetFrequency (txChannel->GetFrequency ());

  // Instruct the PHY on the right Spreading Factor to listen for during the window
  uint8_t replyDataRate = GetFirstReceiveWindowDataRate ();
  NS_LOG_DEBUG ("m_dataRate: " << unsigned (m_dataRate) <<
                ", m_rx1DrOffset: " << unsigned (m_rx1DrOffset) <<
                ", replyDataRate: " << unsigned (replyDataRate) << ".");

  m_phy->GetObject<EndDeviceLoraPhy> ()->SetSpreadingFactor
    (GetSfFromDataRate (replyDataRate));
}

//////////////////////////
//  Receiving methods   //
//////////////////////////

void
EndDeviceLoraMac::Receive (Ptr<Packet const> packet)
{
  NS_LOG_FUNCTION (this << packet);

  // Work on a copy of the packet
  Ptr<Packet> packetCopy = packet->Copy ();

  // Remove the Mac Header to get some information
  LoraMacHeader mHdr;
  packetCopy->RemoveHeader (mHdr);

  // Only keep analyzing the packet if it's downlink
  if (!mHdr.IsUplink ())
    {
      NS_LOG_INFO ("Found a downlink packet.");

      // Remove the Frame Header
      LoraFrameHeader fHdr;
      fHdr.SetAsDownlink ();
      packetCopy->RemoveHeader (fHdr);

      // Determine whether this packet is for us
      bool messageForUs = (m_address == fHdr.GetAddress ());

      if (messageForUs)
        {
          NS_LOG_INFO ("The message is for us!");

          // If it exists, cancel the second receive window event
      //    Simulator::Cancel (m_pingslotwindow);

          // Parse the MAC commands
          ParseCommands (fHdr);

          // TODO Pass the packet up to the NetDevice


          // Call the trace source
          m_receivedPacket (packet);
        }
      else
        {
          NS_LOG_DEBUG ("The message is intended for another recipient.");

          // In this case, we are either receiving in the first receive window
          // and finishing reception inside the second one, or receiving a
          // packet in the second receive window and finding out, after the
          // fact, that the packet is not for us. In either case, if we no
          // longer have any retransmissions left, we declare failure.
          if (m_retxParams.waitingAck && m_pingslotwindow.IsExpired ())
            {
              if (m_retxParams.retxLeft == 0)
                {
                  uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
                  m_requiredTxCallback (txs, false, m_retxParams.firstAttempt, m_retxParams.packet);
                  NS_LOG_DEBUG ("Failure: no more retransmissions left. Used " << unsigned(txs) << " transmissions.");

                  // Reset retransmission parameters
                  resetRetransmissionParameters ();
                }
              else // Reschedule
                {
                  this->Send (m_retxParams.packet);
                  NS_LOG_INFO ("We have " << unsigned(m_retxParams.retxLeft) << " retransmissions left: rescheduling transmission.");
                }
            }
        }
    }
  else if (m_retxParams.waitingAck && m_pingslotwindow.IsExpired ())
    {
      NS_LOG_INFO ("The packet we are receiving is in uplink.");
      if (m_retxParams.retxLeft > 0)
        {
          this->Send (m_retxParams.packet);
          NS_LOG_INFO ("We have " << unsigned(m_retxParams.retxLeft) << " retransmissions left: rescheduling transmission.");
        }
      else
        {
          uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
          m_requiredTxCallback (txs, false, m_retxParams.firstAttempt, m_retxParams.packet);
          NS_LOG_DEBUG ("Failure: no more retransmissions left. Used " << unsigned(txs) << " transmissions.");

          // Reset retransmission parameters
          resetRetransmissionParameters ();
        }
    }

  m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToSleep ();
}

void
EndDeviceLoraMac::FailedReception (Ptr<Packet const> packet)
{
  NS_LOG_FUNCTION (this << packet);

  // Switch to sleep after a failed reception
  m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToSleep ();

  if (m_pingslotwindow.IsExpired () && m_retxParams.waitingAck)
    {
      if (m_retxParams.retxLeft > 0)
        {
          this->Send (m_retxParams.packet);
          NS_LOG_INFO ("We have " << unsigned(m_retxParams.retxLeft) << " retransmissions left: rescheduling transmission.");
        }
      else
        {
          uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
          m_requiredTxCallback (txs, false, m_retxParams.firstAttempt, m_retxParams.packet);
          NS_LOG_DEBUG ("Failure: no more retransmissions left. Used " << unsigned(txs) << " transmissions.");

          // Reset retransmission parameters
          resetRetransmissionParameters ();
        }
    }
}

void
EndDeviceLoraMac::ParseCommands (LoraFrameHeader frameHeader)
{
  NS_LOG_FUNCTION (this << frameHeader);

  if (m_retxParams.waitingAck)
    {
      if (frameHeader.GetAck ())
        {
          NS_LOG_INFO ("The message is an ACK, not waiting for it anymore.");

          NS_LOG_DEBUG ("Reset retransmission variables to default values and cancel retransmission if already scheduled.");

          uint8_t txs = m_maxNumbTx - (m_retxParams.retxLeft);
          m_requiredTxCallback (txs, true, m_retxParams.firstAttempt, m_retxParams.packet);
          NS_LOG_DEBUG ("Received ACK packet after " << unsigned(txs) << " transmissions: stopping retransmission procedure. ");

          // Reset retransmission parameters
          resetRetransmissionParameters ();

        }
      else
        {
          NS_LOG_ERROR ("Received downlink message not containing an ACK while we were waiting for it!");
        }
    }

  std::list<Ptr<MacCommand> > commands = frameHeader.GetCommands ();
  std::list<Ptr<MacCommand> >::iterator it;
  for (it = commands.begin (); it != commands.end (); it++)
    {
      NS_LOG_DEBUG ("Iterating over the MAC commands...");
      enum MacCommandType type = (*it)->GetCommandType ();
      switch (type)
        {
        case (LINK_CHECK_ANS):
          {
            NS_LOG_DEBUG ("Detected a LinkCheckAns command.");

            // Cast the command
            Ptr<LinkCheckAns> linkCheckAns = (*it)->GetObject<LinkCheckAns> ();

            // Call the appropriate function to take action
            OnLinkCheckAns (linkCheckAns->GetMargin (), linkCheckAns->GetGwCnt ());

            break;
          }
        case (LINK_ADR_REQ):
          {
            NS_LOG_DEBUG ("Detected a LinkAdrReq command.");

            // Cast the command
            Ptr<LinkAdrReq> linkAdrReq = (*it)->GetObject<LinkAdrReq> ();

            // Call the appropriate function to take action
            OnLinkAdrReq (linkAdrReq->GetDataRate (), linkAdrReq->GetTxPower (),
                          linkAdrReq->GetEnabledChannelsList (),
                          linkAdrReq->GetRepetitions ());

            break;
          }
        case (DUTY_CYCLE_REQ):
          {
            NS_LOG_DEBUG ("Detected a DutyCycleReq command.");

            // Cast the command
            Ptr<DutyCycleReq> dutyCycleReq = (*it)->GetObject<DutyCycleReq> ();

            // Call the appropriate function to take action
            OnDutyCycleReq (dutyCycleReq->GetMaximumAllowedDutyCycle ());

            break;
          }
        case (RX_PARAM_SETUP_REQ):
          {
            NS_LOG_DEBUG ("Detected a RxParamSetupReq command.");

            // Cast the command
            Ptr<RxParamSetupReq> rxParamSetupReq = (*it)->GetObject<RxParamSetupReq> ();

            // Call the appropriate function to take action
            OnRxParamSetupReq (rxParamSetupReq->GetRx1DrOffset (),
                               rxParamSetupReq->GetRx2DataRate (),
                               rxParamSetupReq->GetFrequency ());

            break;
          }
        case (DEV_STATUS_REQ):
          {
            NS_LOG_DEBUG ("Detected a DevStatusReq command.");

            // Cast the command
            Ptr<DevStatusReq> devStatusReq = (*it)->GetObject<DevStatusReq> ();

            // Call the appropriate function to take action
            OnDevStatusReq ();

            break;
          }
        case (NEW_CHANNEL_REQ):
          {
            NS_LOG_DEBUG ("Detected a NewChannelReq command.");

            // Cast the command
            Ptr<NewChannelReq> newChannelReq = (*it)->GetObject<NewChannelReq> ();

            // Call the appropriate function to take action
            OnNewChannelReq (newChannelReq->GetChannelIndex (), newChannelReq->GetFrequency (), newChannelReq->GetMinDataRate (), newChannelReq->GetMaxDataRate ());

            break;
          }
        case (RX_TIMING_SETUP_REQ):
          {
            break;
          }
        case (TX_PARAM_SETUP_REQ):
          {
            break;
          }
        case (DL_CHANNEL_REQ):
          {
            break;
          }
        default:
          {
            NS_LOG_ERROR ("CID not recognized");
            break;
          }
        }
    }

}

void
EndDeviceLoraMac::ApplyNecessaryOptions (LoraFrameHeader& frameHeader)
{
  NS_LOG_FUNCTION_NOARGS ();

  frameHeader.SetAsUplink ();
  frameHeader.SetFPort (1);           // TODO Use an appropriate frame port based on the application
  frameHeader.SetAddress (m_address);
  frameHeader.SetAdr (0);           // TODO Set ADR if a member variable is true
  frameHeader.SetAdrAckReq (0);           // TODO Set ADRACKREQ if a member variable is true
  if (m_mType == LoraMacHeader::CONFIRMED_DATA_UP)
    {
      frameHeader.SetAck (1);
    }
  else
    {
      frameHeader.SetAck (0);
    }
  // FPending does not exist in uplink messages
  frameHeader.SetFCnt (0);           // TODO Set this based on the counters

  // Add listed MAC commands
  for (const auto &command : m_macCommandList)
    {
      NS_LOG_INFO ("Applying a MAC Command of CID " <<
                   unsigned(MacCommand::GetCIDFromMacCommand
                              (command->GetCommandType ())));

      frameHeader.AddCommand (command);
    }

}

void
EndDeviceLoraMac::ApplyNecessaryOptions (LoraMacHeader& macHeader)
{
  NS_LOG_FUNCTION_NOARGS ();

  macHeader.SetMType (m_mType);
  macHeader.SetMajor (1);
}

void
EndDeviceLoraMac::SetMType (LoraMacHeader::MType mType)
{
  m_mType = mType;
  NS_LOG_DEBUG ("Message type is set to " << mType);
}

LoraMacHeader::MType
EndDeviceLoraMac::GetMType (void)
{
  return m_mType;
}

bool initialisation = false ;


//
// The time carried in the field BCNPayload.Time of the immediately
// preceding beacon frame

size_t beaconTime ;

// Length of a unit ping slot = 30 ms

size_t slotLen = 30 ;

int EndDeviceLoraMac::get_random_slot (int a) {
int i =2 ;
  while (matrice_of_slot[a][i]==0) {
    i++ ;
  }
  return i ;
}

void

EndDeviceLoraMac::schedule_RX (Ptr<const Packet> packet )
{


  Simulator::Schedule (m_receiveDelay1,
                        &EndDeviceLoraMac::OpenFirstReceiveWindow, this);
    //                   m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToSleep ();


}
// 10 num end devices
//4096 num slots
int ligne = 0 ; int colone = 1 ;
int c =0 ; // counter

int x = 0 ;



void  EndDeviceLoraMac::exist() {
  bool exiist ;
  for (int i = 0; i < 200; i++) {
    /* code */
    int y =matrice_of_slot[i][0] ;
if (y!=x) {
//  NS_LOG_INFO("EEEEEEEEEEEEEEEEEEE") ;
  exiist= false ;
}else
{//NS_LOG_INFO("AAAAAAAAAAAAAAAAAAA") ;
  exiist = true ;

}

  }
if(exiist!=true){
int z=0 ;
NS_LOG_INFO("dont exist");
  int y =matrice_of_slot[z][0] ;
  while (y!=0){
      z++ ;
y =matrice_of_slot[z][0] ;

//    NS_LOG_INFO("Z is") ; NS_LOG_INFO(z) ;
  }
  matrice_of_slot[z][0] = x ;
}


// for (int i = 0; i < 2000; i++) {
//   /* code */
//   int y =matrice_of_slot[i][0] ;
//   NS_LOG_INFO("matris is "); NS_LOG_INFO(y) ;
// }
}


bool init_beacon_reception = true ;

void
EndDeviceLoraMac::mac_config (Ptr<const Packet> packet)
{

    NS_LOG_FUNCTION_NOARGS ();

  EndDeviceLoraMac::schedule_RX (packet) ;

   // LoraDeviceAddress deviceAddresssa =SimpleNetworkServer::AddNode.deviceAddress;
   // NS_LOG_INFO(deviceAddresssa) ;



//.----------------------------------------
////////////////////////////////////////////////////////////////////////////////////add aes encryption
//..........................................
//
//
// // void
// // LoRaWANEndDeviceApplication::ClassBSchedulePingSlots ()
// // {
    NS_LOG_DEBUG("timestamp is " << m_timestamp);
    if (m_timestamp.IsZero()) {
      //no beacon was received this time
      m_missedBeaconsCounter++;
      //TODO: increase reception window of all rx slots progressively to account for clock drift without beacon frames

      // "A device shall be capable of maintaining Class B operation for 2 hours (120 minutes) after it received the last beacon" = 7200seconds = 56.25 beacon periods
      if(m_missedBeaconsCounter < 57) {
        //if we haven't missed so many beacons (so the internal clock is still reliable), generate the expected offset (should be calculatable if time sync isn't too far off) and generate the ping slots anyway
        m_timestamp = Simulator::Now() - MilliSeconds(173.056);  // Time to receive a LoRaWAN beacon frame (17 bytes, SF9, no LoRaWAN header - calculated via LoRaWAN Calculator)

        NS_LOG_DEBUG("end device missed beacon frame, generated timestamp is: " << m_timestamp.GetSeconds() );

      } else {
        NS_LOG_DEBUG("missed too many beacons, device turning to Class A mode");
        // If no beacon has been received for a given period (2 hours), synchronisation with the network is lost - the device has to change back to Class A
        // i.e. stop setting the Class B bit in uplink frames.
        // the end device can try to swap back to Class B periodically (i.e. via a beacon search - not currently implemented)
        m_isClassB = false; // this setting gets passed as a bit in the next uplink frame, telling the NS to stop scheduling pings for this device.

        //and don't attempt to calculate ping slots
        return;
      }
    }
else {
      //beacon was successfully received, use the timestamp contained within
      m_missedBeaconsCounter = 0;
      NS_LOG_DEBUG("beacon was received successfully");

      //TODO: and reset the reception window length (as the timestamp can be used to reset the clock)
      //issue with this though: the timestamp in the packet isn't THAT precise (it's in seconds).
    }
//
    NS_LOG_DEBUG("ed: timestamp is: " << m_timestamp.GetSeconds() );

    uint64_t period = std::pow(2.0, 12) / m_ClassBPingSlots;;
    uint8_t key[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // the key. Encryption of packets isn't fully supported right now so assuming a key of all 0s.
    uint8_t buf[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

    // add time from beacon frame to buffer
    double secs = m_timestamp.GetSeconds(); // convert to a 4 byte buffer
    uint32_t secsTruncated = (uint32_t) secs;

    uint8_t *sp = (uint8_t *)&secsTruncated;
    buf[0] = sp[0];
    buf[1] = sp[1];
    buf[2] = sp[2];
    buf[3] = sp[3];

    // add own node address to buffer
//
 LoraDeviceAddress addresstemp =GetDeviceAddress () ;
    LoraDeviceAddress devAddr = addresstemp ;
  //  NS_LOG_DEBUG("ed: device address is: " << devAddr );
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
 uint64_t zzero ;
     zzero = (buf[0] + buf[1]*256);
     zzero = zzero % 6 ;

    uint32_t beacon_reserved = 2120; // ms
    uint32_t slotLength = 30; // ms
    uint32_t dAddr = (uint32_t) devAddr.Get ();

    //calculate and schedule ping slots for this device

    //on the end device pingTime factors in the time in between the given timestamp and now
    Time offset = Simulator::Now() - m_timestamp;

    NS_LOG_DEBUG("Scheduling ping slots for device" << devAddr.Get ());
    for(uint i=0;i< m_ClassBPingSlots; i++){
      uint64_t pingTime = beacon_reserved + (zzero + period*i) * slotLength; // Ping slot time is beacon_reserved + (pingOffset + N*pingPeriod) * slotLength
      Time ping = MilliSeconds(pingTime);
      ping -= offset;
      NS_LOG_DEBUG(this << "ed : ping slot for device " << dAddr << " is at " << Simulator::Now() + ping);
  //    Simulator::Schedule (ping, &LoRaWANEndDeviceApplication::ClassBPingSlot, this);
    }
// //}


//.........................................

    addresstemp =GetDeviceAddress () ;
   int index_en_matrice = SimpleNetworkServer::tessst(addresstemp) ;
   NS_LOG_INFO("index_en_matrice") ;

   NS_LOG_INFO(index_en_matrice) ;
    x =  index_en_matrice;
  EndDeviceLoraMac::exist() ;
  //   while(matrice_of_slot[ligne][colone]!=0){
  //     ligne= ligne+1 ;
  //
  //   }
  //   matrice_of_slot[ligne][0]=x ;
  // ligne= 0 ;
  //
  for (size_t i = 0; i < 200; i++) {
    if (x!=matrice_of_slot[i][0]) {

    }
    else {
      ligne=i ;
    }
    /* code */
  }
  /// chek for and add index
int r = 128 ;

//reception beacon
if (init_beacon_reception == true) {

  for (size_t i = 0; i < 400; i++) {

Time receptionbeacon = Seconds(r) ;
    Simulator::Schedule (receptionbeacon,
  &EndDeviceLoraMac::OpenslotReceiveWindow, this);
    /* code */
r= r+128 ;
init_beacon_reception = false ;

  }
}





//for (size_t i = 0; i < 10; i++) {
  /* code */


  simulationTime = Simulator::Now() ;
Time m_pingslot_time = Seconds(0) ;
  NS_LOG_INFO ("simulationTime is " ) ;
  NS_LOG_INFO (simulationTime) ;
  NS_LOG_INFO(m_pingslot_time) ;
  // NS_LOG_INFO("counter is" ) ;
  //
  // NS_LOG_INFO(c) ;


    NS_LOG_INFO("m_pingslot_time" ) ;

                          NS_LOG_INFO("starting new beacon period") ;
                          NS_LOG_INFO(Simulator::Now().GetSeconds()) ;
                          NS_LOG_INFO("Beacon_reserved time for 2.120s") ;
                          m_pingslot_time = m_pingslot_time + Seconds( 2.12) ;
                          NS_LOG_INFO("Beacon-window for 122.880s") ;
                          NS_LOG_INFO("clacul de ton et schedule opeing recieving window") ;
                            /* code */



                            // parametre de ping slot

                            // Number of ping slots per beacon period. This must be a power of 2
                            // integer: pingNb = 2 k where 1 <= k <=7
                            size_t k = 5;
                            size_t pingNb = pow (2,k);
                            NS_LOG_DEBUG ("pingNb") ;
                            NS_LOG_INFO (pingNb) ;


                            //
                            // Period of the device receiver wake-up expressed in number of slots:
                            // pingPeriod = 2 exp 12 / pingNb

                            size_t pingPeriod =4096 / pingNb  ;
                            NS_LOG_INFO ("pingPeriod" ) ;
                            NS_LOG_INFO ( pingPeriod) ;

                            //
                            // Randomized offset computed at each beacon period start. Values
                            // can range from 0 to (pingPeriod-1)

                            size_t pingOffset = rand() % (pingPeriod );
                            NS_LOG_INFO ("pingOffset" );
                            NS_LOG_INFO ( pingOffset) ;



                            //   Beacon_reserved + (pingOffset + pingPeriod)
                          //size_t m_receiveDelay10 ;
  colone=0 ;

  m_receiveDelay1 = m_receiveDelay1 + Seconds(pingOffset) ;
                          for (size_t i = 0; i < pingNb; i++) {
                            /* code */
    colone = colone + pingPeriod;




                            Simulator::Schedule (m_receiveDelay1,
                         &EndDeviceLoraMac::OpenslotReceiveWindow, this);



m_receiveDelay1 = m_receiveDelay1 + Seconds(pingPeriod) ;
NS_LOG_INFO("m_receiveDelay1") ;
NS_LOG_INFO(m_receiveDelay1) ;



     matrice_of_slot[ligne][colone] = 1 ;

   c= c+1 ;
}
  //for (size_t i = 0; i < 4096; i++) {
//  int sshow=  matrice_of_slot[ligne][i];
  //NS_LOG_INFO("colone") ; NS_LOG_INFO(i) ; NS_LOG_INFO(sshow) ;
    /* code */
//  }


//}
NS_LOG_INFO(c) ;
NS_LOG_INFO("counter for c ") ;
c=0;
    // Switch the PHY to sleep
    m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToSleep ();


}

void
EndDeviceLoraMac::OpenFirstReceiveWindow (void)
{
  //NS_LOG_FUNCTION_NOARGS ();

  // Set Phy in Standby mode
  m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToStandby ();

  // Schedule return to sleep after "at least the time required by the end
  // device's radio transceiver to effectively detect a downlink preamble"
  // (LoraWAN specification)
  m_closeFirstWindow = Simulator::Schedule (m_receiveWindowDuration,
                                            &EndDeviceLoraMac::CloseFirstReceiveWindow, this);
}

void
EndDeviceLoraMac::CloseFirstReceiveWindow (void)
{
//  NS_LOG_FUNCTION_NOARGS ();

  Ptr<EndDeviceLoraPhy> phy = m_phy->GetObject<EndDeviceLoraPhy> ();

  // Check the Phy layer's state:
  // - RX -> We are receiving a preamble.
  // - STANDBY -> Nothing was received.
  // - SLEEP -> We have received a packet.
  // We should never be in TX or SLEEP mode at this point
  switch (phy->GetState ())
    {
    case EndDeviceLoraPhy::TX:
      NS_ABORT_MSG ("PHY was in TX mode when attempting to " <<
                    "close a receive window.");
      break;
    case EndDeviceLoraPhy::RX:
      // PHY is receiving: let it finish. The Receive method will switch it back to SLEEP.
      break;
    case EndDeviceLoraPhy::SLEEP:
      // PHY has received, and the MAC's Receive already put the device to sleep
      break;
    case EndDeviceLoraPhy::STANDBY:
      // Turn PHY layer to SLEEP
      phy->SwitchToSleep ();
      break;
    }
}

void
EndDeviceLoraMac::OpenslotReceiveWindow (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Check for receiver status: if it's locked on a packet, don't open this
  // window at all.
  if (m_phy->GetObject<EndDeviceLoraPhy> ()->GetState () == EndDeviceLoraPhy::RX)
    {
      NS_LOG_INFO ("Won't open slot receive window since we are in RX mode.");

      return;
    }

  // Set Phy in Standby mode
  m_phy->GetObject<EndDeviceLoraPhy> ()->SwitchToStandby ();

  // Switch to appropriate channel and data rate
  NS_LOG_INFO ("Using parameters: " << m_pingslotwindowFrequency << "Hz, DR"
                                    << unsigned(m_pingslotwindowDataRate));

  m_phy->GetObject<EndDeviceLoraPhy> ()->SetFrequency
    (m_pingslotwindowFrequency);
  m_phy->GetObject<EndDeviceLoraPhy> ()->SetSpreadingFactor (GetSfFromDataRate
                                                               (m_pingslotwindowDataRate));

  // Schedule return to sleep after "at least the time required by the end
  // device's radio transceiver to effectively detect a downlink preamble"
  // (LoraWAN specification)
  m_closepingslot = Simulator::Schedule (m_receiveWindowDuration,
                                             &EndDeviceLoraMac::CloseslotReceiveWindow, this);
}

void
EndDeviceLoraMac::CloseslotReceiveWindow (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  Ptr<EndDeviceLoraPhy> phy = m_phy->GetObject<EndDeviceLoraPhy> ();

  // NS_ASSERT (phy->m_state != EndDeviceLoraPhy::TX &&
  // phy->m_state != EndDeviceLoraPhy::SLEEP);

  // Check the Phy layer's state:
  // - RX -> We have received a preamble.
  // - STANDBY -> Nothing was detected.
  switch (phy->GetState ())
    {
    case EndDeviceLoraPhy::TX:
      break;
    case EndDeviceLoraPhy::SLEEP:
      break;
    case EndDeviceLoraPhy::RX:
      // PHY is receiving: let it finish
      NS_LOG_DEBUG ("PHY is receiving: Receive will handle the result.");
      return;
    case EndDeviceLoraPhy::STANDBY:
      // Turn PHY layer to sleep
      phy->SwitchToSleep ();
      break;
    }


      // Reset retransmission parameters
      resetRetransmissionParameters ();

}

Time
EndDeviceLoraMac::GetNextTransmissionDelay (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  //    Check duty cycle    //

  // Pick a random channel to transmit on
  std::vector<Ptr<LogicalLoraChannel> > logicalChannels;
  logicalChannels = m_channelHelper.GetEnabledChannelList ();           // Use a separate list to do the shuffle
  //logicalChannels = Shuffle (logicalChannels);

  NS_LOG_DEBUG ("longueur de list " << logicalChannels.size ());

  Time waitingTime = Time::Max ();

  // Try every channel
  std::vector<Ptr<LogicalLoraChannel> >::iterator it;
  for (it = logicalChannels.begin (); it != logicalChannels.end (); ++it)
    {
      // Pointer to the current channel
      Ptr<LogicalLoraChannel> logicalChannel = *it;
      double frequency = logicalChannel->GetFrequency ();

      waitingTime = std::min (waitingTime, m_channelHelper.GetWaitingTime (logicalChannel));

      NS_LOG_DEBUG ("Waiting time before the next transmission in channel with frequecy " <<
                    frequency << " is = " << waitingTime.GetSeconds () << ".");
    }


  //    Check if there are receiving windows    //

  if (!m_closeFirstWindow.IsExpired () || !m_closepingslot.IsExpired () || !m_pingslotwindow.IsExpired () )
    {
      NS_LOG_WARN ("Attempting to send when there are receive windows:" <<
                   " Transmission postponed.");
      Time endRxWindow = (m_receiveDelay2 + m_receiveWindowDuration);
      waitingTime = std::max (waitingTime, endRxWindow);
    }

  return waitingTime;
}

Ptr<LogicalLoraChannel>
EndDeviceLoraMac::GetChannelForTx (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  // Pick a random channel to transmit on
  std::vector<Ptr<LogicalLoraChannel> > logicalChannels;
  logicalChannels = m_channelHelper.GetEnabledChannelList ();           // Use a separate list to do the shuffle
  logicalChannels = Shuffle (logicalChannels);

  // Try every channel
  std::vector<Ptr<LogicalLoraChannel> >::iterator it;
  for (it = logicalChannels.begin (); it != logicalChannels.end (); ++it)
    {
      // Pointer to the current channel
      Ptr<LogicalLoraChannel> logicalChannel = *it;
      double frequency = logicalChannel->GetFrequency ();

      NS_LOG_DEBUG ("Frequency of the current channel: " << frequency);

      // Verify that we can send the packet
      Time waitingTime = m_channelHelper.GetWaitingTime (logicalChannel);

      NS_LOG_DEBUG ("Waiting time for current channel = " <<
                    waitingTime.GetSeconds ());

      // Send immediately if we can
      if (waitingTime == Seconds (0))
        {
          return *it;
        }
      else
        {
          NS_LOG_DEBUG ("Packet cannot be immediately transmitted on " <<
                        "the current channel because of duty cycle limitations.");
        }
    }
  return 0;           // In this case, no suitable channel was found
}


std::vector<Ptr<LogicalLoraChannel> >
EndDeviceLoraMac::Shuffle (std::vector<Ptr<LogicalLoraChannel> > vector)
{
  NS_LOG_FUNCTION_NOARGS ();

  int size = vector.size ();

  for (int i = 0; i < size; ++i)
    {
      uint16_t random = std::floor (m_uniformRV->GetValue (0, size));
      Ptr<LogicalLoraChannel> temp = vector.at (random);
      vector.at (random) = vector.at (i);
      vector.at (i) = temp;
    }

  return vector;
}

/////////////////////////
// Setters and Getters //
/////////////////////////

void EndDeviceLoraMac::resetRetransmissionParameters ()
{
  m_retxParams.waitingAck = false;
  m_retxParams.retxLeft = m_maxNumbTx;
  m_retxParams.packet = 0;
  m_retxParams.firstAttempt = Seconds (0);
}


void
EndDeviceLoraMac::SetDataRateAdaptation (bool adapt)
{
  NS_LOG_FUNCTION (this << adapt);
  m_enableDRAdapt = adapt;
}

bool
EndDeviceLoraMac::GetDataRateAdaptation (void)
{
  return m_enableDRAdapt;
}

void
EndDeviceLoraMac::SetMaxNumberOfTransmissions (uint8_t maxNumbTx)
{
  NS_LOG_FUNCTION (this << unsigned(maxNumbTx));
  m_maxNumbTx = maxNumbTx;
  m_retxParams.retxLeft = maxNumbTx;
}

uint8_t
EndDeviceLoraMac::GetMaxNumberOfTransmissions (void)
{
  NS_LOG_FUNCTION (this );
  return m_maxNumbTx;
}


void
EndDeviceLoraMac::SetDataRate (uint8_t dataRate)
{
  NS_LOG_FUNCTION (this << unsigned (dataRate));

  m_dataRate = dataRate;
}

uint8_t
EndDeviceLoraMac::GetDataRate (void)
{
  NS_LOG_FUNCTION (this);

  return m_dataRate;
}

void
EndDeviceLoraMac::SetDeviceAddress (LoraDeviceAddress address)
{
  NS_LOG_FUNCTION (this << address);

  m_address = address;
}

LoraDeviceAddress
EndDeviceLoraMac::GetDeviceAddress (void)
{
//  NS_LOG_FUNCTION (this);

  return m_address;
}

void
EndDeviceLoraMac::OnLinkCheckAns (uint8_t margin, uint8_t gwCnt)
{
  NS_LOG_FUNCTION (this << unsigned(margin) << unsigned(gwCnt));

  m_lastKnownLinkMargin = margin;
  m_lastKnownGatewayCount = gwCnt;
}

void
EndDeviceLoraMac::OnLinkAdrReq (uint8_t dataRate, uint8_t txPower,
                                std::list<int> enabledChannels, int repetitions)
{
  NS_LOG_FUNCTION (this << unsigned (dataRate) << unsigned (txPower) <<
                   repetitions);

  // Three bools for three requirements before setting things up
  bool channelMaskOk = true;
  bool dataRateOk = true;
  bool txPowerOk = true;

  // Check the channel mask
  /////////////////////////
  // Check whether all specified channels exist on this device
  auto channelList = m_channelHelper.GetChannelList ();
  int channelListSize = channelList.size ();

  for (auto it = enabledChannels.begin (); it != enabledChannels.end (); it++)
    {
      if ((*it) > channelListSize)
        {
          channelMaskOk = false;
          break;
        }
    }

  // Check the dataRate
  /////////////////////
  // We need to know we can use it at all
  // To assess this, we try and convert it to a SF/BW combination and check if
  // those values are valid. Since GetSfFromDataRate and
  // GetBandwidthFromDataRate return 0 if the dataRate is not recognized, we
  // can check against this.
  uint8_t sf = GetSfFromDataRate (dataRate);
  double bw = GetBandwidthFromDataRate (dataRate);
  NS_LOG_DEBUG ("SF: " << unsigned (sf) << ", BW: " << bw);
  if (sf == 0 || bw == 0)
    {
      dataRateOk = false;
      NS_LOG_DEBUG ("Data rate non valid");
    }

  // We need to know we can use it in at least one of the enabled channels
  // Cycle through available channels, stop when at least one is enabled for the
  // specified dataRate.
  if (dataRateOk && channelMaskOk)           // If false, skip the check
    {
      bool foundAvailableChannel = false;
      for (auto it = enabledChannels.begin (); it != enabledChannels.end (); it++)
        {
          NS_LOG_DEBUG ("MinDR: " << unsigned (channelList.at (*it)->GetMinimumDataRate ()));
          NS_LOG_DEBUG ("MaxDR: " << unsigned (channelList.at (*it)->GetMaximumDataRate ()));
          if (channelList.at (*it)->GetMinimumDataRate () <= dataRate
              && channelList.at (*it)->GetMaximumDataRate () >= dataRate)
            {
              foundAvailableChannel = true;
              break;
            }
        }

      if (!foundAvailableChannel)
        {
          dataRateOk = false;
          NS_LOG_DEBUG ("Available channel not found");
        }
    }

  // Check the txPower
  ////////////////////
  // Check whether we can use this transmission power
  if (GetDbmForTxPower (txPower) == 0)
    {
      txPowerOk = false;
    }

  NS_LOG_DEBUG ("Finished checking. " <<
                "ChannelMaskOk: " << channelMaskOk << ", " <<
                "DataRateOk: " << dataRateOk << ", " <<
                "txPowerOk: " << txPowerOk);

  // If all checks are successful, set parameters up
  //////////////////////////////////////////////////
  if (channelMaskOk && dataRateOk && txPowerOk)
    {
      // Cycle over all channels in the list
      for (uint32_t i = 0; i < m_channelHelper.GetChannelList ().size (); i++)
        {
          if (std::find (enabledChannels.begin (), enabledChannels.end (), i) != enabledChannels.end ())
            {
              m_channelHelper.GetChannelList ().at (i)->SetEnabledForUplink ();
              NS_LOG_DEBUG ("Channel " << i << " enabled");
            }
          else
            {
              m_channelHelper.GetChannelList ().at (i)->DisableForUplink ();
              NS_LOG_DEBUG ("Channel " << i << " disabled");
            }
        }

      // Set the data rate
      m_dataRate = dataRate;

      // Set the transmission power
      m_txPower = GetDbmForTxPower (txPower);
    }

  // Craft a LinkAdrAns MAC command as a response
  ///////////////////////////////////////////////
  m_macCommandList.push_back (CreateObject<LinkAdrAns> (txPowerOk, dataRateOk,
                                                        channelMaskOk));
}

void
EndDeviceLoraMac::OnDutyCycleReq (double dutyCycle)
{
  NS_LOG_FUNCTION (this << dutyCycle);

  // Make sure we get a value that makes sense
  NS_ASSERT (0 <= dutyCycle && dutyCycle < 1);

  // Set the new duty cycle value
  m_aggregatedDutyCycle = dutyCycle;

  // Craft a DutyCycleAns as response
  NS_LOG_INFO ("Adding DutyCycleAns reply");
  m_macCommandList.push_back (CreateObject<DutyCycleAns> ());
}

void
EndDeviceLoraMac::OnRxParamSetupReq (uint8_t rx1DrOffset, uint8_t rx2DataRate, double frequency)
{
  NS_LOG_FUNCTION (this << unsigned (rx1DrOffset) << unsigned (rx2DataRate) <<
                   frequency);

  bool offsetOk = true;
  bool dataRateOk = true;

  // Check that the desired offset is valid
  if ( !(0 <= rx1DrOffset && rx1DrOffset <= 5))
    {
      offsetOk = false;
    }

  // Check that the desired data rate is valid
  if (GetSfFromDataRate (rx2DataRate) == 0
      || GetBandwidthFromDataRate (rx2DataRate) == 0)
    {
      dataRateOk = false;
    }

  // For now, don't check for validity of frequency
  if (offsetOk && dataRateOk)
    {
      m_pingslotwindowDataRate = rx2DataRate;
      m_rx1DrOffset = rx1DrOffset;
      m_pingslotwindowFrequency = frequency;
    }

  // Craft a RxParamSetupAns as response
  NS_LOG_INFO ("Adding RxParamSetupAns reply");
  m_macCommandList.push_back (CreateObject<RxParamSetupAns> (offsetOk,
                                                             dataRateOk, true));
}

void
EndDeviceLoraMac::OnDevStatusReq (void)
{
  NS_LOG_FUNCTION (this);

  uint8_t battery = 10;           // XXX Fake battery level
  uint8_t margin = 10;           // XXX Fake margin

  // Craft a RxParamSetupAns as response
  NS_LOG_INFO ("Adding DevStatusAns reply");
  m_macCommandList.push_back (CreateObject<DevStatusAns> (battery, margin));
}

void
EndDeviceLoraMac::OnNewChannelReq (uint8_t chIndex, double frequency, uint8_t minDataRate, uint8_t maxDataRate)
{
  NS_LOG_FUNCTION (this);

  bool dataRateRangeOk = true;           // XXX Check whether the new data rate range is ok
  bool channelFrequencyOk = true;           // XXX Check whether the frequency is ok

  // TODO Return false if one of the checks above failed
  // TODO Create new channel in the LogicalLoraChannelHelper

  SetLogicalChannel (chIndex, frequency, minDataRate, maxDataRate);

  NS_LOG_INFO ("Adding NewChannelAns reply");
  m_macCommandList.push_back (CreateObject<NewChannelAns> (dataRateRangeOk,
                                                           channelFrequencyOk));
}

void
EndDeviceLoraMac::AddLogicalChannel (double frequency)
{
  NS_LOG_FUNCTION (this << frequency);

  m_channelHelper.AddChannel (frequency);
}

void
EndDeviceLoraMac::AddLogicalChannel (Ptr<LogicalLoraChannel> logicalChannel)
{
  NS_LOG_FUNCTION (this << logicalChannel);

  m_channelHelper.AddChannel (logicalChannel);
}

void
EndDeviceLoraMac::SetLogicalChannel (uint8_t chIndex, double frequency,
                                     uint8_t minDataRate, uint8_t maxDataRate)
{
  NS_LOG_FUNCTION (this << unsigned (chIndex) << frequency <<
                   unsigned (minDataRate) << unsigned(maxDataRate));

  m_channelHelper.SetChannel (chIndex, CreateObject<LogicalLoraChannel>
                                (frequency, minDataRate, maxDataRate));
}

void
EndDeviceLoraMac::AddSubBand (double startFrequency, double endFrequency, double dutyCycle, double maxTxPowerDbm)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_channelHelper.AddSubBand (startFrequency, endFrequency, dutyCycle, maxTxPowerDbm);
}

uint8_t
EndDeviceLoraMac::GetFirstReceiveWindowDataRate (void)
{
  return m_replyDataRateMatrix.at (m_dataRate).at (m_rx1DrOffset);
}

void
EndDeviceLoraMac::SetslotWindowDataRate (uint8_t dataRate)
{
  m_pingslotwindowDataRate = dataRate;
}

uint8_t
EndDeviceLoraMac::GetslotWindowDataRate (void)
{
  return m_pingslotwindowDataRate;
}

void
EndDeviceLoraMac::SetslotWindowFrequency (double frequencyMHz)
{
  m_pingslotwindowFrequency = frequencyMHz;
}

double
EndDeviceLoraMac::GetslotWindowFrequency (void)
{
  return m_pingslotwindowFrequency;
}

double
EndDeviceLoraMac::GetAggregatedDutyCycle (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  return m_aggregatedDutyCycle;
}
}
