#pragma once

#include <functional>

#include "core/time.h"

class UICallback
{
public:
	UICallback();
	UICallback(const std::function<void()> &p_on_call);

#if 0
	template<typename T>
	void bind_method(const std::function<void(const T&)> method, T* instance)
	{
		is_init = true;
		auto f = std::bind(method, instance);
		on_call = f;
	}
#endif

	void operator ()();

private:
	bool is_init = false;
	std::function<void()> on_call;
};

class UITimer
{
public:
	UITimer() : timer(1000) { }
	UITimer(UICallback *p_callback, int time);

	void update();
	void set_enabled(bool enabled);

private:
	UICallback *callback;
	Timer timer;
};