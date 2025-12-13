#ifndef YADE_AST_H
#define YADE_AST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
  - Statements: MAKE / GET / PUT / REMOVE / REDO
  - Expressions: literals, identifiers, field access, binary ops (==, !=, and, or)
  - Assignments: <field> = <expr>
*/

/* Operators | Kinds */

typedef enum {
  YADE_OP_EQ,     // is  (==)
  YADE_OP_NEQ,    // not (!=)
  YADE_OP_AND,    // and
  YADE_OP_OR      // or
} yade_op_t;

typedef enum {
  YADE_EXPR_NUMBER,
  YADE_EXPR_STRING,
  YADE_EXPR_IDENT,
  YADE_EXPR_FIELD,
  YADE_EXPR_BINARY
} yade_expr_kind_t;

typedef enum {
  YADE_STMT_MAKE,
  YADE_STMT_GET,
  YADE_STMT_PUT,
  YADE_STMT_REMOVE,
  YADE_STMT_REDO
} yade_stmt_kind_t;

/* Expressions */

typedef struct yade_expr yade_expr;

struct yade_expr {
  yade_expr_kind_t kind;

  union {
    int64_t num;   // EXPR_NUMBER
    char *str;     // EXPR_STRING or EXPR_IDENT

    struct {       // EXPR_FIELD
      char *table; // nullable (for "email" vs "users.email")
      char *field;
    } field;

    struct {       // EXPR_BINARY
      yade_op_t op;
      yade_expr *left;
      yade_expr *right;
    } bin;
  } as;
};

/* Assignments */

typedef struct {
  yade_expr *target; // must be EXPR_FIELD (like users.email)
  yade_expr *value;  // literal or identifier (string/number/field/etc)
} yade_assignment;

/* Statement nodes */

typedef struct {
  char *table_name;
  size_t columns_len;
  char **columns;
} yade_make_stmt;

typedef struct {
  char *table_name;

  size_t fields_len;
  yade_expr **fields;

  yade_expr *condition;
} yade_get_stmt;

typedef struct {
  char *table_name;

  size_t assignments_len;
  yade_assignment **assignments;
} yade_put_stmt;

typedef struct {
  char *table_name;
  yade_expr *condition;
} yade_remove_stmt;

typedef struct {
  char *table_name;

  size_t assignments_len;
  yade_assignment **assignments;

  yade_expr *condition;
} yade_redo_stmt;

/* Top-level AST node */

typedef struct yade_stmt {
  yade_stmt_kind_t kind;

  union {
    yade_make_stmt   make_s;
    yade_get_stmt    get_s;
    yade_put_stmt    put_s;
    yade_remove_stmt remove_s;
    yade_redo_stmt   redo_s;
  } as;
} yade_stmt;

/* Constructors */

yade_expr *yade_expr_number(int64_t v);
yade_expr *yade_expr_string(char *owned_str); // takes ownership
yade_expr *yade_expr_ident(char *owned_str);  // takes ownership
yade_expr *yade_expr_field(char *owned_table, char *owned_field); // table can be NULL
yade_expr *yade_expr_binary(yade_op_t op, yade_expr *left, yade_expr *right);

yade_assignment *yade_assignment_new(yade_expr *target_field, yade_expr *value);

yade_stmt *yade_stmt_make(char *owned_table);
yade_stmt *yade_stmt_get(char *owned_table, yade_expr **fields, size_t fields_len, yade_expr *cond);
yade_stmt *yade_stmt_put(char *owned_table, yade_assignment **asgn, size_t asgn_len);
yade_stmt *yade_stmt_remove(char *owned_table, yade_expr *cond);
yade_stmt *yade_stmt_redo(char *owned_table, yade_assignment **asgn, size_t asgn_len, yade_expr *cond);

/* Destructors (prototypes) */

void yade_expr_free(yade_expr *e);
void yade_assignment_free(yade_assignment *a);
void yade_stmt_free(yade_stmt *s);

#endif // YADE_AST_H
