#include <assert.h>
#include "backtest.h"

int main(void) 
{
    int r;

    assert(backtest_file_init("./files/DAT_MT_USDCAD_M1_201212.csv", BACKTEST_FILE_CSV) == 0);
    assert(backtest_add_price_condition(LOWER_THAN, 100)
    backtest_add_price_condition(GREATER_THAN, 50);



    r = backtest_evaluate_conditions(99);
    printf("%d", r);

    r = backtest_evaluate_conditions(102.00);
    printf("%d", r);

    r = backtest_evaluate_conditions(50.00001);
    printf("%d", r);

    backtest_file_destroy();

    return EXIT_SUCCESS;

}
