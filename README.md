# php-asio

[![](https://travis-ci.org/CismonX/php-asio.svg?branch=master)](https://travis-ci.org/CismonX/php-asio)

## 1. About

This extension is a PHP wrapper for the Boost.Asio library, hence the name php-asio.

The extension provides a high-performance event-driven model for networking, which makes asynchronous and concurrency simple.

Now php-asio is **in development**, do not use it in production.

Bug reports and pull requests are welcome.

## 2. Build

### 2.1 Requirements

* 64-bit PHP 7
* [PHP-CPP](http://www.php-cpp.com/) \(GitHub master branch\)
* The [Boost](http://www.boost.org/) C++ libraries
* C++ compiler which fully supports C++11 \(g++ 4.8.5 and above, clang 3.3 and above\)

### 2.2 Notes

* PHP-CPP and this extension **MUST** be built with the same version of PHP source code.
* Do not use the 2.0.0 release of PHP-CPP. Use the GitHub master branch instead. See [here](https://cismon.net/2017/09/19/PHP-CPP-bug-which-causes-memory-leak/) for details.
* Write your own cmake script if you want to build it on Windows. Now PHP-CPP officially supports Windows, which makes things easier.
* It is strongly recommended that you use the latest stable version of libboost. I once used version 1.55, but it does not work well with PHP, sometimes you may get some absurd segfaults which are not fixable. Version 1.63 works fine. And perhaps you should do the same with your compiler.

## 3. Documentation

Full documentation can be found within [stub.php](stub.php). With it, you can also enable code hinting for this library in your IDE to make your life easier.

There are also some examples in the [examples/](examples/) directory.