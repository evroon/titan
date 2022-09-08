#pragma once

#include <algorithm>
#include <map>

#include "resources/audio.h"
#include "resources/file.h"
#include "resources/font.h"
#include "resources/shader.h"
#include "resources/textfile.h"
#include "resources/texture.h"
#include "types/typemanager.h"
#include "utility/stringutils.h"

#define CONTENT ContentManager::get_singleton()

#define ASSETS_DIR CONTENT->get_assets_dir()

class ContentManager : public Object {
    OBJ_DEFINITION(ContentManager, Object);

   public:
    ContentManager();

    static void Init();
    static ContentManager* get_singleton();
    static void bind_methods();

    void setup();
    void load_default_resources();

    void AddTexture(Texture2D* tex);

    // load
    Object* Load(const File& p_file);
    Mesh* load_mesh(const File& p_file);
    Texture2D* LoadTexture(const File& p_file);
    Texture2D* LoadFontAwesomeIcon(const String& p_name,
                                   const vec2i& p_size = vec2i(16),
                                   const Color& p_color = Color::White);
    RawTexture2D* LoadRawTexture(const File& p_file);
    TextFile* LoadTextFile(const File& p_file);
    Shader* LoadShader(const File& p_file);
    Font* LoadFont(const File& p_file, int size);
    Music* LoadMusic(const File& p_file);
    SoundEffect* LoadSoundEffect(const File& p_file);
    VariantType GetType(const File& p_file);
    void ReloadAll();
    void FreeAll();

    // free
    void free_textfile(const File& p_file);

    File get_assets_dir() const;

    // Default
    Shader *Shader2D, *SimpleShader, *ShadowShader;
    Font* DefaultFont;

    // Containers
    Vector<Texture2D> textures;
    Vector<Mesh> meshes;
    Vector<TextFile> textfiles;
    Vector<Shader> shaders;
    Vector<Font> fonts;
    Vector<Music> musics;
    Vector<SoundEffect> soundeffects;
    Dictionary<String, VariantType> extensions;

    // Dont delete these data
    Array<String> ProtectedList;

    static ContentManager* singleton;

   private:
    File assets_directory;
};
