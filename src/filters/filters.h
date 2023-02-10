#ifndef FILTERS_H
#define FILTERS_H

// -- COLOR THRESHOLD HEADER -- //

// Colorize pixel using a defined threshold and two RGB colors.
Uint32 colorize_pixel(Uint32 pixel_color, SDL_PixelFormat* format,
                      Uint8 threshold,
                      Uint8 r1, Uint8 g1, Uint8 b1,
                      Uint8 r2, Uint8 g2, Uint8 b2);

// Colorize SDL_Surface* using a defined threshold and two RGB colors.
SDL_Surface* colorize(SDL_Surface *s_surface, Uint8 threshold,
                      int s_width, int s_height,
                      Uint8 r1, Uint8 g1, Uint8 b1,
                      Uint8 r2, Uint8 g2, Uint8 b2);

// Apply color thresholding to SDL_Surface*.
SDL_Surface* color_threshold(SDL_Surface *s_surface, Uint8 threshold,
                            Uint8 r1, Uint8 g1, Uint8 b1,
                            Uint8 r2, Uint8 g2, Uint8 b2);

// -- GAUSSIAN HEADER -- //

// Compute gaussian mean.
Uint32 mean(SDL_Surface *s_surface, int i, int j, int radius,
            int s_width, int s_height);

// Apply gaussian blur.
SDL_Surface* gaussian(SDL_Surface *s_surface, int radius);

// -- OTSU THRESHOLD HEADER -- //

// Compute otsu's histogram.
Uint8 get_histogram_threshold_otsu(int *histogram, int start, int end);

// Call otsu's histogram calculation function.
Uint8 get_histogram_threshold(int *histogram, int start, int end);

// Apply otsu thresholding to SDL_Surface*.
SDL_Surface* otsu_threshold(SDL_Surface *s_surface);

// -- THRESHOLD HEADER -- //

// Binarize pixel using a defined threshold.
Uint32 binarize_pixel(Uint32 pixel_color, SDL_PixelFormat* format,
                      Uint8 threshold);

// Binarize SDL_Surface* using a defined threshold.
SDL_Surface* binarize(SDL_Surface *s_surface, Uint8 threshold,
                      int s_width, int s_height);

// Apply otsu thresholding to SDL_Surface*.
SDL_Surface* threshold(SDL_Surface *s_surface, Uint8 threshold);

// -- SEPIA HEADER -- //

// Apply sepia filter.
SDL_Surface* sepia(SDL_Surface* s_surface);

#endif