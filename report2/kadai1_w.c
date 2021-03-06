#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_SIZE 100
#define OPE_SIZE 20

/*
double plus_n(double x, double y) { return x + y; }
double times_n(double x, double y) { return x * y; }
double substract_n(double x, double y) { return x - y; }
double divide_n(double x, double y) { return x / y; }
double sin_n(double x, double dummy) { return sin(x); }
double cos_n(double x, double dummy) { return cos(x); }
double exp_n(double x, double dummy) { return exp(x); }
double log_n(double x, double dummy) { return log(x); }
double power_n(double x, double y) { return pow(x, y); }
*/

typedef struct Operator {
  char name[OPE_SIZE + 1];
  int operand;
  //  double (*function)(double, double);
} operator;

/*
const operator operator_list[] = {{"Plus", 2, plus_n},
                                  {"Times", 2, times_n},
                                  {"Substract", 2, substract_n},
                                  {"Divide", 2, divide_n},
                                  {"Sin", 1, sin_n},
                                  {"Cos", 1, cos_n},
                                  {"Exp", 1, exp_n},
                                  {"Log", 1, log_n},
                                  {"Power", 2, power_n}};
*/
const operator operator_list[] = {{"Plus", 2},   {"Times", 2}, {"Subtract", 2},
                                  {"Divide", 2}, {"Sin", 1},   {"Cos", 1},
                                  {"Exp", 1},    {"Log", 1},   {"Power", 2}};

int operator_list_size;

typedef struct node {
  int type;  // 0: number, 1: operand
  int operator;
  double value;
  struct node *left;
  struct node *right;
} node;

/*
typedef struct String {
  int index;
  char str[STR_SIZE + 1];
  struct String *prev;
  struct String *next;
} string;
*/

node *create_operator(int operator) {
  node *new = (node *)malloc(sizeof(node));
  new->type = 1;
  new->value = 0;
  new->operator= operator;
  new->left = NULL;
  new->right = NULL;

  return new;
}

node *create_value(double value) {
  node *new = (node *)malloc(sizeof(node));
  new->type = 0;
  new->value = value;
  new->operator= -100;  // usually unused
  new->left = NULL;
  new->right = NULL;

  return new;
}

// for displaying tree
void traverse(node *n) {
  if (n->type == 1) {
    printf("%s", operator_list[n->operator].name);
    printf("[");
    if (n->left != NULL) {
      traverse(n->left);
    }
    if (n->right != NULL) {
      printf(",");
      traverse(n->right);
    }
    printf("]");
  } else {
    printf("%f", n->value);
  }
}

double culculate(node *n) {
  // if operator
  if (n->type == 1) {
    if (n->operator== 0) {
      return culculate(n->left) + culculate(n->right);
    } else if (n->operator== 1) {
      double temp_times_left = culculate(n->left);
      double temp_times_right = culculate(n->right);
      if (temp_times_left == 0.0 || temp_times_right == 0.0) {
        return 0;
      } else {
        return temp_times_left * temp_times_right;
      }
    } else if (n->operator== 2) {
      return culculate(n->left) - culculate(n->right);
    } else if (n->operator== 3) {
      return culculate(n->left) / culculate(n->right);
    } else if (n->operator== 4) {
      return sin(culculate(n->left));
    } else if (n->operator== 5) {
      return cos(culculate(n->left));
    } else if (n->operator== 6) {
      return exp(culculate(n->left));
    } else if (n->operator== 7) {
      double temp_log;
      temp_log = culculate(n->left);
      if (temp_log < 0) {  // avoid minus error
        return log(-temp_log);
      } else {
        return log(temp_log);
      }
    } else if (n->operator== 8) {
      return pow(culculate(n->left), culculate(n->right));
    }
  } else {
    return n->value;
  }

  /*
    if (n->type == 1) {
      return operator_list[n->operator].function(culculate(n->left),
    culculate(n->right));
    } else {
      return n->value;
    }
  */
}

