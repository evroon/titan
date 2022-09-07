#include "file.h"

#if PLATFORM == WINDOWS
#include "core/platform/dirent.h"
#elif PLATFORM == LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#endif
#include "core/contentmanager.h"
#include "core/definitions.h"

File::File()
{
	path = "";
}

File::File(const String& p_path)
{
	go_to(p_path);
}

File::File(const char* p_path) : File(String(p_path))
{
}


File::~File()
{
}

void File::go_up()
{
	int last = path.find_last('/');

	go_to(path.substr(0, last));
}

void File::go_up(int p_times)
{
	Array<String> elements = path.split('/');

	int length = 0;

	for (int c = 0; c < elements.size() - p_times; c++)
		length += elements[c].length() + 1;

	go_to(path.substr(0, length - 1));
}

void File::go_into(const String & p_name)
{
	go_to(path + "/" + p_name);
}

void File::go_to(const String & p_path)
{
	path = p_path;
	correct_path();
}

String File::get_absolute_path() const
{
	if (!is_absolute_path())
		return ASSETS_DIR + path;

	return path;
}

String File::get_relative_path() const
{
	if (path.starts_with(ASSETS_DIR))
	{
		int size = ASSETS_DIR.get_absolute_path().size();

		String new_path = path.substr(size, path.size() - size);
		new_path.replace('\\', '/');

		while (new_path.starts_with("/"))
			new_path = new_path.substr(1, -1);

		if (new_path.size() == 0)
			new_path = "/";

		return new_path;
	}

	return String();
}

String File::get_name() const
{
	Array<String> elements = path.split('/');

	return elements[elements.size() - 1];
}

String File::get_extension() const
{
	auto elements = path.split('.');
	return elements[elements.size() - 1];
}

File File::operator+(const String & r)
{
	return File(path + '/' + r);
}

void File::operator+=(const String & r)
{
	go_into(r);
}

bool File::operator==(const File & r)
{
	return get_absolute_path() == r.get_absolute_path();
}

bool File::operator!=(const File & r)
{
	return get_absolute_path() != r.get_absolute_path();
}

File::operator String() const
{
	return path;
}

bool File::is_absolute_path() const
{
	return path.starts_with(ASSETS_DIR);
}

Array<File> File::listdir() const
{
	Array<File> result = Array<File>();

	DIR *d = opendir(path.c_str());

	if (!d)
		return result;

	dirent *r;

	while ((r = readdir(d)) != nullptr)
	{
		String s = r->d_name;
		if (s != ".." && s != ".")
			result.push_back(path + "/" + s);
	}

	closedir(d);

	return result;
}

#if PLATFORM == WINDOWS
bool File::is_directory() const
{
	return (get_attributes() & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool File::is_file() const
{
	DWORD attributes = GetFileAttributes(path.c_str());

	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}
unsigned int File::get_attributes() const
{
	DWORD attributes = GetFileAttributes(path.c_str());

	if (attributes == INVALID_FILE_ATTRIBUTES)
	{
		LPSTR messageBuffer = NULL;

		size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

		std::string message(messageBuffer, size);

		T_ERROR(message);

	}
	return attributes;
}
void File::correct_path()
{
	if (!is_absolute_path())
		path = ASSETS_DIR + path;

	path.replace('\\', '/');

	if (path.find_last('/') == path.length() - 1)
		path = path.substr(0, path.length() - 1);

	directory = GetFileAttributes(path.c_str()) == FILE_ATTRIBUTE_DIRECTORY;
	hidden = GetFileAttributes(path.c_str()) == FILE_ATTRIBUTE_HIDDEN;
}
#elif PLATFORM == LINUX
bool File::is_directory() const
{
    struct stat path_stat;
	if (stat(path.c_str(), &path_stat) != 0)
		return false;

    return S_ISDIR(path_stat.st_mode);
}

bool File::is_file() const
{
    struct stat path_stat;
	if (stat(path.c_str(), &path_stat) != 0)
		return false;

    return S_ISREG(path_stat.st_mode);
}

unsigned int File::get_attributes() const
{
	// Not relevant for Linux.
	return 0;
}

void File::correct_path()
{
	path = get_absolute_path();
	path.replace('\\', '/');

	if (path.find_last('/') == path.length() - 1)
		path = path.substr(0, path.length() - 1);

	directory = is_directory();
	hidden = get_name()[0] == '.';
}

#endif