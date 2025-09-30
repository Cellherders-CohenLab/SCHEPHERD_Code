// File: ControlBoard.h

#ifndef CONTROL_BOARD_H
#define CONTROL_BOARD_H

#include <Adafruit_DACX578.h>
#include <ADS7828.h>
#include "Adafruit_MCP23008.h"

class ControlBoard {
public:
  ControlBoard();

  void begin();
  void setCurrent(int channel, float current_mA);
  int readADC(int module, int channel);
  void updateAllADC();
  void sendFormattedData(elapsedMillis PWRTimer, elapsedMillis PGMTimer);

private:
  void tcaselect(uint8_t i);
  void setDACCurrent(int channel, float current);
  void setSwitchPosition(int channel, int state);

  Adafruit_DACX578 dac;
  Adafruit_MCP23008 mcp[4];
  ADS7828 adc[4];

  static const uint8_t TCAADDR = 0x70;
  static const uint8_t DACChannelMap[8];
  static const uint8_t swPinMap[2][4];
  static const float Curr_correction[8];

public:
  float voltage[8];
  float current[8];
  float currentCommand[8];
  int adcData[4][6];
};

#endif // CONTROL_BOARD_H

void initializeScheduleArrays(unsigned long* time_arr, float (*curr_CMD_arr)[8], int N);
bool decodeCMD (const String& input, unsigned long* time_arr, float (*curr_CMD_arr)[8], int max_entries);
