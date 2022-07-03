  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef LORA_DEVICE_ADDRESS_GENERATOR_H
#define LORA_DEVICE_ADDRESS_GENERATOR_H

#include "ns3/lora-device-address.h"
#include "ns3/object.h"

namespace ns3 {

/**
  * This class generates sequential LoraDeviceAddress instances.
  */
class LoraDeviceAddressGenerator : public Object
{
public:

  static TypeId GetTypeId (void);

  /**
   * Initialise the base NwkID and the first NwkAddr to be used by the
   * generator.
   *
   * The first call to NextAddress() or GetAddress() will return these values.
   *
   * \param nwkId The first network id.
   * \param nwkAddr The first address.
   */
  LoraDeviceAddressGenerator (const uint8_t nwkId = 0,
                              const uint32_t nwkAddr = 0);

  /**
   * Get the first address from the next network.
   *
   * This resets the address to the base address that was used for
   * initialization.
   *
   * \return the LoraDeviceAddress address of the next network
   */
  LoraDeviceAddress NextNetwork (void);

  /**
   * Allocate the next LoraDeviceAddress.
   *
   * This operation is a post-increment, meaning that the first address
   * allocated will be the one that was initially configured.
   *
   * This keeps the nwkId constant, only incrementing nwkAddr.
   *
   * \return the LoraDeviceAddress address
   */
  LoraDeviceAddress NextAddress (void);

  /**
   * Get the LoraDeviceAddress that will be allocated upon a call to
   * NextAddress.
   *
   * Does not change the internal state; is just used to peek at the next
   * address that will be allocated upon a call to NextAddress
   *
   * \return the LoraDeviceAddress
   */
  LoraDeviceAddress GetNextAddress (void);

private:

  NwkID m_currentNwkId; //!< The current Network Id value
  NwkAddr m_currentNwkAddr; //!< The current Network Address value
};
} //namespace ns3
#endif
