  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef PERIODIC_SENDER_H
#define PERIODIC_SENDER_H

#include "ns3/application.h"
#include "ns3/nstime.h"
#include "ns3/lora-mac.h"
#include "ns3/attribute.h"

namespace ns3 {

class PeriodicSender : public Application
{
public:
  PeriodicSender ();
  ~PeriodicSender ();

  static TypeId GetTypeId (void);

  /**
   * Set the sending interval
   * \param interval the interval between two packet sendings
   */
  void SetInterval (Time interval);

  /**
   * Get the sending inteval
   * \returns the interval between two packet sends
   */
  Time GetInterval (void) const;

  /**
   * Set the initial delay of this application
   */
  void SetInitialDelay (Time delay);

  /**
   * Set packet size
   */
  void SetPacketSize (uint8_t size);

  /**
   * Set if using randomness in the packet size
   */
  void SetPacketSizeRandomVariable (Ptr <RandomVariableStream> rv);

  /**
   * Send a packet using the LoraNetDevice's Send method
   */
  void SendPacket (void);

  /**
   * Start the application by scheduling the first SendPacket event
   */
  void StartApplication (void);

  /**
   * Stop the application
   */
  void StopApplication (void);

private:
  /**
   * The interval between to consecutive send events
   */
  Time m_interval;

  /**
   * The initial delay of this application
   */
  Time m_initialDelay;

  /**
   * The sending event scheduled as next
   */
  EventId m_sendEvent;

  /**
   * The MAC layer of this node
   */
  Ptr<LoraMac> m_mac;

  /**
   * The packet size.
   */
  uint8_t m_basePktSize;


  /**
   * The random variable that adds bytes to the packet size
   */
  Ptr<RandomVariableStream> m_pktSizeRV;


};

} //namespace ns3

#endif /* SENDER_APPLICATION */

