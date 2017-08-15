#pragma once

/**
 * Wrapper for Boost.Asio deadline timer.
 */
class Timer
{
    /**
     * Boost.Asio timer instance.
     */
    boost::asio::deadline_timer *_timer;

    /**
     * Map for all timer instances.
     */
    static std::map<int64_t, std::map<int64_t, Timer *>> _timer_map;

    /**
     * Timer interval(milliseconds).
     */
    int64_t _interval;

    /**
     * Argument to be passed to timer callback.
     */
    Php::Value _argument;

    /**
     * Timer callback.
     */
    Php::Value _callback;

    /**
     * Whether timer is persistent.
     */
    bool _persistent;

    /**
     * ID of this timer.
     */
    int64_t _id;

    /**
     * Service Id of this timer.
     */
    int64_t _service_id;

    /**
     * Defer the timer.
     */
    void _defer();

    /**
     * Handler for timer callback.
     */
    void _handler();

    /**
     * Delete timer.
     * @param service_id : ID of io service which the timer is based on
     * @param timer_id
     */
    static void del(int64_t service_id, int64_t timer_id);

public:

    /**
     * Timer constructor.
     * @param io_service : Wrapper for boost::asio::io_service
     * @param interval : Timer interval
     * @param argument : Argument to be passed to timer callback
     * @param callback : Timer callback
     * @param persistent : Whether timer repeats
     */
    explicit Timer(
        IoService& io_service,
        int64_t interval,
        const Php::Value& argument,
        const Php::Value& callback,
        bool persistent);

    /**
     * Timer destructor.
     */
    ~Timer();

    /**
     * Get timer id.
     * @return timer_id
     */
    int64_t getId() const;

    /**
     * Delete all timers based on the given io_service.
     * @param service_id
     */
    static void delAll(int64_t service_id);

    /**
     * Set a timer's persistant attribute to false.
     * @param service_id : ID of io service which the timer is based on
     * @param timer_id
     */
    static void cancel(int64_t service_id, int64_t timer_id);
};
