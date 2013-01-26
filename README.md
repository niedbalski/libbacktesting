Build Status
============
[![Build Status](https://travis-ci.org/niedbalski/libbacktesting.png?branch=master)](https://travis-ci.org/niedbalski/libbacktesting)

libBacktesting
==============

Open-source library written in plain C for back-testing of trading strategies.

Description
===========

The library is composed of the following 3 components:

  1) The feed/file loader: Loads a stream of ticks from a stream source or directly
  from a CSV/Txt file.

  * Example of usage: Creates a new csv loader and set 2 price conditions for
  * each tick
  
  Example usage:
  ```C
  
  #include <backtest.h>
  
  int main(void) {
      int r;
      r  = backtest_file_init("/home/user/forex-data.csv", BACKTEST_FILE_CSV);
      
      if ( r == -1 ) 
         return -1;

      backtest_add_price_condition(LOWER_THAN, 100.00);
      backtest_add_price_condition(GREATER_AND_EQUALS_TO, 50.00);
      
      backtest_file_destroy();
  }
  ```

  *) Lua plugin interface: Associates a set of callbacks to be
  run on every time interval already set.
   
  ```C
  lua = backtest_lua_init("./plugins/3-medium-average.lua")
  lua->hook("on_tick", price)
  lua->destroy("before_order"; order);
  ```

  Note: To bind a loaded file with a lua runner:

  ```C
  binded = backtest_bind(loaded, lua);
  binded->start() ... binded->stop();
  ```

  *) Strategy information: To be done. Exposes the statistics 
  about the current trading strategy.





