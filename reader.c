#include "reader.h"


static struct file_context *context = NULL;

static inline void print_conditions() {
    int i;
    for(i = 0; i <= context->current_conditions - 1; i++) {
        printf("%f\n", context->conditions[i].value);
    }
}

file_context *backtest_file_get_ctx(void) {
    return (file_context *)&context;
}

int backtest_evaluate_conditions(float value) 
{
    price_condition condition;
    int i;

    if(! context->current_conditions ) {
        return -1;
    }

    for(i = 0; i <= context->current_conditions - 1; i++) {
        condition = context->conditions[i];
        switch(condition.type) {
        case LOWER_THAN:
            if(!(value < condition.value)) {
                return -1;
            }
            break;
        case LOWER_AND_EQUALS_TO:
            if(!(value <= condition.value)) {
                return -1;
            }
            break;
        case GREATER_THAN:
            if(!(value > condition.value)) {
                return -1;
            }
            break;
        case GREATER_AND_EQUALS_TO:
            if(!(value >= condition.value)) {
                return -1;
            }
            break;
        case EQUALS_TO:
            if(!(value == condition.value)) {
                return -1;
            }
            break;
        default:
            return -1;
        }
    }    

    return 0;
}


static inline int add_condition(enum PRICE_CONDITION type, float value) 
{
    if(context->conditions == NULL) {
        context->conditions = malloc(sizeof(price_condition));

        if(context->conditions == NULL)
            return -1;

        context->conditions[0].type = type;
        context->conditions[0].value = value;

    } else {
        context->conditions = realloc(context->conditions, sizeof(price_condition) * (context->current_conditions + 1));

        if(context->conditions == NULL)
            return -1;

        context->conditions[context->current_conditions].type = type;
        context->conditions[context->current_conditions].value = value;        
    }

    context->current_conditions += 1;
    return 0;
} 

static inline int add_equals_condition(float value) {
    return add_condition(EQUALS_TO, value);
}

static inline int add_lower_than_condition(float value) 
{
    struct price_condition previous_condition;

    if ( context->current_conditions == 1 ) {
        previous_condition = context->conditions[0];

        if ( previous_condition.type == GREATER_THAN || previous_condition.type == GREATER_AND_EQUALS_TO) {
            if ( value < previous_condition.value )
                return -1;
        }
    }
    return add_condition(LOWER_THAN, value);
}

static inline int add_lower_and_equals_condition(float value) 
{
    struct price_condition previous_condition;

    if ( context->current_conditions == 1 ) {
        previous_condition = context->conditions[0];

        if ( previous_condition.type == GREATER_THAN || previous_condition.type == GREATER_AND_EQUALS_TO){
            if ( value <= previous_condition.value ) 
                return -1;
        }
    }

    return add_condition(LOWER_AND_EQUALS_TO, value);
}

static inline int add_greater_than_condition(float value) 
{
    struct price_condition previous_condition;

    if( context->current_conditions ) {
        previous_condition = context->conditions[0];

        if ( previous_condition.type == LOWER_THAN || previous_condition.type == LOWER_AND_EQUALS_TO) {
            if ( value > previous_condition.value ) {
                return -1;
            }
        }
    }

    return add_condition(GREATER_THAN, value);
}

static inline int add_greater_and_equals_condition(float value) 
{
    struct price_condition previous_condition;

    if( context->current_conditions == 1) {
        previous_condition = context->conditions[0];

        if ( previous_condition.type == LOWER_THAN || previous_condition.type == LOWER_AND_EQUALS_TO){
            if ( value >= previous_condition.value )
                return -1;
        }
    }

    return add_condition(GREATER_AND_EQUALS_TO, value);
}

int backtest_add_price_condition(enum PRICE_CONDITION price_condition, float value)
{ 
    int ret;

    if ( context->current_conditions >= MAX_PRICE_CONDITIONS && price_condition != EQUALS_TO)
        return -1;

    switch(price_condition) {
    case LOWER_THAN:
        ret = add_lower_than_condition(value);
        break;      
    case LOWER_AND_EQUALS_TO:
        ret = add_lower_and_equals_condition(value);
    case GREATER_THAN:
        ret = add_greater_than_condition(value);
        break;
    case GREATER_AND_EQUALS_TO:
        ret = add_greater_and_equals_condition(value);
        break;
    case EQUALS_TO:
        ret = add_equals_condition(value);
        break;
    default:
        ret = -1;
        break;
    }

    return ret;
}

int backtest_file_init(const char *filename, enum FILE_TYPE file_type) 
{
    struct stat st;
    int r, fd;

    if( (r = stat(filename, &st)) != 0) {
        debug("Filename %s not found\n", filename);
        return -1;
    }

    if ( st.st_size == 0 ) {
        debug("Zero size file %s\n", filename);
        return -1;
    }

    fd = open(filename, O_RDONLY);

    if ( fd == -1 ) {
        debug("Cannot open filename %s\n", filename);
        return -1;
    }

    context = malloc(sizeof(file_context));

    if(context == NULL)
        return -1;

    context->mapped = (char *)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   
    if(context->mapped == NULL) {
        debug("Cannot map filename:%s to memory\n", filename);
        return -1;
    }

    context->mapped_size = st.st_size;
    context->file_type = file_type;
    context->conditions = NULL;
    context->current_conditions = 0;

    return 0;
} 

void backtest_file_destroy(void) {
    if(context->mapped) 
        munmap(context->mapped, context->mapped_size);

    if(context->conditions != NULL) 
        free(context->conditions);

    free(context);
}
