  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/gateway-status.h"
#include "ns3/log.h"
#include <fstream>      // std::fstream
namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("GatewayStatus");

GatewayStatus::GatewayStatus ()
{
  NS_LOG_FUNCTION (this);
}

GatewayStatus::~GatewayStatus ()
{
  NS_LOG_FUNCTION (this);
}

GatewayStatus::GatewayStatus (Address address, Ptr<NetDevice> netDevice,
                              Ptr<GatewayLoraMac> gwMac) :
  m_address (address),
  m_netDevice (netDevice),
  m_gatewayMac (gwMac),
  m_nextTransmissionTime (Seconds (0))
{
  NS_LOG_FUNCTION (this);
}

Address
GatewayStatus::GetAddress ()
{
  NS_LOG_FUNCTION (this);

  return m_address;
}

void
GatewayStatus::SetAddress (Address address)
{
  NS_LOG_FUNCTION (this);

  m_address = address;
}

Ptr<NetDevice>
GatewayStatus::GetNetDevice ()
{
  return m_netDevice;
}

void
GatewayStatus::SetNetDevice (Ptr<NetDevice> netDevice)
{
  m_netDevice = netDevice;
}

Ptr<GatewayLoraMac>
GatewayStatus::GetGatewayMac (void)
{
  return m_gatewayMac;
}

//ofstream log;
int no_gw_transmi=0 ;
int gw_booked_int =0 ;
int gw_tx_mode_int =0 ;


//static int not_send_duty () ;
bool
GatewayStatus::IsAvailableForTransmission (double frequency)
{
  // We can't send multiple packets at once, see SX1301 V2.01 page 29

  // Check that the gateway was not already "booked"
  if (m_nextTransmissionTime > Simulator::Now () - MilliSeconds (1))
    {
      gw_booked_int++ ;
      NS_LOG_INFO ("This gateway is already booked for a transmission");
      return false;

    }

  // Check that the gateway is not already in TX mode
  if (m_gatewayMac->IsTransmitting ())
    {
      NS_LOG_INFO ("This gateway is currently transmitting");
      return false;
    }

  // Check that the gateway is not constrained by the duty cycle
  Time waitingTime = m_gatewayMac->GetWaitingTime (frequency);




  if (waitingTime > Seconds (0))
    {
no_gw_transmi++ ;
     NS_LOG_INFO ("Gateway cannot be used because of duty cycle");
     NS_LOG_INFO ("Waiting time at current GW: " << waitingTime.GetSeconds ()
                                                 << " seconds");
 NS_LOG_INFO ("no_gw_transmi=:  ");   NS_LOG_INFO (no_gw_transmi  );

      return false;
    }
else {   NS_LOG_INFO ("waiting time = 00.00s");}
 //no_gw_transmi =0 ;
  return true;
}
int GatewayStatus::gateway_no_DC (){
  return  no_gw_transmi;
}

int GatewayStatus::gateway_booked (){
  return  gw_booked_int;
}
  int GatewayStatus::gateway_tx_on (){
    return  gw_tx_mode_int;

}

void
GatewayStatus::SetNextTransmissionTime (Time nextTransmissionTime)
{
  m_nextTransmissionTime = nextTransmissionTime;
}
}
