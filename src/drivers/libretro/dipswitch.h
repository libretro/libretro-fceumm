#ifndef __VSUNI_DIPSWITCH__
#define __VSUNI_DIPSWITCH__

#include <libretro.h>

void set_dipswitch_variables(unsigned *index, struct retro_variable vars[]);
void update_dipswitch(void);

extern retro_environment_t environ_cb;

#endif /* __VSUNI_DIPSWITCH__ */
