// YD Numpad — automatyczne odzyskiwanie po zawieszeniu / zerwaniu USB
#include "quantum.h"
#include "suspend.h"

/* ---------- Watchdog RP2040 (rejestry wg datasheetu RP2040, rozdz. 4.7 i 2.13) ---------- */
#define YD_WDG_BASE        0x40058000u
#define YD_WDG_CTRL        (*(volatile uint32_t *)(YD_WDG_BASE + 0x00u))
#define YD_WDG_LOAD        (*(volatile uint32_t *)(YD_WDG_BASE + 0x04u))
#define YD_WDG_CTRL_ENABLE (1u << 30)
#define YD_WDG_CTRL_PAUSE  ((1u << 24) | (1u << 25) | (1u << 26)) /* pauza przy debugerze */

#define YD_PSM_BASE        0x40010000u
#define YD_PSM_WDSEL       (*(volatile uint32_t *)(YD_PSM_BASE + 0x08u))
/* resetuj wszystko poza oscylatorami ROSC/XOSC (jak w pico-sdk) */
#define YD_PSM_WDSEL_VALUE (0x0001FFFFu & ~0x3u)

/* Errata RP2040-E1: licznik watchdoga liczy 2x na tick (1 us) -> mnożnik 2 */
#define YD_WDG_LOAD_VALUE  ((uint32_t)YD_WATCHDOG_TIMEOUT_MS * 1000u * 2u)

static bool wdg_running = false;

static inline void wdg_feed(void) {
    if (wdg_running) {
        YD_WDG_LOAD = YD_WDG_LOAD_VALUE;
    }
}

static void wdg_start(void) {
    YD_WDG_CTRL &= ~YD_WDG_CTRL_ENABLE;
    YD_PSM_WDSEL = YD_PSM_WDSEL_VALUE;
    YD_WDG_LOAD  = YD_WDG_LOAD_VALUE;
    YD_WDG_CTRL |= YD_WDG_CTRL_PAUSE | YD_WDG_CTRL_ENABLE;
    wdg_running = true;
}

static void wdg_stop(void) {
    YD_WDG_CTRL &= ~YD_WDG_CTRL_ENABLE;
    wdg_running = false;
}

/* ---------- Hooki QMK ---------- */

void keyboard_post_init_kb(void) {
    wdg_start();
    keyboard_post_init_user();
}

/* normalna pętla główna */
void housekeeping_task_kb(void) {
    wdg_feed();
    housekeeping_task_user();
}

/* Przed restartem / skokiem do bootloadera wyłącz watchdog,
 * żeby nie przerwał trybu UF2 */
bool shutdown_kb(bool jump_to_bootloader) {
    wdg_stop();
    return shutdown_user(jump_to_bootloader);
}

/* ---------- Wykrywanie "martwego uśpienia" ----------
 * Gdy hub/port wyłączy numpad, RP2040 widzi to tak samo jak uśpienie hosta
 * i czeka w nieskończoność. Jeśli w tym stanie wciśniesz klawisz, a host nie
 * wznowi połączenia w ciągu YD_DEAD_SUSPEND_TIMEOUT_MS -> restart numpada.
 */
static bool     wake_pending = false;
static uint32_t wake_timer   = 0;

/* wywoływane w pętli, dopóki USB jest w stanie SUSPENDED */
void suspend_power_down_kb(void) {
    wdg_feed();

    if (!wake_pending) {
        if (suspend_wakeup_condition()) {
            wake_pending = true;
            wake_timer   = timer_read32();
        }
    } else if (timer_elapsed32(wake_timer) > YD_DEAD_SUSPEND_TIMEOUT_MS) {
        soft_reset_keyboard();
    }

    suspend_power_down_user();
}

/* host się wybudził — wszystko OK */
void suspend_wakeup_init_kb(void) {
    wake_pending = false;
    wdg_feed();
    suspend_wakeup_init_user();
}
