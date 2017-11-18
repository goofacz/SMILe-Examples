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

#pragma once

#include <IdealApplication.h>

namespace smile_examples {
namespace ping_pong {

class PingApplication : public smile::IdealApplication
{
 public:
  PingApplication() = default;
  PingApplication(const PingApplication& source) = delete;
  PingApplication(PingApplication&& source) = delete;
  ~PingApplication();

  PingApplication& operator=(const PingApplication& source) = delete;
  PingApplication& operator=(PingApplication&& source) = delete;

 private:
  void initialize(int stage) override;

  void handleSelfMessage(cMessage* message) override;

  void handleIncommingMessage(cMessage* newMessage) override;

  void handleTxCompletionSignal(const smile::IdealTxCompletion& completion) override;

  void handleRxCompletionSignal(const smile::IdealRxCompletion& completion) override;

  std::unique_ptr<omnetpp::cMessage> periodicTxMessage;
  unsigned int frameSequenceNumber{0};
};

}  // namespace ping_pong
}  // namespace smile_examples
