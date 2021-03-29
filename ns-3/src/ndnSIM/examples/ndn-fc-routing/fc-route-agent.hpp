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

// fc-route-agent.hpp

#ifndef FC_ROUTE_AGENT_H_
#define FC_ROUTE_AGENT_H_

#include <ndn-cxx/mgmt/nfd/controller.hpp>
#include <ndn-cxx/face.hpp>
#include <ndn-cxx/interest.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/util/scheduler.hpp>

#include <iostream>

namespace app {

/**
 * @brief A simple custom application
 *
 * This applications demonstrates how to send Interests and respond with Datas to incoming interests
 *
 * When application starts it "sets interest filter" (install FIB entry) for /prefix/sub, as well as
 * sends Interest for this prefix
 *
 * When an Interest is received, it is replied with a Data with 1024-byte fake payload
 */
class FCRouteAgent {
public:
  FCRouteAgent(ndn::KeyChain&);

  void
  run();

private:
  void
  onInterest(const ndn::InterestFilter&, const ndn::Interest& interest);

  void
  onRegisterFailed(const ndn::Name&, const std::string&);

	void
	onData(const ndn::Interest& interest, const ndn::Data& data);

	void
	onNack(const ndn::Interest& interest, const ndn::lp::Nack& nack);

	void
	onTimeout(const ndn::Interest& interest);

private:
  void
  SendInterest();

  ndn::KeyChain& m_keyChain;
  ndn::Face m_face;
  ndn::Scheduler m_scheduler;

  ::ndn::nfd::Controller m_nfdController;

};

} // namespace ns3

#endif // FC_ROUTE_AGENT_H_
