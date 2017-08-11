# Readme

## 1. About

This extension is a wrapper for the Boost.Asio library, hence the name php-asio.

Now php-asio is **in development**, currently supported functionalities can be found in *examples/stub.php*.

Pull requests are welcome.

## 2. Build

### 2.1 Requirements

* PHP 7
* [PHP-CPP](http://www.php-cpp.com/)
* [Boost](http://www.boost.org/)
* C++ compiler which supports C++11

### 2.2 Notes

* If you need multithreading, follow the instructions in *src/includes.hpp*.
* Write your own cmake script if you want to build it on Windows.

## 3. Documentation

Currently, the only documentation is the comments in the sources files. ~~Good luck with that.~~

There are also some examples in the */examples* directory.