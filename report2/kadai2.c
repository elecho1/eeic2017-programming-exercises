#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STR_SIZE 100
#define OPE_SIZE 20

typedef struct Operator {
  char name[OPE_SIZE + 1];
  int operand;
} operator;

// index of array is being id of each array
const operator operator_list[] = {{"Plus", 2},   {"Times", 2}, {"Subtract", 2},
                                  {"Divide", 2}, {"Sin", 1},   {"Cos", 1},
                                  {"Exp", 1},    {"Log", 1},   {"Power", 2}};
int operator_list_size;

const char variable_list[][OPE_SIZE + 1] = {"x", "y"};
const int variable_list_size = 2;

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
  new->value = 0;  // initialized value
  new->operator= operator;
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
  } else if (n->type == 2) {
    printf("%s", variable_list[-n->operator-1]);
  } else {
    printf("%f", n->value);
  }
}

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

    // to assign into *new
    // if the end of word
    if (c == '[' || c == ']' || c == ',' || c == EOF) {
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
          if (strcmp(temp, variable_list[i]) == 0) {
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
      return log(culculate(n->left));
    } else if (n->operator== 8) {
      return pow(culculate(n->left), culculate(n->right));
    }
  } else {
    return n->value;
  }
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
      new->right->right = n->left;
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
      new = create_operator(1);  //Times
      new->left = create_operator(5);  // Cos
      new->left->left = n->left;
      new->right = differential_culc(n->left);
    }
    // if Cos
    else if (n->operator== 5) {
      new = create_operator(1);  // Times
      new->left = create_value(-1);
      new->right = create_operator(1);  //Times
      node *current = new->right; // descend one floor
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

int main(int argc, char *argv[]) {
  operator_list_size = sizeof(operator_list) / sizeof(operator);
  
  FILE *fp;
  fp = fopen(argv[1], "r");

  initialize_temps();
  node *root = create_node(fp);
  traverse(root);
  printf("\n");

  node *diff = differential_culc(root);
  traverse(diff);
  printf("\n");

  return 0;
}
