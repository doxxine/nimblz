# nimblz — BLE Reliability Layer for BlueZ

**nimblz** is a C library that wraps BlueZ/D-Bus and presents a [NimBLE](https://github.com/apache/mynewt-nimble)-like API for embedded Linux. It handles the reliability issues that every team fights when using BlueZ on Yocto/embedded:

- Stale connection caches and ghost devices
- Advertising not restarting after disconnect
- Chip-specific firmware quirks (Intel, Broadcom, Realtek)
- Multi-connection and dual-role (server + client) complexity

## Features

- **NimBLE-like API** — init/destroy lifecycle, register services, notify, callbacks
- **Automatic advertising restart** — re-advertise after peer disconnect
- **Stale device cleanup** — remove ghost D-Bus device objects
- **Multi-connection** — track multiple peers simultaneously
- **Dual-role** — run GATT server and client at the same time
- **Adapter watchdog** — detect unhealthy adapters, auto-reset via HCI
- **Chip quirks** — auto-detect Intel/Broadcom/Realtek and apply workarounds

## Quick Start

```bash
# Build
mkdir build && cd build
cmake ..
make

# Run tests
ctest

# Or use the Makefile wrapper
make build
make test
```

## Dependencies

- **GLib 2.0** / **GIO 2.0** — D-Bus communication
- **BlueZ 5.x** — runtime BLE stack
- **CMake >= 3.16** — build system
- **pkg-config** — dependency discovery

On Debian/Ubuntu:
```bash
sudo apt install libglib2.0-dev cmake pkg-config bluez
```

## API Overview

```c
#include <nimblz/nimblz.h>

// Initialize (NULL config = sensible defaults)
nimblz_t *ctx = nimblz_init(NULL);

// Register GATT services
nimblz_gatt_server_register(ctx, services, count);

// Start advertising
nimblz_adv_params_t adv = { .local_name = "MyDevice" };
nimblz_advertise_start(ctx, &adv);

// Run event loop (blocking)
nimblz_run(ctx);

// Cleanup
nimblz_destroy(ctx);
```

### Peripheral (GATT Server)

See [`examples/peripheral.c`](examples/peripheral.c) — minimal GATT server exposing temperature + command characteristics.

### Central (GATT Client)

See [`examples/central.c`](examples/central.c) — scan for peripherals, connect, subscribe to notifications.

### Dual-Role

See [`examples/dual_role.c`](examples/dual_role.c) — simultaneous server (mobile app) + client (temperature probes).

## Architecture

```
┌────────────────────────────────────┐
│          Your Application          │
├────────────────────────────────────┤
│         nimblz Public API          │
│  (nimblz.h — NimBLE-like calls)   │
├──────────┬──────────┬──────────────┤
│ advertise│connection│  gatt_server │
│          │          │  gatt_client │
├──────────┴──────────┴──────────────┤
│          D-Bus Helpers             │
│     (GDBus → BlueZ org.bluez)     │
├──────────┬──────────┬──────────────┤
│  Intel   │ Broadcom │   Realtek    │
│  quirks  │  quirks  │   quirks     │
├──────────┴──────────┴──────────────┤
│       BlueZ 5.x + Kernel          │
└────────────────────────────────────┘
```

## Project Structure

```
include/nimblz/     Public API headers (nimblz.h is single-include)
src/                Implementation
src/quirks/         Chip-specific workarounds
test/               Unit tests (plain assert + mock D-Bus)
examples/           Complete working examples
yocto/              BitBake recipe for Yocto integration
```

## Yocto Integration

Copy `yocto/nimblz.bb` to your Yocto layer and add `nimblz` to your image recipe:

```bitbake
IMAGE_INSTALL:append = " nimblz nimblz-examples"
```

## Status

**v0.1.0** — API design complete, compilable stubs with TODO markers. All public headers are final. Implementation of D-Bus integration is in progress.

## License

MIT — see [LICENSE](LICENSE).
