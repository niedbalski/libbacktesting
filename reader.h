#ifndef reader_h
#define reader_h
#endif

#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_PRICE_CONDITIONS 2

#ifdef DEBUG
#define debug(fmt, args...) printf("%s:%d "fmt,__FILE__,__LINE__,args)
#else
#define debug(fmt, args...)
#endif


enum FILE_TYPE {
    BACKTEST_FILE_CSV,
    BACKTEST_FILE_TXT,
};

enum PRICE_CONDITION {
    LOWER_THAN,
    LOWER_AND_EQUALS_TO,
    GREATER_THAN,
    GREATER_AND_EQUALS_TO,
    EQUALS_TO,
};

typedef struct price_condition {
    enum PRICE_CONDITION type;
    float value;
} price_condition;


typedef struct file_context {
    const char *filename;
    enum FILE_TYPE file_type;
    struct price_condition *conditions;
    char *mapped;
    size_t current_conditions;
    size_t mapped_size;
} file_context;

//public methods
int backtest_evaluate_conditions(float value);
int backtest_add_price_condition(enum PRICE_CONDITION price_condition, float value);
int backtest_file_init(const char *filename, enum FILE_TYPE file_type);
void backtest_file_destroy(void);
