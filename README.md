build status
============
[![Build Status](https://travis-ci.org/SobanVuex/chef-nginx.png?branch=master)](https://travis-ci.org/SobanVuex/chef-nginx)

libBacktesting
==============

Open-source library written in plain C for back-testing of trading strategies.

Description
===========

The library is composed of the following components:

  *) File loader: Creates a new file-loader and
  associates a time interval in wich a tick will be generated.

  Example usage:
  ```C
  loaded = backtest_load_file("/home/user/forex-data.csv", BACKTEST_FILE_CSV, time_t since, 
                                      time_t until, time_t interval, void *callback);
  
  loaded->skip_values(LOWER_THAN, 100.00);
  loaded->skip_values(GREATER_THAN, 150.00);
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





