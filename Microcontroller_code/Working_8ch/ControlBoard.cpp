// File: ControlBoard.cpp

#include "ControlBoard.h"
#include <Wire.h>

const uint8_t ControlBoard::DACChannelMap[8] = { 0, 2, 4, 6, 1, 3, 5, 7 };
const uint8_t ControlBoard::swPinMap[2][4] = {
  { 1, 0, 6, 5 },
  { 3, 2, 4, 7 },
};
// current readout ADC compensation (in mA)
// calibrate using 1kohm resistors

const float ControlBoard::Curr_correction[8] = { 0.03, -0.13, 0.02, -0.13, 0.06, -0.13, 0.08, -0.13};  //board #1
//const float ControlBoard::Curr_correction[8] = { 0.12, -0.03, 0.12, -0.02, 0.12, -0.02, 0.11, -0.02};  //board #2

ControlBoard::ControlBoard()
  : dac(12) {}

void ControlBoard::begin() {
  Wire.begin();
  Wire.setClock(400000L);
  dac.begin();

  // Enable all modules via Teensy GPIOs 2–5
  for (int pin = 2; pin <= 5; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  for (int module = 1; module <= 4; module++) {
    tcaselect(module);
    // enable all ADC, internal reference
    adc[module - 1].begin(0);
    adc[module - 1].setpd(IREF_ON_AD_ON);
    // enable all I/O expander, set all pin to output LOW
    mcp[module - 1].begin();
    for (uint8_t i = 0; i < 8; i++) {
      mcp[module - 1].pinMode(i, OUTPUT);
      mcp[module - 1].digitalWrite(i, LOW);
    }
  }
}

void ControlBoard::tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}



void ControlBoard::setDACCurrent(int channel, float current) {
  int setting = int(current * 200.0);
  dac.writeAndUpdateChannelValue(DACChannelMap[channel], setting);
}

void ControlBoard::setCurrent(int channel, float current_mA) {
  if (channel < 0 || channel > 7) return;
  if (current_mA > 0) setSwitchPosition(channel, 1);
  else if (current_mA < 0) setSwitchPosition(channel, -1);
  else setSwitchPosition(channel, 0);

  setDACCurrent(channel, abs(current_mA));
}

void ControlBoard::setSwitchPosition(int channel, int state) {
  int module = channel / 2 + 1;
  int idx = channel % 2;
  uint8_t SWP = swPinMap[idx][0];
  uint8_t SWN = swPinMap[idx][1];
  uint8_t MUXP = swPinMap[idx][2];
  uint8_t MUXN = swPinMap[idx][3];

  tcaselect(module);
  Adafruit_MCP23008* target = &mcp[module - 1];

  switch (state) {
    case 1:
      target->digitalWrite(SWP, HIGH);
      target->digitalWrite(SWN, HIGH);
      target->digitalWrite(MUXP, HIGH);
      target->digitalWrite(MUXN, LOW);
      break;
    case -1:
      target->digitalWrite(SWP, HIGH);
      target->digitalWrite(SWN, HIGH);
      target->digitalWrite(MUXP, LOW);
      target->digitalWrite(MUXN, HIGH);
      break;
    default:
      target->digitalWrite(SWP, LOW);
      target->digitalWrite(SWN, LOW);
      target->digitalWrite(MUXP, LOW);
      target->digitalWrite(MUXN, LOW);
      break;
  }
}

int ControlBoard::readADC(int module, int channel) {
  if (module < 1 || module > 4 || channel < 0 || channel > 7) return -1;
  tcaselect(module);
  return adc[module - 1].read(channel);
}

