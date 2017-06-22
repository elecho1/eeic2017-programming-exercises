#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_SIZE 100
#define OPE_SIZE 20
#define NEWTON_RANGE 10.0  // range to find extreme value
#define E 0.01             // epsilon
#define D 0.01             // delta
#define EXT_NUM 100        // the naumber of extreme value to store
#define INTERVAL 0.1

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

// index of array is being id of each array
const operator operator_list[] = {{"Plus", 2},   {"Times", 2}, {"Subtract", 2},
                                  {"Divide", 2}, {"Sin", 1},   {"Cos", 1},
                                  {"Exp", 1},    {"Log", 1},   {"Power", 2}};
int operator_list_size;

typedef struct Variable {
  char name[OPE_SIZE + 1];
  double value;
} variable;

variable variable_list[] = {{"x", 0}, {"y", 0}};
int variable_list_size;

typedef struct node {
  int type;  // 0: value, 1: operand, 2: variable
  int operator;
  double value;
  struct node *left;
  struct node *right;
} node;

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

node *create_variable(int operator) {
  node *new = (node *)malloc(sizeof(node));
  new->type = 2;
  new->value = 0;  // initial value
  new->operator= operator;
  new->left = NULL;
  new->right = NULL;

  return new;
}

void change_var_value(int operator, double var_value) {
  variable_list[-(operator+ 1)].value = var_value;
  return;
}

/*
typedef struct String {
  int index;
  char str[STR_SIZE + 1];
  struct String *prev;
  struct String *next;
} string;
*/

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
  } else if (n->type == 2) {
    printf("%s", variable_list[-n->operator-1].name);
  } else {
    printf("%f", n->value);
  }
}

