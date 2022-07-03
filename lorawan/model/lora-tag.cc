  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 
#include "ns3/lora-tag.h"
#include "ns3/tag.h"
#include "ns3/uinteger.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LoraTag);

TypeId
LoraTag::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraTag")
    .SetParent<Tag> ()
    .SetGroupName ("lorawan")
    .AddConstructor<LoraTag> ()
  ;
  return tid;
}

TypeId
LoraTag::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

LoraTag::LoraTag (uint8_t sf, uint8_t destroyedBy) :
  m_sf (sf),
  m_destroyedBy (destroyedBy),
  m_receivePower (0),
  m_dataRate (0),
  m_frequency (0)
{
}

LoraTag::~LoraTag ()
{
}

uint32_t
LoraTag::GetSerializedSize (void) const
{
  // Each datum about a SF is 1 byte + receivePower (the size of a double) +
  // frequency (the size of a double)
  return 3 + 2*sizeof(double);
}

void
LoraTag::Serialize (TagBuffer i) const
{
  i.WriteU8 (m_sf);
  i.WriteU8 (m_destroyedBy);
  i.WriteDouble (m_receivePower);
  i.WriteU8 (m_dataRate);
  i.WriteDouble (m_frequency);
}

void
LoraTag::Deserialize (TagBuffer i)
{
  m_sf = i.ReadU8 ();
  m_destroyedBy = i.ReadU8 ();
  m_receivePower = i.ReadDouble ();
  m_dataRate = i.ReadU8 ();
  m_frequency = i.ReadDouble ();
}

void
LoraTag::Print (std::ostream &os) const
{
  os << m_sf << " " << m_destroyedBy << " " << m_receivePower << " " <<
  m_dataRate;
}

uint8_t
LoraTag::GetSpreadingFactor () const
{
  return m_sf;
}

uint8_t
LoraTag::GetDestroyedBy () const
{
  return m_destroyedBy;
}

double
LoraTag::GetReceivePower () const
{
  return m_receivePower;
}

void
LoraTag::SetDestroyedBy (uint8_t sf)
{
  m_destroyedBy = sf;
}

void
LoraTag::SetSpreadingFactor (uint8_t sf)
{
  m_sf = sf;
}

void
LoraTag::SetReceivePower (double receivePower)
{
  m_receivePower = receivePower;
}

void
LoraTag::SetFrequency (double frequency)
{
  m_frequency = frequency;
}

double
LoraTag::GetFrequency (void)
{
  return m_frequency;
}

uint8_t
LoraTag::GetDataRate (void)
{
  return m_dataRate;
}

void
LoraTag::SetDataRate (uint8_t dataRate)
{
  m_dataRate = dataRate;
}

} // namespace ns3
