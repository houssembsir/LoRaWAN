  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#include "ns3/lora-mac.h"
#include "ns3/log.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("LoraMac");

NS_OBJECT_ENSURE_REGISTERED (LoraMac);

TypeId
LoraMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraMac")
    .SetParent<Object> ()
    .SetGroupName ("lorawan")
    .AddTraceSource ("SentNewPacket",
                     "Trace source indicating a new packet "
                     "arrived at the MAC layer",
                     MakeTraceSourceAccessor (&LoraMac::m_sentNewPacket),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("ReceivedPacket",
                     "Trace source indicating a packet "
                     "was correctly received at the MAC layer",
                     MakeTraceSourceAccessor (&LoraMac::m_receivedPacket),
                     "ns3::Packet::TracedCallback")
    .AddTraceSource ("CannotSendBecauseDutyCycle",
                     "Trace source indicating a packet "
                     "could not be sent immediately because of duty cycle limitations",
                     MakeTraceSourceAccessor (&LoraMac::m_cannotSendBecauseDutyCycle),
                     "ns3::Packet::TracedCallback");
  return tid;
}

LoraMac::LoraMac ()
{
  NS_LOG_FUNCTION (this);
}

LoraMac::~LoraMac () {
  NS_LOG_FUNCTION (this);
}

void
LoraMac::SetDevice (Ptr<NetDevice> device)
{
  m_device = device;
}

Ptr<NetDevice>
LoraMac::GetDevice (void)
{
  return m_device;
}

Ptr<LoraPhy>
LoraMac::GetPhy (void)
{
  return m_phy;
}

void
LoraMac::SetPhy (Ptr<LoraPhy> phy)
{
  // Set the phy
  m_phy = phy;

  // Connect the receive callbacks
  m_phy->SetReceiveOkCallback (MakeCallback (&LoraMac::Receive, this));
  m_phy->SetReceiveFailedCallback (MakeCallback (&LoraMac::FailedReception, this));
  m_phy->Setmac_configCallback (MakeCallback (&LoraMac::mac_config, this));
}

LogicalLoraChannelHelper
LoraMac::GetLogicalLoraChannelHelper (void)
{
  return m_channelHelper;
}

void
LoraMac::SetLogicalLoraChannelHelper (LogicalLoraChannelHelper helper)
{
  m_channelHelper = helper;
}

uint8_t
LoraMac::GetSfFromDataRate (uint8_t dataRate)
{
  NS_LOG_FUNCTION (this << unsigned(dataRate));

  // Check we are in range
  if (dataRate >= m_sfForDataRate.size ())
    {
      return 0;
    }

  return m_sfForDataRate.at (dataRate);
}

double
LoraMac::GetBandwidthFromDataRate (uint8_t dataRate)
{
  NS_LOG_FUNCTION (this << unsigned(dataRate));

  // Check we are in range
  if (dataRate > m_bandwidthForDataRate.size ())
    {
      return 0;
    }

  return m_bandwidthForDataRate.at (dataRate);
}

double
LoraMac::GetDbmForTxPower (uint8_t txPower)
{
  NS_LOG_FUNCTION (this << unsigned (txPower));

  if (txPower > m_txDbmForTxPower.size ())
    {
      return 0;
    }

  return m_txDbmForTxPower.at (txPower);
}

void
LoraMac::SetSfForDataRate (std::vector<uint8_t> sfForDataRate)
{
  m_sfForDataRate = sfForDataRate;
}

void
LoraMac::SetBandwidthForDataRate (std::vector<double> bandwidthForDataRate)
{
  m_bandwidthForDataRate = bandwidthForDataRate;
}

void
LoraMac::SetMaxAppPayloadForDataRate (std::vector<uint32_t> maxAppPayloadForDataRate)
{
  m_maxAppPayloadForDataRate = maxAppPayloadForDataRate;
}

void
LoraMac::SetTxDbmForTxPower (std::vector<double> txDbmForTxPower)
{
  m_txDbmForTxPower = txDbmForTxPower;
}

void
LoraMac::SetNPreambleSymbols (int nPreambleSymbols)
{
  m_nPreambleSymbols = nPreambleSymbols;
}

int
LoraMac::GetNPreambleSymbols (void)
{
  return m_nPreambleSymbols;
}

void
LoraMac::SetReplyDataRateMatrix (ReplyDataRateMatrix replyDataRateMatrix)
{
  m_replyDataRateMatrix = replyDataRateMatrix;
}
}
