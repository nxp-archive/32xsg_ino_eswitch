// Copyright 2017 NXP
#include "eSwitch.h"

boolean WD = 0;
byte address = 0;
unsigned int cmd;
unsigned int data = 0;

void SendCmd() {
  WD = !WD;
  data = 0;
  cmd = (cmd & 0xF7FF) | (WD << 11);
  digitalWrite(SPI_CSB, LOW); // Set CSB to Low state
  data = SPI.transfer16(cmd);
  digitalWrite(SPI_CSB, HIGH); // Set CSB to High state
}

// Timer1 interrupt handler
void InterruptTimer1() { SendCmd(); }

/////////////////////////////////////////////
void Init(void) {
  pinMode(SPI_CSB, OUTPUT);
  pinMode(RSTB, OUTPUT);
  pinMode(CSNS_Sync, INPUT);
  setPwmFrequency(CLK, 1);
  pinMode(CLK, OUTPUT);
  SPI.begin();
  Serial.begin(9600);
  digitalWrite(RSTB, HIGH);
  analogWrite(CLK, 127);

  Timer1.initialize(30000);
  Timer1.attachInterrupt(InterruptTimer1);
}

/////////////////////////////////////////////
void WriteRegister(byte address, unsigned int data) {
  cmd = 0;
  cmd = (address << 12) | (WD << 11) | (data & 0x0FFF);
}

/////////////////////////////////////////////
void SetOutputRegister(unsigned int reg) {
  cmd = 0;
  cmd = reg;
}

/////////////////////////////////////////////
unsigned int ReadRegister(byte adress) {
  cmd = 0;
  cmd = address;

  digitalWrite(SPI_CSB, LOW); // Set CSB to Low state
  SPI.transfer16(cmd);
  data = SPI.transfer16(cmd);
  digitalWrite(SPI_CSB, HIGH); // Set CSB to High state

  return data;
}

// Output : 1..6
void SetOutputState(byte output, boolean state) {
  cmd = 0;
  cmd = ((output + 1) << 12) | (WD << 11) | (state << 8) | 255;
  SendCmd();
}

// output 1..6
// value 0..255
void SetPWMDuty(byte output, byte value) {
  cmd = 0;
  cmd = ((output + 1) << 12) | (1 << 8) | value;
  SendCmd();
}

void SetMonitoring(byte mux) {
  cmd = 0;
  cmd = (mux << 5);
  SendCmd();
}

// channel 1..6
// Set the current sense on specified output
float GetOutputCurrent() {
  float current = 0.0;
  unsigned int ratio = 11.0;
  unsigned int Rcsns = 3.0;
  float csnsVal = analogRead(CSNS);

  current = (((csnsVal * 5.0) / 1023.0) * ratio) / Rcsns;
  return current;
}

float GetVPWR() {
  float voltage = 0;
  voltage = (analogRead(CSNS) * 5.0 / 1023.0) * 4.0;
  return voltage;
}

float GetTemperature() {
  float temperature = 0;
  // temperature = 25 - ((analogRead(CSNS) * 5.0 / 1023.0) - 2.31);
  temperature = (analogRead(CSNS) * 5.0 / 1023.0) / 0.0924;
  return temperature;
}

// This function was created by 'evaltoollab' for the project 'bicyclelab-manager'
// Copyright (c) 2012 Olivier Evalet (http://evaletolab.ch/)
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if (pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch (divisor) {
    case 1:
      mode = 0x01;
      break;
    case 8:
      mode = 0x02;
      break;
    case 64:
      mode = 0x03;
      break;
    case 256:
      mode = 0x04;
      break;
    case 1024:
      mode = 0x05;
      break;
    default:
      return;
    }
    if (pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if (pin == 3 || pin == 11) {
    switch (divisor) {
    case 1:
      mode = 0x01;
      break;
    case 8:
      mode = 0x02;
      break;
    case 32:
      mode = 0x03;
      break;
    case 64:
      mode = 0x04;
      break;
    case 128:
      mode = 0x05;
      break;
    case 256:
      mode = 0x06;
      break;
    case 1024:
      mode = 0x7;
      break;
    default:
      return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}
