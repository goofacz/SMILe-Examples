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

#include "PongApplication.h"

namespace smile_examples {

Define_Module(PongApplication);

void PongApplication::handleIncommingMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  EV_INFO << "Received frame (" << message->getClassName() << ") \"" << message->getFullName() << "\"" << endl;

  // Compose reply MAC frame
  const inet::IdealMacFrame* sourceMacFrame{check_and_cast<inet::IdealMacFrame*>(message.get())};
  const auto& sourceAddress = sourceMacFrame->getSrc();

  const auto name = std::string{"Pong reply!"};
  auto replyMacFrame = createFrame<inet::IdealMacFrame>(sourceAddress, name.c_str());
  replyMacFrame->setBitLength(10);

  EV_INFO << "Sending frame (" << replyMacFrame->getClassName() << ")\"" << replyMacFrame->getFullName() << "\""
          << endl;

  // Pass MAC frame down the stack to NIC driver
  send(replyMacFrame.release(), "out");
}

void PongApplication::handleTxCompletionSignal(const smile::IdealTxCompletion& completion)
{
  EV_INFO << "Transmission of frame (" << completion.getFrame()->getClassName() << ") \""
          << completion.getFrame()->getFullName() << "\" started at " << completion.getOperationBeginClockTimestamp()
          << endl;
}

void PongApplication::handleRxCompletionSignal(const smile::IdealRxCompletion& completion)
{
  EV_INFO << "Reception of frame (" << completion.getFrame()->getClassName() << ") \""
          << completion.getFrame()->getFullName() << "\" started at " << completion.getOperationBeginClockTimestamp()
          << endl;
}

}  // namespace smile_examples
