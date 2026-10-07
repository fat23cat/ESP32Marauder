# Cardputer ADV build for the CRUB shared `extra` slot

This branch integrates upstream ESP32 Marauder v1.18.0 and is intended for the 8 MiB
Cardputer ADV layout maintained by
[`cardputer-firmware-manager`](https://github.com/fat23cat/cardputer-firmware-manager).
It builds an **application image**, not a merged image for flashing at address
zero. Install it through CRUB's `upmarauder` command after staging it on SD.

The build mounts `marauder_fs` at `0x7d0000` and leaves Bruce's `spiffs` at
`0x7b0000` untouched. It isolates Marauder's backlight preference and Bluetooth
bonds in NVS, disables its own firmware updater on this layout, and avoids
stopping stale NimBLE objects after BLE spam exits. The manager's build script
applies [`patches/nimble-crub.patch`](patches/nimble-crub.patch) to the pinned
NimBLE library; a plain upstream build without that patch does not isolate
Bluetooth bonds.

On Cardputer ADV, hold **Shift+9** to leave an active scan or BLE screen. The
key produces `(`, which Marauder uses for exit; Fn+9 produces `9`. `BLE Spam
All` now handles one payload per main loop so the keyboard is checked between
steps.

The focused native tests are:

```sh
pio test -e native -f test_ble_spam_cycle -f test_marauder_ble_lifecycle
```

For the full build and installation steps, use the manager's
`docs/install-marauder.md` guide and verify the selected branch commit before
staging the application image.

The manager currently pins the original v1.17.0 custom commit. Updating this
fork alone does not change that pin: its build script and `firmware-manager.json`
must select the same updated commit before rebuilding through the manager.

## v1.18.0 compatibility

All CRUB customizations remain necessary. Upstream's NimBLE client lifecycle
repair applies to NimBLE 2; Cardputer ADV still uses NimBLE 1.3.8 and needs the
stale advertising/scan pointer protection above. Upstream's iBeacon callback
drain and memory pressure safeguards complement the per-loop keyboard handling.
The SD updater null-menu fix already present in v1.17.0 is retained through
upstream history, without an additional local copy.
