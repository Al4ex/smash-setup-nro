# Smash Setup NRO

Homebrew para Nintendo Switch (CFW) que reinstala lo minimo de mods de Smash si Nextendo u otra cosa lo borro.

## Que instala (solo si falta)

- `atmosphere/contents/01006A800016E000/exefs/main.npdm`
- `atmosphere/contents/01006A800016E000/exefs/subsdk9`
- `atmosphere/contents/01006A800016E000/romfs/skyline/plugins/libnro_hook.nro`
- `atmosphere/contents/01006A800016E000/romfs/skyline/plugins/libarcropolis.nro`
- `atmosphere/contents/01006A800016E000/romfs/skyline/plugins/libthe_csk_collection.nro`
- `ultimate/mods/Unlock All Characters/flags/unlock_all_fighters.flag`

No sobrescribe archivos que ya existen.

## Compilar

Requisitos: [devkitPro](https://devkitpro.org/) con libnx.

```bash
make
```

Genera `smash_setup.nro`.

Tambien puedes compilar con GitHub Actions (workflow incluido) y descargar el artifact.

## Instalar en la Switch

Copia `smash_setup.nro` a:

`sd:/switch/smash_setup.nro`

Ejecutalo desde el Homebrew Menu despues de instalar Nextendo si Smash dejo de cargar mods.

## Flujo recomendado

1. Instalas/actualizas Nextendo.
2. Abres **Smash Setup**.
3. Abres Smash y verificas ARCropolis + desbloqueo de personajes.
