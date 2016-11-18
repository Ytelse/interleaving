#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "i_defs.h"
#include "interleave.h"


int main(int argc, char *argv[]) {

  if(argc != 4) {
    printf("Usage: %s number_of_images number_of_interleaved bit_width\n", argv[0]);
    exit(1);
  }

  int number_of_images = atoi(argv[1]);
  int number_of_interleaved = atoi(argv[2]);
  int bit_width = atoi(argv[3]);

  /* Allocate space for all the images */
  byte_t** img;
  img = (byte_t**) malloc(sizeof(byte_t*) * number_of_images);
  for (int i = 0; i < number_of_images; i++) {
    img[i] = (byte_t*)malloc(sizeof(byte_t)*IMG_SIZE);
  }

  /* Input image file */
  FILE* f;
  f = fopen(IMG_FP, "rb");

  if (!f) {
    printf("ERROR: Failed to open image set!\n");
    exit(1);
  }

  /* Load the images into memory */
  for (int n = 0; n < number_of_images; n++) {
    for (int px = 0; px < IMG_SIZE; px++) {
      img[n][px] = getc(f);
    }
    printf("\rReading images -- %2f%% done...", n / (number_of_images/100.0f));
  }
  printf("\rReading images -- 100%% done.\n");

  
  fclose(f);


  /* Allocate memory for interleaved images */
  byte_t *i_img_buf;
  init_i_img_buffer(&i_img_buf, IMG_X, IMG_Y, number_of_interleaved, INTERLEAVE_PACKED);


  puts("Initialized interleave buffer");

  
  if(number_of_images % number_of_interleaved != 0) {
    puts("ERROR: Number of images to interleave has to divide to total number of images");
    exit(1);
  }

  /* Create data folder if it does not exist */
  struct stat st = {0};
  if (stat("data", &st) == -1) {
    puts("Creating the data directory");
    mkdir("data", 0700);
  }


  // Construct the name for the interleaved file
  char out_name[80];
  sprintf(out_name, "data/total_%d_interleaved_%d_width_%d", number_of_images, number_of_interleaved, bit_width);
  f = fopen(out_name, "wb");

  if(!f) {
    printf("ERROR: Could not open %s\n", out_name);
    exit(1);
  }

  /* Interleave the images and write one byte at a time */
  for (int i = 0; i < number_of_images/number_of_interleaved; i++) {
    interleave(&img[i * number_of_interleaved], i_img_buf, number_of_interleaved, bit_width, IMG_X, IMG_Y, INTERLEAVE_PACKED, THRESHOLD);
    for (int i = 0; i < (IMG_X*IMG_Y*number_of_interleaved)/8; i++) {
      putc(i_img_buf[i], f);
    }
  }

  puts("Interleaving was successful!");

  fclose(f);

  return 0;
}
