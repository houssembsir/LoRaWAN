  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
 /*
 * This script simulates a complex scenario with multiple gateways and end
 * devices. The metric of interest for this script is the throughput of the
 * network.
 */



 #include "ns3/point-to-point-module.h"
 #include "ns3/forwarder-helper.h"
 #include "ns3/network-server-helper.h"
 #include "ns3/lora-channel.h"
#include "ns3/gateway-status.h"
#include "ns3/end-device-lora-phy.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/end-device-lora-mac_B.h"
#include "ns3/gateway-lora-mac_B.h"
#include "ns3/simulator.h"
#include "ns3/log.h"
#include "ns3/network-server.h"
#include "ns3/pointer.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/lora-helper.h"
#include "ns3/node-container.h"
#include "ns3/mobility-helper.h"
#include "ns3/position-allocator.h"
#include "ns3/double.h"
#include "ns3/simple-network-server.h"
#include "ns3/random-variable-stream.h"
#include "ns3/beacon-sender-helper.h"
#include "ns3/lora-phy-helper.h"
#include "ns3/lora-mac-helper.h"
#include "ns3/lora-helper.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/periodic-sender.h"
#include "ns3/beacon-sender-helper.h"
#include "ns3/log.h"
#include "ns3/string.h"
#include "ns3/command-line.h"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/lora-device-address-generator.h"
#include "ns3/sender-helper.h"
#include "ns3/command-line.h"
#include <algorithm>
#include <ctime>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("LorawanClassBExample");

// Network settings
int nDevices = 100; //
int gatewayRings = 1;
int nGateways = 3 * gatewayRings * gatewayRings - 3 * gatewayRings + 1;
double radius = 7500;
double gatewayRadius = 7500 / ((gatewayRings - 1) * 2 + 1);
double simulationTime = 21600;   //86400
int appPeriodSeconds = 1000;   //1000
std::vector<int> sfQuantity (6);


/********************** The above code is declaring the variables that will be used in the program.********/
int noMoreReceivers = 0;
int interfered = 0;
int received = 0;
int underSensitivity = 0;

// Output control
bool printEDs = true;
bool buildingsEnabled = false;

/**********************
 *  Global Callbacks  *
 **********************/

enum PacketOutcome
{
  RECEIVED,
  INTERFERED,
  NO_MORE_RECEIVERS,
  UNDER_SENSITIVITY,
  UNSET
};

struct PacketStatus
{
  Ptr<Packet const> packet;
  uint32_t senderId;
  int outcomeNumber;
  std::vector<enum PacketOutcome> outcomes;
};

std::map<Ptr<Packet const>, PacketStatus> packetTracker;

//The above code is checking whether a packet is received by all gateways. If so, it updates the statistics and removes the packet from the tracker.
void
CheckReceptionByAllGWsComplete (std::map<Ptr<Packet const>, PacketStatus>::iterator it)
{
  // Check whether this packet is received by all gateways
  if ((*it).second.outcomeNumber == nGateways)
    {
      // Update the statistics
      PacketStatus status = (*it).second;
      for (int j = 0; j < nGateways; j++)
        {
          switch (status.outcomes.at (j))
            {
            case RECEIVED:
              {
                received += 1;
                break;
              }
            case UNDER_SENSITIVITY:
              {
                underSensitivity += 1;
                break;
              }
            case NO_MORE_RECEIVERS:
              {
                noMoreReceivers += 1;
                break;
              }
            case INTERFERED:
              {
                interfered += 1;
                break;
              }
            case UNSET:
              {
                break;
              }
            }
        }
      // Remove the packet from the tracker
      packetTracker.erase (it);
    }
}

void
TransmissionCallback (Ptr<Packet const> packet, uint32_t systemId)
{
  // NS_LOG_INFO ("Transmitted a packet from device " << systemId);
  // Create a packetStatus
  PacketStatus status;
  status.packet = packet;
  status.senderId = systemId;
  status.outcomeNumber = 0;
  status.outcomes = std::vector<enum PacketOutcome> (nGateways, UNSET);

  packetTracker.insert (std::pair<Ptr<Packet const>, PacketStatus> (packet, status));
}

