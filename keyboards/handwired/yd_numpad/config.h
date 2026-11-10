#pragma once

/* Bootloader: dwa szybkie wciśnięcia RESET -> tryb UF2 (RPI-RP2) */
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 200U

/* USB: odczekaj po wybudzeniu hosta (Mac sleep -> wake), zanim numpad zacznie wysyłać raporty */
#define USB_SUSPEND_WAKEUP_DELAY 200

/* Stabilność (logika w yd_numpad.c):
 * - sprzętowy watchdog RP2040: gdy firmware się zawiesi -> automatyczny restart po ~2 s
 * - "martwe uśpienie": numpad myśli, że host śpi, wciskasz klawisz, a host nie wraca
 *   przez 5 s -> automatyczny restart (zamiast odłączania kabla)
 */
#define YD_WATCHDOG_TIMEOUT_MS 2000
#define YD_DEAD_SUSPEND_TIMEOUT_MS 5000
