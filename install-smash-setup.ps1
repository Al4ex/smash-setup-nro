# Repara Smash en la SD desde PC (misma logica que el NRO)
param(
    [string]$Drive = 'E'
)

$root = "${Drive}:\"
if (-not (Test-Path "$root\atmosphere")) {
    Write-Error "No se encontro SD en $root"
    exit 1
}

$items = @(
    @{ Src = 'romfs\exefs\main.npdm'; Dst = 'atmosphere\contents\01006A800016E000\exefs\main.npdm' },
    @{ Src = 'romfs\exefs\subsdk9'; Dst = 'atmosphere\contents\01006A800016E000\exefs\subsdk9' },
    @{ Src = 'romfs\plugins\libnro_hook.nro'; Dst = 'atmosphere\contents\01006A800016E000\romfs\skyline\plugins\libnro_hook.nro' },
    @{ Src = 'romfs\plugins\libarcropolis.nro'; Dst = 'atmosphere\contents\01006A800016E000\romfs\skyline\plugins\libarcropolis.nro' },
    @{ Src = 'romfs\plugins\libthe_csk_collection.nro'; Dst = 'atmosphere\contents\01006A800016E000\romfs\skyline\plugins\libthe_csk_collection.nro' },
    @{ Src = 'romfs\mods\Unlock All Characters\flags\unlock_all_fighters.flag'; Dst = 'ultimate\mods\Unlock All Characters\flags\unlock_all_fighters.flag' }
)

$project = Split-Path -Parent $MyInvocation.MyCommand.Path
$installed = 0
$skipped = 0

foreach ($item in $items) {
    $src = Join-Path $project $item.Src
    $dst = Join-Path $root $item.Dst
    if (-not (Test-Path $src)) {
        Write-Host "FALTA en proyecto: $($item.Src)" -ForegroundColor Red
        continue
    }
    if (Test-Path $dst) {
        Write-Host "OK (ya existe): $($item.Dst)"
        $skipped++
        continue
    }
    $dir = Split-Path $dst -Parent
    New-Item -ItemType Directory -Path $dir -Force | Out-Null
    Copy-Item $src $dst -Force
    Write-Host "INSTALADO: $($item.Dst)" -ForegroundColor Green
    $installed++
}

Write-Host "`nListo: $installed instalados, $skipped omitidos."
