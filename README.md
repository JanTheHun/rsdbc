## Ridiculously Simple DB Client

A very simple MySQL Database Client written in C. So far it's nothing more than a bloated "Hello World!" app...

## Mission Objective

The goal is to build a very simple but flexible generic (meaning: multi-DB) database client that runs in command line.

## Installation

You need ncurses and MySQL Connector/C installed in order to make it work.

Compile it like this (apply your directories where needed):

gcc -o mysql mysql.c -I /usr/include/mysql -lncurses -L/usr/lib/x86_64-linux-gnu -lmysqlclient -lpthread -lz -lm -ldl

Explanation: you may need to include your mysql directory with "-I".

-lncurses links ncurses.

The "-L" part may seem intimidating but it's actually quite simple to retrieve, just run "mysql_config --libs" after you installed mysql.

The result is an executable called "mysql".

## Contributors

JanTheHun

## License

Copyright (c) 2011-2016 GitHub Inc.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
