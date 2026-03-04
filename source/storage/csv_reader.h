#ifndef CSV_READER_H
#define CSV_READER_H

typedef struct csv_row csv_row;
struct csv_row
{
    // array of size col_count, points into mmap buffer
    string8 *fields;
    i32      count;
};

typedef struct csv_table csv_table;
struct csv_table
{
    // first row, col names
    // all data rows
    string8  *headers;
    csv_row  *rows;
    i32       col_count;
    i32       row_count;
};

read_only global_variable
csv_row  nil_csv_row =
{
    .fields = {NULL,  0},
    .count = 0,
};



read_only global_variable
csv_table nil_csv_table =
{
    .string8     = {NULL, 0},
    .csv_row     = &nil_csv_row,
    .col_count   = 0,
    .row_count   = 0,
};



#endif /* CSV_READER_H */
