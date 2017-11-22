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

#include "AnchorApplication.h"
#include "ResponseFrame_m.h"

#include <utilities.h>

namespace smile_examples {
namespace ss_twr {

Define_Module(AnchorApplication);

const std::string AnchorApplication::pollFrameName{"POLL"};
const std::string AnchorApplication::responseFrameName{"RESPONSE"};

static auto formatTimestamp = [](const auto& timestamp) { return timestamp.format(SIMTIME_FS, ".", "", true); };

void AnchorApplication::handleIncommingMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  handlePollFrame(smile::dynamic_unique_ptr_cast<PollFrame>(std::move(message)));
}

void AnchorApplication::handleRxCompletionSignal(const smile::IdealRxCompletion& completion)
{
  const auto& frame = completion.getFrame();
  if (pollFrameName == frame->getName()) {
    pollRxBeginTimestamp = completion.getOperationBeginClockTimestamp();
    EV_INFO << "POLL reception started at " << formatTimestamp(pollRxBeginTimestamp) << " and finished at "
            << formatTimestamp(clockTime()) << endl;
  }
  else {
    throw cRuntimeError{"Received RX completion signal for unexpected packet of type %s and name \"%s\"",
                        frame->getClassName(), frame->getName()};
  }
}

void AnchorApplication::handlePollFrame(std::unique_ptr<PollFrame> pollFrame)
{
  const auto& mobileAddress = pollFrame->getSrc();
  auto responseFrame = createFrame<ResponseFrame>(mobileAddress, responseFrameName.c_str());
  responseFrame->setBitLength(10);

  const auto processingTime = SimTime{par("messageProcessingTime").longValue(), SIMTIME_MS};
  sendDelayed(responseFrame.release(), processingTime, "out");
}

}  // namespace ss_twr
}  // namespace smile_examples
