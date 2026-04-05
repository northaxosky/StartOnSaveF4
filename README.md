# Start On Save F4

F4SE plugin that automatically loads a save game when Fallout 4 starts, skipping the main menu. Supports filtering by save type (quicksave, autosave, manual, exit), character name, or specific save file. Hold SHIFT during startup to skip auto-load. Configurable via INI. Works on OG (1.10.163), NG, and AE runtimes.

Originally by [powerofthree](https://www.nexusmods.com/fallout4/users/2148728). This fork adds OG runtime support using [Dear Modding FO4's CommonLibF4](https://github.com/Dear-Modding-FO4/commonlibf4).

## Requirements

- [Fallout 4 Script Extender (F4SE)](https://www.nexusmods.com/fallout4/mods/42147)
- [Address Library for F4SE Plugins](https://www.nexusmods.com/fallout4/mods/47327)

## Installation

Drop the DLL (and optionally the PDB) into `Data/F4SE/Plugins/`. The INI file is auto-generated on first launch at `Data/F4SE/Plugins/po3_StartOnSave.ini`.

## Configuration

All settings are in `po3_StartOnSave.ini`:

| Setting | Default | Description |
|---------|---------|-------------|
| Save File | *(blank)* | Load a specific save by filename. If blank, loads the last save. |
| Character Name | *(blank)* | Filter saves by character name. If blank, considers all characters. |
| Save Type | 0 | 0-4: last (any/quick/auto/manual/exit). 5-9: first (same order). |
| Skip AutoLoad Hotkey | 16 | Virtual key code to skip auto-load (default: SHIFT). |
| Start New Game | true | Start a new game if no saves exist. |
| Disable Missing Content Warning | false | Suppress the missing mods warning on load. |

## Building

**Prerequisites:** Visual Studio 2022, CMake 3.21+, vcpkg (`VCPKG_ROOT` env var), [CommonLibF4](https://github.com/Dear-Modding-FO4/commonlibf4) at `../commonlibf4` (or set `CommonLibF4Path` env var).

```bash
cmake --preset vs2022-windows-vcpkg-f4
cmake --build build --config Release
```

Output: `build/Release/po3_StartOnSaveF4.dll`
