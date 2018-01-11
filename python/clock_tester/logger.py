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

import re
import glob
import os.path
import numpy


def load_file(file_path):
    pattern = re.compile('(?:.*_)(\w+_[\d\w]+)(?:.*)')
    match = pattern.match(file_path)
    if match is None:
        raise RuntimeError('Failed to extract clock name from file name: {0}'.format(file_path))

    columns = numpy.dtype({'names': ('simulation_timestamp', 'clock_timestamp'), 'formats': (int, int)})
    return {'name' : match[1], 'timestamps' : numpy.loadtxt(file_path, dtype=columns, delimiter=',', ndmin=1)}


def load_directory(directory_path):
    directory_path = os.path.join(directory_path, 'clock_tester_*csv')
    file_paths = glob.glob(directory_path)
    return tuple((load_file(file_path)) for file_path in file_paths)