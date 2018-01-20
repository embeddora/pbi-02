/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */



#ifndef __GFXEVENT_H__
#define __GFXEVENT_H__

#include "stdgfx.h"
#include "gfxtypes.h"

#if defined (__FORWINDOWS__)
  #define VK_A  'A'
  #define VK_B  'B'
  #define VK_C  'C'
  #define VK_D  'D'
  #define VK_E  'E'
  #define VK_F  'F'
  #define VK_G  'G'
  #define VK_H  'H'
  #define VK_I  'I'
  #define VK_J  'J'
  #define VK_K  'K'
  #define VK_L  'L'
  #define VK_M  'M'
  #define VK_N  'N'
  #define VK_O  'O'
  #define VK_P  'P'
  #define VK_Q  'Q'
  #define VK_R  'R'
  #define VK_S  'S'
  #define VK_T  'T'
  #define VK_U  'U'
  #define VK_V  'V'
  #define VK_W  'W'
  #define VK_X  'X'
  #define VK_Y  'Y'
  #define VK_Z  'Z'
  
  
  #define SC_ESC      VK_ESCAPE
  #define SC_1        VK_1
  #define SC_2        VK_2
  #define SC_3        VK_3
  #define SC_4        VK_4
  #define SC_5        VK_5
  #define SC_6        VK_6
  #define SC_7        VK_7
  #define SC_8        VK_8
  #define SC_9        VK_9
  #define SC_0        VK_0
  #define SC_USCORE   0x0C
  #define SC_EQUAL    0x0D
  #define SC_BKSPACE  VK_BACK
  #define SC_TAB      VK_TAB
  #define SC_Q        VK_Q
  #define SC_W        VK_W
  #define SC_E        VK_E
  #define SC_R        VK_R
  #define SC_T        VK_T
  #define SC_Y        VK_Y
  #define SC_U        VK_U
  #define SC_I        VK_I
  #define SC_O        VK_O
  #define SC_P        VK_P
  #define SC_OBLKT    0X1A
  #define SC_CBLKT    0x1B
  #define SC_ENTER    VK_RETURN
  #define SC_LCTRL    VK_CONTROL
  #define SC_A        VK_A
  #define SC_S        VK_S
  #define SC_D        VK_D
  #define SC_F        VK_F
  #define SC_G        VK_G
  #define SC_H        VK_H
  #define SC_J        VK_J
  #define SC_K        VK_K
  #define SC_L        VK_L
  #define SC_COLON    0X27
  #define SC_QUATS    0x28
  #define SC_WAVE     0x29
  #define SC_LSHIFT   VK_SHIFT
  #define SC_POST     0x2B
  #define SC_Z        VK_Z
  #define SC_X        VK_X
  #define SC_C        VK_C
  #define SC_V        VK_V
  #define SC_B        VK_B
  #define SC_N        VK_N
  #define SC_M        VK_M
  #define SC_COMMA    0x33
  #define SC_PERIOD   0x34
  #define SC_FSLASH   0x35
  #define SC_RSHIFT   0x36
  #define SC_PRTSCR   VK_SNAPSHOT
  #define SC_LALT     VK_MENU
  #define SC_SPACE    VK_SPACE
  #define SC_CAPLOCK  VK_CAPITAL
  #define SC_F1       VK_F1
  #define SC_F2       VK_F2
  #define SC_F3       VK_F3
  #define SC_F4       VK_F4
  #define SC_F5       VK_F5
  #define SC_F6       VK_F6
  #define SC_F7       VK_F7
  #define SC_F8       VK_F8
  #define SC_F9       VK_F9
  #define SC_F10      VK_F10
  #define SC_NUMLOCK  VK_NUMLOCK
  #define SC_SCRLOCK  0x46
  #define SC_PAD_7    VK_NUMPAD7
  #define SC_PAD_8    VK_NUMPAD8
  #define SC_PAD_9    VK_NUMPAD9
  #define SC_PAD_MINUS 0x4A
  #define SC_PAD_4     VK_NUMPAD4
  #define SC_PAD_5     VK_NUMPAD5
  #define SC_PAD_6     VK_NUMPAD6
  #define SC_PAD_PLUS  0x4E
  #define SC_PAD_1     VK_NUMPAD1
  #define SC_PAD_2     VK_NUMPAD2
  #define SC_PAD_3     VK_NUMPAD3
  #define SC_PAD_0     VK_NUMPAD0
  #define SC_PAD_DEL   0x53

  #define SC_F11       VK_F11
  #define SC_F12       VK_F12

  #define SC_PAD_ENTER 0x9C
  #define SC_RCTRL     0x9D
  #define SC_PAD_SLASH 0xB5
  #define SC_RALT      0xB8
  #define SC_PAUSE     VK_PAUSE
  #define SC_HOME      VK_HOME
  #define SC_PAGEUP    VK_PRIOR
  #define SC_END       VK_END
  #define SC_PAGEDOWN  VK_NEXT
  #define SC_INSERT    VK_INSERT
  #define SC_DELETE    VK_DELETE

  #define SC_UP        VK_UP
  #define SC_LEFT      VK_LEFT
  #define SC_RIGHT     VK_RIGHT
  #define SC_DOWN      VK_DOWN

