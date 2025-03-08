// config.h for DBoard MJ

#pragma once

// Matrix dimensions (1x1 since there's just one button)
#define MATRIX_ROWS 1
#define MATRIX_COLS 1

// Pro Micro pins connected to the switch
#define MATRIX_ROW_PINS { B5 }  // Pin 9 on Pro Micro
#define MATRIX_COL_PINS { D7 }  // Pin 6 on Pro Micro
#define DIODE_DIRECTION COL2ROW

// Set debounce time
#define DEBOUNCE 5
