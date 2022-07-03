

#include "ns3/sender-helper.h"
#include "ns3/one-shot-sender.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/trace-source-accessor.h"
#include "ns3/simulator.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("OneShotSenderHelper");

OneShotSenderHelper::OneShotSenderHelper ()
{
  m_factory.SetTypeId ("ns3::OneShotSender");
}

OneShotSenderHelper::~OneShotSenderHelper ()
{
}

void
OneShotSenderHelper::SetSendTime (Time sendTime)
{
  m_sendTime = sendTime;
}

void
OneShotSenderHelper::SetAttribute (std::string name,
                                   const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
OneShotSenderHelper::Install (Ptr<Node> node) const
{
  return ApplicationContainer (InstallPriv (node));
}

ApplicationContainer
OneShotSenderHelper::Install (NodeContainer c) const
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      apps.Add (InstallPriv (*i));
    }

  return apps;
}

Ptr<Application>
OneShotSenderHelper::InstallPriv (Ptr<Node> node) const
{
  NS_LOG_FUNCTION (this << node->GetId());

  Ptr<OneShotSender> app = m_factory.Create<OneShotSender> ();

  app->SetSendTime (m_sendTime);

  app->SetNode (node);
  node->AddApplication (app);

  return app;
}
} // namespace ns3
