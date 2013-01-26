Build Status
============
[![Build Status](https://travis-ci.org/niedbalski/libbacktesting.png?branch=master)](https://travis-ci.org/niedbalski/libbacktesting)

LibBacktesting
==============

Open-source (BSD Licensed) library useful to test trading strategies using back testing. This library has been
written in plain C to allow and motivate other language bindings.

- Strategies can be written in LUA just by using a callback mechanism.
- The library exposes plot routines to draw strategies results.
- The library offers a collection of already-tested strategies written as LUA scripts
- Will be possible in the near-future to implement direct connection to some brokers in order to put real trade positions
- using this library.

Description
===========

The library is composed of the following 3 components:

  1) *The feed/file loader*: Loads a stream of ticks from a stream source or directly
  from a CSV/Txt file.

  * Example of usage: Creates a new csv loader and set 2 price conditions for each found tick.
  
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

  2) *LUA strategies*: Every time an event ocurrs inside a trading backtesting session , the following
  callbacks are generated:
  
      1) *on_tick* : is called when a new tick passing the price conditions is found.
      2) *on_new_position* : is called before a new position is gonna be submitted to the trading system.
      3) *on_position*: is called after a new position is submitted to the trading system.
      4) *on_exit_position*: is called when a position is closed
      
    
  ```C
  
  #include <backtest.h>
  
  int main(void) {
      backtest_lua_init("./strategies/3-sma-average.lua")
      
      backtest_lua_add_hook("on_hook", 100.0);
      backtest_lua_add_hook("on_exit_position");
      
      //free and destroy resources
      backtest_lua_destroy();
  }
  
  Once you defined your hooks, you need to write the LUA plugin "3-sma-average.lua" :
  
  ```lua
  function on_tick(tick_value)
    io.write(string.format("Passed value: %s\n", tick_value))
  end

  function on_exit_position(position)
    io.write(string.format("Closed at value: %s\n", position.closed_at()))
  end
  
  ```
  
  3) *Bind*: Once you define your *Strategy* and creates a *Tick feed* you need to 
  bind and start simulating trades.
  
  ```C
  #include <backtest.h>
  
  int main(void) {
    binded = backtest_bind(backtest_lua_get_ctx(), backtest_file_get_ctx());
    binded->start(-500); // start since -500 ticks , calculate this :)
  }
  ```

  *) Trading API: Not ready yet.