void ControlBoard::updateAllADC() {
  for (int module = 1; module <= 4; module++) {
    int m = module - 1;
    tcaselect(module);

    float adc_sum[6] = { 0 };
    for (int i = 0; i < 5; i++) {
      for (int ch = 0; ch < 6; ch++) {
        adc_sum[ch] += adc[m].read(ch);  // take 6 averages per reading
      }
    }

    for (int ch = 0; ch < 6; ch++) {
      adcData[m][ch] = round(adc_sum[ch] / 5.0);
    }

    // Update voltage: channels 2-3 and 4-5 as differential pairs
    voltage[m * 2] = (float(adcData[m][2]) - float(adcData[m][3])) / 4096.0 * 2.5 * 10;
    voltage[m * 2 + 1] = (float(adcData[m][4]) - float(adcData[m][5])) / 4096.0 * 2.5 * 10;

    // Update current: channels 0 and 1
    current[m * 2] = (1 - 2 * (voltage[m * 2] < 0)) * ((float(adcData[m][0]) / 4096.0 * 2.5 * 10) + Curr_correction[m * 2]);
    current[m * 2 + 1] = (1 - 2 * (voltage[m * 2 + 1] < 0)) * ((float(adcData[m][1]) / 4096.0 * 2.5 * 10) + Curr_correction[m * 2 + 1]);

    // Optional debug print
  }
  // for (int ch = 0; ch < 8; ch++) {
  //   Serial.print(voltage[ch], 3);
  //   Serial.print("V ");
  //   Serial.print(current[ch], 3);
  //   Serial.print("mA ");
  //   Serial.print(voltage[ch] / current[ch], 3);
  //   Serial.print("KOHM   ");
  // }
  // Serial.println();
}

void ControlBoard::sendFormattedData(elapsedMillis PWRTimer, elapsedMillis PGMTimer) {
  Serial.print("TIME::");
  Serial.print(float(PWRTimer) / 1000.0, 1);
  Serial.print("::PGMTime::");
  Serial.print(float(PGMTimer) / 1000.0, 1);
  Serial.print("::SMPL::");

  for (int i = 0; i < 8; i++) {
    Serial.print(voltage[i], 3);
    Serial.print(" ");
  }
  for (int i = 0; i < 8; i++) {

    if (abs(voltage[i]) < 0.08) {
      Serial.print(0.000, 3);  //"mute" when there's no voltage reading
    } else {
      Serial.print(current[i], 3);
    }
    if (i < 7) Serial.print(" ");
  }
  Serial.println();
}

void initializeScheduleArrays(unsigned long* time_arr, float (*curr_CMD_arr)[8], int N) {
  for (int i = 0; i < N; i++) {
    time_arr[i] = 0;
    for (int ch = 0; ch < 8; ch++) {
      curr_CMD_arr[i][ch] = 0.0;
    }
  }
}

bool decodeCMD(const String& input, unsigned long* time_arr, float (*curr_CMD_arr)[8], int max_entries) {
  // Check prefix
  if (!input.startsWith("PGM::")) return false;

  // Extract number of entries (N)
  int startN = 5;
  int endN = input.indexOf("::", startN);
  if (endN == -1) return false;

  int N = input.substring(startN, endN).toInt();
  if (N <= 0 || N > max_entries) return false;

  // Split all tokens after the second "::"
  String payload = input.substring(endN + 2);
  payload.trim();

  String tokens[max_entries*9];
  int numTokens = 0;
  int start = 0;
  for (unsigned int i = 0; i < payload.length(); i++) {
    if (payload[i] == ' ' || i == payload.length() - 1) {
      int end = (payload[i] == ' ') ? i : i + 1;
      if (start < end) {
        tokens[numTokens++] = payload.substring(start, end);
      }
      start = i + 1;
    }
  }

  if (numTokens < N + N * 8) return false;

  // Parse time values
  for (int i = 0; i < N; i++) {
    time_arr[i] = tokens[i].toInt();
  }

  // Parse current commands
  int base = N;
  for (int i = 0; i < N; i++) {
    for (int ch = 0; ch < 8; ch++) {
      curr_CMD_arr[i][ch] = tokens[base + i * 8 + ch].toFloat();
    }
  }

  // Zero pad rest
  for (int i = N; i < max_entries; i++) {
    time_arr[i] = 0;
    for (int ch = 0; ch < 8; ch++) curr_CMD_arr[i][ch] = 0.0;
  }

  return true;
}
