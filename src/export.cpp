#include "includes.hpp"

extern "C" PHPCPP_EXPORT void *get_module()
{
	static Php::Extension extension("asio", "1.0");
	Php::Class<Asio> asio("Asio");

	//Methods for timers.
	asio.method<&Asio::addTimer>("addTimer", {
		Php::ByVal("interval", Php::Type::Numeric),
		Php::ByVal("callback", Php::Type::Callable),
		Php::ByVal("argument", Php::Type::Null, false),
		Php::ByVal("persistent", Php::Type::Bool, false)
	});
	asio.method<&Asio::delTimer>("delTimer", {
		Php::ByVal("timer_id", Php::Type::Numeric)
	});

	//Methods for execution control.
	asio.method<&Asio::run>("run");
	asio.method<&Asio::runOne>("runOne");
	asio.method<&Asio::poll>("poll");
	asio.method<&Asio::pollOne>("pollOne");
	asio.method<&Asio::stop>("stop");
	asio.method<&Asio::stopped>("stopped");
	asio.method<&Asio::reset>("reset");
	asio.method<&Asio::post>("post", {
		Php::ByVal("callback", Php::Type::Callable),
		Php::ByVal("argument", Php::Type::Null, false)
	});

	extension.add(asio);
	return extension;
}