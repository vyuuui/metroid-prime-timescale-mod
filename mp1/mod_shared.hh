#pragma once

#include "freestanding.hh"
#include "mp1/rstl/string.hh"

constexpr float kNormalDt = 1.f / 60.f;
constexpr double kNormalDtLong = 1.0 / 60.0;

// Release for each source file
void cmfgame_hooks_release();
void partfx_timescale_release();

// Suspend for each source file
void cmfgame_hooks_suspend();
void partfx_timescale_suspend();

// Good mod-global vars
extern bool dt_just_changed;

extern "C" {
// CMFGame Hooks
extern float game_timescale;
extern bool show_logs;

// Mod Shared
extern char debug_output[4096];
}

// Cleanup stuffs
extern "C" {
extern int shutdown_signal;
void release_mod();
}