double culculate(node *n) {
  // if operator
  if (n->type == 1) {
    if (n->operator== 0) {
      // printf("type:%d  left:%f  right:%f\n", n->operator, culculate(n->left),
      // culculate(n->right));
      return culculate(n->left) + culculate(n->right);
    } else if (n->operator== 1) {
      // printf("type:%d  left:%f  right:%f\n", n->operator, culculate(n->left),
      // culculate(n->right));
      double temp_times_left = culculate(n->left);
      double temp_times_right = culculate(n->right);
      if (temp_times_left == 0.0 || temp_times_right == 0.0) {
        return 0;
      } else {
        return culculate(n->left) * culculate(n->right);
      }
    } else if (n->operator== 2) {
      // printf("type:%d  left:%f  right:%f\n", n->operator, culculate(n->left),
      // culculate(n->right));
      return culculate(n->left) - culculate(n->right);
    } else if (n->operator== 3) {
      // printf("type:%d  left:%f  right:%f\n", n->operator,
      // culculate(n->left),culculate(n->right));
      return culculate(n->left) / culculate(n->right);
    } else if (n->operator== 4) {
      // printf("type:%d  left:%f  right:NULL\n", n->operator,
      // culculate(n->left));
      return sin(culculate(n->left));
    } else if (n->operator== 5) {
      // printf("type:%d  left:%f  right:NULL\n", n->operator,
      // culculate(n->left));
      return cos(culculate(n->left));
    } else if (n->operator== 6) {
      // printf("type:%d  left:%f  right:NULL\n", n->operator,
      // culculate(n->left));
      return exp(culculate(n->left));
    } else if (n->operator== 7) {
      // printf("type:%d  left:%f  right:NULL\n", n->operator,
      // culculate(n->left));
      /*
      double temp_log;
      temp_log = culculate(n->left);
      if (temp_log < 0) {  // avoid minus error
        return log(-temp_log);
      } else {
        return log(temp_log);
      }
      */
           return log(culculate(n->left));
    } else if (n->operator== 8) {
      // printf("type:%d  left:%f  right:%f\n", n->operator,
      // culculate(n->left),culculate(n->right));
      return pow(culculate(n->left), culculate(n->right));
    }
  } else if (n->type == 2) {
    return variable_list[-(n->operator+ 1)].value;
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

double culculate_var(int operator, double var_value, node *n) {
  change_var_value(operator, var_value);
  return culculate(n);
}
double culculate_x(double var_value, node *n) {
  change_var_value(-1, var_value);
  return culculate(n);
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
  if (feof(fp) != 0) {
    return NULL;
  }

  node *new;

  while (1) {
    c = fgetc(fp);
    // printf("%c", c);  // temp

    // to assign into *new
    // if the end of word
    if (c == '[' || c == ']' || c == ',' || c == EOF) {
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
      // check if there is valid variables
      if (flag == 0) {
        for (i = 0; i < variable_list_size; i++) {
          if (strcmp(temp, variable_list[i].name) == 0) {
            flag = 1;
            new = create_variable(-(i + 1));
            break;
          }
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

node *differential_culc(node *n) {
  if (n->type == 1) {
    node *new;
    node *new1;
    // if Plus or Substract
    if (n->operator== 0 || n->operator== 2) {
      new = create_operator(n->operator);  // Plus of Substruct
      new->left = differential_culc(n->left);
      new->right = differential_culc(n->right);
    }
    // if Times
    else if (n->operator== 1) {
      new = create_operator(0);        // Plus
      new->left = create_operator(1);  // times
      new->left->left = n->left;
      new->left->right = differential_culc(n->right);
      new->right = create_operator(1);  // times
      new->right->left = differential_culc(n->left);
      new->right->right = n->right;
    }
    // if Divide
    else if (n->operator== 3) {
      new1 = create_operator(1);  // Times
      new1->left = n->left;
      new1->right = create_operator(8);  // Power
      new1->right->left = n->right;
      new1->right->right = create_value(-1);
      new = differential_culc(new1);
    }
    // if Sin
    else if (n->operator== 4) {
      new = create_operator(1);        // Times
      new->left = create_operator(5);  // Cos
      new->left->left = n->left;
      new->right = differential_culc(n->left);
    }
    // if Cos
    else if (n->operator== 5) {
      new = create_operator(1);  // Times
      new->left = create_value(-1);
      new->right = create_operator(1);     // Times
      node *current = new->right;          // descend one floor
      current->left = create_operator(4);  // sin
      current->left->left = n->left;
      current->right = differential_culc(n->left);
    }
    // if Exp
    else if (n->operator== 6) {
      new = create_operator(1);  // Times
      new->left = differential_culc(n->left);
      new->right = create_operator(6);  // Exp
      new->right->left = n->left;
    }
    // if Log
    else if (n->operator== 7) {
      new = create_operator(1);  // Times
      new->left = differential_culc(n->left);
      new->right = create_operator(8);
      new->right->left = n->left;
      new->right->right = create_value(-1);
    }
    // if Power
    else if (n->operator== 8) {
      new = create_operator(1);  // Times
      new->left = n;
      new1 = create_operator(1);        // Times
      new1->left = n->right;
      new1->right = create_operator(7);  // Log
      new1->right->left = n->left;
      new->right = differential_culc(new1);
    }
    // other
    else {
      return NULL;
    }
    return new;
  }
  // if valuable
  else if (n->type == 2) {
    // if valuable'x'
    if (n->operator== - 1) {
      return create_value(1);
    }
    // if other
    else {
      return create_value(0);
    }
  }
  // if value
  else {
    return create_value(0);
  }
}

double ext_var[EXT_NUM];
int ext_var_size = 0;

// To find x which satisfy "f(x) == 0"
double newton_culc(node *f, node *dfdx, double xn) {
  double xn1;
  xn1 = xn - culculate_x(xn, f) / culculate_x(xn, dfdx);

  if (fabs(xn1 - xn) < E && fabs(culculate_x(xn1, f)) < D) {
    return xn1;
  } else {
    return newton_culc(f, dfdx, xn1);
  }
}

void find_ext_var(node *dfdx, node *d2fdx2) {
  double i;
  double left, right;
  right = culculate_x(-NEWTON_RANGE, dfdx);
  for (i = -NEWTON_RANGE; i <= NEWTON_RANGE; i += INTERVAL) {
    left = right;
    right = culculate_x(i + INTERVAL, dfdx);
    if (left * right < 0) {
      printf("%f\n", i);
      ext_var[ext_var_size] = newton_culc(dfdx, d2fdx2, i);
      ext_var_size++;
    }
    if (ext_var_size >= EXT_NUM) {
      printf("極値はこれ以上あります。\n");
      break;
    }
  }
  return;
}

/*
void find_ext_var(node *dfdx, node *d2fdx2) {
  ext_var_size = 0;
  ext_var[ext_var_size] = newton_culc(dfdx, d2fdx2, 0.2);
  printf("newton:%f", ext_var[0]);
  ext_var_size++;
  return;
}
*/

void print_ext_var(node *f, node *dfdx, node *d2fdx2) {
  double check;
  double check1;
  double val;
  for (int i = 0; i < ext_var_size; i++) {
    check1 = culculate_x(ext_var[i], dfdx);
    check = culculate_x(ext_var[i], d2fdx2);
    val = culculate_x(ext_var[i], f);
    printf("1階微分%f ２階微分%f\n", check1, check);
    if (check > D) {
      printf("極小値:%f (x = %f)\n", val, ext_var[i]);
    } else if (check < -D) {
      printf("極大値:%f (x = %f)\n", val, ext_var[i]);
    } else {
      printf("停留点:%f (x = %f)\n", val, ext_var[i]);
    }
  }
  return;
}

int main(int argc, char *argv[]) {
  operator_list_size = sizeof(operator_list) / sizeof(operator);
  variable_list_size = sizeof(variable_list) / sizeof(variable);

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
  printf("\n");
  // ans = culculate(root);
  // printf("%f\n", ans);

  node *diff = differential_culc(root);
  traverse(diff);
  printf("\n");

  node *diff2 = differential_culc(diff);
  traverse(diff2);
  printf("\n");

  find_ext_var(diff, diff2);
  print_ext_var(root, diff, diff2);

  // printf("%f\n", culculate_x(0, diff2));
  printf("%f\n", culculate_x(-1.0, diff));
  printf("%f\n", culculate_x(-1.0, diff2));
  printf("%f\n", culculate_x(-1.0, root));
  printf("%f\n", pow(0.0, 0.0));
  return 0;
}
