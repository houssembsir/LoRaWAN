  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#ifndef SIMPLE_GATEWAY_LORA_PHY_H
#define SIMPLE_GATEWAY_LORA_PHY_H

#include "ns3/object.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/mobility-model.h"
#include "ns3/node.h"
#include "ns3/gateway-lora-phy.h"
#include "ns3/traced-value.h"
#include <list>

namespace ns3 {

class LoraChannel;

/**
 * Class modeling a Lora SX1301 chip.
 */
class SimpleGatewayLoraPhy : public GatewayLoraPhy
{
public:

  static TypeId GetTypeId (void);

  SimpleGatewayLoraPhy();
  virtual ~SimpleGatewayLoraPhy();

  virtual void StartReceive (Ptr<Packet> packet, double rxPowerDbm, uint8_t sf,
                             Time duration, double frequencyMHz);

  virtual void EndReceive (Ptr<Packet> packet,
                           Ptr<LoraInterferenceHelper::Event> event);

  virtual void Send (Ptr<Packet> packet, LoraTxParameters txParams,
                     double frequencyMHz, double txPowerDbm);

private:
};

} /* namespace ns3 */

#endif /* SIMPLE_GATEWAY_LORA_PHY_H */
