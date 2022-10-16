#include "mp1/mod_shared.hh"

extern "C" {
int shutdown_signal = 0;
void release_mod() {
   if (shutdown_signal == 1) {
      cmfgame_hooks_release();
      partfx_timescale_release();
      shutdown_signal = 2;
   } else if (shutdown_signal == 3) {
      cmfgame_hooks_suspend();
      partfx_timescale_suspend();
      shutdown_signal = 4;
   }
}
}
