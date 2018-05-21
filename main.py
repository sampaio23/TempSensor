# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'qt.ui'
#
# Created by: PyQt4 UI code generator 4.11.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

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
        self.tempDisplay = QtGui.QLCDNumber(Form)
        self.tempDisplay.setGeometry(QtCore.QRect(80, 110, 64, 23))
        self.tempDisplay.setAutoFillBackground(False)
        self.tempDisplay.setNumDigits(2)
        self.tempDisplay.setSegmentStyle(QtGui.QLCDNumber.Flat)
        self.tempDisplay.setProperty("intValue", 25)
        self.tempDisplay.setObjectName(_fromUtf8("tempDisplay"))
        self.humDisplay = QtGui.QLCDNumber(Form)
        self.humDisplay.setGeometry(QtCore.QRect(260, 110, 64, 23))
        self.humDisplay.setNumDigits(2)
        self.humDisplay.setSegmentStyle(QtGui.QLCDNumber.Flat)
        self.humDisplay.setProperty("intValue", 50)
        self.humDisplay.setObjectName(_fromUtf8("humDisplay"))
        self.label = QtGui.QLabel(Form)
        self.label.setGeometry(QtCore.QRect(50, 20, 311, 21))
        font = QtGui.QFont()
        font.setPointSize(18)
        font.setBold(True)
        font.setWeight(75)
        self.label.setFont(font)
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(Form)
        self.label_2.setGeometry(QtCore.QRect(70, 90, 91, 16))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.label_3 = QtGui.QLabel(Form)
        self.label_3.setGeometry(QtCore.QRect(260, 90, 91, 16))
        self.label_3.setObjectName(_fromUtf8("label_3"))
        self.kseparator = KSeparator(Form)
        self.kseparator.setGeometry(QtCore.QRect(-10, 150, 431, 20))
        self.kseparator.setLineWidth(2)
        self.kseparator.setObjectName(_fromUtf8("kseparator"))
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
        self.radioButton = QtGui.QRadioButton(Form)
        self.radioButton.setGeometry(QtCore.QRect(30, 210, 141, 20))
        self.radioButton.setObjectName(_fromUtf8("radioButton"))
        self.radioButton_2 = QtGui.QRadioButton(Form)
        self.radioButton_2.setGeometry(QtCore.QRect(30, 240, 131, 20))
        self.radioButton_2.setObjectName(_fromUtf8("radioButton_2"))
        self.pushButton = QtGui.QPushButton(Form)
        self.pushButton.setGeometry(QtCore.QRect(265, 210, 85, 27))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.label_6 = QtGui.QLabel(Form)
        self.label_6.setGeometry(QtCore.QRect(270, 270, 50, 12))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.comboBox = QtGui.QComboBox(Form)
        self.comboBox.setGeometry(QtCore.QRect(310, 265, 71, 24))
        self.comboBox.setEditable(True)
        self.comboBox.setObjectName(_fromUtf8("comboBox"))

        self.retranslateUi(Form)
        QtCore.QObject.connect(self.radioButton, QtCore.SIGNAL(_fromUtf8("pressed()")), self.tempDisplay.setDecMode)
        QtCore.QObject.connect(self.radioButton_2, QtCore.SIGNAL(_fromUtf8("released()")), self.humDisplay.update)
        QtCore.QObject.connect(self.pushButton, QtCore.SIGNAL(_fromUtf8("clicked()")), self.label_5.show)
        QtCore.QObject.connect(self.comboBox, QtCore.SIGNAL(_fromUtf8("currentIndexChanged(int)")), self.kseparator.show)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Temperature and Humidity Monitor", None))
        self.label.setText(_translate("Form", "Temperature and Humidity Monitor", None))
        self.label_2.setText(_translate("Form", "Temperature (ºC)", None))
        self.label_3.setText(_translate("Form", "Humidity (%)", None))
        self.label_4.setText(_translate("Form", "Command", None))
        self.label_5.setText(_translate("Form", " Get Data ", None))
        self.radioButton.setText(_translate("Form", "Air Conditioner ON", None))
        self.radioButton_2.setText(_translate("Form", "Humidifier ON", None))
        self.pushButton.setText(_translate("Form", "Start", None))
        self.label_6.setText(_translate("Form", "Port", None))

from kseparator import KSeparator

if __name__ == "__main__":
    import sys
    app = QtGui.QApplication(sys.argv)
    Form = QtGui.QWidget()
    ui = Ui_Form()
    ui.setupUi(Form)
    Form.show()
    sys.exit(app.exec_())

