/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "quantum.h"

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // keymap for default (VIA)
  [0] = LAYOUT_universal(
    KC_GRV   , KC_Q         , KC_W         , KC_E     , KC_R            , KC_T       ,                                      KC_Y        , KC_U           , KC_I     , KC_O         , KC_P             , KC_EQUAL ,
    KC_LBRC  , LCTL_T(KC_A) , LSFT_T(KC_S) , KC_D     , LT(2,KC_F)            , KC_G       ,                                      KC_H        , KC_J           , KC_K     , RSFT_T(KC_L) , RCTL_T(KC_SCLN)  , KC_QUOTE ,
    KC_RBRC  , KC_Z         , KC_X         , KC_C     , KC_V            , KC_B       ,                                      KC_N        , KC_M           , KC_COMM  , KC_DOT       , KC_MINUS         , KC_SLASH ,
                              KC_LALT      , KC_LGUI  , _______         , LSFT_T(KC_SPC), _______ ,                  KC_BSPC, LT(1,KC_ENT), XXXXXXX        , XXXXXXX  , KC_BSLS	
  ),

  [1] = LAYOUT_universal(
    _______  , S(KC_1)  , S(KC_2)  , S(KC_3)  , S(KC_4)  , S(KC_5)  ,                                        S(KC_6)  , S(KC_7)  , S(KC_8)  , S(KC_9)  , S(KC_0)  , _______  ,
    _______  , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                        KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , _______  ,
    _______  , KC_F1     , KC_F2     , KC_F3     , KC_F4     , KC_F5     ,                                        KC_F6     , KC_F7     , KC_F8     , KC_F9     , KC_F10     , _______  ,
                          _______  , _______  , _______  , _______  , _______  ,                  _______  , _______  , XXXXXXX  , XXXXXXX  , _______
  ),

  [2] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , KC_BTN1  , KC_BTN2  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______  , _______  , _______  , _______  ,                  _______  , _______  , XXXXXXX  , XXXXXXX  , _______
  ),

  [3] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______  , _______  , _______  , _______  ,                  _______  , _______  , XXXXXXX  , XXXXXXX  , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
  // Auto enable scroll mode when the highest layer is 3
  uint8_t layer = biton32(state);
  switch (layer) {
  case 0:
    rgblight_sethsv(HSV_WHITE);
    break;
  case 1:
    rgblight_sethsv(HSV_RED);
    break;
  case 2:
    rgblight_sethsv(HSV_GREEN);
    break;
  case 3:
    rgblight_sethsv(HSV_BLUE);
    break;
  }
  return state;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif
