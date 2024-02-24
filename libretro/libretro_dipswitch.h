#ifndef __VSUNI_DIPSWITCH__
#define __VSUNI_DIPSWITCH__

#include <libretro.h>

void set_dipswitch_variables(retro_environment_t *_environ_cb, unsigned current_index, struct retro_core_option_v2_definition *vars);
void check_dipswitch_variables(void);
void dipswitch_close(void);

#endif /* __VSUNI_DIPSWITCH__ */
