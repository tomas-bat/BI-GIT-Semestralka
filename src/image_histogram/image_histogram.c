#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void count_hist(const unsigned char *img, int width, int height, int *hist) {
    for (int i = 0; i < width * height; i++) {
        hist[img[i]]++;
    }
}

void print_hist(const int *hist) {
    for (int i = 0; i < 256; i++)
        printf("%d, ", hist[i]);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image path>\n", argv[0]);
        return 0;
    }

    int width, height, channels;
    unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 1);
    if (img == NULL) {
        fprintf(stderr, "Error loading image");
        return 1;
    }

    int hist[256] = {0};
    count_hist(img, width, height, hist);

    print_hist(hist);

    return 0;
}
