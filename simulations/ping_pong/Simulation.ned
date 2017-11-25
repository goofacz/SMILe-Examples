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

package smile_examples.simulations.ping_pong;

import smile.RadioNode;
import smile.Logger;
import smile.IdealClock;
import smile.IdealRangingNicDriver;
import inet.mobility.single.LinearMobility;
import inet.physicallayer.idealradio.IdealRadioMedium;
import inet.visualizer.integrated.IntegratedCanvasVisualizer;

network Simulation
{
    submodules:
        radioMedium: IdealRadioMedium {
            @display("p=193,30");
        }
        
        visualizer: IntegratedCanvasVisualizer {
            @display("p=287,30");
        }
        
        Logger: Logger {
            fileNamePrefix = "ping_pong";
            @display("p=69,30");
        }

        pingNode: RadioNode {
            mobilityType = "LinearMobility";
            applicationType = "PingApplication";
            nicDriverType = "IdealRangingNicDriver";
            nicType = "IdealWirelessNic";
            clockType = "IdealClock";
        }

        pongNode: RadioNode {
            mobilityType = "LinearMobility";
            applicationType = "PongApplication";
            nicDriverType = "IdealRangingNicDriver";
            nicType = "IdealWirelessNic";
            clockType = "IdealClock";
        }
}
