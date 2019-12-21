#pragma once

#include <algorithm>
#include <chrono>

#include "core/Definitions.h"

#if PLATFORM == LINUX
#include "SDL2/SDL.h"
#else
#include "SDL.h"
#endif

#include "Vector.h"
#include "ContentManager.h"

#include "Object.h"

#define TIME Time::get_singleton()

class UITimer;
class Timer;
class TimedCaller;

class Time : public Object
{
	OBJ_DEFINITION(Time, Object);

public:
	Time();

	static void Init();
	static Time* get_singleton();

	void register_timedcaller(TimedCaller *timed_caller);
	void register_timer(Timer *timer);
	void register_uitimer(UITimer *timer);

	void Update();
	void OnUpdate();
	void restart();

	int get_deltatime() const;
	int get_absolutetime() const;
	int get_gametime() const;

	bool game_paused;
	long absolute_time, game_time, LastTime, delta_time;

	static void bind_methods();

private:
	int get_time_since_start();

	std::chrono::high_resolution_clock::time_point start_time;

	static Time *singleton;

	Vector<TimedCaller> timed_callers;
	Vector<Timer> timers;
	Vector<UITimer> uitimers;
};

class Timer
{
public:
	Timer(int p_waittime)
	{
		waittime = p_waittime;
		start();
	}

	bool update()
	{
		if (enabled && TIME->absolute_time - starttime >= waittime)
		{
			running = false;
			return true;
		}
		return false;
	}

	void start()
	{
		starttime = TIME->absolute_time;
		enabled = true;
		running = true;
	}

	int waittime, starttime;
	bool running, enabled;
};

struct FPSLimiter
{
	FPSLimiter(int tar = 16667);

	struct FPSInfo
	{
		bool needsupdate = false;
		bool passedsec = false;
		float delta_time = 0.0f;
		int FPS = 0;
	};

	FPSInfo update();

private:
	int counter = 0;
	int target = 0;
	int lastupdatetime = 0;
	int lastsectime = 0;
	int AbsoluteTime = 0;
};

class TimedCaller
{
public:
	TimedCaller() : timer(1000) { }
	TimedCaller(std::function<void()> p_on_call) : TimedCaller(1000, p_on_call) { }
	TimedCaller(int repeat_time, std::function<void()> p_on_call);

	void update();

private:
	std::function<void()> on_call;
	Timer timer;
};

struct RenderTarget
{
	RenderTarget();

	bool should_update();

	bool is_default_target;
	bool fps_locked;

	int fps_lock;
	int fps_measured;
	int fps_guessed;

	float delta_time;

	FPSLimiter limiter;
};