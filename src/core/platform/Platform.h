#pragma once
class Platform
{
public:
	enum PlatformType
	{
		UNDEF,
		WINDOWS,
		ANDROID,
		LINUX
	};

	Platform() { }
	~Platform() { }

	virtual void InitSDL() { }
	virtual void InitGL() { }
};

