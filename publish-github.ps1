# Crea el repo en GitHub, hace push y espera el build del NRO.
$ErrorActionPreference = 'Stop'
$env:Path = [System.Environment]::GetEnvironmentVariable('Path','Machine') + ';' + [System.Environment]::GetEnvironmentVariable('Path','User')

$project = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $project

gh auth status | Out-Null

$scopes = (gh auth status 2>&1 | Out-String)
if ($scopes -notmatch 'workflow') {
    Write-Host "Falta permiso 'workflow' para subir GitHub Actions." -ForegroundColor Yellow
    Write-Host "Ejecuta: gh auth refresh -h github.com -s workflow" -ForegroundColor Yellow
    exit 1
}

$hasOrigin = [bool](git remote 2>$null | Where-Object { $_ -eq 'origin' })
if (-not $hasOrigin) {
    gh repo create smash-setup-nro --public --source=. --remote=origin --push --description "Switch homebrew to repair Smash Skyline and plugins after Nextendo"
} else {
    git push -u origin main
}

Write-Host "`nEsperando workflow de build..."
gh run watch --exit-status

$runId = (gh run list --workflow=build.yml --limit 1 --json databaseId --jq '.[0].databaseId')
Write-Host "Descargando artifact del run $runId..."
gh run download $runId -n smash-setup-nro -D "$project\dist"

Write-Host "`nListo:"
Get-ChildItem "$project\dist" -Recurse -Filter '*.nro' | Select-Object FullName, Length
