  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/lora-device-address-generator.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraDeviceAddressGenerator");

TypeId
LoraDeviceAddressGenerator::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraDeviceAddressGenerator")
    .SetParent<Object> ()
    .SetGroupName ("lorawan")
    .AddConstructor<LoraDeviceAddressGenerator> ();
  return tid;
}

LoraDeviceAddressGenerator::LoraDeviceAddressGenerator (const uint8_t nwkId,
                                                        const uint32_t nwkAddr)
{
  NS_LOG_FUNCTION (this << unsigned(nwkId) << nwkAddr);

  m_currentNwkId.Set (nwkId);
  m_currentNwkAddr.Set (nwkAddr);
}

LoraDeviceAddress
LoraDeviceAddressGenerator::NextNetwork (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  m_currentNwkId.Set (m_currentNwkId.Get () + 1);
  m_currentNwkAddr.Set (0);

  return LoraDeviceAddress (m_currentNwkId, m_currentNwkAddr);
}

LoraDeviceAddress
LoraDeviceAddressGenerator::NextAddress (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  NwkAddr oldNwkAddr = m_currentNwkAddr;
  m_currentNwkAddr.Set (m_currentNwkAddr.Get () + 1);

  return LoraDeviceAddress (m_currentNwkId, oldNwkAddr);
}

LoraDeviceAddress
LoraDeviceAddressGenerator::GetNextAddress (void)
{
  NS_LOG_FUNCTION_NOARGS ();

  return LoraDeviceAddress (m_currentNwkId.Get (), m_currentNwkAddr.Get () + 1);
}
}
