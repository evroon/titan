#include "ContentManager.h"

#include <string>
#include <iostream>
#include <fstream>

#include "core/platform/dirent.h"

#include "resources/File.h"

ContentManager* ContentManager::singleton;

ContentManager::ContentManager()
{
}

void ContentManager::Init()
{

	singleton = new ContentManager;
}

ContentManager* ContentManager::get_singleton()
{
	return singleton;
}

#undef CLASSNAME
#define CLASSNAME ContentManager

void ContentManager::bind_methods()
{
	REG_SINGLETON(CONTENT);

	//REG_METHOD(Load);
}

void ContentManager::setup()
{
	extensions["png"] = GETTYPE(StringName("Texture2D"));
	extensions["jpg"] = GETTYPE(StringName("Texture2D"));
	extensions["txt"] = GETTYPE(StringName("TextFile"));
	extensions["xml"] = GETTYPE(StringName("TextFile"));
	extensions["wav"] = GETTYPE(StringName("SoundEffect"));
	extensions["ttf"] = GETTYPE(StringName("Font"));
	extensions["frag"] = GETTYPE(StringName("Shader"));
	extensions["vert"] = GETTYPE(StringName("Shader"));
	extensions["ts"] = GETTYPE(StringName("TitanScript"));

	char buffer[256];
	DWORD len = 256;

	int bytes = GetModuleFileName(NULL, buffer, len);
	if (bytes == 0)
	{
		T_ERROR("Could not find directory, error: " + GetLastError());
	}

	File temp = buffer;
	temp.go_up(3);
	temp.go_into("Titan Designer\\assets");

	assets_directory = temp;

	Shader2D = LoadShader(File("EngineCore/Shader2D"));
	SimpleShader = LoadShader(File("EngineCore/SimpleShader"));
	ShadowShader = LoadShader(File("EngineCore/ShadowShader"));

	//DefaultFont = LoadFont("EngineCore/Fonts/consola.ttf", 15);
}

Object* ContentManager::Load(const File &p_file)
{
	VariantType type = GetType(p_file);

	static StringName Texture2D = "Texture2D";
	static StringName Shader = "Shader";
	static StringName Font = "Font";
	static StringName SoundEffect = "SoundEffect";
	static StringName TextFile = "TextFile";

	if (type == Texture2D)
		return LoadTexture(p_file);
	else if (type == Shader)
		return LoadShader(p_file);
	else if (type == Font)
		return LoadFont(p_file, 20);
	else if (type == SoundEffect)
		return LoadSoundEffect(p_file);
	else if (type == TextFile)
		return LoadTextFile(p_file);
	else
		return 0;
}

void ContentManager::AddTexture(Texture2D *tex)
{
	textures.push_back(tex);
}

Texture2D* ContentManager::LoadTexture(const File &p_file)
{
	for (int c = 0; c < textures.size(); c++)
		if (textures[c]->file.get_absolute_path() == p_file)
			return textures[c];

	Texture2D *tex = new Texture2D(p_file);
	tex->file = p_file;
	
	textures.push_back(tex);
	return tex;
}

RawTexture2D* ContentManager::LoadRawTexture(const File &p_file)
{
	RawTexture2D *tex = new RawTexture2D(p_file);
	tex->file = p_file;

	return tex;
}

TextFile* ContentManager::LoadTextFile(const File &p_file)
{
	for (int c = 0; c < textfiles.size(); c++)
		if (textfiles[c]->file.get_absolute_path() == p_file)
			return textfiles[c];

	TextFile *text = new TextFile(p_file);
	textfiles.push_back(text);
	return text;
}

Shader* ContentManager::LoadShader(const File &p_file)
{
	for (Shader *s : shaders)
		if (s->file.get_absolute_path() == p_file)
			return s;

	Shader *s = new Shader(p_file);
	shaders.push_back(s);
	return s;
}

Font* ContentManager::LoadFont(const File &p_file, int size)
{
	for (int c = 0; c < fonts.size(); c++)
		if (fonts[c]->file.get_absolute_path() == p_file)
			return fonts[c];

	Font *f = new Font(p_file, size);
	fonts.push_back(f);
	return f;
}

Music* ContentManager::LoadMusic(const File &p_file)
{
	for (int c = 0; c < musics.size(); c++)
		if (musics[c]->file.get_absolute_path() == p_file)
			return musics[c];

	Music *s = new Music(p_file);
	musics.push_back(s);
	return s;
}

SoundEffect* ContentManager::LoadSoundEffect(const File &p_file)
{
	for (int c = 0; c < soundeffects.size(); c++)
		if (soundeffects[c]->file.get_absolute_path() == p_file)
			return soundeffects[c];

	SoundEffect *s = new SoundEffect(p_file);
	soundeffects.push_back(s);
	return s;
}

VariantType ContentManager::GetType(const File& p_file)
{
	String filename = p_file.get_absolute_path();
	String ext = filename.substr(StringUtils::GetLastIndexOf(filename, '.') + 1, filename.size() - 1);
	return extensions[ext];
}

#define RELOAD(X) for (int c = 0; c < X.size(); c++) \
					  X[c]->Reload();

void ContentManager::ReloadAll()
{
	RELOAD(fonts)
	RELOAD(musics)
	RELOAD(soundeffects)
	RELOAD(shaders)
	RELOAD(textures)
	RELOAD(textfiles)
}

void ContentManager::FreeAll()
{
	fonts.clean();
	musics.clean();
	soundeffects.clean();
	shaders.clean();
	textures.clean();
	textfiles.clean();

	Font::Quit();
}

void ContentManager::free_textfile(const File & p_file)
{
	for (int c = 0; c < textfiles.size(); c++)
		if (textfiles[c]->file.get_absolute_path() == p_file)
			textfiles.clear(c);
}

File ContentManager::get_assets_dir() const
{
	return assets_directory;
}
