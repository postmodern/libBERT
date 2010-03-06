libBERT
-------

libBERT is a C library for encoding and decoding BERT (Binary ERlang Term)
data [1]. Inspired by the Ruby BERT library [2], libBERT aims to provide the
full BERT format to C programmers.

Benefits of BERT
----------------

* Compact binary representation.
* Defined by a specification [1].

Benefits of libBERT
-------------------

* Written as a C library.
* Portable.
* Robust.
* Small.
* Fast (did I mention it was written in C, the language of fast?).
* Supports encoding/decoding of:
  * Integers.
  * Floating point numbers.
  * Atoms.
  * Strings.
  * Binary data.
  * Tuples.
  * Lists.
  * Complex types:
    * Nil.
    * Boolean True and False values.
    * Dictionaries.
    * Time values.
    * Regular Expressions.
* Supports buffered, streaming or callback-based reading/writing of
  BERT data.
* Distributed under the MIT "as-is" license.

Build Dependencies
------------

* CMake >= 2.6.0 [2]

Installing libBERT
----------------

  $ cmake .

  * Or run "ccmake ." for a ncurses based configuration menu.

  $ make
  $ sudo make install

References
----------

[1] BERT and BERT-RPC 1.0 Specification.
    http://bert-rpc.org/
[2] A BERT (Binary ERlang Term) serialization library for Ruby.
    http://github.com/mojombo/bert
[3] CMake, the cross-platform, open-source build system.
    http://www.cmake.org/

License
-------

The MIT License

Copyright (c) 2009-2010 Hal Brodigan

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
