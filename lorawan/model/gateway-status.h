
  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#ifndef GATEWAY_STATUS_H
#define GATEWAY_STATUS_H

#include "ns3/object.h"
#include "ns3/address.h"
#include "ns3/net-device.h"
#include "ns3/gateway-lora-mac_B.h"

namespace ns3 {

class GatewayStatus
{


public:
int not_send_duty () ;
  GatewayStatus ();
  virtual ~GatewayStatus ();
  GatewayStatus (Address address, Ptr<NetDevice> netDevice, Ptr<GatewayLoraMac> gwMac);

static int gateway_booked()  ;

static int gateway_no_DC()  ;

static int gateway_tx_on()  ;
  /**
   * Get this gateway's P2P link address.
   */
  Address GetAddress ();

  /**
   * Set this gateway's P2P link address.
   */
  void SetAddress (Address address);

  /**
   * Get the NetDevice through which it's possible to contact this gateway from the server.
   */
  Ptr<NetDevice> GetNetDevice ();

  /**
   * Set the NetDevice through which it's possible to contact this gateway from the server.
   */
  void SetNetDevice (Ptr<NetDevice> netDevice);

  /**
   * Get a pointer to this gateway's MAC instance.
   */
  Ptr<GatewayLoraMac> GetGatewayMac (void);

  /**
   * Set a pointer to this gateway's MAC instance.
   */
  // void SetGatewayMac (Ptr<GatewayLoraMac> gwMac);

  /**
   * Query whether or not this gateway is available for immediate transmission
   * on this frequency.
   *
   * \param frequency The frequency at which the gateway's availability should
   * be queried.
   * \return True if the gateway's available, false otherwise.
   */
  bool IsAvailableForTransmission (double frequency);

  void SetNextTransmissionTime (Time nextTransmissionTime);
  // Time GetNextTransmissionTime (void);

private:

  Address m_address; //!< The Address of the P2PNetDevice of this gateway

  Ptr<NetDevice> m_netDevice;   //!< The NetDevice through which to reach this gateway from the server

  Ptr<GatewayLoraMac> m_gatewayMac;   //!< The Mac layer of the gateway

  bool m_isTransmitting; //!< Whether this gateway is already booked for
                         //!transmission or not

  Time m_nextTransmissionTime; //!< This gateway's next transmission time

};
}

#endif /* DEVICE_STATUS_H */
