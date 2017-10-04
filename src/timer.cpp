#include "includes.hpp"

namespace Asio
{
    void Timer::_defer()
    {
        _timer.expires_from_now(boost::posix_time::milliseconds(_interval));
        _timer.async_wait(boost::bind(&Timer::_handler, this, boost::asio::placeholders::error));
    }

    void Timer::_handler(const boost::system::error_code& error)
    {
        if (error.value() == 125)
            goto End;
        _context_flag = true;
        _callback(this, _argument, boost::numeric_cast<int64_t>(error.value()));
        if (_persistent || !_context_flag)
        {
            _context_flag = false;
            _defer();
        }
        // Delete this Php::Object when timer stops.
        // Refcount will fall to zero and timer will be destroyed.
        else End:
        {
            _context_flag = false;
            delete _wrapper;
            _wrapper = nullptr;
        }
    }

    Timer::Timer(
        boost::asio::io_service& io_service,
        int64_t interval,
        const Php::Value& argument,
        const Php::Value& callback,
        bool persistent
    ) : Base(io_service), _timer(io_service), _interval(interval), _argument(argument), _callback(callback), _persistent(persistent)
    {
        // Objects instantiated in C++ must be wrapped within a Php::Object to make it accessible by Zend Engine.
        // Store this Php::Object in class property to make sure it stays alive until timer stopped.
        // Otherwise the refcount of wrapped object will fall to zero and object will be destroyed by Zend Engine.
        // The C++ object will also be destroyed by PHP-CPP, which will cause segmentation fault.
        _wrapper = new Php::Object("Asio\\Timer", this);
        _defer();
    }

    void Timer::defer(Php::Parameters& params)
    {
        if (!_context_flag)
            throw Php::Exception("Manual defer is not allowed outside handler callback.");
        _context_flag = false;
        auto param_count = params.size();
        if (param_count)
        {
            _interval = params[0];
            if (param_count > 1)
            {
                _callback = params[1];
                _argument = param_count == 2 ? Php::Value() : params[2];
            }
        }
    }

    void Timer::cancel()
    {
        if (_context_flag)
            _persistent = false;
        else
            _timer.cancel();
    }

}