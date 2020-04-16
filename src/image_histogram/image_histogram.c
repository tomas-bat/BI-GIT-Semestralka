#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void count_hist(const unsigned char *img, int width, int height, int *hist) {
    for (int i = 0; i < width * height; i++) {
        hist[img[i]]++;
    }
}

float *count_bins(const int *hist) {
    float *bins = calloc(64, sizeof(float));

    for (int i = 0; i < 256; i += 4) {
        for (int j = i; j < i + 4; j++) {
            bins[i / 4] += (float) hist[j];
        }
    }

    return bins;
}

void normalise_bins(float *bins) {
    float max = 0;
    for (int i = 0; i < 64; i++) {
        if (bins[i] > max)
            max = bins[i];
    }

    for (int i = 0; i < 64; i++) {
        bins[i] = bins[i] / max * 10;
    }
}

void print_hist(const float *bins) {
    char *boxes[16] = {" ", "▁", "▁", "▂", "▂", "▃", "▃", "▄", "▄", "▅", "▅", "▆", "▆", "▇", "▇", "█"};

    for (int y = 10; y > 0; y--) {
        for (int x = 0; x < 64; x++) {
            int index = (int) floorf((bins[x] - (float) y + 1) * 16);
            if (index < 0)
                index = 0;
            else if (index > 15)
                index = 15;
            printf("%s", boxes[index]);
        }
        printf("\n");
    }
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

    float *bins = count_bins(hist);

    normalise_bins(bins);

    print_hist(bins);

    free(bins);

    return 0;
}
