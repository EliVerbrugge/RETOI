
import serial, re, struct, math, time
from struct import *
import serial.tools.list_ports
import xml.etree.ElementTree as ET
from PyQt5 import QtCore, QtWidgets, QtGui
from PyQt5.QtWidgets import QFileDialog, QWidget, QLabel, QPushButton, QHBoxLayout, QVBoxLayout, QApplication, QScrollArea, QMainWindow, QProgressBar
from Utilities import grab_xml_info, calc_distance

class Window(QtWidgets.QWidget):
    def __init__(self, parent=None):
        super(Window, self).__init__(parent)
        self.cmpt_button = QtWidgets.QPushButton(
            text="Compute points",
            clicked=self.compute
        )
        self.send_button = QtWidgets.QPushButton(
            text="Send",
            clicked=self.send
        )
        self.browse_button = QtWidgets.QPushButton(
            text="Browse for GPX file",
            clicked=self.openFile
        )
        self.connect_button = QtWidgets.QPushButton(
            text="Connect", 
            clicked=self.connect
            
        )
        self.fileName = ""
        lay = QtWidgets.QVBoxLayout(self)
        lay.addWidget(self.browse_button)
        hlay = QtWidgets.QHBoxLayout()
        self.itemsList = ItemsList()
        hlay.addWidget(self.itemsList)
        vlay = QtWidgets.QVBoxLayout(self)
        vlay.addWidget(self.cmpt_button)

        #display all of the available ports with a descriptor so the user can select the device to communicate with
        self.ser = serial.Serial()
        self.ports = serial.tools.list_ports.comports()
        self.comboBox = QtWidgets.QComboBox(self)
        for comport in self.ports:
            self.comboBox.addItem(comport.description)
        vlay.addWidget(self.comboBox)
        vlay.addWidget(self.connect_button)
        vlay.addWidget(self.send_button)

        hlay.addLayout(vlay)
        lay.addLayout(hlay)

    def connect(self):
        self.ser.baudrate = 115200
        self.ser.port = self.ports[self.comboBox.currentIndex()].device
        self.ser.open()
        time.sleep(0.1)
        self.ser.flushInput()
        self.ser.flushOutput()

    def openFile(self):
        options = QFileDialog.Options()
        options |= QFileDialog.DontUseNativeDialog
        self.fileName, _ = QFileDialog.getOpenFileName(self,"Select a GPX file with the desired route", "","All Files (*)", options=options)

    def buildExamplePopup(self):
            name = "boo"
            self.Popup = PopupWindow(name)
            self.Popup.setGeometry(100, 200, 100, 100)
            self.Popup.show()

    def send(self):
        self.ser.write(pack('c',b'S'))
        self.ser.write(pack('h',len(self.data_points)))
        print(len(self.data_points))
        print(pack('h',len(self.data_points)))
        print(self.ser.read())
        print(self.ser.read())
        print(self.ser.read())
        print(self.ser.read())
        print(self.ser.read())

        for i in range(len(self.data_points)):
            dist = pack('h',int(round(self.data_points[i][0],2)*1000))
            self.ser.write(dist)
            #print(int(round(self.data_points[i][0],2)*1000))
            #print(self.ser.read())
            #print(self.ser.read())
            incline = pack('h',int(round(self.data_points[i][1],2)*1000))
            self.ser.write(incline)
            #print(int(round(self.data_points[i][1],2)*1000))
            #print(self.ser.read())
            #print(self.ser.read())
        print(self.ser.read())
        print(self.ser.read())
        return
        
    def compute(self):
        #grab the array of points containing lat/lon and elevation
        points = grab_xml_info(self.fileName)
        #now ready to start computation
        first_point = True
        self.data_points = []
        for leg in points:
            lat = float(leg.get('lat'))
            lon = float(leg.get('lon'))
            elev = float(leg.find('ele').text)
            if first_point:
                first_point = False
            else:
                distance = calc_distance(prev_point, (lat, lon))
                incline = ((elev-prev_elev)/(distance*1000))*100
                self.data_points.append((distance,incline))
            prev_point = (lat, lon)
            prev_elev = elev
        self.buildExamplePopup()
        for i in range(len(self.data_points)):
            self.itemsList.addItem(QLabel("Distance: " + str(round(self.data_points[i][0],2)) + "  Incline: " + str(round(self.data_points[i][1],2))))

class ItemsList(QWidget):
    def __init__(self, parent=None):
        super(ItemsList, self).__init__(parent)
        self.initWidget()

    def initWidget(self):
        listBox = QVBoxLayout(self)
        self.setLayout(listBox)

        scroll = QScrollArea(self)
        listBox.addWidget(scroll)
        scroll.setWidgetResizable(True)
        scrollContent = QWidget(scroll)

        self.scrollLayout = QVBoxLayout(scrollContent)
        scrollContent.setLayout(self.scrollLayout)
        scroll.setWidget(scrollContent)

    def addItem(self, item):
        self.scrollLayout.addWidget(item)

class PopupWindow(QWidget):
    def __init__(self, name):
        super().__init__()

        self.name = name

        self.initUI()

    def initUI(self):
        lblName = QLabel(self.name, self)
        self.progress = QProgressBar(self)
        self.progress.setGeometry(100, 200, 25, 40)
        self.show()
        
if __name__ == '__main__':
    import sys
    app = QtWidgets.QApplication(sys.argv)
    win = Window()
    w = 600; h = 300
    win.resize(w, h)
    win.show()
    sys.exit(app.exec_())