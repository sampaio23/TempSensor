# -*- coding: utf-8 -*-

from PyQt4 import QtCore, QtGui
import threading
import serial
import time

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(404, 300)

        self.tempDisplay = QtGui.QLabel(Form)
        self.tempDisplay.setGeometry(QtCore.QRect(100, 110, 64, 23))
        self.tempDisplay.setObjectName(_fromUtf8("tempDisplay"))
        font = QtGui.QFont("Times", 26)
        self.tempDisplay.setFont(font)

        self.humDisplay = QtGui.QLabel(Form)
        self.humDisplay.setGeometry(QtCore.QRect(280, 110, 64, 23))
        self.humDisplay.setObjectName(_fromUtf8("humDisplay"))
        font =  QtGui.QFont("Times", 26)
        self.humDisplay.setFont(font)
        
        font = QtGui.QFont()
        font.setPointSize(18)
        font.setBold(True)
        font.setWeight(75)
        self.label = QtGui.QLabel(Form)
        self.label.setGeometry(QtCore.QRect(50, 20, 311, 21))
        self.label.setFont(font)
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(Form)
        self.label_2.setGeometry(QtCore.QRect(70, 90, 91, 16))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.label_3 = QtGui.QLabel(Form)
        self.label_3.setGeometry(QtCore.QRect(260, 90, 91, 16))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.label_4 = QtGui.QLabel(Form)
        self.label_4.setGeometry(QtCore.QRect(50, 180, 61, 16))
        font = QtGui.QFont()
        font.setBold(True)
        font.setUnderline(True)
        font.setWeight(75)
        self.label_4.setFont(font)
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.label_5 = QtGui.QLabel(Form)
        self.label_5.setGeometry(QtCore.QRect(280, 180, 61, 16))
        font = QtGui.QFont()
        font.setBold(True)
        font.setUnderline(True)
        font.setWeight(75)
        font.setStrikeOut(False)
        self.label_5.setFont(font)
        self.label_5.setObjectName(_fromUtf8("label_5"))

        self.pushButton = QtGui.QPushButton(Form)
        self.pushButton.setGeometry(QtCore.QRect(265, 210, 85, 27))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        
        self.label_6 = QtGui.QLabel(Form)
        self.label_6.setGeometry(QtCore.QRect(270, 270, 50, 12))
        self.label_6.setObjectName(_fromUtf8("label_6"))

        self.portChoose = QtGui.QComboBox(Form)
        self.portChoose.setGeometry(QtCore.QRect(310, 265, 71, 24))
        self.portChoose.setEditable(False)
        self.portChoose.setObjectName(_fromUtf8("portChoose"))
        self.portChoose.addItem("")
        self.portChoose.addItem("ttyACM0")
        self.portChoose.addItem("ttyACM1")
        self.portChoose.addItem("ttyACM2")
        self.portChoose.addItem("ttyACM3")
        self.portChoose.addItem("ttyACM4")
        self.portChoose.addItem("tty0")
        self.portChoose.addItem("tty1")
        self.portChoose.addItem("tty2")
        self.portChoose.addItem("tty3")
        self.portChoose.addItem("tty4")

        self.line = QtGui.QFrame(Form)
        self.line.setGeometry(QtCore.QRect(-70, 150, 581, 20))
        self.line.setFrameShape(QtGui.QFrame.HLine)
        self.line.setFrameShadow(QtGui.QFrame.Sunken)
        self.line.setObjectName(_fromUtf8("line"))

        self.tempBox = QtGui.QCheckBox(Form)
        self.tempBox.setGeometry(QtCore.QRect(40, 210, 121, 20))
        self.tempBox.setObjectName(_fromUtf8("tempBox"))
        
        self.humBox = QtGui.QCheckBox(Form)
        self.humBox.setGeometry(QtCore.QRect(40, 250, 111, 20))
        self.humBox.setObjectName(_fromUtf8("humBox"))

        self.retranslateUi(Form)
        QtCore.QObject.connect(self.portChoose, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), self.definePort)
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.toggleDataCapture)
        QtCore.QObject.connect(self.tempBox, QtCore.SIGNAL(_fromUtf8("pressed()")), self.toggleAir)
        QtCore.QObject.connect(self.humBox, QtCore.SIGNAL(_fromUtf8("pressed()")), self.toggleHum)

        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Temperature and Humidity Monitor", None))
        self.tempDisplay.setText(_translate("Form","25", None))
        self.humDisplay.setText(_translate("Form","25", None))
        self.label.setText(_translate("Form", "Temperature and Humidity Monitor", None))
        self.label_2.setText(_translate("Form", "Temperature (ºC)", None))
        self.label_3.setText(_translate("Form", "Humidity (%)", None))
        self.label_4.setText(_translate("Form", "Command", None))
        self.label_5.setText(_translate("Form", " Get Data ", None))
        self.pushButton.setText(_translate("Form", "Start", None))
        self.label_6.setText(_translate("Form", "Port", None))
        self.tempBox.setText(_translate("Form", "Air Conditioner ON", None))
        self.humBox.setText(_translate("Form", "Humidifier ON", None))

    # Changes value on displays
    
    input = 1
    ser = 1

    def getData(self):
    	self.input = "mult 2 3"
        self.ser.write(self.input + '\r\n')
        out = ''
        time.sleep(1)
        while self.ser.inWaiting() > 0:
            out += self.ser.read(1)
        
        if out != '':
            print out.split() # check how arguments will come to python to select

        #self.tempDisplay.setText(_translate("Form",out,None))
        #self.humDisplay.setText(_translate("Form",out, None))

    	if self.clicked%2 == 1:
    		threading.Timer(1.0, self.getData).start() # adjust time to match sensors
    	else:
    		threading.Timer(1.0, self.getData).cancel()

    # Function that chooses the port

    def definePort(self):
        ports = ["","ttyACM0","ttyACM1","ttyACM2","ttyACM3","ttyACM4","tty0","tty1","tty2","tty3","tty4"]
        portChosen =  ports[self.portChoose.currentIndex()]
        print portChosen
        self.ser = serial.Serial(
            port = '/dev/'+portChosen,
            baudrate = 9600,
            parity = serial.PARITY_ODD,
            stopbits = serial.STOPBITS_TWO,
            bytesize = serial.SEVENBITS
        )

        print self.ser.isOpen() # returns True if serial is open
        
        
    # Function that saves data from sensor

    clicked = 0

    def toggleDataCapture(self):
        if self.clicked%2 == 0:
            self.pushButton.setText(_translate("Form", "Stop", None))
            # Start data capture
            print "Começando captura de dados"
            self.clicked+=1
            self.getData()
        else:
            self.pushButton.setText(_translate("Form", "Start", None))
            # Stop data capture
            print "Finalizando captura de dados"
            self.clicked=0

    # Function that turns the air conditioner on/off

    def toggleAir(self):
        if self.tempBox.isChecked():
            # Turn Off
            print "Desligando Ar"
            self.input = "ligarAr 0"
            self.ser.write(self.input + '\r\n')
            out = ''
            while self.ser.inWaiting() > 0:
                out += self.ser.read(1)

        else:
            # Turn On
            print "Ligando Ar"
            self.input = "ligarAr 1"
            self.ser.write(self.input + '\r\n')
            out = ''
            while self.ser.inWaiting() > 0:
                out += self.ser.read(1)

    # Function that turns the humidifier on/off

    def toggleHum(self):
        if self.humBox.isChecked():
            # Turn off
            print "Desligando Umidificador"
            self.input = "ligarUmid 0"
            self.ser.write(self.input + '\r\n')
            out = ''
            while self.ser.inWaiting() > 0:
                out += self.ser.read(1)
        else:
            # Turn on
            print "Ligando Umidificador"
            self.input = "ligarUmid 1"
            self.ser.write(self.input + '\r\n')
            out = ''
            while self.ser.inWaiting() > 0:
                out += self.ser.read(1)

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Form = QtGui.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())

