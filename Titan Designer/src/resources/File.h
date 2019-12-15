#pragma once

#include "core/Object.h"

#if PLATFORM == WINDOWS
#include "core/platform/dirent.h"
#endif


class File : public Object
{
	OBJ_DEFINITION(File, Object);

public:
	File();
	File(const String& p_path);
	File(char* p_path); //helper

	virtual ~File();

	void go_up();
	void go_up(int p_times);
	void go_into(const String& p_name);
	void go_to(const String& p_path);

	String get_absolute_path() const;
	String get_relative_path() const;

	String get_name() const;

	bool is_directory() const;	
	bool is_file() const;

	String get_extension() const;

	Array<File> listdir() const;

	File operator+(const String& r);
	void operator+=(const String &r);

	bool operator==(const File& r);
	bool operator!=(const File& r);

	operator String() const;



private:
	void correct_path();

	bool is_absolute_path() const;

	unsigned int get_attributes() const;


	bool directory;
	bool hidden;
	bool archive;
	String path;
};

