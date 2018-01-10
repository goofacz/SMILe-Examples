//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package smile_examples.simulations.clock_tester;

import smile.Logger;
import smile_examples.clock_tester.Tester;

network Simulation
{
    submodules:
        logger: Logger {
            fileNamePrefix = "clock_tester";
            @display("p=69,30");
        }

        simpleClock_0PPM: Tester {
            clockType = "SimpleClock";
            @display("p=187,140");
        }

        simpleClock_10PPM: Tester {
            clockType = "SimpleClock";
            @display("p=328,140");
        }

        simpleClock_5PPM: Tester {
            clockType = "SimpleClock";
            @display("p=461,140");
        }
}
