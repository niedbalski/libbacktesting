#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#ifdef DEBUG
#define debug(fmt, args...) printf("%s:%d "fmt,__FILE__,__LINE__,args)
#else
#define debug(fmt, args...)
#endif

/* typedef struct tick { */
/*     GDateTime *datetime; */
/*     float average, max, min; */
/* } tick; */

enum FILE_TYPE {
    BACKTEST_FILE_CSV,
    BACKTEST_FILE_TXT,
};

enum PRICE_CONDITION {
    LOWER_THAN,
    GREATER_THAN,
    EQUALS_TO,
};

typedef struct price_condition {
    enum PRICE_CONDITION condition;
    float value;
} price_condition;


typedef struct file_context {
    const char *filename;
    enum FILE_TYPE file_type;
    struct price_condition **conditions;
    char *mapped;
    size_t current_conditions;
    size_t mapped_size;
    int(*add_price_condition)(struct file_context *context, enum PRICE_CONDITION condition, float value);
} file_context;

static inline int add_price_condition(struct file_context *context, enum PRICE_CONDITION price_condition, float value){ 
   return 0;
}

file_context *backtest_file_init(const char *filename, enum FILE_TYPE file_type) 
{
    struct file_context *ctx;
    struct stat st;
    int r, fd;

    if( (r = stat(filename, &st)) != 0) {
        debug("Filename %s not found\n", filename);
        return NULL;
    }

    if ( st.st_size == 0 ) {
        debug("Zero size file %s\n", filename);
        return NULL;
    }

    fd = open(filename, O_RDONLY);

    if ( fd == -1 ) {
        debug("Cannot open filename %s\n", filename);
        return NULL;
    }

    ctx = malloc(sizeof(file_context));

    if(ctx == NULL)
        return NULL;

    ctx->mapped = (char *)mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   
    if(ctx->mapped == NULL) {
        debug("Cannot map filename:%s to memory\n", filename);
        return NULL;
    }

    ctx->mapped_size = st.st_size;
    ctx->file_type = file_type;
    ctx->add_price_condition = &add_price_condition;

    return ctx;
} 

void backtest_file_destroy(file_context *context) {
    if(context->mapped) 
        munmap(context->mapped, context->mapped_size);

    if(context->conditions != NULL) 
        free(context->conditions);

    free(context);
}

int main(void) {
    struct file_context *ctx;

    ctx = backtest_file_init("./tests/DAT_MT_USDCAD_M1_201212.csv", BACKTEST_FILE_CSV);

    if(ctx == NULL)  
        return EXIT_FAILURE;

    ctx->add_price_condition(ctx, LOWER_THAN, 100.0);

    backtest_file_destroy(ctx);
    return 0;
}


/* int main(void) { */

/*     gchar *contents, **lines, **fields; */
/*     GMappedFile *file; */
/*     GError *error = NULL; */
/*     gint fields_size = 0; */

/*     file = g_mapped_file_new("./tests/DAT_MT_USDCAD_M1_201212.csv", FALSE, NULL); */
/*     contents = g_mapped_file_get_contents(file); */

/*     lines = g_strsplit(contents, "\n", 0); */

/*     if(lines == NULL) */
/*         return -1; */

/*     for(int i=0; lines[i] != NULL; i++) {         */
/*         fields = g_strsplit(lines[i], ",", 0); */
/*         for(i */
/*     } */

/*     return 0; */
/* } */
