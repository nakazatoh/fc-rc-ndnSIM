#include "fc-route-agent-starter.hpp"
#include "ns3/simulator.h"
#include <ndn-cxx/util/time.hpp>

NS_LOG_COMPONENT_DEFINE("FCRouteAgentStarter");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(FCRouteAgentStarter);

TypeId
FCRouteAgentStarter::GetTypeId()
{
  static TypeId tid = TypeId("FCRouteAgentStarter")
    .SetParent<Application>()
    .AddConstructor<FCRouteAgentStarter>();

  return tid;
}

// inherited from Application base class.
void
FCRouteAgentStarter::StartApplication()
{
  // Create an instance of the app, and passing the dummy version of KeyChain (no real signing)
  m_instance.reset(new app::FCRouteAgent(ndn::StackHelper::getKeyChain()));
//  m_instance->run(); // can be omitted
  Simulator::Schedule(Seconds(1.0), &app::FCRouteAgent::run, m_instance.get());
}

void
FCRouteAgentStarter::StopApplication()
{
  // Stop and destroy the instance of the app
  m_instance.reset();
}

} // namespace ns3
