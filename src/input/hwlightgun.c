/* FCE Ultra - NES/Famicom Emulator
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <string.h>
#include "share.h"

int hwlightgun_trigger_invert = 1;
int hwlightgun_sensor_invert = 1;

typedef struct {
  uint32 trigger;
  uint32 detect;
} HWLIGHTGUN;

static HWLIGHTGUN LG[2];

static uint8 FP_FASTAPASS(1) ReadHWLightgun(int w) {
  uint8 ret = 0;
  if ((!hwlightgun_trigger_invert && LG[w].trigger) || (hwlightgun_trigger_invert && !LG[w].trigger)) {
    ret |= 0x10;
  }
  if ((!hwlightgun_sensor_invert && LG[w].detect) || (hwlightgun_sensor_invert && !LG[w].detect)) {
    ret |= 0x8;
  }
  return ret;
}

void FP_FASTAPASS(3) UpdateHWLightgun(int w, void *data, int arg) {
  uint32* ptr = (uint32*)data;

  LG[w].trigger = ptr[0];
  LG[w].detect = ptr[1];
}

static INPUTC HWLightgunCtrl = { ReadHWLightgun, 0, 0, UpdateHWLightgun, 0, 0 };

INPUTC *FCEU_InitHWLightgun(int w) {
  memset(&LG[w], 0, sizeof(HWLIGHTGUN));
  return(&HWLightgunCtrl);
}
