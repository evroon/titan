#include "Time.h"
#include "ui/UICallback.h"

Time *Time::singleton;


//=========================================================================
//Time
//=========================================================================

Time::Time()
{
	start_time = std::chrono::high_resolution_clock::now();

	LastTime = get_time_since_start();
}

Time* Time::get_singleton()
{
	return singleton;
}

void Time::register_timedcaller(TimedCaller *timed_caller)
{
	timed_callers.push_back(timed_caller);
}
void Time::register_timer(Timer *timer)
{
	timers.push_back(timer);
}
void Time::register_uitimer(UITimer *timer)
{
	uitimers.push_back(timer);
}

void Time::Init()
{
	singleton = new Time;
}

void Time::Update()
{
	AbsoluteTime = get_time_since_start();
}

int Time::get_time_since_start()
{
	auto now = std::chrono::high_resolution_clock::now();

	return to_int(std::chrono::duration_cast<std::chrono::microseconds>(now - start_time).count());
}

void Time::OnUpdate()
{
	Update();
	DeltaTime = AbsoluteTime - LastTime;

	GameTime += DeltaTime;
	LastTime = AbsoluteTime;

	T_LOG(DeltaTime);

	//for_each(timers.begin(), timers.end(), [](Timer* timer) { timer->update(); });
	//for_each(uitimers.begin(), uitimers.end(), [](UITimer* timer) { timer->update(); });
	//for_each(timed_callers.begin(), timed_callers.end(), [](TimedCaller *timed_caller) { timed_caller->update(); });

	for (int c = 0; c < uitimers.size(); c++)
		uitimers[c]->update();

	//for_each(ContentManager::shaders.begin(), ContentManager::shaders.end(), [](pair<string, Shader*> shader) { shader.second->Bind(); shader.second->setUniform("time", (float)GameTime); });
}

void Time::restart()
{
	Time::AbsoluteTime = 0;
}

int Time::get_deltatime() const
{
	return DeltaTime;
}

int Time::get_absolutetime() const
{
	return AbsoluteTime;
}

int Time::get_gametime() const
{
	return GameTime;
}

#undef CLASSNAME
#define CLASSNAME Time

void Time::bind_methods()
{
	REG_SINGLETON(TIME);

	REG_METHOD(get_deltatime);
	REG_METHOD(get_absolutetime);
	REG_METHOD(get_gametime);

}

//=========================================================================
//FPSLimiter
//=========================================================================

FPSLimiter::FPSLimiter(int tar)
{
	AbsoluteTime = TIME->AbsoluteTime;
	target = tar;
	lastsectime = AbsoluteTime;
	lastupdatetime = AbsoluteTime;
}

FPSLimiter::FPSInfo FPSLimiter::update()
{
	AbsoluteTime = TIME->get_absolutetime();
	FPSInfo info;

	if (AbsoluteTime - lastupdatetime > target)
	{
		info.needsupdate = true;
		info.delta_time = (AbsoluteTime - lastupdatetime) / 1000.0f;

		lastupdatetime = AbsoluteTime;

		counter++;
	}

	if (AbsoluteTime - lastsectime > 1000 * 1000)
	{
		info.FPS = counter;
		info.passedsec = true;

		counter = 0;
		lastsectime = AbsoluteTime;
	}

	return info;
}

//=========================================================================
//TimedCaller
//=========================================================================

TimedCaller::TimedCaller(int repeat_time, std::function<void()> p_on_call) : timer(repeat_time)
{
	on_call = p_on_call;
	timer.start();
}

void TimedCaller::update()
{
	bool finished = timer.update();

	if (finished)
	{
		on_call();
		timer.start();
	}
}