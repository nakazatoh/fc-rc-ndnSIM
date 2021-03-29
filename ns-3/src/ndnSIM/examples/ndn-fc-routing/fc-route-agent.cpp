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

// fc-route-agent.cpp

#include "fc-route-agent.hpp"
#include "ndn-cxx/encoding/tlv-nfd.hpp"
#include "ndn-cxx/util/exception.hpp"

#include "ns3/log.h"
#include "ns3/simulator.h"

#include <iostream>

NS_LOG_COMPONENT_DEFINE("FCRouteAgent");

namespace app {

FCRouteAgent::FCRouteAgent(ndn::KeyChain& keyChain)
  : m_keyChain(keyChain)
  , m_scheduler(m_face.getIoService())
  , m_nfdController(m_face, keyChain)
{
}

void
FCRouteAgent::run()
{
//  ndn::Interest interest(ndn::Name("/example/testApp/randomData"));
//	interest.setInterestLifetime(ndn::time::milliseconds(1000));
//	interest.setMustBeFresh(true);

//	m_face.expressInterest(interest,
//                        std::bind(&FCRouteAgent::onData, this, _1, _2),
//                        std::bind(&FCRouteAgent::onNack, this, _1, _2),
//                        std::bind(&FCRouteAgent::onTimeout, this, _1));
//	std::cout << "Sending: " << interest << std::endl;

 	m_face.setInterestFilter("/ndn-fc/f1",
 														std::bind(&FCRouteAgent::onInterest, this, _1, _2),
 														ndn::RegisterPrefixSuccessCallback(),
 														std::bind(&FCRouteAgent::onRegisterFailed, this, _1, _2));

  ndn::nfd::ControlParameters parameters;
  parameters.setName("/ndn-fc/f1").setFaceId(0);
  m_nfdController.start<::ndn::nfd::RibRegisterCommand>(parameters,
      [=] (const ndn::nfd::ControlParameters& res) {
      NS_LOG_DEBUG("Successfully registered /ndn-fc/f1 with NFD");
    },
    [=] (const ndn::nfd::ControlResponse& res) {
//      NDN_THROW(Error("Cannot add RIB entry /ndn-fc/f1 (" +
//                      to_string(res.getCode()) + " " + res.getText() + ")"));
        std::cout << "Cannot add RIB entry /ndn-fc/f1 (" <<
                      std::to_string(res.getCode()) << " " << res.getText() << ")\n";
    });

//  SendInterest();

  m_face.processEvents(); // ok (will not block and do nothing)
  // m_faceConsumer.getIoService().run(); // will crash
}

//void
//FCRouteAgent::onData(const ndn::Interest& interest, const ndn::Data& data)
//{
//	std::cout << data << std::endl;
//}

void
FCRouteAgent::onNack(const ndn::Interest& interest, const ndn::lp::Nack& nack)
{
	std::cout << "received Nack with reason " << nack.getReason() << " for interest" << interest << std::endl;
}

void
FCRouteAgent::onTimeout(const ndn::Interest& interest)
{
	std::cout << "Timeout " << interest << std::endl;
}

void
FCRouteAgent::SendInterest()
{
/////////////////////////////////////
// Sending one Interest packet out //
/////////////////////////////////////

// Create and configure ndn::Interest
//  auto interest = std::make_shared<ndn::Interest>("/prefix/sub");
//  auto interest = std::make_shared<ndn::Interest>("/localhost/nfd/rib/register/");
//  ndn::Name commandName("/localhost/nfd/rib");
//  commandName.append("register");
  ndn::nfd::ControlParameters parameters;
  parameters.setName("/ndn-fc/f1").setFaceId(0);
//  ::ndn::Block encodedParameters(parameters.wireEncode());
//  commandName.append(encodedParameters);
//  commandName.appendTimestamp(::ndn::time::system_clock::now());
//  std::shared_ptr<ndn::Interest> command(std::make_shared<ndn::Interest>(commandName));
//  ::ndn::security::CommandInterestSigner signer(ns3::ndn::StackHelper::getKeyChain());
//  std::shared_ptr<ndn::Interest> command(std::make_shared<ndn::Interest>(signer.makeCommandInterest(commandName)));

//  command->setCanBePrefix(false);
//  ns3::ndn::StackHelper::getKeyChain().sign(*command);
//  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
//  Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
//  interest->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
//  command->setNonce(rand->GetValue(0, std::numeric_limits<uint32_t>::max()));
//  interest->setInterestLifetime(ndn::time::seconds(1));
//  command->setInterestLifetime(ndn::time::seconds(1));

//  NS_LOG_DEBUG("Sending Interest packet for " << *interest);
//  NS_LOG_DEBUG("Sending Interest packet for " << command);
  m_nfdController.start<::ndn::nfd::RibRegisterCommand>(parameters,
      [=] (const ndn::nfd::ControlParameters& res) {
      NS_LOG_DEBUG("Successfully registered /ndn-fc/f1 with NFD");
    },
    [=] (const ndn::nfd::ControlResponse& res) {
//      NDN_THROW(Error("Cannot add RIB entry /ndn-fc/f1 (" +
//                      to_string(res.getCode()) + " " + res.getText() + ")"));
        std::cout << "Cannot add RIB entry /ndn-fc/f1 (" <<
                      std::to_string(res.getCode()) << " " << res.getText() << ")\n";
    });

  // Call trace (for logging purposes)
//  m_transmittedInterests(interest, this, m_face);
//  m_transmittedInterests(command, this, m_face);

//  m_appLink->onReceiveInterest(*interest);
//  m_appLink->onReceiveInterest(*command);
  m_face.processEvents();
}

// Callback that will be called when Interest arrives
void
FCRouteAgent::onInterest(const ndn::InterestFilter& filter, const ndn::Interest& interest)
{

  NS_LOG_DEBUG("Received Interest packet for " << interest.getName());

  // Note that Interests send out by the app will not be sent back to the app !

  auto data = std::make_shared<ndn::Data>(interest.getName());
  data->setFreshnessPeriod(ndn::time::milliseconds(1000));
  data->setContent(std::make_shared< ::ndn::Buffer>(1024));
  m_keyChain.sign(*data);

  NS_LOG_DEBUG("Sending Data packet for " << data->getName());

  // Call trace (for logging purposes)
  //m_transmittedDatas(data, this, m_face);

  m_face.put(*data);
}

void
FCRouteAgent::onRegisterFailed(const ndn::Name& prefix, const std::string& reason)
{
	std::cerr << "ERROR: Failed to register prefix \""
						<< prefix <<"\" in local hub's daemon (" << reason << ")"
						<< std::endl;

	m_face.shutdown();
}

// Callback that will be called when Data arrives
void
FCRouteAgent::onData(const ndn::Interest& interest, const ndn::Data& data)
{
  NS_LOG_DEBUG("Receiving Data packet for " << data.getName());
  ::ndn::Block content = data.getContent();
  content.parse();
  ::ndn::Block responseBlock = content.get(ndn::tlv::nfd::ControlResponse);
  ::ndn::nfd::ControlResponse cr(responseBlock);
  std::cout << "DATA received for name " << data.getName() << " StatusCode:" << cr.getCode() 
    << " StatusText:" << cr.getText() << std::endl;
//  std::cout << "DATA received for name " << data->getName() << std::endl;
}

} // namespace ns3
