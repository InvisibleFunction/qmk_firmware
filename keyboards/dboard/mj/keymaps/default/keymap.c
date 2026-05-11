// keymap.c for DBoard MJ

#include QMK_KEYBOARD_H
#include <stdlib.h>

// Define a reasonable range for the random intervals (in milliseconds)
#define MIN_INTERVAL 30000  // 30 seconds
#define MAX_INTERVAL 120000 // 2 minutes

// Variables to track timing
static uint32_t next_shift_press = 0;
static bool shift_pressed = false;
static uint32_t shift_duration = 0;
static bool anti_sleep_enabled = false;

// Define layers
enum {
    BASE_LAYER = 0,
};

// Define keymaps - we'll use this as a toggle switch, not actually sending a keycode
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [BASE_LAYER] = LAYOUT_ortho_1x1(
        KC_NO  // Don't send any keycode when the button is pressed
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Toggle anti-sleep mode when the switch changes state
    if (record->event.pressed) {
        anti_sleep_enabled = true;
        // Reset timing on enable
        next_shift_press = timer_read32() + 5000; // First press in 5 seconds
    } else {
        anti_sleep_enabled = false;
        // If shift is currently pressed when disabling, release it
        if (shift_pressed) {
            unregister_code(KC_LSFT);
            shift_pressed = false;
        }
    }
    return false; // Don't actually send any keycode
}

// This function runs with each matrix scan
void matrix_scan_user(void) {
    // Only run anti-sleep behavior if enabled
    if (!anti_sleep_enabled) {
        return;
    }

    // Get current time
    uint32_t current_time = timer_read32();

    // If it's time to press shift and shift is not already pressed
    if (current_time >= next_shift_press && !shift_pressed) {
        register_code(KC_LSFT);
        shift_pressed = true;
        shift_duration = 100 + (rand() % 200); // Hold shift for 100-300ms
    }

    // If shift is currently pressed and it's time to release it
    if (shift_pressed && (current_time - next_shift_press >= shift_duration)) {
        unregister_code(KC_LSFT);
        shift_pressed = false;

        // Set next press time to a random interval in the future
        next_shift_press = current_time + MIN_INTERVAL + (rand() % (MAX_INTERVAL - MIN_INTERVAL));
    }
}
