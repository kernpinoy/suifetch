#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>


int main(void) {
    // ASCII logo || ADD YOUR OWN ASCII IF YOU WANT.
    const char* ascii_cat =
        " /\\_/\\  \n"
        "( o.o ) \n"
        " > ^ <  \n";

    printf("\e[33m%s\x1b[0m", ascii_cat);

    // Open the /etc/os-release file to read the Distro name of your sys.
    FILE* os_release_file = fopen("/etc/os-release", "r");
    if (os_release_file == NULL) {
        perror("Failed to fetch OS-NAME");
        return 1;
    }

    char line[256];
    char os_name[256] = "";

    // Reading the file line by line.
    while (fgets(line, sizeof(line), os_release_file)) {
        // Checks for specific lines containing OS/Version & info.
        if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
            sscanf(line, "PRETTY_NAME=\"%[^\"]\"", os_name);
        }
    }

    // Close the file
    fclose(os_release_file);

    // Prints the OS and version INFO.
    if (strlen(os_name) > 0) {
        printf("\n\e[36mos:\e[0m %s", os_name);
    } else {
        printf("OS Information not available!");
    }

    // Fetching the username of your sys.
    if (SHOW_USER == 1) {
        char* username = getenv("USER");
        if (username != NULL)
            printf("\n\e[31muser:\e[0m %s\n", username);
        else {
            perror("Failed to fetch user data.");
        }
    }

    // Fetching the window manager name
if (SHOW_WM == 1) {
    Display* display = XOpenDisplay(NULL);
    if (display != NULL) {
        Window root = DefaultRootWindow(display);

        // Try to get the window manager name using XFetchName
        char* wm_name = NULL;
        if (XFetchName(display, root, &wm_name) != 0) {
            printf("\x1b[35mwm:\x1b[0m %s\n", wm_name);
            XFree(wm_name);
        } else {
            printf("Window Manager information not available.\n");
        }

        XCloseDisplay(display);
    } else {
        perror("Failed to open X display");
    }
}
    if (SHOW_TERM == 1) {
        char* term = getenv("TERM");
        if (term != NULL) {
            printf("\x1b[32mterm:\x1b[0m %s \n", getenv("TERM"));
        }
    }

    // Fetching the uptime of your sys.
    FILE* uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        perror("Failed to open /proc/uptime");
        return 1;
    }

    int uptime_seconds;
    if (fscanf(uptime_file, "%d", &uptime_seconds) != 1) {
        perror("Failed to read uptime from /proc/uptime");
        fclose(uptime_file);
        return 1;
    }

    fclose(uptime_file);

    int hours = (int)(uptime_seconds / 3600);
    int minutes = ((int)uptime_seconds % 3600) / 60;
    printf("\x1b[34mup:\x1b[0m %d hours, %d mins\n", hours, minutes);

    return 0;
}
