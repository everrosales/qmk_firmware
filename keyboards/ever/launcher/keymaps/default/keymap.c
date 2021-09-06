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
  FORCE_SUBMIT = SAFE_RANGE,
  PREP_SUBMIT,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Held: Layer 2     |      |     |
        |                   |      |     |
     */
    [0] = LAYOUT(
        MO(1), _______, _______ ,
        _______  , _______  , _______
    ),
    /*
        |      | Held: Layer 3  |      |
        |      |                |      |
     */
    [1] = LAYOUT(
        _______  , MO(2), _______ ,
        _______, _______ , _______
    ),
    /*
        |              |      | PREP_SUBMIT |
        | FORCE_SUBMIT |      |             |
     */
    [2] = LAYOUT(
        _______  , _______ , PREP_SUBMIT,
        FORCE_SUBMIT, _______ , _______
    ),
};

static bool prep_state_triggered = false;
static bool fired = false;
const int length_of_string = 12;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case PREP_SUBMIT:
      if (record->event.pressed) {
        SEND_STRING("g4 submit -f");
        prep_state_triggered = true;
        backlight_enable();
      } else {
        // TODO: delete line
        backlight_disable();
        if (!fired) {
          for (int i = 0; i < length_of_string; i++) {
            tap_code(KC_BSPACE);
          }
        }
        prep_state_triggered = false;
        fired = false;
      }
      break;
    case FORCE_SUBMIT:
      if (record->event.pressed) {
        if (prep_state_triggered && !fired) {
          fired = true;
          tap_code(KC_ENTER);
          backlight_disable();
        }
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
