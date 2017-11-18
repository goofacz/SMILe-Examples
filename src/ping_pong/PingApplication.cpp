//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#include "PingApplication.h"

#include <cassert>

namespace smile_examples {
namespace ping_pong {

Define_Module(PingApplication);

PingApplication::~PingApplication()
{
  if (periodicTxMessage) {
    cancelEvent(periodicTxMessage.get());
  }
}

void PingApplication::initialize(int stage)
{
  IdealApplication::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    periodicTxMessage = std::make_unique<cMessage>("periodicTxMessage");
    const auto delay = SimTime{par("delay").longValue(), SIMTIME_MS};
    scheduleAt(clockTime() + delay, periodicTxMessage.get());
  }
}

void PingApplication::handleSelfMessage(cMessage* message)
{
  // Compose MAC frame
  const auto destinationAddress = inet::MACAddress{par("remoteMacAddress").stringValue()};
  const auto name = std::string{"Frame no. "} + std::to_string(frameSequenceNumber);
  auto frame = createFrame<inet::IdealMacFrame>(destinationAddress, name.c_str());
  frame->setBitLength(10);

  EV_INFO << "Sending frame (" << frame->getClassName() << ")\"" << frame->getFullName() << "\"" << endl;

  // Pass MAC frame down the stack to NIC driver
  send(frame.release(), "out");
  frameSequenceNumber++;

  // Schedule timer to send next MAC frame
  const auto delay = SimTime{par("delay").longValue(), SIMTIME_MS};
  scheduleAt(clockTime() + delay, periodicTxMessage.get());
}

void PingApplication::handleIncommingMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  EV_INFO << "Received frame (" << message->getClassName() << ") \"" << message->getFullName() << "\"" << endl;
}

void PingApplication::handleTxCompletionSignal(const smile::IdealTxCompletion& completion)
{
  EV_INFO << "Transmission of frame (" << completion.getFrame()->getClassName() << ") \""
          << completion.getFrame()->getFullName() << "\" started at " << completion.getOperationBeginClockTimestamp()
          << endl;
}

void PingApplication::handleRxCompletionSignal(const smile::IdealRxCompletion& completion)
{
  EV_INFO << "Reception of frame (" << completion.getFrame()->getClassName() << ") \""
          << completion.getFrame()->getFullName() << "\" started at " << completion.getOperationBeginClockTimestamp()
          << endl;
}

}  // namespace ping_pong
}  // namespace smile_examples
