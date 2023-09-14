# APS_Sine_CS
PC program for control and calibration of sine wave current source device. Control interface is USB with WinUSB driver. Developed on Qt platform
# Directory contents
  - /SineCS_Control/SineCS_Control.pro          Qt project configuration file
  - /SineCS_Control/main.cpp                    Main program file
  - /SineCS_Control/mainwindow.h                Main window class header file
  - /SineCS_Control/mainwindow.cpp              Main window class source file. Realize current source control logic
  - /SineCS_Control/mainwindow.ui               Qt UI form file for main window
  - /SineCS_Control/calibrationdialog.h         Current source calibration dialog class header file
  - /SineCS_Control/calibrationdialog.cpp       Current source calibration dialog class source file. Realize current source calibration
  - /SineCS_Control/calibrationdialog.ui        Qt UI form file for calibration dialog
  - /SineCS_Control/cs_control.h                Current source USB control interface class header file
  - /SineCS_Control/cs_control.c                Current source USB control interface class source file. Realize sending commands via USB control endpoint
  - /SineCS_Control/SineCS_Control_ru_RU.ts     Russian translation of string constants
  - /SineCS_Control/Libs                        Libusb header file and dll
