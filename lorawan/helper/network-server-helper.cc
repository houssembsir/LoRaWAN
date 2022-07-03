
#include "ns3/network-server-helper.h"
#include "ns3/network-server.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("NetworkServerHelper");

NetworkServerHelper::NetworkServerHelper ()
{
  m_factory.SetTypeId ("ns3::SimpleNetworkServer");
  p2pHelper.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
  p2pHelper.SetChannelAttribute ("Delay", StringValue ("2ms"));
}

NetworkServerHelper::~NetworkServerHelper ()
{
}

void
NetworkServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

void
NetworkServerHelper::SetGateways (NodeContainer gateways)
{
  m_gateways = gateways;
}

void
NetworkServerHelper::SetEndDevices (NodeContainer endDevices)
{
  m_endDevices = endDevices;
}

ApplicationContainer
NetworkServerHelper::Install (Ptr<Node> node)
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
NetworkServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
NetworkServerHelper::InstallPriv (Ptr<Node> node)
{
  NS_LOG_FUNCTION (this << node);

  Ptr<SimpleNetworkServer> app = m_factory.Create<SimpleNetworkServer> ();

  app->SetNode (node);
  node->AddApplication (app);

  // Cycle on each gateway
  for (NodeContainer::Iterator i = m_gateways.Begin ();
       i != m_gateways.End ();
       i++)
    {
      // Add the connections with the gateway
      // Create a PointToPoint link between gateway and NS
      NetDeviceContainer container = p2pHelper.Install (node, *i);

      // Add the gateway to the NS list
      app->AddGateway (*i, container.Get (0));
    }

  // Link the SimpleNetworkServer to its NetDevices
  for (uint32_t i = 0; i < node->GetNDevices (); i++)
    {
      Ptr<NetDevice> currentNetDevice = node->GetDevice (i);
      currentNetDevice->SetReceiveCallback (MakeCallback
                                              (&SimpleNetworkServer::Receive,
                                              app));
    }

  // Add the end devices
  app->AddNodes (m_endDevices);

  return app;
}
} // namespace ns3
