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

#include <algorithm>

#include "MobileApplication.h"
#include "PollFrame_m.h"

namespace smile_examples {
namespace ds_twr {

Define_Module(MobileApplication);

const std::string MobileApplication::pollFrameName{"POLL"};

MobileApplication::~MobileApplication()
{
  if (rxTimeoutTimerMessage) {
    cancelEvent(rxTimeoutTimerMessage.get());
  }
}

void MobileApplication::initialize(int stage)
{
  IdealApplication::initialize(stage);
  if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
    rxTimeoutTimerMessage = std::make_unique<cMessage>("Ranging RX timeout");
    anchorAddresses.emplace_back("DE-AD-BE-EF-10-01");
    anchorAddresses.emplace_back("DE-AD-BE-EF-10-02");
    anchorAddresses.emplace_back("DE-AD-BE-EF-10-03");

    startRanging();
  }
}

void MobileApplication::handleSelfMessage(cMessage* message)
{
  if (message == rxTimeoutTimerMessage.get()) {
    // Start ranging with next anchor
    std::rotate(anchorAddresses.begin(), std::next(anchorAddresses.begin()), anchorAddresses.end());
    startRanging();
  }
}

void MobileApplication::handleIncommingMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  // TODO
}

void MobileApplication::handleTxCompletionSignal(const smile::IdealTxCompletion& completion)
{
  auto formatTimestamp = [](const auto& timestamp) { return timestamp.format(SIMTIME_FS, ".", "", true); };

  const auto& frame = completion.getFrame();
  if (pollFrameName == frame->getName()) {
    pollTxBeginTimestamp = completion.getOperationBeginClockTimestamp();
    EV_INFO << "POLL transmission started at " << formatTimestamp(pollTxBeginTimestamp) << " and finished at "
            << formatTimestamp(clockTime()) << endl;
  }
  else {
    throw cRuntimeError{"Received TX completion signal for unexpected packet of type %s and name \"%s\"",
                        frame->getClassName(), frame->getName()};
  }
}

void MobileApplication::handleRxCompletionSignal(const smile::IdealRxCompletion& completion)
{
  // TODO
}

void MobileApplication::startRanging()
{
  const auto& anchorAddress = anchorAddresses.front();
  auto frame = createFrame<PollFrame>(anchorAddress, pollFrameName.c_str());
  frame->setBitLength(10);

  sendDelayed(frame.release(), 0, "out");

  const auto rxTimeout = clockTime() + SimTime{par("rangingRxTimeout").longValue(), SIMTIME_MS};
  scheduleAt(rxTimeout, rxTimeoutTimerMessage.get());
}

}  // namespace ds_twr
}  // namespace smile_examples