#elif defined (__FOROS2__)
#elif defined (__FORUNIX__)
  
  #define SC_ESC      0x01
  #define SC_1        0x02
  #define SC_2        0x03
  #define SC_3        0x04
  #define SC_4        0x05
  #define SC_5        0x06
  #define SC_6        0x07
  #define SC_7        0x08
  #define SC_8        0x09
  #define SC_9        0x0A
  #define SC_0        0x0B
  #define SC_USCORE   0x0C
  #define SC_EQUAL    0x0D
  #define SC_BKSPACE  0x0E
  #define SC_TAB      0x0F
  #define SC_Q        0x10
  #define SC_W        0x11
  #define SC_E        0x12
  #define SC_R        0x13
  #define SC_T        0x14
  #define SC_Y        0x15
  #define SC_U        0x16
  #define SC_I        0x17
  #define SC_O        0x18
  #define SC_P        0x19
  #define SC_OBLKT    0x1A
  #define SC_CBLKT    0x1B
  #define SC_ENTER    0x1C
  #define SC_LCTRL    0x1D
  #define SC_A        0x1E
  #define SC_S        0x1F
  #define SC_D        0x20
  #define SC_F        0x21
  #define SC_G        0x22
  #define SC_H        0x23
  #define SC_J        0x24
  #define SC_K        0x25
  #define SC_L        0x26
  #define SC_COLON    0x27
  #define SC_QUATS    0x28
  #define SC_WAVE     0x29
  #define SC_LSHIFT   0x2A
  #define SC_POST     0x2B
  #define SC_Z        0x2C
  #define SC_X        0x2D
  #define SC_C        0x2E
  #define SC_V        0x2F
  #define SC_B        0x30
  #define SC_N        0x31
  #define SC_M        0x32
  #define SC_COMMA    0x33
  #define SC_PERIOD   0x34
  #define SC_FSLASH   0x35
  #define SC_RSHIFT   0x36
  #define SC_PRTSCR   0x37
  #define SC_LALT     0x38
  #define SC_SPACE    0x39
  #define SC_CAPLOCK  0x3A
  #define SC_F1       0x3B
  #define SC_F2       0x3C
  #define SC_F3       0x3D
  #define SC_F4       0x3E
  #define SC_F5       0x3F
  #define SC_F6       0x40
  #define SC_F7       0x41
  #define SC_F8       0x42
  #define SC_F9       0x43
  #define SC_F10      0x44
  #define SC_NUMLOCK  0x45
  #define SC_SCRLOCK  0x46
  #define SC_PAD_7    0x47
  #define SC_PAD_8    0x48
  #define SC_PAD_9    0x49
  #define SC_PAD_MINUS 0x4A
  #define SC_PAD_4     0x4B
  #define SC_PAD_5     0x4C
  #define SC_PAD_6     0x4D
  #define SC_PAD_PLUS  0x4E
  #define SC_PAD_1     0x4F
  #define SC_PAD_2     0x50
  #define SC_PAD_3     0x51
  #define SC_PAD_0     0x52
  #define SC_PAD_DEL   0x53

  #define SC_F11       0x57
  #define SC_F12       0x58

  #define SC_PAD_ENTER 0x9C
  #define SC_RCTRL     0x9D
  #define SC_PAD_SLASH 0xB5
  #define SC_RALT      0x38
  #define SC_PAUSE     0xC5

  #define SC_INSERT    0x52
  #define SC_DELETE    0x53

  #define SC_HOME      0x47
  #define SC_END       0x4F

  #define SC_PAGEUP    0x49
  #define SC_PAGEDOWN  0x51

  #define SC_UP        0x48
  #define SC_LEFT      0x4B
  #define SC_RIGHT     0x4D
  #define SC_DOWN      0x50
#endif

#endif

