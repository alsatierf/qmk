#include <stdint.h>
#include "annepro2.h"
#include "config.h"
#include "g/keymap_combo.h"
#include "features/layer_lock.h"
#include QMK_KEYBOARD_H

enum anne_pro_layers {
  _BASE_LAYER,
  _FN1_LAYER,
};

enum custom_keycodes {
  LLOCK = SAFE_RANGE,
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_INTERRUPTED_SINGLE_TAP,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD,
    TD_INTERRUPTED_DOUBLE_TAP,
    TD_TRIPLE_TAP,
    TD_TRIPLE_HOLD,
    TD_INTERRUPTED_TRIPLE_TAP,
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
    SPC_LSFT_LT1,
    SOME_OTHER_DANCE
};

td_state_t cur_dance(tap_dance_state_t *state);

void spc_lsft_lt1__finished(tap_dance_state_t *state, void *user_data);
void spc_lsft_lt1__reset(tap_dance_state_t *state, void *user_data);

// Create an instance of 'td_tap_t' for the 'x' tap dance.
static td_tap_t spc_lsft_lt1__tap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

void spc_lsft_lt1__finished(tap_dance_state_t *state, void *user_data) {
    spc_lsft_lt1__tap_state.state = cur_dance(state);
    switch (spc_lsft_lt1__tap_state.state) {
        case TD_SINGLE_TAP: register_code(KC_SPC); break;
        case TD_INTERRUPTED_SINGLE_TAP:
        case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
        case TD_DOUBLE_TAP:
        case TD_INTERRUPTED_DOUBLE_TAP:
        case TD_DOUBLE_HOLD: layer_on(_FN1_LAYER); break;
        default: break;
    }
}

void spc_lsft_lt1__reset(tap_dance_state_t *state, void *user_data) {
    switch (spc_lsft_lt1__tap_state.state) {
        case TD_SINGLE_TAP: unregister_code(KC_SPC); break;
        case TD_INTERRUPTED_SINGLE_TAP:
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case TD_DOUBLE_TAP:
        case TD_INTERRUPTED_DOUBLE_TAP:
        case TD_DOUBLE_HOLD: layer_off(_FN1_LAYER); break;
        default: break;
    }
    spc_lsft_lt1__tap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [SPC_LSFT_LT1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, spc_lsft_lt1__finished, spc_lsft_lt1__reset)
};

const uint16_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE_LAYER] = LAYOUT_60_ansi( /* Base */
    KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC,
    KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS,
    OSL(_FN1_LAYER), KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_ENT,
    MT(MOD_LSFT, KC_NUBS), KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, MT(MOD_RSFT, KC_INT1),
    KC_LCTL, KC_LGUI, KC_LALT, TD(SPC_LSFT_LT1), KC_RALT, KC_RGUI, KC_RCTL, KC_NUBS

  ),
  [_FN1_LAYER] = LAYOUT_60_ansi(
    KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_BSPC,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_PGUP, KC_HOME, KC_UP, KC_END, KC_DEL, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, OSM(MOD_LSFT), OSM(MOD_LALT), OSM(MOD_LGUI), OSM(MOD_LCTL), KC_TRNS, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, LLOCK, KC_GRV, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
    KC_TRNS, KC_TRNS, KC_TRNS, OSM(MOD_LSFT), KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),
};

const uint16_t keymaps_size = sizeof(keymaps);

void matrix_init_user(void) {}

void matrix_scan_user(void) {
  layer_lock_task();
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_layer_lock(keycode, record, LLOCK)) { return false; }
    return true;
};

td_state_t cur_dance(tap_dance_state_t *state) {
    switch (state->count) {
      case 1:
        if (state->interrupted) return TD_INTERRUPTED_SINGLE_TAP;
        else if (state->pressed) return TD_SINGLE_HOLD;
        else return TD_SINGLE_TAP;
      case 2:
        if (state->interrupted) return TD_INTERRUPTED_DOUBLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
      case 3:
        if (state->interrupted) return TD_INTERRUPTED_TRIPLE_TAP;
        else if (state->pressed) return TD_TRIPLE_HOLD;
        else return TD_TRIPLE_TAP;
      default:
        return TD_UNKNOWN;
    }
}