void
PacketReceptionCallback (Ptr<Packet const> packet, uint32_t systemId)
{
  // Remove the successfully received packet from the list of sent ones
  // NS_LOG_INFO ("A packet was successfully received at gateway " << systemId);

  std::map<Ptr<Packet const>, PacketStatus>::iterator it = packetTracker.find (packet);
  (*it).second.outcomes.at (systemId - nDevices) = RECEIVED;
  (*it).second.outcomeNumber += 1;

  CheckReceptionByAllGWsComplete (it);
}

void
InterferenceCallback (Ptr<Packet const> packet, uint32_t systemId)
{
  // NS_LOG_INFO ("A packet was lost because of interference at gateway " << systemId);

  std::map<Ptr<Packet const>, PacketStatus>::iterator it = packetTracker.find (packet);
  (*it).second.outcomes.at (systemId - nDevices) = INTERFERED;
  (*it).second.outcomeNumber += 1;

  CheckReceptionByAllGWsComplete (it);
}

void
NoMoreReceiversCallback (Ptr<Packet const> packet, uint32_t systemId)
{
  // NS_LOG_INFO ("A packet was lost because there were no more receivers at gateway " << systemId);

  std::map<Ptr<Packet const>, PacketStatus>::iterator it = packetTracker.find (packet);
  (*it).second.outcomes.at (systemId - nDevices) = NO_MORE_RECEIVERS;
  (*it).second.outcomeNumber += 1;

  CheckReceptionByAllGWsComplete (it);
}

void
UnderSensitivityCallback (Ptr<Packet const> packet, uint32_t systemId)
{
  // NS_LOG_INFO ("A packet arrived at the gateway under sensitivity at gateway " << systemId);

  std::map<Ptr<Packet const>, PacketStatus>::iterator it = packetTracker.find (packet);
  (*it).second.outcomes.at (systemId - nDevices) = UNDER_SENSITIVITY;
  (*it).second.outcomeNumber += 1;

  CheckReceptionByAllGWsComplete (it);
}

time_t oldtime = std::time (0);

// Periodically print simulation time
void PrintSimulationTime (void)
{
 // Printing the simulated time and the real time from the last call.
  //Printing the simulated time and the real time from the last call.
  // NS_LOG_INFO ("Time: " << Simulator::Now().GetHours());
  std::cout << "Simulated time: " << Simulator::Now ().GetHours () << " hours" << std::endl;
  std::cout << "Real time from last call: " << std::time (0) - oldtime << " seconds" << std::endl;
  oldtime = std::time (0);
  Simulator::Schedule (Minutes (30), &PrintSimulationTime);
}

void
PrintEndDevices (NodeContainer endDevices, NodeContainer gateways, std::string filename)
{
  const char * c = filename.c_str ();
  std::ofstream spreadingFactorFile;
  spreadingFactorFile.open (c);
  for (NodeContainer::Iterator j = endDevices.Begin (); j != endDevices.End (); ++j)
    {
      Ptr<Node> object = *j;
      Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      NS_ASSERT (position != 0);
      Ptr<NetDevice> netDevice = object->GetDevice (0);
      Ptr<LoraNetDevice> loraNetDevice = netDevice->GetObject<LoraNetDevice> ();
      NS_ASSERT (loraNetDevice != 0);
      Ptr<EndDeviceLoraMac> mac = loraNetDevice->GetMac ()->GetObject<EndDeviceLoraMac> ();
      int sf = int(mac->GetDataRate ());
      Vector pos = position->GetPosition ();
      spreadingFactorFile << pos.x << " " << pos.y << " " << sf << std::endl;
    }
  // Also print the gateways
  for (NodeContainer::Iterator j = gateways.Begin (); j != gateways.End (); ++j)
    {
      Ptr<Node> object = *j;
      Ptr<MobilityModel> position = object->GetObject<MobilityModel> ();
      Vector pos = position->GetPosition ();
      spreadingFactorFile << pos.x << " " << pos.y << " GW" << std::endl;
    }
  spreadingFactorFile.close ();
}

