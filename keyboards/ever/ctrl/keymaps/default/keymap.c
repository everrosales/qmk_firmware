/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

enum custom_keycodes {
  OS_CTRL = SAFE_RANGE,
  PREP_SUBMIT,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Held: Layer 2     |    C  |   V  |
     */
    [0] = LAYOUT(
        KC_RCTRL, KC_C, KC_V,
       KC_A,  KC_B, KC_C
    ),
    /*
        |      | Held: Layer 3  |      |
     */
    [1] = LAYOUT(
        _______  , KC_X, KC_Z ,
        _______, _______, _______
    ),
    /*
        |              |      | PREP_SUBMIT |
     */
    [2] = LAYOUT(
        _______  , _______ , PREP_SUBMIT,
        _______, _______, _______

    ),
};

static bool prep_state_triggered = false;
static bool fired = false;
const int length_of_string = 12;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case OS_CTRL:
      if (record->event.pressed) {
        SEND_STRING("g4 submit -f");
        prep_state_triggered = true;
      } else {
        if (!fired) {
          for (int i = 0; i < length_of_string; i++) {
            tap_code(KC_BSPACE);
          }
        }
        prep_state_triggered = false;
        fired = false;
      }
      break;
    }
  return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == _LEFT) {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
    else if (index == _MIDDLE) {
        if (clockwise) {
            tap_code(KC_DOWN);
        } else {
            tap_code(KC_UP);
        }
    }
    else if (index == _RIGHT) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    }
    return true;
}
