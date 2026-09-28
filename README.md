# YD Numpad firmware (QMK + Vial)

Handwired 7x5 numpad on YD-RP2040 / Raspberry Pi Pico.

## Build

GitHub Actions builds firmware automatically on every push.

1. Open the **Actions** tab.
2. Wait for **Build firmware** to finish.
3. Download the artifact `yd-numpad-vial`.
4. Put the Pico into BOOTSEL mode and copy the `.uf2` file to the `RPI-RP2` drive.

You can also start a build manually: Actions → Build firmware → Run workflow.

## After flashing

Open https://vial.rocks and unlock with **ESC + VOLU** (top-left + top-right).

## Pins

- Columns: GP11 GP10 GP9 GP8 GP7
- Rows: GP6 GP5 GP4 GP3 GP2 GP1 GP0
- Diodes: COL2ROW
- RGB: GP12, 27x WS2812B
