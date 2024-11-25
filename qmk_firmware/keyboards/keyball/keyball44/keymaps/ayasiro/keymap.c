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
    KC_GRV   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                        KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_EQUAL ,
    KC_LBRC  , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                        KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_QUOTE ,
    KC_RBRC  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                        KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_MINUS , KC_SLASH ,
              KC_LALT,KC_LGUI,LCTL_T(KC_LNG2)     ,LT(1,KC_SPC),LT(3,KC_LNG1),                  KC_BSPC,LT(2,KC_ENT), RCTL_T(KC_LNG2),     KC_RALT  , KC_BSLS	
  ),

  [1] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______  , _______  , _______  , _______  ,                  _______  , _______  , XXXXXXX  , XXXXXXX  , _______
  ),

  [2] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                        _______  , _______  , _______  , _______  , _______  , _______  ,
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

layer_state_t layer_state_set_user(layer_state_t state)
{
  // Auto enable scroll mode when the highest layer is 3
  keyball_set_scroll_mode(get_highest_layer(state) == 3);
  return state;
}

__attribute__((weak)) void keyball_on_apply_motion_to_mouse_scroll(keyball_motion_t *m, report_mouse_t *r, bool is_left)
{
  // consume motion of trackball.
  int16_t div = 1 << (keyball_get_scroll_div() - 1);
  int16_t x = divmod16(&m->x, div);
  int16_t y = divmod16(&m->y, div);

  // apply to mouse report.
#if KEYBALL_MODEL == 61 || KEYBALL_MODEL == 39 || KEYBALL_MODEL == 147 || KEYBALL_MODEL == 44
  r->h = clip2int8(y);
  r->v = -clip2int8(x);
  if (is_left)
  {
    r->h = -r->h;
    r->v = -r->v;
  }
#elif KEYBALL_MODEL == 46
  r->h = clip2int8(x);
  r->v = clip2int8(y);
#else
#error("unknown Keyball model")
#endif

  // Scroll snapping
#if KEYBALL_SCROLLSNAP_ENABLE == 1
  // Old behavior up to 1.3.2)
  uint32_t now = timer_read32();
  if (r->h != 0 || r->v != 0)
  {
    keyball.scroll_snap_last = now;
  }
  else if (TIMER_DIFF_32(now, keyball.scroll_snap_last) >= KEYBALL_SCROLLSNAP_RESET_TIMER)
  {
    keyball.scroll_snap_tension_h = 0;
  }
  if (abs(keyball.scroll_snap_tension_h) < KEYBALL_SCROLLSNAP_TENSION_THRESHOLD)
  {
    keyball.scroll_snap_tension_h += y;
    r->h = 0;
  }
#elif KEYBALL_SCROLLSNAP_ENABLE == 2
  // New behavior
  switch (keyball_get_scrollsnap_mode())
  {
  case KEYBALL_SCROLLSNAP_MODE_VERTICAL:
    r->h = 0;
    break;
  case KEYBALL_SCROLLSNAP_MODE_HORIZONTAL:
    r->v = 0;
    break;
  default:
    // pass by without doing anything
    break;
  }
#endif

  // --- ここから追記 ---
  // windowsOSでスクロール方向反転
  if (detected_host_os() == OS_WINDOWS || detected_host_os() == OS_LINUX)
  {
    r->h = -r->h;
    r->v = -r->v;
  }
  // --- ここまで追記 ---
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void)
{
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif
