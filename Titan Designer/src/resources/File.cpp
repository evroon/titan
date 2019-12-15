#include "File.h"

#include "core/platform/dirent.h"

#include "core/ContentManager.h"

File::File()
{
	
}

File::File(const String & p_path)
{
	go_to(p_path);
}

File::File(char * p_path) : File(String(p_path))
{
}


File::~File()
{
}

void File::go_up()
{
	int last = path.find_last('\\');

	go_to(path.substr(0, last));
}

void File::go_up(int p_times)
{
	Array<String> elements = path.split('\\');

	int length = 0;
	
	for (int c = 0; c < elements.size() - p_times; c++)
		length += elements[c].length() + 1;

	go_to(path.substr(0, length - 1));
}

void File::go_into(const String & p_name)
{
	go_to(path + "\\" + p_name);
}

void File::go_to(const String & p_path)
{
	path = p_path;
	correct_path();
}

String File::get_absolute_path() const
{
	if (!path.starts_with(ASSETS_DIR))
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
	Array<String> elements = path.split('\\');

	return elements[elements.size() - 1];
}

bool File::is_directory() const
{
	return (get_attributes() & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool File::is_file() const
{
	DWORD attributes = GetFileAttributes(path.c_str());

	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

String File::get_extension() const
{
	auto elements = path.split('.');
	return elements[elements.size() - 1];
}

File File::operator+(const String & r)
{
	return File(path + '\\' + r);
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
	return path.size() > 2 && path[1] == ':' && path[2] == '\\';
}

unsigned int File::get_attributes() const
{
	#if 0
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

	#else
	return 0;
	#endif
}

void File::correct_path()
{
	if (is_absolute_path())
	{

	}
	else
		path = ASSETS_DIR + path;

	path.replace('/', '\\');

	if (path.find_last('\\') == path.length() - 1)
		path = path.substr(0, path.length() - 1);

	directory = GetFileAttributes(path.c_str()) == FILE_ATTRIBUTE_DIRECTORY;
	hidden = GetFileAttributes(path.c_str()) == FILE_ATTRIBUTE_HIDDEN;
}

Array<File> File::listdir() const
{
	Array<File> result;

	DIR *d = opendir(path.c_str());

	if (!d)
		return{};

	dirent *r;

	while ((r = readdir(d)) != NULL)
	{
		if (String(r->d_name) != ".." && String(r->d_name) != ".")
			result.push_back(path + "\\" + r->d_name);
	}

	closedir(d);

	return result;
}