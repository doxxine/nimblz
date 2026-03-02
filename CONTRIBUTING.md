# Contributing to nimblz

## Build

```bash
# Prerequisites (Debian/Ubuntu)
sudo apt install libglib2.0-dev cmake pkg-config bluez

# Build
mkdir build && cd build
cmake ..
make

# Or use the convenience Makefile
make build
```

## Test

```bash
cd build && ctest --output-on-failure

# Or from repo root
make test
```

Tests use plain `assert()` and a mock D-Bus layer (`test/mock_dbus.c`). No external test framework required.

## Code Style

- **C11**, compiled with `-Wall -Wextra -Wpedantic`
- All public symbols prefixed with `nimblz_`
- Functions return `0` on success, negative `errno` on failure
- Opaque context pattern: `nimblz_t*`
- Callbacks follow `nimblz_<type>_cb(ctx, ..., void* user)` pattern
- camelCase for multi-word identifiers in public API
- snake_case for internal functions

## File Organization

| Directory | Purpose |
|-----------|---------|
| `include/nimblz/` | Public API headers — changes here are breaking |
| `src/` | Implementation (D-Bus interaction with BlueZ) |
| `src/quirks/` | Chip-specific workarounds |
| `test/` | Unit tests |
| `examples/` | Working examples for peripheral, central, dual-role |
| `yocto/` | BitBake recipe |

## Adding a New Chip Quirk

1. Create `src/quirks/<vendor>.c` with `nimblz_quirks_<vendor>_pre_init()` and `_post_init()`
2. Add the vendor to `nimblz_chip_vendor_t` enum in `src/quirks/quirks.h`
3. Add detection logic in `nimblz_quirks_detect()` in `src/quirks/quirks.c`
4. Add dispatch cases in `nimblz_quirks_apply_pre_init()` and `_post_init()`
5. Add the `.c` file to `CMakeLists.txt`

## Commit Style

```
<emoji> <concise description>

Co-Authored-By: ...
```

Example: `🔧 Fix Intel AX210 advertising restart delay`

## Pull Requests

- One feature/fix per PR
- Tests must pass (`ctest`)
- Code must compile without warnings
- Update examples if the API changes
