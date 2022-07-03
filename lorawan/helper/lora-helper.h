

#ifndef LORA_HELPER_H
#define LORA_HELPER_H

#include "ns3/lora-phy-helper.h"
#include "ns3/lora-mac-helper.h"
#include "ns3/node-container.h"
#include "ns3/net-device-container.h"
#include "ns3/net-device.h"
#include "ns3/lora-net-device.h"

namespace ns3 {

/**
 * Helps to create LoraNetDevice objects
 *
 * This class can help create a large set of similar LoraNetDevice objects and
 * configure a large set of their attributes during creation.
 */
class LoraHelper
{
public:
  virtual ~LoraHelper ();

  LoraHelper ();

  /**
   * Install LoraNetDevices on a list of nodes
   *
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param c the set of nodes on which a lora device must be created
   * \returns a device container which contains all the devices created by this
   * method.
   */
  virtual NetDeviceContainer Install (const LoraPhyHelper &phyHelper,
                                      const LoraMacHelper &macHelper,
                                      NodeContainer c) const;

  /**
   * Install LoraNetDevice on a single node
   *
   * \param phy the PHY helper to create PHY objects
   * \param mac the MAC helper to create MAC objects
   * \param node the node on which a lora device must be created
   * \returns a device container which contains all the devices created by this
   * method.
   */
  virtual NetDeviceContainer Install (const LoraPhyHelper &phyHelper,
                                      const LoraMacHelper &macHelper,
                                      Ptr<Node> node) const;
};

} //namespace ns3

#endif /* LORA_HELPER_H */
