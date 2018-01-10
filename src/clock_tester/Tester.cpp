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

#include <inet/common/ModuleAccess.h>
#include <CsvLogger.h>
#include "Tester.h"

namespace smile_examples {
namespace clock_tester {

Define_Module(Tester);

void Tester::initialize(int stage)
{
  ClockDecorator<omnetpp::cSimpleModule>::initialize(stage);
  if (stage == inet::INITSTAGE_LOCAL) {
    logger = inet::getModuleFromPar<smile::Logger>(par("loggerModule"), this, true);
    logHandle = logger->obtainHandle(par("logPrefix").stdstringValue());

    auto message = std::make_unique<cMessage>("timer");
    const auto timestamp = clockTime() + SimTime(1, omnetpp::SIMTIME_NS);
    scheduleAt(timestamp, message.release());
  }
}

void Tester::handleSelfMessage(cMessage* newMessage)
{
  std::unique_ptr<cMessage> message{newMessage};
  const auto timestamp = clockTime() + SimTime(1, omnetpp::SIMTIME_NS);
  scheduleAt(timestamp, message.release());

  const auto entry = smile::csv_logger::compose(simTime(), clockTime());
  logger->append(logHandle,  entry);
}

}  // namespace clock_tester
}  // namespace smile_examples
