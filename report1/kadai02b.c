#include <stdio.h>
#include <stdlib.h>
#define OUTPUTFILE "result.pbm"
#define DECODEFILE "decoderesult.pbm"
#define BETA 1
#define ETA 2

short int *basearray;
short int *basearray_new;
short int *basearray_backup;
short int **array;
short int **array_new;
short int **array_backup;
int width, height;

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

void load(FILE *fp, short int **array) {
  int i, j;
  for (i = 0; i < height; i++) {
    array[i] = &basearray[i * width];
    for (j = 0; j < width; j++) {
      fscanf(fp, "%d", &array[i][j]);
    }
  }
}

void output(FILE *fp, short int **arrayname, const char file_format[]) {
  fprintf(fp, "%s\n", file_format);
  fprintf(fp, "%d %d\n", width, height);
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      fprintf(fp, "%d ", arrayname[i][j]);
    }
    fprintf(fp, "\n");
  }
}

double culculate_energy(const int x, const int y, const short int value,
                        double beta, double eta) {
  double deltaE;
  double Xi = 2 * value - 1;
  deltaE = -1 * eta * Xi * (2 * array_new[x][y] - 1);

  beta = 2*beta;

  if (x == 0) {
    deltaE -= beta * Xi * (2 * array_new[height - 1][y] - 1);
    deltaE -= beta * Xi * (2 * array_new[1][y] - 1);
  } else if (x == height - 1) {
    deltaE -= beta * Xi * (2 * array_new[0][y] - 1);
    deltaE -= beta * Xi * (2 * array_new[height - 2][y] - 1);
  } else {
    deltaE -= beta * Xi * (2 * array_new[x + 1][y] - 1);
    deltaE -= beta * Xi * (2 * array_new[x - 1][y] - 1);
  }

  if (y == 0) {
    deltaE -= beta * Xi * (2 * array_new[x][width - 1] - 1);
    deltaE -= beta * Xi * (2 * array_new[x][1] - 1);
  } else if (y == width - 1) {
    deltaE -= beta * Xi * (2 * array_new[x][0] - 1);
    deltaE -= beta * Xi * (2 * array_new[x][width - 2] - 1);
  } else {
    deltaE -= beta * Xi * (2 * array_new[x][y + 1] - 1);
    deltaE -= beta * Xi * (2 * array_new[x][y - 1] - 1);
  }
  return deltaE;
}

int count_dif(short int *array1, short int *array2) {
  int count = 0;
  int i;
  for (i = 0; i < height*width; i++) {
    if (array1[i] != array2[i]) {
      count++;
    }
  }
  return count;
}

void copy_array(short int *to, short int *from) {
  int i;
  for (i = 0; i < height * width; i++) {
    to[i] = from[i];
  }
  return;
}

void decode(const int width, const int height, const double beta,
            const double eta) {
  // FILE *fptest = fopen("test.pbm", "w");
  int i, j;
  int x, y;
  double deltaE0, deltaE1;

  basearray_new = (short int *)malloc(sizeof(short int) * width * height);
  array_new = (short int **)malloc(sizeof(short int *) * height);
  for (i = 0; i < height; i++) {
    array_new[i] = &basearray_new[i * width];
    for (j = 0; j < width; j++) {
      array_new[i][j] = array[i][j];
    }
  }

  while (1) {
    for (x = 0; x < height; x++) {
      for (y = 0; y < width; y++) {
        deltaE0 = culculate_energy(x, y, 0, beta, eta);
        deltaE1 = culculate_energy(x, y, 1, beta, eta);
        // fprintf(fptest,"%f %f\n", deltaE0, deltaE1);
        if (deltaE0 < deltaE1) {
          array_new[x][y] = 0;
        } else {
          array_new[x][y] = 1;
        }
      }
      // fprintf(fptest, "\n");
    }
    if (count_dif(basearray_new, basearray) == 0) {
      break;
    }
    copy_array(basearray, basearray_new);
  }

  // fclose(fptest);

  return;
}

int main(int argc, char *argv[]) {
  int i;
  double noiserate;
  double p = atof(argv[1]);
  FILE *fp1 = fopen(argv[2], "r");
  FILE *fp2;
  FILE *fp3;
  char file_format[20];
  fscanf(fp1, "%[^\n]%*c", file_format);

  double beta, eta;

  fscanf(fp1, "%d %d", &width, &height);
  //連続した配列を用意する
  //(http://tondol.hatenablog.jp/entry/20090713/1247426321)
  basearray = (short int *)malloc(sizeof(short int) * width * height);
  array = (short int **)malloc(sizeof(short int *) * height);

  load(fp1, array);
  fclose(fp1);

  basearray_backup = (short int *)malloc(sizeof(short int) * width * height);
  array_backup = (short int **)malloc(sizeof(short int *) * height);
  array_new = (short int **)malloc(sizeof(short int *) * height);
  for (i = 0; i < height; i++) {
    array_backup[i] = &basearray_backup[i * width];
  }
  copy_array(basearray_backup, basearray);

  for (i = 0; i <= 20;i++) {
    printf("p = %f\n", 0.05 * i);
    copy_array(basearray, basearray_backup);
    add_noise(i * 0.05, width * height);

    noiserate =
        (double)count_dif(basearray, basearray_backup) / (height * width) * 100;

    printf("noise rate = %f%%\n", noiserate);

    fp2 = fopen(OUTPUTFILE, "w");
    output(fp2, array, file_format);
    fclose(fp2);

    beta = BETA;
    eta = ETA;

    decode(width, height, beta, eta);

    fp3 = fopen(DECODEFILE, "w");
    output(fp3, array_new, file_format);
    fclose(fp3);

    noiserate = (double)count_dif(basearray_new, basearray_backup) /
                (height * width) * 100;

    printf("noise rate = %f%%\n", noiserate);
  }

  return 0;
}