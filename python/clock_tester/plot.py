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

import matplotlib.pyplot as pyplot


def display_signle(clock_model_log):
    name = clock_model_log['name']
    timestamps = clock_model_log['timestamps']
    pyplot.plot(timestamps['simulation_timestamp'], timestamps['clock_timestamp'])
    pyplot.title('\'{0}\' clock model'.format(name))
    pyplot.xlabel('Simulation timestamps [ps]')
    pyplot.ylabel('Clock timestamps [ps]')
    pyplot.grid(True)
    pyplot.show()


def display_multiple(clock_model_logs):
    for clock_model_log in clock_model_logs:
        name = clock_model_log['name']
        timestamps = clock_model_log['timestamps']
        pyplot.plot(timestamps['simulation_timestamp'], timestamps['clock_timestamp'], label=name)

    pyplot.title('Clock models evaluation')
    pyplot.xlabel('Simulation timestamps [ps]')
    pyplot.ylabel('Clock timestamps [ps]')
    pyplot.legend()
    pyplot.grid(True)
    pyplot.show()
