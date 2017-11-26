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

#include <utilities.h>
#include <algorithm>

#include "CsvLogger.h"
#include "MobileApplication.h"
#include "PollFrame_m.h"

namespace smile_examples {
namespace ss_twr {

Define_Module(MobileApplication);

const std::string MobileApplication::pollFrameName{"POLL"};
const std::string MobileApplication::responseFrameName{"RESPONSE"};

static auto formatTimestamp = [](const auto& timestamp) { return timestamp.format(SIMTIME_FS, ".", "", true); };

MobileApplication::~MobileApplication()
{
  if (rxTimeoutTimerMessage) {
    cancelEvent(rxTimeoutTimerMessage.get());
  }
}

void MobileApplication::initialize(int stage)
{
  IdealApplication::initialize(stage);
  if (stage == inet::INITSTAGE_PHYSICAL_ENVIRONMENT_2) {
    auto& logger = getLogger();
    framesLog = logger.obtainHandle("frames");

    const auto& nicDriver = getNicDriver();
    auto anchorsLog = logger.obtainHandle("mobile_nodes");
    const auto entry = smile::csv_logger::compose(nicDriver.getMacAddress(), getCurrentTruePosition());
    logger.append(anchorsLog, entry);
  }
  else if (stage == inet::INITSTAGE_APPLICATION_LAYER) {
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
  handleResponseFrame(smile::dynamic_unique_ptr_cast<ResponseFrame>(std::move(message)));
}

void MobileApplication::handleTxCompletionSignal(const smile::IdealTxCompletion& completion)
{
  const auto& frame = completion.getFrame();
  if (pollFrameName == frame->getName()) {
    pollTxBeginTimestamp = completion.getOperationBeginClockTimestamp();
    EV_INFO << "POLL transmission started at " << formatTimestamp(pollTxBeginTimestamp) << " and finished at "
            << formatTimestamp(clockTime()) << endl;

    const auto frame = dynamic_cast<const PollFrame*>(completion.getFrame());
    if (!frame) {
      throw cRuntimeError{"Received signal for %s message, expected PollFrame", frame->getClassName()};
    }

    auto& logger = getLogger();
    const auto entry =
        smile::csv_logger::compose(completion, frame->getSrc(), frame->getDest(), frame->getSequenceNumber());
    logger.append(framesLog, entry);
  }
  else {
    throw cRuntimeError{"Received TX completion signal for unexpected packet of type %s and name \"%s\"",
                        frame->getClassName(), frame->getName()};
  }
}

void MobileApplication::handleRxCompletionSignal(const smile::IdealRxCompletion& completion)
{
  const auto& frame = completion.getFrame();
  if (responseFrameName == frame->getName()) {
    responseRxBeginTimestamp = completion.getOperationBeginClockTimestamp();
    EV_INFO << "RESPONSE reception started at " << formatTimestamp(pollTxBeginTimestamp) << " and finished at "
            << formatTimestamp(clockTime()) << endl;

    const auto frame = dynamic_cast<const ResponseFrame*>(completion.getFrame());
    if (!frame) {
      throw cRuntimeError{"Received signal for %s message, expected ResponseFrame", frame->getClassName()};
    }

    auto& logger = getLogger();
    const auto entry =
        smile::csv_logger::compose(completion, frame->getSrc(), frame->getDest(), frame->getSequenceNumber());
    logger.append(framesLog, entry);
  }
  else {
    throw cRuntimeError{"Received RX completion signal for unexpected packet of type %s and name \"%s\"",
                        frame->getClassName(), frame->getName()};
  }
}

void MobileApplication::startRanging()
{
  sequenceNumber++;

  const auto& anchorAddress = anchorAddresses.front();
  auto frame = createFrame<PollFrame>(anchorAddress, pollFrameName.c_str());
  frame->setBitLength(10);
  frame->setSequenceNumber(sequenceNumber);

  sendDelayed(frame.release(), 0, "out");

  const auto rxTimeout = clockTime() + SimTime{par("rangingRxTimeout").longValue(), SIMTIME_MS};
  scheduleAt(rxTimeout, rxTimeoutTimerMessage.get());

  pollTxBeginTimestamp = SimTime::ZERO;
  responseRxBeginTimestamp = SimTime::ZERO;
}

void MobileApplication::handleResponseFrame(std::unique_ptr<ResponseFrame> frame)
{
  const auto processingTime = SimTime{par("messageProcessingTime").longValue(), SIMTIME_MS};
  const auto tof = ((responseRxBeginTimestamp - pollTxBeginTimestamp) - processingTime) / 2;
  EV_INFO << "ToF " << formatTimestamp(tof) << endl;
}

}  // namespace ss_twr
}  // namespace smile_examples
