#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see http:#www.gnu.org/licenses/.
#

import argparse
import glob
import re
import clock_tester.logger as logger
import clock_tester.plot as plot

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process Clock Tester data..')
    parser.add_argument('logs_directory_path', type=str, nargs=1, help='Path to directory holding CSV logs')
    arguments = parser.parse_args()

    clock_model_logs = logger.load_directory(*arguments.logs_directory_path)
    plot.display_multiple(clock_model_logs)