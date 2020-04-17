#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define HIST_HEIGHT 10

/// Get width of terminal as a power of 2 (for easy binning) up to 256.
/// \return Terminal width or 64 if fails.
int get_hist_width() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    int hist_width = w.ws_col;
    if (hist_width == 0 || hist_width > 400)
        return 64;

    hist_width = 256;
    while (hist_width > w.ws_col)
        hist_width /= 2;
    return hist_width;
}

/// Count 8bpp histogram of the image.
/// \param img          Image array.
/// \param width        Width of the image.
/// \param height       Height of the image.
/// \param hist[out]    Pointer to array where histrogram is accumulated.
void count_hist(const unsigned char *img, int width, int height, int *hist) {
    for (int i = 0; i < width * height; i++) {
        hist[img[i]]++;
    }
}

/// Group histogram to bins.
/// \param hist         Original histogram.
/// \param bin_count    Number of output bins
/// \return             Newly allocated array with absolute number of pixels in each bin.
float *count_bins(const int *hist, int bin_count) {
    float *bins = calloc(bin_count, sizeof(float));

    int acc_per_bin = 256 / bin_count;
    for (int i = 0; i < 256; i += acc_per_bin) {
        for (int j = i; j < i + acc_per_bin; j++) {
            bins[i / acc_per_bin] += (float) hist[j];
        }
    }

    return bins;
}

/// Normalize bins such that the maximum value is HIST_HEIGHT
/// \param bins[in,out] Bins to be normalised in place.
/// \param bin_count    Bin count.
void normalise_bins(float *bins, int bin_count) {
    float max = 0;
    for (int i = 0; i < bin_count; i++) {
        if (bins[i] > max)
            max = bins[i];
    }

    for (int i = 0; i < bin_count; i++) {
        bins[i] = bins[i] / max * HIST_HEIGHT;
    }
}

/// Print the histogram.
/// \param bins     Bins to be printed.
/// \param width    Number of bins.
void print_hist(const float *bins, int width) {
    char *boxes[16] = {" ", "▁", "▁", "▂", "▂", "▃", "▃", "▄", "▄", "▅", "▅", "▆", "▆", "▇", "▇", "█"};

    for (int y = HIST_HEIGHT; y > 0; y--) {
        for (int x = 0; x < width; x++) {
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

    int bin_count = get_hist_width();
    float *bins = count_bins(hist, bin_count);

    normalise_bins(bins, bin_count);

    print_hist(bins, bin_count);

    free(bins);

    return 0;
}
