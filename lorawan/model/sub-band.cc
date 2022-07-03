  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#include "ns3/sub-band.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("SubBand");

NS_OBJECT_ENSURE_REGISTERED (SubBand);

TypeId
SubBand::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::SubBand")
    .SetParent<Object> ()
    .SetGroupName ("lorawan");
  return tid;
}

SubBand::SubBand ()
{
  NS_LOG_FUNCTION (this);
}

SubBand::SubBand (double firstFrequency, double lastFrequency, double dutyCycle,
                  double maxTxPowerDbm) :
  m_firstFrequency (firstFrequency),
  m_lastFrequency (lastFrequency),
  m_dutyCycle (dutyCycle),
  m_maxTxPowerDbm (maxTxPowerDbm)
{
  NS_LOG_FUNCTION (this << firstFrequency << lastFrequency << dutyCycle <<
                   maxTxPowerDbm);
}

SubBand::~SubBand ()
{
  NS_LOG_FUNCTION (this);
}

double
SubBand::GetFirstFrequency (void)
{
  return m_firstFrequency;
}

double
SubBand::GetDutyCycle (void)
{
  return m_dutyCycle;
}

bool
SubBand::BelongsToSubBand (double frequency)
{
  return (frequency > m_firstFrequency) && (frequency < m_lastFrequency);
}

bool
SubBand::BelongsToSubBand (Ptr<LogicalLoraChannel> logicalChannel)
{
  double frequency = logicalChannel->GetFrequency ();
  return BelongsToSubBand (frequency);
}

void
SubBand::SetNextTransmissionTime (Time nextTime)
{
  m_nextTransmissionTime = nextTime;
}

Time
SubBand::GetNextTransmissionTime (void)
{
  return m_nextTransmissionTime;
}

void
SubBand::SetMaxTxPowerDbm (double maxTxPowerDbm)
{
  m_maxTxPowerDbm = maxTxPowerDbm;
}

double
SubBand::GetMaxTxPowerDbm (void)
{
  return m_maxTxPowerDbm;
}
}
