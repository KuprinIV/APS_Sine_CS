# APS_Sine_CS
PC program for control and calibration of sine wave current source device. Control interface is USB with WinUSB driver
# Directory contents
  - /SineCS_Control/main.cpp                    Main program file
  - /SineCS_Control/mainwindow.h                Main window class header file
  - /SineCS_Control/mainwindow.cpp              Main window class source file. Realize current source control logic
  - /SineCS_Control/mainwindow.ui               Qt UI form file for main window
  - /SineCS_Control/calibrationdialog.h         Current source calibration dialog class header file
  - /SineCS_Control/calibrationdialog.cpp       Current source calibration dialog class source file. Realize current source calibration
  - /SineCS_Control/calibrationdialog.ui        Qt UI form file for calibration dialog
  - /SineCS_Control/cs_control.h                Current source USB control interface class header file
  - /SineCS_Control/cs_control.c                Current source USB control interface class source file. Realize sending commands via USB control endpoint
