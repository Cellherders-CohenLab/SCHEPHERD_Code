#include "ControlBoard.h"
elapsedMillis sincePWR;
elapsedMillis sincePGM;
static int lastIdx = -1;  // Remembers last index that was applied
static int latestIdx = -1;


const int scheduleLength = 100;
unsigned long time_arr[scheduleLength];  // = {2000, 5000, 15000, 20000};

float curr_CMD_arr[scheduleLength][8];  // = {


ControlBoard board;

void setup() {
  Serial.begin(115200);
  board.begin();
  delay(200);

  initializeScheduleArrays(time_arr, curr_CMD_arr, scheduleLength);
  for (int i = 0; i < 8; i++) {
    board.setCurrent(i, 0);
  }

  sincePWR = 0;
}

void loop() {


  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();  // Clean up whitespace and newline

    if (decodeCMD(line, time_arr, curr_CMD_arr, scheduleLength)) {
      //Serial.println("Program received and loaded.");
      lastIdx = -1;
      latestIdx = -1;
      for (int i = 0; i < 8; i++) {
        board.setCurrent(i, 0);
      }
      sincePGM = 0;
    } else {
      //Serial.println("Invalid program format.");
    }
  }

  for (int i = 0; i < scheduleLength; i++) {
    if (time_arr[i] == 0 || time_arr[i] > (sincePGM / 1000)) break;
    latestIdx = i;
  }

  // If a new time index is found, apply the current command
  if (latestIdx >= 0 && latestIdx != lastIdx) {
    for (int ch = 0; ch < 8; ch++) {
      board.setCurrent(ch, curr_CMD_arr[latestIdx][ch]);
    }

    // Update tracker so we don’t repeat this index
    lastIdx = latestIdx;
  }

  board.updateAllADC();
  board.sendFormattedData(sincePWR, sincePGM);

  delay(1000);
}
