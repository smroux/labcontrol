"""
A simple application that communicates with an Arduino and processes the data.

Should be run using Python 3.x

Created by SM Roux
"""

import serial
from PyQt4 import QtGui, QtCore
import pyqtgraph as pg
from time import sleep, time

class DataLogger(QtGui.QWidget):
    def __init__(self, variables=['time', 'y1', 'y2', 'y3']):
        """
        Construction of the GUI interface.
        """
        QtGui.QWidget.__init__(self)
        self.running = False
        self.data = {}
        self.variables = variables
        for item in variables:
            self.data[item] = []
        self.max_size = 10
        self.scales = []
        self.zeros = []

        ## Create widgets
        self.btn_start = QtGui.QPushButton('Start')
        self.btn_set_cal = QtGui.QPushButton('Set Cal Data')
        self.txt_scales = QtGui.QListWidget()
        self.txt_scales.addItems(['0.001', '1', '1', '1'])
        self.txt_zeros = QtGui.QListWidget()
        self.txt_zeros.addItems(['0', '0', '0', '0'])
        self.txt_output = QtGui.QListWidget()
        self.plot = pg.PlotWidget()
        self.lbl_output = QtGui.QLabel()

        ## Signals and slots
        self.set_cal()
        self.btn_set_cal.clicked.connect(self.set_cal)
        self.btn_start.clicked.connect(self.start)
        self.timer = QtCore.QTimer()
        self.timer.start(1000)
        self.timer.timeout.connect(self.loop)

        ## Create grid
        self.layout = QtGui.QGridLayout()
        self.setLayout(self.layout)

        ## Add widgets to the layout
        # Layout:
        # btn_start    txt_scale
        # txt_output   txt_zero
        # plot
        # output
        # First row
        self.layout.addWidget(self.btn_start, 0, 0)
        self.layout.addWidget(self.txt_scales, 0, 1)
        # Second row
        self.layout.addWidget(self.txt_output, 1, 0)
        self.layout.addWidget(self.txt_zeros, 1, 1)
        # Third row
        self.layout.addWidget(self.btn_set_cal, 2, 1)
        # Fourth row
        self.layout.addWidget(self.plot, 3, 0, 1, 2)
        # Fifth row
        self.layout.addWidget(self.lbl_output, 4, 0, 1, 2)

        ## Open serial port
        self.ard_serial = serial.Serial('/dev/ttyUSB0', 9600)
        sleep(2)

    def start(self):
        """
        Alternates between starting and stopping data transfer with the Arduino.
        """
        if self.btn_start.text() == 'Start':
            self.running = True
            self.btn_start.setText('Stop')
        else:
            self.running = False
            self.btn_start.setText('Start')

    def loop(self):
        """
        Standard process that loops continually in the background.
        """
        if self.running:
            ard_data = self.get_data()
            data = self.apply_calibration(ard_data)
            for i in range(len(data)):
                self.data[self.variables[i]].append(data[i])
            if len(self.data[self.variables[0]]) > self.max_size:
                for var in self.variables:
                    del self.data[var][0]
            for i in range(1, len(self.variables)):
                if i == 1:
                    self.plot.plot(self.data[self.variables[0]],
                                self.data[self.variables[i]],
                                clear=True)
                else:
                    self.plot.plot(self.data[self.variables[0]],
                                self.data[self.variables[i]])

    def get_data(self):
        """
        Get data from Arduino.
        """
        self.ard_serial.write(b'g')
        data_retrieved = False
        data = ""
        while not data_retrieved:
            data += self.ard_serial.readline().decode("ascii")
            if data[-1] == '\n':
                data_retrieved = True
            sleep(0.1)
        data = data.split(';')
        data = [float(value) for value in data]
        return data

    def set_cal(self):
        """
        Set calibration values to those inputted in the scales and zeros
        textboxes.
        """
        self.scales = []
        for i in range(self.txt_scales.count()):
            self.scales.append(float(self.txt_scales.item(i).text()))
        self.zeros = []
        for i in range(self.txt_zeros.count()):
            self.zeros.append(float(self.txt_zeros.item(i).text()))

    def apply_calibration(self, data):
        """
        Apply calibration properties to measured data.
        """
        for i in range(len(data)):
            data[i] = self.scales[i] * (data[i] - self.zeros[i])
        return data

if __name__ == "__main__":
    app = QtGui.QApplication([])
    widget_master = DataLogger()
    widget_master.show()
    app.exec_()
