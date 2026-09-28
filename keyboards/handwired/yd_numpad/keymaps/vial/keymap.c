#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_ESC,    KC_CAPS,   KC_MUTE,   KC_VOLD,   KC_VOLU,
        KC_F1,     KC_F2,     KC_H,      KC_F,      KC_TAB,
        KC_PMNS,   KC_PAST,   KC_PSLS,   KC_NO,     KC_BSPC,
        KC_PPLS,   KC_P7,     KC_P8,     KC_P9,     KC_NO,
        KC_NO,     KC_P4,     KC_P5,     KC_P6,     KC_NO,
        KC_PENT,   KC_P1,     KC_P2,     KC_P3,     KC_NO,
        KC_NO,     KC_P0,     KC_NO,     KC_PDOT,   KC_NO
    )
};
