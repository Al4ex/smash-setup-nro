#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <switch.h>

#define TITLE_ID "01006A800016E000"
#define SD_TITLE "sdmc:/atmosphere/contents/" TITLE_ID
#define SD_PLUGINS SD_TITLE "/romfs/skyline/plugins"
#define SD_EXEFS SD_TITLE "/exefs"
#define SD_UNLOCK_FLAG "sdmc:/ultimate/mods/Unlock All Characters/flags/unlock_all_fighters.flag"

typedef struct {
    const char *romfs_path;
    const char *sd_path;
    const char *label;
} InstallItem;

static const InstallItem kItems[] = {
    {"romfs:/exefs/main.npdm", SD_EXEFS "/main.npdm", "Skyline main.npdm"},
    {"romfs:/exefs/subsdk9", SD_EXEFS "/subsdk9", "Skyline subsdk9"},
    {"romfs:/plugins/libnro_hook.nro", SD_PLUGINS "/libnro_hook.nro", "nro-hook"},
    {"romfs:/plugins/libarcropolis.nro", SD_PLUGINS "/libarcropolis.nro", "ARCropolis"},
    {"romfs:/plugins/libthe_csk_collection.nro", SD_PLUGINS "/libthe_csk_collection.nro", "CSK Collection"},
    {"romfs:/mods/Unlock All Characters/flags/unlock_all_fighters.flag", SD_UNLOCK_FLAG,
     "Unlock all fighters flag"},
};

static bool path_is_file(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISREG(st.st_mode);
}

static bool path_is_dir(const char *path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

static bool ensure_parent_dirs(const char *file_path) {
    char dir[512];
    size_t len = strnlen(file_path, sizeof(dir) - 1);
    if (len == 0 || len >= sizeof(dir)) {
        return false;
    }

    memcpy(dir, file_path, len + 1);
    for (size_t i = len; i > 0; i--) {
        if (dir[i] == '/') {
            dir[i] = '\0';
            if (dir[0] != '\0' && !path_is_dir(dir)) {
                if (mkdir(dir, 0777) != 0 && !path_is_dir(dir)) {
                    return false;
                }
            }
            break;
        }
    }
    return true;
}

static bool copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    if (!in) {
        return false;
    }

    if (!ensure_parent_dirs(dst)) {
        fclose(in);
        return false;
    }

    FILE *out = fopen(dst, "wb");
    if (!out) {
        fclose(in);
        return false;
    }

    unsigned char buffer[8192];
    size_t read = 0;
    while ((read = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, read, out) != read) {
            fclose(in);
            fclose(out);
            return false;
        }
    }

    fclose(in);
    fclose(out);
    return true;
}

static void print_header(void) {
    printf(CONSOLE_GREEN "Smash Setup" CONSOLE_RESET " v1.0.0\n");
    printf("Repara Skyline + plugins despues de Nextendo.\n\n");
}

int main(int argc, char **argv) {
    PadState pad;

    (void)argc;
    (void)argv;

    consoleInit(NULL);
    romfsInit();
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);
    padInitializeAny(&pad);

    print_header();

    int installed = 0;
    int skipped = 0;
    int failed = 0;

    for (size_t i = 0; i < sizeof(kItems) / sizeof(kItems[0]); i++) {
        const InstallItem *item = &kItems[i];
        printf("[%zu/%zu] %s\n", i + 1, sizeof(kItems) / sizeof(kItems[0]), item->label);

        if (path_is_file(item->sd_path)) {
            printf("  -> ya existe, omitido\n");
            skipped++;
            continue;
        }

        if (!path_is_file(item->romfs_path)) {
            printf(CONSOLE_RED "  -> falta en romfs del NRO\n" CONSOLE_RESET);
            failed++;
            continue;
        }

        if (copy_file(item->romfs_path, item->sd_path)) {
            printf(CONSOLE_GREEN "  -> instalado\n" CONSOLE_RESET);
            installed++;
        } else {
            printf(CONSOLE_RED "  -> error al copiar\n" CONSOLE_RESET);
            failed++;
        }
    }

    printf("\nResumen: %d instalados, %d omitidos, %d errores\n", installed, skipped, failed);
    printf("\nAbre Smash y revisa ARCropolis en el menu eShop.\n");
    printf("Presiona + para salir.\n");

    while (appletMainLoop()) {
        padUpdate(&pad);
        u64 k = padGetButtonsDown(&pad);
        if (k & HidNpadButton_Plus) {
            break;
        }
    }

    romfsExit();
    consoleExit(NULL);
    return failed > 0 ? 1 : 0;
}
