  /****************
  *  This project is under development using DAVID: https://github.com/signetlabdei/lorawan/ original class A as a principal source  *
  *  This project can not be used for industrial purpose , it's not updated regularly and some function can be modified or cease to function  *
  *  This project is developed in the purpose of Ph.D. research.
  *  Note that the author of this project: elbsir houssem eddin gives the authorisation for any researcher to use this model with no prior authorization
  ****************/
 

#ifndef BUILDING_PENETRATION_LOSS_H
#define BUILDING_PENETRATION_LOSS_H

#include "ns3/propagation-loss-model.h"
#include "ns3/mobility-model.h"
#include "ns3/vector.h"
#include "ns3/random-variable-stream.h"

namespace ns3 {

  class MobilityModel;

  /**
   * A class implementing the TR 45.820 model for building losses
   */
  class BuildingPenetrationLoss : public PropagationLossModel
  {
  public:

    static TypeId GetTypeId (void);

    BuildingPenetrationLoss ();

    ~BuildingPenetrationLoss ();

  private:

    /**
     * Perform the computation of the received power according to the current
     * model.
     */
    virtual double DoCalcRxPower (double txPowerDbm,
                                  Ptr<MobilityModel> a,
                                  Ptr<MobilityModel> b) const;

    virtual int64_t DoAssignStreams (int64_t stream);

    /**
     * Generate a random p value.
     * The distribution of the returned value is as specified in TR 45.820.
     * \returns A value in the 0-3 range.
     */
    int GetPValue (void) const;

    /**
     * Get a value to compute the wall loss.
     * The distribution of the returned value is as specified in TR 45.820.
     * \returns A value in the 0-2 range.
     */
    int GetWallLossValue (void) const;

    /**
     * Compute the wall loss associated to this mobility model
     * \param b The mobility model associated to the node whose wall loss we need
     * to compute.
     * \returns The power loss due to external walls.
     */
    double GetWallLoss (Ptr<MobilityModel> b) const;

    /**
     * Get the Tor1 value used in the TR 45.820 standard to account for internal
     * wall loss.
     * \param b The mobility model of the node we want to compute the value for.
     * \returns The tor1 value.
     */
    double GetTor1 (Ptr<MobilityModel> b) const;

    Ptr<UniformRandomVariable> m_uniformRV; //!< An uniform RV

    /**
     * A map linking each mobility model to a p value
     */
    mutable std::map<Ptr<MobilityModel>, int> m_pMap;

    /**
     * A map linking each mobility model to a value deciding its external wall
     * loss.
     */
    mutable std::map<Ptr<MobilityModel>, int> m_wallLossMap;
  };
}
#endif
