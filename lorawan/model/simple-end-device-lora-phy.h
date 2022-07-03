  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef SIMPLE_END_DEVICE_LORA_PHY_H
#define SIMPLE_END_DEVICE_LORA_PHY_H

#include "ns3/object.h"
#include "ns3/traced-value.h"
#include "ns3/net-device.h"
#include "ns3/nstime.h"
#include "ns3/mobility-model.h"
#include "ns3/node.h"
#include "ns3/end-device-lora-phy.h"

namespace ns3 {

class LoraChannel;

/**
 * Class representing a simple LoRa transceiver, with an error model based
 * on receiver sensitivity and a SIR table.
 */
class SimpleEndDeviceLoraPhy : public EndDeviceLoraPhy
{
public:

  static TypeId GetTypeId (void);

  // Constructor and destructor
  SimpleEndDeviceLoraPhy ();
  virtual ~SimpleEndDeviceLoraPhy ();

  // Implementation of EndDeviceLoraPhy's pure virtual functions
  virtual void StartReceive (Ptr<Packet> packet, double rxPowerDbm,
                             uint8_t sf, Time duration, double frequencyMHz);

  // Implementation of LoraPhy's pure virtual functions
  virtual void EndReceive (Ptr<Packet> packet,
                           Ptr<LoraInterferenceHelper::Event> event);

  // Implementation of LoraPhy's pure virtual functions
  virtual void Send (Ptr<Packet> packet, LoraTxParameters txParams,
                     double frequencyMHz, double txPowerDbm);

private:

};

} /* namespace ns3 */

#endif /* SIMPLE_END_DEVICE_LORA_PHY_H */
