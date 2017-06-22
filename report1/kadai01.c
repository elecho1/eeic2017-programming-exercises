#include <stdio.h>
#include <stdlib.h>
#define OUTPUTFILE "result.pbm"

short int *basearray; 
short int **array;  

unsigned int xorshift() {
  static unsigned int x = 1;
  static unsigned int y = 2;
  static unsigned int z = 3;
  static unsigned int w = 4;
  unsigned int t;

  t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;

  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

double random_double() { return xorshift() * 1.0 / 4294967295.0; }

short int digital_generator(double p) {
  if (random_double() < p) {
    return 1;
  } else {
    return 0;
  }
}

void turn_value(short int *n) {
  if (*n == 0) {
    *n = 1;
  } else {
    *n = 0;
  }
}

void add_noise(double p, const int size_array) {
  for (int i = 0; i < size_array; i++)
    if (digital_generator(p) == 1) {
      turn_value(&basearray[i]);
    }
}

void load(FILE *fp, const int width, const int height) {
  int i, j;
  for (i = 0; i < height; i++) {
    array[i] = &basearray[i * width];
    for (j = 0; j < width; j++) {
      fscanf(fp, "%d", &array[i][j]);
    }
  }
}

void output(FILE *fp ,const char file_format[],const int width, const int height) {
  fprintf(fp, "%s\n", file_format);
  fprintf(fp, "%d %d\n", width, height);
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fprintf(fp, "%d ", array[i][j]);
    }
    fprintf(fp, "\n");
  }
}

int main(int argc, char *argv[]) {
  double p = atof(argv[1]);         
  FILE *fp1 = fopen(argv[2], "r"); 
  FILE *fp2;             
  char file_format[20]; 
  fscanf(fp1, "%[^\n]%*c", file_format);

  int width, height;

  fscanf(fp1, "%d %d", &width, &height);

  basearray = (short int *)malloc(sizeof(short int) * width * height);
  array = (short int **)malloc(sizeof(short int *) * height);

  load(fp1, width, height);
  fclose(fp1);
  add_noise(p, width*height);

  fp2 = fopen(OUTPUTFILE, "w");
  output(fp2,file_format,width, height);
  fclose(fp2);

  return 0;
}