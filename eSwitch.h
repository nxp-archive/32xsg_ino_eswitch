// Copyright 2017 NXP
// Arduino eSwitch driver
#include <SPI.h>
#include <TimerOne.h>
#include <stdint.h>

#ifndef eSwitch_H
#define eSwitch_H

#endif

#define CLK 6
#define RSTB 9
#define SPI_CSB 10
#define SPI_MOSI 11
#define SPI_MISO 12
#define SPI_CLK 13
#define CSNS A0
#define CSNS_Sync 0

#define Init1 0
#define Init2 1
#define CH1_Ctrl 2
#define CH2_Ctrl 3
#define CH3_Ctrl 4
#define CH4_Ctrl 5
#define CH5_Ctrl 6
#define CH6_Ctrl 7
#define OUT_Ctrl 8
#define GPWM1 9
#define GPWM2 10
#define OC_Ctrl1 11
#define OC_Ctrl2 12
#define Input_EN 13
#define PRS_Settings1 14
#define PRS_Settings2 15
#define OL_Ctrl 16
#define OLLED_Ctrl 17
#define Incr_Decr 1

void SendCmd(unsigned int cmd);

void InterruptTimer1(void);

// Init the SPI communication
void Init(void);

/*
This function is used to send
a SPI command to the eSwitch
*/
void WriteRegister(byte address, unsigned int data);

/*
This function allows to change the output register
of the product (1..9)
*/
void SetOutputRegister(unsigned int reg);

/*
This function is used to read
a SPI command to the eSwitch
*/
unsigned int ReadRegister(byte address);

void SetOutputState(byte channel, boolean state);

void SetPWMDuty(byte channel, byte value);

/*this function is used to set the monit*/
void SetMonitoring(byte mux);

/*This function returns the output current value*/
float GetOutputCurrent();

/*This function returns the power supply voltage*/
float GetVPWR();

/*This function returns the die temperature*/
float GetTemperature();

void setPwmFrequency(int pin, int divisor);