/*
node *create_node(int *pos, char *s) {
  node *new = (node *)malloc(sizeof(node));
  new->value = s[*pos];
  (*pos)++;
  if (new->value >= '1' && new->value <= '9') {
    new->left = NULL;
    new->right = NULL;
  } else {
    new->left = create_node(pos, s);
    new->right = create_node(pos, s);
  }
  return new;
}
*/

//共用のtemp
char temp[100];
int temp_count = 0;
char c;
int i;

void initialize_temps() {
  temp[0] = '\0';
  temp_count = 0;
  c = 0;
}

node *create_node(FILE *fp) {
  int flag = 0;
  int partition_type = 0;
  int count = 0;
  if (feof(fp) != 0) {
    return NULL;
  }

  node *new;

  while (1) {
    c = fgetc(fp);

    // printf("%c", c);  // temp

    // to assign into *new
    // if the end of word
    if (c == '[' || c == ']' || c == ',') {
      partition_type = 1;
      // don't make node start with partitition character
      /*
      if (count == 0){
        continue;
      }
      */
    }
    if (c == EOF) {
      partition_type = 2;
    }
    if (partition_type > 0) {
      // printf("@");  // temp
      temp[temp_count] = '\0';
      // check if there is valid operator
      for (i = 0; i < operator_list_size; i++) {
        // if there is
        if (strcmp(temp, operator_list[i].name) == 0) {
          flag = 1;
          new = create_operator(i);

          // if the operator has 2 operand
          if (operator_list[i].operand == 2) {
            // making first operand
            initialize_temps();
            new->left = create_node(fp);

            // making second operand
            initialize_temps();
            c = fgetc(fp);
            // printf("%c", c);  // temp

            // eliminating extra ','
            do {
              c = fgetc(fp);
            } while (c == '[' || c == ']' || c == ',');
            temp[0] = c;
            temp_count = 1;

            new->right = create_node(fp);
          }
          // if the operator has 1 operand
          else if (operator_list[i].operand == 1) {
            initialize_temps();
            new->left = create_node(fp);

            new->right = NULL;
          }
          // other number of operands
          else {
          }
          // end searching operand
          break;
        }
        // if there isn't now
        else {
        }
      }
      // if there is no valid operator
      if (flag == 0) {
        new = create_value(atof(temp));
      }
      // end inputting character
      break;
    }
    // if not at the end of word
    else {
      temp[temp_count] = c;
      temp_count++;
    }
    count = 1;
  }

  return new;
}
/*
while (1) {
  if (count == STR_SIZE) {
    if (str_current->next == NULL){
      break;
    }
    else {
      str_current = str_current->next;
      c = 0;
      continue;
    }
  }
  else if (str_current->str[c] == '\0') {
    break;
  }
  else if(str_current)
}
*/
/*
node *new = (node *)malloc(sizeof(node));
new->value = s[*pos];
(*pos)++;
if (new->value >= '1' && new->value <= '9') {
  new->left = NULL;
  new->right = NULL;
} else {
  new->left = create_node(pos, s);
  new->right = create_node(pos, s);
}
return new;
}
*/

int main(int argc, char *argv[]) {
  operator_list_size = sizeof(operator_list) / sizeof(operator);
  double ans;

  FILE *fp;
  fp = fopen(argv[1], "r");

  /*
    string *str_cur;
    string *str_begin;
    string *str_new;




      str_begin = (string *)malloc(sizeof(string));

      str_cur = str_begin;
      str_cur->index = 1;
      str_cur->prev = NULL;
      str_cur->next = NULL;
      str_cur->str = (char *)malloc(sizeof(char) * (STR_SIZE + 1));
      fgets(str_cur->str, STR_SIZE + 1, fp);

      while (feof(fp) != EOF){
        str_new = (string *)malloc(sizeof(string));
        str_cur->next = str_new;
        str_new->prev = str_cur;
        str_new->next = NULL;
        str_new->index = str_new->prev->index + 1;
        fgets(str_new->str, STR_SIZE + 1, fp);
        str_cur = str_new;
      }
      str_cur = str_begin;

      fclose(fp);
    */
  // int p = 0;
  initialize_temps();
  node *root = create_node(fp);
  traverse(root);
  // ans = culculate(root);
  // printf("%f\n", ans);

  return 0;
}
