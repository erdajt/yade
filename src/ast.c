#include "yade/ast.h"
#include <stdlib.h>

// yade_expr_free: frees an expression and all its sub-expressions.
void yade_expr_free(yade_expr *e) {
  if (!e)
    return;

  switch (e->kind) {
  case YADE_EXPR_STRING:
    free(e->as.str);
    break;
  case YADE_EXPR_IDENT:
    free(e->as.str);
    break;
  case YADE_EXPR_FIELD:
    free(e->as.field.table);
    free(e->as.field.field);
    break;
  case YADE_EXPR_BINARY:
    yade_expr_free(e->as.bin.left);
    yade_expr_free(e->as.bin.right);
    break;
  default:
    break;
  }

  free(e);
}

// yade_assignment_free: frees an assignment.
void yade_assignment_free(yade_assignment *a) {
  if (!a)
    return;

  yade_expr_free(a->target);
  yade_expr_free(a->value);
  free(a);
}

// yade_stmt_free: frees a statement and all its sub-nodes.
void yade_stmt_free(yade_stmt *s) {
  if (!s)
    return;

  switch (s->kind) {
  case YADE_STMT_MAKE:
    free(s->as.make_s.table_name);
    for (size_t i = 0; i < s->as.make_s.columns_len; i++) {
      free(s->as.make_s.columns[i]);
    }
    free(s->as.make_s.columns);
    break;
  case YADE_STMT_GET:
    free(s->as.get_s.table_name);
    for (size_t i = 0; i < s->as.get_s.fields_len; i++) {
      yade_expr_free(s->as.get_s.fields[i]);
    }
    free(s->as.get_s.fields);
    yade_expr_free(s->as.get_s.condition);
    break;
  case YADE_STMT_PUT:
    free(s->as.put_s.table_name);
    for (size_t i = 0; i < s->as.put_s.assignments_len; i++) {
      yade_assignment_free(s->as.put_s.assignments[i]);
    }
    free(s->as.put_s.assignments);
    break;
  case YADE_STMT_REMOVE:
    free(s->as.remove_s.table_name);
    yade_expr_free(s->as.remove_s.condition);
    break;
  case YADE_STMT_REDO:
    free(s->as.redo_s.table_name);
    for (size_t i = 0; i < s->as.redo_s.assignments_len; i++) {
      yade_assignment_free(s->as.redo_s.assignments[i]);
    }
    free(s->as.redo_s.assignments);
    yade_expr_free(s->as.redo_s.condition);
    break;
  default:
    break;
  }

  free(s);
}

// yade_expr_number: creates a number expression.
yade_expr *yade_expr_number(int64_t v) {
  yade_expr *expr = malloc(sizeof(yade_expr));
  if (!expr)
    return NULL;

  expr->kind = YADE_EXPR_NUMBER;
  expr->as.num = v;
  return expr;
}

// yade_expr_string: creates a string expression (takes ownership of the
// string).
yade_expr *yade_expr_string(char *owned_str) {
  yade_expr *expr = malloc(sizeof(yade_expr));
  if (!expr)
    return NULL;

  expr->kind = YADE_EXPR_STRING;
  expr->as.str = owned_str;
  return expr;
}

// yade_expr_ident: creates an identifier expression (takes ownership of the
// string).
yade_expr *yade_expr_ident(char *owned_str) {
  yade_expr *expr = malloc(sizeof(yade_expr));
  if (!expr)
    return NULL;

  expr->kind = YADE_EXPR_IDENT;
  expr->as.str = owned_str;
  return expr;
}

// yade_expr_field: creates a field expression (takes ownership of the table)
yade_expr *yade_expr_field(char *owned_table, char *owned_field) {
  yade_expr *expr = malloc(sizeof(yade_expr));
  if (!expr)
    return NULL;

  expr->kind = YADE_EXPR_FIELD;
  expr->as.field.table = owned_table;
  expr->as.field.field = owned_field;
  return expr;
}

// yade_expr_binary: creates a binary expression.
yade_expr *yade_expr_binary(yade_op_t op, yade_expr *left, yade_expr *right) {
  yade_expr *expr = malloc(sizeof(yade_expr));
  if (!expr)
    return NULL;

  expr->kind = YADE_EXPR_BINARY;
  expr->as.bin.op = op;
  expr->as.bin.left = left;
  expr->as.bin.right = right;
  return expr;
}

// yade_assignment_new: creates a new assignment.
yade_assignment *yade_assignment_new(yade_expr *target_field,
                                     yade_expr *value) {
  yade_assignment *assign = malloc(sizeof(yade_assignment));
  if (!assign)
    return NULL;

  assign->target = target_field;
  assign->value = value;
  return assign;
}

// yade_stmt_make: creates a MAKE statement.
yade_stmt *yade_stmt_make(char *owned_table) {
  yade_stmt *stmt = malloc(sizeof(yade_stmt));
  if (!stmt)
    return NULL;

  stmt->kind = YADE_STMT_MAKE;
  stmt->as.make_s.table_name = owned_table;
  stmt->as.make_s.columns = NULL;
  stmt->as.make_s.columns_len = 0;
  return stmt;
}

// yade_stmt_get: creates a GET statement.
yade_stmt *yade_stmt_get(char *owned_table, yade_expr **fields,
                         size_t fields_len, yade_expr *cond) {
  yade_stmt *stmt = malloc(sizeof(yade_stmt));
  if (!stmt)
    return NULL;

  stmt->kind = YADE_STMT_GET;
  stmt->as.get_s.table_name = owned_table;
  stmt->as.get_s.fields = fields;
  stmt->as.get_s.fields_len = fields_len;
  stmt->as.get_s.condition = cond;
  return stmt;
}

// yade_stmt_put: creates a PUT statement.
yade_stmt *yade_stmt_put(char *owned_table, yade_assignment **asgn,
                         size_t asgn_len) {
  yade_stmt *stmt = malloc(sizeof(yade_stmt));
  if (!stmt)
    return NULL;

  stmt->kind = YADE_STMT_PUT;
  stmt->as.put_s.table_name = owned_table;
  stmt->as.put_s.assignments = asgn;
  stmt->as.put_s.assignments_len = asgn_len;
  return stmt;
}

// yade_stmt_remove: creates a REMOVE statement.
yade_stmt *yade_stmt_remove(char *owned_table, yade_expr *cond) {
  yade_stmt *stmt = malloc(sizeof(yade_stmt));
  if (!stmt)
    return NULL;

  stmt->kind = YADE_STMT_REMOVE;
  stmt->as.remove_s.table_name = owned_table;
  stmt->as.remove_s.condition = cond;
  return stmt;
}

// yade_stmt_redo: creates a REDO statement.
yade_stmt *yade_stmt_redo(char *owned_table, yade_assignment **asgn,
                          size_t asgn_len, yade_expr *cond) {
  yade_stmt *stmt = malloc(sizeof(yade_stmt));
  if (!stmt)
    return NULL;

  stmt->kind = YADE_STMT_REDO;
  stmt->as.redo_s.table_name = owned_table;
  stmt->as.redo_s.assignments = asgn;
  stmt->as.redo_s.assignments_len = asgn_len;
  stmt->as.redo_s.condition = cond;
  return stmt;
}
