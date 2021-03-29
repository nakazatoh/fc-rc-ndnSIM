#ifndef FC_ROUTE_AGENT_STARTER_HPP
#define FC_ROUTE_AGENT_STARTER_HPP

#include "fc-route-agent.hpp"
#include "ns3/simulator.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/application.h"

namespace ns3 {

// Class inheriting from ns3::Application
class FCRouteAgentStarter : public Application
{
public:
  static TypeId
  GetTypeId();

protected:
  // inherited from Application base class.
  virtual void
  StartApplication();

  virtual void
  StopApplication();

private:
  std::shared_ptr<app::FCRouteAgent> m_instance;
};

} // namespace ns3

#endif // FC_ROUTE_AGENT_STARTER_HPP