# Readme

## 1. About

This extension is a PHP wrapper for the Boost.Asio library, hence the name php-asio.

Now php-asio is **in development**, currently supported functionalities can be found [here](examples/stub.php).

Bug reports and pull requests are welcome.

## 2. Build

### 2.1 Requirements

* 64-bit PHP 7
* [PHP-CPP](http://www.php-cpp.com/) \(GitHub master branch\)
* The [Boost](http://www.boost.org/) C++ libraries
* C++ compiler which supports C++11 (namely g++ 4.8.5 and above)

### 2.2 Notes

* PHP-CPP and this extension **MUST** be built with the same version of PHP source code.
* Do not use the 2.0.0 release of PHP-CPP. Use the GitHub master branch instead. See [here](https://cismon.net/2017/09/19/PHP-CPP-bug-which-causes-memory-leak/) for details.
* Write your own cmake script if you want to build it on Windows. Now PHP-CPP officially supports Windows, which makes things easier.

## 3. Documentation

Currently, the only documentation is the comments in the sources files. ~~Good luck with that.~~

There are also some examples in the [examples/](examples/) directory.