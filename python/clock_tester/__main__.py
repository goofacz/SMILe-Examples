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

import os.path
import numpy
import argparse
import glob
import matplotlib.pyplot as pyplot
import re


def load_log_file(file_path):
    columns = numpy.dtype({'names': ('simulation_timestamp', 'clock_timestamp'), 'formats': (int, int)})
    return numpy.loadtxt(file_path, dtype=columns, delimiter=',', ndmin=1)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Process Clock Tester data..')
    parser.add_argument('logs_directory_path', type=str, nargs=1, help='Path to directory holding CSV logs')
    arguments = parser.parse_args()

    logs_directory_path = os.path.join(*arguments.logs_directory_path, 'clock_tester_*csv')
    log_file_paths = glob.glob(logs_directory_path)

    for log_file_path in log_file_paths:
        pattern = re.compile('(?:.*_)(\w+_[\d\w]+)(?:.*)')
        elements = pattern.match(log_file_path)
        timestamps = load_log_file(log_file_path)
        pyplot.plot(timestamps['simulation_timestamp'], timestamps['clock_timestamp'], label=elements[1])

    pyplot.title('Clock models evaluation')
    pyplot.xlabel('Simulation timestamps [ns]')
    pyplot.ylabel('Clock timestamps [ns]')
    pyplot.legend()
    pyplot.grid(True)
    pyplot.show()
