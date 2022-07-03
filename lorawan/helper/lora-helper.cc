
#include "ns3/lora-helper.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraHelper");

LoraHelper::LoraHelper ()
{
}

LoraHelper::~LoraHelper ()
{
}

NetDeviceContainer
LoraHelper::Install ( const LoraPhyHelper &phyHelper,
                      const LoraMacHelper &macHelper,
                      NodeContainer c) const
{
  NS_LOG_FUNCTION_NOARGS ();

  NetDeviceContainer devices;

  // Go over the various nodes in which to install the NetDevice
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      // Create the LoraNetDevice
      Ptr<LoraNetDevice> device = CreateObject<LoraNetDevice> ();

      // Create the PHY
      Ptr<LoraPhy> phy = phyHelper.Create (node, device);
      NS_ASSERT (phy != 0);
      device->SetPhy (phy);
      NS_LOG_DEBUG ("Done creating the PHY");

      // Create the MAC
      Ptr<LoraMac> mac = macHelper.Create (node, device);
      NS_ASSERT (mac != 0);
      mac->SetPhy (phy);
      NS_LOG_DEBUG ("Done creating the MAC");
      device->SetMac (mac);

      node->AddDevice (device);
      devices.Add (device);
      NS_LOG_DEBUG ("node=" << node << ", mob=" << node->GetObject<MobilityModel> ()-> GetPosition());
    }
  return devices;
}

NetDeviceContainer
LoraHelper::Install ( const LoraPhyHelper &phy,
                      const LoraMacHelper &mac,
                      Ptr<Node> node) const
{
  return Install (phy, mac, NodeContainer (node));
}
}
