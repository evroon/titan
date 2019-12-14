#pragma once

#include <map>
#include <algorithm>

#include "resources/Texture.h"
#include "resources/Shader.h"
#include "resources/Font.h"
#include "resources/Audio.h"
#include "utility/StringUtils.h"
#include "resources/TextFile.h"
#include "types/TypeManager.h"

#include "resources/File.h"

#define CONTENT ContentManager::get_singleton()

#define ASSETS_DIR CONTENT->get_assets_dir()

class ContentManager : public Object
{
	OBJ_DEFINITION(ContentManager, Object);

public:
	ContentManager();

	static void Init();
	static ContentManager* get_singleton();
	static void bind_methods();

	void setup();

	void AddTexture(Texture2D *tex);

	// load
	Object* Load(const File &p_file);
	Mesh* load_mesh(const File& p_file);
	Texture2D* LoadTexture(const File& p_file);
	Texture2D* LoadFontAwesomeIcon(const String& p_name, const vec2i& p_size = vec2i(16), const Color& p_color = Color::White);
	RawTexture2D* LoadRawTexture(const File & p_file);
	TextFile* LoadTextFile(const File& p_file);
	Shader* LoadShader(const File& p_file);
	Font* LoadFont(const File& p_file, int size);
	Music* LoadMusic(const File& p_file);
	SoundEffect* LoadSoundEffect(const File& p_file);
	VariantType GetType(const File& p_file);
	void ReloadAll();
	void FreeAll();

	// reload


	// free
	void free_textfile(const File& p_file);

	File get_assets_dir() const;

	//Default
	Shader *Shader2D, *SimpleShader, *ShadowShader;
	Font* DefaultFont;

	//Containers
	Vector<Texture2D> textures;
	Vector<Mesh> meshes;
	Vector<TextFile> textfiles;
	Vector<Shader> shaders;
	Vector<Font> fonts;
	Vector<Music> musics;
	Vector<SoundEffect> soundeffects;
	Dictionary<String, VariantType> extensions;

	//Dont delete these data
	Array<String> ProtectedList;

	static ContentManager *singleton;

private:
	File assets_directory;
};