/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2011-2015  Regents of the University of California.
 *
 * This file is part of ndnSIM. See AUTHORS for complete list of ndnSIM authors and
 * contributors.
 *
 * ndnSIM is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * ndnSIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * ndnSIM, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

// my-custom-app.cpp

#include "my-custom-app.hpp"

#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/node.h"

#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include "ns3/ndnSIM/helper/ndn-fib-helper.hpp"

#include "ns3/random-variable-stream.h"

#include <ndn-cxx/util/time.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/mgmt/nfd/control-parameters.hpp>
#include <ndn-cxx/mgmt/nfd/control-response.hpp>
#include "ns3/ndnSIM/helper/ndn-stack-helper.hpp"
#include <ndn-cxx/encoding/tlv-nfd.hpp>
#include <ndn-cxx/security/command-interest-signer.hpp>

NS_LOG_COMPONENT_DEFINE("MyCustomApp");

using ::ndn::nfd::ControlParameters;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED(MyCustomApp);

// register NS-3 type
TypeId
MyCustomApp::GetTypeId()
{
  static TypeId tid = TypeId("MyCustomApp").SetParent<ndn::App>().AddConstructor<MyCustomApp>();
  return tid;
}

// Processing upon start of the application
void
MyCustomApp::StartApplication()
{
  // initialize ndn::App
  ndn::App::StartApplication();

  // Add entry to FIB for `/prefix/sub`
  ndn::FibHelper::AddRoute(GetNode(), "/prefix/sub", m_face, 0);

  // Schedule send of first interest
  Simulator::Schedule(Seconds(1.0), &MyCustomApp::SendInterest, this);
}

// Processing when application is stopped
void
MyCustomApp::StopApplication()
{
  // cleanup ndn::App
  ndn::App::StopApplication();
}

void
MyCustomApp::SendInterest()
{
  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  // Create and configure ndn::Interest
//  auto interest = std::make_shared<ndn::Interest>("/prefix/sub");
//  auto interest = std::make_shared<ndn::Interest>("/localhost/nfd/rib/register/");
  ndn::Name commandName("/localhost/nfd/rib");
  commandName.append("register");
  ControlParameters parameters;
  parameters.setName("/ndn-fc/f1");
  ::ndn::Block encodedParameters(parameters.wireEncode());
  commandName.append(encodedParameters);
  //commandName.appendTimestamp(::ndn::time::system_clock::now());
//std::shared_ptr<ndn::Interest> command(std::make_shared<ndn::Interest>(commandName));
  ::ndn::security::CommandInterestSigner signer(ns3::ndn::StackHelper::getKeyChain());
  std::shared_ptr<ndn::Interest> command(std::make_shared<ndn::Interest>(signer.makeCommandInterest(commandName)));

//  command->setCanBePrefix(false);
//  ns3::ndn::StackHelper::getKeyChain().sign(*command);
//  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
//  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
  command->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
//  interest->setInterestLifetime(ndn::time::seconds(1));
  command->setInterestLifetime(ndn::time::seconds(1));

//  NS_LOG_DEBUG("Sending Interest packet for " << *interest);
  NS_LOG_DEBUG("Sending Interest packet for " << command);

  // Call trace (for logging purposes)
//  m_transmittedInterests(interest, this, m_face);
  m_transmittedInterests(command, this, m_face);

//  m_appLink->onReceiveInterest(*interest);
  m_appLink->onReceiveInterest(*command);
}

// Callback that will be called when Interest arrives
void
MyCustomApp::OnInterest(std::shared_ptr<const ndn::Interest> interest)
{
  ndn::App::OnInterest(interest);

  NS_LOG_DEBUG("Received Interest packet for " << interest->getName());

  // Note that Interests send out by the app will not be sent back to the app !

  auto data = std::make_shared<ndn::Data>(interest->getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));
  ndn::StackHelper::getKeyChain().sign(*data);

  NS_LOG_DEBUG("Sending Data packet for " << data->getName());

  // Call trace (for logging purposes)
  m_transmittedDatas(data, this, m_face);

  m_appLink->onReceiveData(*data);
}

// Callback that will be called when Data arrives
void
MyCustomApp::OnData(std::shared_ptr<const ndn::Data> data)
{
  NS_LOG_DEBUG("Receiving Data packet for " << data->getName());
  ::ndn::Block content = data->getContent();
  content.parse();
  ::ndn::Block responseBlock = content.get(::ndn::tlv::nfd::ControlResponse);
  ::ndn::nfd::ControlResponse cr(responseBlock);
  std::cout << "DATA received for name " << data->getName() << " StatusCode:" << cr.getCode() 
    << " StatusText:" << cr.getText() << std::endl;
//  std::cout << "DATA received for name " << data->getName() << std::endl;
}

} // namespace ns3
