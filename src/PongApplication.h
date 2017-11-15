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

class PongApplication : public smile::IdealApplication
{
 public:
  PongApplication() = default;
  PongApplication(const PongApplication& source) = delete;
  PongApplication(PongApplication&& source) = delete;
  ~PongApplication() = default;

  PongApplication& operator=(const PongApplication& source) = delete;
  PongApplication& operator=(PongApplication&& source) = delete;

 private:
  void handleIncommingMessage(cMessage* newMessage) override;

  void handleTxCompletionSignal(const smile::IdealTxCompletion& completion) override;

  void handleRxCompletionSignal(const smile::IdealRxCompletion& completion) override;
};

}  // namespace smile_examples
