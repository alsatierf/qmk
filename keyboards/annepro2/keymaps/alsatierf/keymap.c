#include <stdint.h>
#include "annepro2.h"
#include "config.h"
// #include "g/keymap_combo.h"
#include QMK_KEYBOARD_H
#include "features/layer_lock.h"
#include "features/achordion.h"

enum anne_pro_layers {
  _BASE_LAYER,
  _FN1_LAYER
};

enum custom_keycodes {
  LLOCK = SAFE_RANGE,
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE_LAYER] = LAYOUT_60_ansi( /* Base */
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, LGUI_T(KC_E), KC_R, KC_T, KC_Y, KC_U, LGUI_T(KC_I), KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    RALT_T(KC_GRV), KC_A, LALT_T(KC_S), LSFT_T(KC_D), LCTL_T(KC_F), KC_G, KC_H, LCTL_T(KC_J), LSFT_T(KC_K), LALT_T(KC_L), KC_SCLN, KC_QUOT, KC_ENT,
    SC_LSPO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, SC_RSPC,
    KC_LCTL, KC_LGUI, KC_LALT, LT(_FN1_LAYER, KC_SPC), KC_RALT, KC_RGUI, KC_RCTL, KC_NUBS

  ),
  [_FN1_LAYER] = LAYOUT_60_ansi(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_BSPC,
    KC_TRNS, KC_TRNS, KC_TRNS, OSM(MOD_LGUI), KC_TRNS, KC_TRNS, KC_TRNS, KC_HOME, KC_UP, KC_END, KC_DEL, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_CAPS, KC_TRNS, OSM(MOD_LALT), OSM(MOD_LSFT), OSM(MOD_LCTL), KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_BSPC, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_PGDN, KC_TRNS, LLOCK, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
};

const uint16_t keymaps_size = sizeof(keymaps);

void matrix_init_user(void) {}

void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode,
                     keyrecord_t* tap_hold_record,
                     uint16_t other_keycode,
                     keyrecord_t* other_record) {
  switch (other_keycode) {
    case QK_MOD_TAP ... QK_MOD_TAP_MAX:
    case QK_LAYER_TAP ... QK_LAYER_TAP_MAX:
      other_keycode &= 0xff;  // Get base keycode.
  }
  if (other_keycode == KC_B) { return true; }
  if (other_keycode > KC_Z) { return true; }

  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case LT(_FN1_LAYER, KC_SPC):
    case RALT_T(KC_GRV):
    case SC_LSPO:
    case SC_RSPC:
      return 0;  // Bypass Achordion for these keys.
  }
  return 1000;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    switch (get_highest_layer(state)) {
        case _FN1_LAYER:
            ap2_led_set_foreground_color(0x00, 0x00, 0xFF);
            break;
        default:
            ap2_led_set_foreground_color(0xFF, 0xFF, 0xFF);
            break;
    }
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
  if (!process_achordion(keycode, record)) { return false; }
  if (!process_layer_lock(keycode, record, LLOCK)) { return false; }
  return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case LGUI_T(KC_E):
    case LGUI_T(KC_I):
    case LALT_T(KC_L):
    case LALT_T(KC_S):
      return 499;
    case LSFT_T(KC_D):
    case LCTL_T(KC_F):
    case LSFT_T(KC_K):
    case LCTL_T(KC_J):
      return 250;
    case LT(_FN1_LAYER, KC_SPC):
      return 130;
    default:
      return TAPPING_TERM;
  }
}

bool get_retro_tapping(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case LT(_FN1_LAYER, KC_SPC):
            return true;
        default:
            return false;
    }
}