int main (int argc, char *argv[])
{

  CommandLine cmd;
  cmd.AddValue ("nDevices", "Number of end devices to include in the simulation", nDevices);
  cmd.AddValue ("gatewayRings", "Number of gateway rings to include", gatewayRings);
  cmd.AddValue ("radius", "The radius of the area to simulate", radius);
  cmd.AddValue ("gatewayRadius", "The distance between two gateways", gatewayRadius);
  cmd.AddValue ("simulationTime", "The time for which to simulate", simulationTime);
  cmd.AddValue ("appPeriod", "The period in seconds to be used by periodically transmitting applications", appPeriodSeconds);
  cmd.AddValue ("printEDs", "Whether or not to print a file containing the ED's positions", printEDs);

  cmd.Parse (argc, argv);

  // Set up logging
  //
  LogComponentEnableAll (LOG_PREFIX_FUNC);
  LogComponentEnableAll (LOG_PREFIX_NODE);
  LogComponentEnableAll (LOG_PREFIX_TIME);

//   LogComponentEnable ("SimpleNetworkServer", LOG_LEVEL_ALL);
 //   LogComponentEnable ("EndDeviceLoraPhy", LOG_LEVEL_ALL);
 //     LogComponentEnable ("GatewayLoraPhy", LOG_LEVEL_ALL);
  LogComponentEnable ("EndDeviceLoraMac", LOG_LEVEL_ALL);
 //      LogComponentEnable ("GatewayLoraMac", LOG_LEVEL_ALL);
 //     LogComponentEnable ("LoraPhy", LOG_LEVEL_ALL);
 //     LogComponentEnable ("LoraPhyHelper", LOG_LEVEL_ALL);
 //       LogComponentEnable ("LoraMacHelper", LOG_LEVEL_ALL);
 //


 //
 //   LogComponentEnable ("LorawanClassBExample", LOG_LEVEL_ALL);
 // LogComponentEnable ("GatewayStatus", LOG_LEVEL_ALL);
 //   LogComponentEnable("LoraChannel", LOG_LEVEL_INFO);
 //   LogComponentEnable("LoraPhy", LOG_LEVEL_ALL);
 //   LogComponentEnable("EndDeviceLoraPhy", LOG_LEVEL_ALL);
 //  LogComponentEnable("GatewayLoraPhy", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraInterferenceHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraMac", LOG_LEVEL_ALL);
 //  LogComponentEnable("EndDeviceLoraMac", LOG_LEVEL_ALL);
 //  LogComponentEnable("GatewayLoraMac", LOG_LEVEL_ALL);
 //  LogComponentEnable("LogicalLoraChannelHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("LogicalLoraChannel", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraPhyHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraMacHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("PeriodicSenderHelper", LOG_LEVEL_ALL);
 //  LogComponentEnable("PeriodicSender", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraMacHeader", LOG_LEVEL_ALL);
 //  LogComponentEnable("LoraFrameHeader", LOG_LEVEL_ALL);
  //
  // RngSeedManager::SetSeed (1);  // Changes seed from default of 1 to 3
  // RngSeedManager::SetRun (2);
// NS_GLOBAL_VALUE="RngRun=3" ./waf
  /***********
  *  Setup  *
  ***********/

  // Compute the number of gateways
  nGateways = 3 * gatewayRings * gatewayRings - 3 * gatewayRings + 1;

  // Create the time value from the period
  Time appPeriod = Seconds (appPeriodSeconds);

  // Mobility
  MobilityHelper mobility;
  mobility.SetPositionAllocator ("ns3::UniformDiscPositionAllocator",
                                 "rho", DoubleValue (radius),
                                 "X", DoubleValue (0.0),
                                 "Y", DoubleValue (0.0));
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");

  /************************
  *  Create the channel  *
  ************************/

  // Create the lora channel object
  Ptr<LogDistancePropagationLossModel> loss = CreateObject<LogDistancePropagationLossModel> ();
  loss->SetPathLossExponent (3.76);
  loss->SetReference (1, 7.7);

  Ptr<PropagationDelayModel> delay = CreateObject<ConstantSpeedPropagationDelayModel> ();

  Ptr<LoraChannel> channel = CreateObject<LoraChannel> (loss, delay);

  /************************
  *  Create the helpers  *
  ************************/

  // Create the LoraPhyHelper
  LoraPhyHelper phyHelper = LoraPhyHelper ();
  phyHelper.SetChannel (channel);

  // Create the LoraMacHelper
  LoraMacHelper macHelper = LoraMacHelper ();

  // Create the LoraHelper
  LoraHelper helper = LoraHelper ();

  /************************
  *  Create End Devices  *
  ************************/

  // Create a set of nodes
  NodeContainer endDevices;
  endDevices.Create (nDevices);

  // Assign a mobility model to each node
  mobility.Install (endDevices);

  // Create a LoraDeviceAddressGenerator
  uint8_t nwkId = 54;
  uint32_t nwkAddr = 1864;
  Ptr<LoraDeviceAddressGenerator> addrGen = CreateObject<LoraDeviceAddressGenerator> (nwkId,nwkAddr);


  // Make it so that nodes are at a certain height > 0
  for (NodeContainer::Iterator j = endDevices.Begin ();
       j != endDevices.End (); ++j)
    {
      Ptr<MobilityModel> mobility = (*j)->GetObject<MobilityModel> ();
      Vector position = mobility->GetPosition ();
      position.z = 1.2;
      mobility->SetPosition (position);
    }
  //
  // // Create the LoraNetDevices of the end devices
  // phyHelper.SetDeviceType (LoraPhyHelper::ED);
  // macHelper.SetDeviceType (LoraMacHelper::ED);
  // helper.Install (phyHelper, macHelper, endDevices);


  // Create the LoraNetDevices of the end devices
  phyHelper.SetDeviceType (LoraPhyHelper::ED);
  macHelper.SetDeviceType (LoraMacHelper::ED);
  macHelper.SetAddressGenerator (addrGen);
  macHelper.SetRegion (LoraMacHelper::EU);
  helper.Install (phyHelper, macHelper, endDevices);




  // Now end devices are connected to the channel

  // Connect trace sources
  for (NodeContainer::Iterator j = endDevices.Begin ();
       j != endDevices.End (); ++j)
    {
      Ptr<Node> node = *j;
      Ptr<LoraNetDevice> loraNetDevice = node->GetDevice (0)->GetObject<LoraNetDevice> ();
      Ptr<LoraPhy> phy = loraNetDevice->GetPhy ();
      phy->TraceConnectWithoutContext ("StartSending",
                                       MakeCallback (&TransmissionCallback));

                                       Ptr<EndDeviceLoraMac> mac = loraNetDevice->GetMac ()->GetObject<EndDeviceLoraMac>();
                                       mac->SetMType (LoraMacHeader::UNCONFIRMED_DATA_UP);
                                       mac->SetMaxNumberOfTransmissions (1);

    }

  /*********************
  *  Create Gateways  *
  *********************/

  // Create the gateway nodes (allocate them uniformely on the disc)
  NodeContainer gateways;
  gateways.Create (nGateways);

  Ptr<ListPositionAllocator> allocator = CreateObject<ListPositionAllocator> ();
  // Make it so that nodes are at a certain height > 0
  allocator->Add (Vector (0.0, 0.0, 15.0));
  mobility.SetPositionAllocator (allocator);
  mobility.Install (gateways);


  // Create a netdevice for each gateway
  phyHelper.SetDeviceType (LoraPhyHelper::GW);
  macHelper.SetDeviceType (LoraMacHelper::GW);
  helper.Install (phyHelper, macHelper, gateways);

  NodeContainer networkServers;
  networkServers.Create (1);

  // Install the SimpleNetworkServer application on the network server
  NetworkServerHelper networkServerHelper;
  networkServerHelper.SetGateways (gateways);
  networkServerHelper.SetEndDevices (endDevices);
  networkServerHelper.Install (networkServers);

  // Install the Forwarder application on the gateways
  ForwarderHelper forwarderHelper;
  forwarderHelper.Install (gateways);




  /************************
  *  Configure Gateways  *
  ************************/

  // Install reception paths on gateways
  for (NodeContainer::Iterator j = gateways.Begin (); j != gateways.End (); j++)
    {

      Ptr<Node> object = *j;
      // Get the device
      Ptr<NetDevice> netDevice = object->GetDevice (0);
      Ptr<LoraNetDevice> loraNetDevice = netDevice->GetObject<LoraNetDevice> ();
      NS_ASSERT (loraNetDevice != 0);
      Ptr<GatewayLoraPhy> gwPhy = loraNetDevice->GetPhy ()->GetObject<GatewayLoraPhy> ();


      // Global callbacks (every gateway)
      gwPhy->TraceConnectWithoutContext ("ReceivedPacket",
                                         MakeCallback (&PacketReceptionCallback));
      gwPhy->TraceConnectWithoutContext ("LostPacketBecauseInterference",
                                         MakeCallback (&InterferenceCallback));
      gwPhy->TraceConnectWithoutContext ("LostPacketBecauseNoMoreReceivers",
                                         MakeCallback (&NoMoreReceiversCallback));
      gwPhy->TraceConnectWithoutContext ("LostPacketBecauseUnderSensitivity",
                                         MakeCallback (&UnderSensitivityCallback));
    }

  /**********************************************
  *  Set up the end device's spreading factor  *
  **********************************************/

  sfQuantity = macHelper.SetSpreadingFactorsUp (endDevices, gateways, channel);

  NS_LOG_DEBUG ("Completed configuration");

  /*********************************************
  *  Install applications on the end devices  *
  *********************************************/

 
  //Creating a periodic sender application that sends packets of size 23 bytes every appPeriodSeconds seconds.
  Time appStopTime = Seconds (simulationTime);
  PeriodicSenderHelper appHelper = PeriodicSenderHelper ();
  appHelper.SetPeriod (Seconds (appPeriodSeconds));
  appHelper.SetPacketSize (23);
  Ptr <RandomVariableStream> rv = CreateObjectWithAttributes<UniformRandomVariable> ("Min", DoubleValue (0), "Max", DoubleValue (10));
  ApplicationContainer appContainer = appHelper.Install (endDevices);

  appContainer.Start (Seconds (0));
  appContainer.Stop (appStopTime);

  /**********************
   * Print output files *
   *********************/
  if (printEDs)
    {
      PrintEndDevices (endDevices, gateways,
                       "src/lorawan/examples/endDevices.dat");
    }



  /****************
  *  Simulation  *
  ****************/

  Simulator::Stop (appStopTime + Hours (0));

  // PrintSimulationTime ();

  Simulator::Run ();

  Simulator::Destroy ();

  /*************
  *  Results  *
  *************/
 int no_gw_DC = GatewayStatus::gateway_no_DC();
int totalpkNS= SimpleNetworkServer::return_count_total_pck_from_NS () ;
int no_gw_booked = GatewayStatus::gateway_booked();
int no_gw_TX = GatewayStatus::gateway_tx_on();
  //double receivedProb = double(received) / nDevices;
  double interferedProb = double(interfered) / nDevices;
  // double noMoreReceiversProb = double(noMoreReceivers) / nDevices;
  // double underSensitivityProb = double(underSensitivity) / nDevices;
  //
  // double receivedProbGivenAboveSensitivity = double(received) / (nDevices - underSensitivity);
  // double interferedProbGivenAboveSensitivity = double(interfered) / (nDevices - underSensitivity);
  // double noMoreReceiversProbGivenAboveSensitivity = double(noMoreReceivers) / (nDevices - underSensitivity);
  // // std::cout << " nDevices:  "  << nDevices << " \n received " <<  received << " \n (nDevices) / simulationTime:  " << double(nDevices) / simulationTime << " \n receivedProbGivenAboveSensitivity: " << receivedProb << " \n interferedProb: " << interferedProb << " \n noMoreReceiversProb: " << noMoreReceiversProb << " \n underSensitivityProb: " << underSensitivityProb <<
  // // "\n receivedProbGivenAboveSensitivity:  " << receivedProbGivenAboveSensitivity << " \n interferedProbGivenAboveSensitivity: " << interferedProbGivenAboveSensitivity << " \n noMoreReceiversProbGivenAboveSensitivity: " << noMoreReceiversProbGivenAboveSensitivity << std::endl;






  
  std::fstream fs;
    fs.open ("gw.txt", std::fstream::in | std::fstream::out | std::fstream::app);

    fs << " nDevices:  "  << nDevices << " \n interferedProb: " << interferedProb <<
    " \n  nb total packet from NS:  "  << totalpkNS << " \n  nb paket loss DC:  "  << no_gw_DC << " \n"
    << " \n no_gw_booked:   "  << no_gw_booked
    << " \n  no_gw_TX:  "  << no_gw_TX
    << " \n"<< " \n"<< " \n"<< std::endl;
;

    fs.close();





  return 0;
}
