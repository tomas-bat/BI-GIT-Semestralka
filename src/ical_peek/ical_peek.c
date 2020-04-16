#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int starts_with(const char *str, const char *prefix) {
    size_t len_prefix = strlen(prefix),
            len_str = strlen(str);
    return len_str < len_prefix ? 0 : memcmp(prefix, str, len_prefix) == 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <iCal path>\n", argv[0]);
        return 0;
    }

    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Unable to open file");
        return 1;
    }

    read = getline(&line, &len, fp);
    if (read == -1) {
        fprintf(stderr, "Empty file");
        return 1;
    }

    if (!starts_with(line, "BEGIN:VCALENDAR")) {
        fprintf(stderr, "Not an iCal file, missing VCALENDAR object.");
        return 1;
    }


    char event_summary[300];

    while (getline(&line, &len, fp) != -1) {
        if (starts_with(line, "BEGIN:VEVENT")) {
            event_summary[0] = '\0';
        }
        if (starts_with(line, "SUMMARY")) {
            strlcpy(event_summary, strchr(line, ':') + 1, 300);
            *strchr(event_summary,'\r') = '\0';
        }
        if (starts_with(line, "END:VEVENT")) {
            printf("%s\n", event_summary);
        }
    }

    free(line);
    return 0;
}
