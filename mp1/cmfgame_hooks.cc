#include "freestanding.hh"
#include "mp1/mod_shared.hh"
#include "mp1/mpsdk/cmfgame.hh"
#include "mp1/mpsdk/text_support.hh"
#include "mp1/mpsdk/alloc.hh"

bool dt_just_changed = false;
static float last_dt = kNormalDt;

// Exposed mod cvars
extern "C" {
float game_timescale = 0.5f;
}

extern "C" {
using reg_ctx = u32[18];
extern void _save_reg_ctx(volatile reg_ctx);
extern void _restore_reg_ctx(volatile reg_ctx);
}

void update_dt_changed_flag(float dt) {
   dt_just_changed = (dt != last_dt);
   last_dt = dt;
}

void cmfgame_hooks_release() {}

// Don't free
void cmfgame_hooks_suspend() {}

// Exposed mod functions
extern "C" {
int hooked_cmfgame_onmessage(CMFGame* cmfgame, CArchitectureMessage* arch_message,
                             void* arch_queue) {
   uint16_t* VI_clock = reinterpret_cast<uint16_t*>(0xcc00206c);
   if (*VI_clock != 1) {
      *VI_clock = 1;
   }
   EArchMsgType message_type = arch_message->type;
   const float scaled_dt = kNormalDt * game_timescale;

   if (message_type == EArchMsgType::TimerTick) {
      update_dt_changed_flag(scaled_dt);
      arch_message->get_param_val<float>() = scaled_dt;
   } else if (message_type == EArchMsgType::UserInput) {
      arch_message->get_param_val<float>() = scaled_dt;
   }

   // fucking stupid code warrior garbage has decided NOT to save registers
   // so thanks, great, now I've got to resort to this shit, but that's fine
   // and dandy i love this
   volatile reg_ctx saved_regs;
   _save_reg_ctx(saved_regs);
   volatile int iowin_return_code = cmfgame->original_onmessage(arch_message, arch_queue);
   _restore_reg_ctx(saved_regs);

   return iowin_return_code;
}
}
