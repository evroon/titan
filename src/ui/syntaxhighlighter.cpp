#include "syntaxhighlighter.h"

SyntaxMaster* SyntaxMaster::singleton;

SyntaxMaster::SyntaxMaster() {
    // GLSL
    glsl.definitions.push_back({"if ", Color::FromRGB(vec3i(255, 32, 125))});
    glsl.definitions.push_back({"else ", Color::FromRGB(vec3i(255, 32, 125))});
    glsl.definitions.push_back({"for ", Color::FromRGB(vec3i(255, 32, 125))});
    glsl.definitions.push_back({"while ", Color::FromRGB(vec3i(255, 32, 125))});

    glsl.definitions.push_back(
        {"attribute ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"uniform ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back({"in ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back({"out ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"gl_Position ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"gl_FragColor ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"layout ", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"varying ", Color::FromRGB(vec3i(255, 187, 40))});

    glsl.definitions.push_back(
        {"EndPrimitive", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"EmitVertex", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"texture", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"texture1D", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"texture2D", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"texture3D", Color::FromRGB(vec3i(255, 187, 40))});
    glsl.definitions.push_back(
        {"texture3D", Color::FromRGB(vec3i(255, 187, 40))});

    glsl.definitions.push_back({"void ", Color::LightBlue});
    glsl.definitions.push_back({"bool ", Color::LightBlue});
    glsl.definitions.push_back({"int ", Color::LightBlue});
    glsl.definitions.push_back({"float ", Color::LightBlue});
    glsl.definitions.push_back({"vec2", Color::LightBlue});
    glsl.definitions.push_back({"vec3", Color::LightBlue});
    glsl.definitions.push_back({"vec4", Color::LightBlue});
    glsl.definitions.push_back({"mat4 ", Color::LightBlue});
    glsl.definitions.push_back({"sampler2D ", Color::LightBlue});
    glsl.definitions.push_back({"sampler3D ", Color::LightBlue});
    glsl.definitions.push_back({"struct ", Color::LightBlue});

    // TitanScript
    titanscript.definitions.push_back(
        {"if ", Color::FromRGB(vec3i(255, 32, 125))});
    titanscript.definitions.push_back(
        {"else ", Color::FromRGB(vec3i(255, 32, 125))});
    titanscript.definitions.push_back(
        {"for ", Color::FromRGB(vec3i(255, 32, 125))});
    titanscript.definitions.push_back(
        {"while ", Color::FromRGB(vec3i(255, 32, 125))});
    titanscript.definitions.push_back(
        {"define ", Color::FromRGB(vec3i(255, 32, 125))});

    titanscript.definitions.push_back(
        {"true", Color::FromRGB(vec3i(255, 187, 40))});
    titanscript.definitions.push_back(
        {"false", Color::FromRGB(vec3i(255, 187, 40))});

    titanscript.definitions.push_back({"func ", Color::LightBlue});
    titanscript.definitions.push_back({"extends ", Color::LightBlue});
    titanscript.definitions.push_back({"bool", Color::LightBlue});
    titanscript.definitions.push_back({"vec2", Color::LightBlue});
    titanscript.definitions.push_back({"vec3", Color::LightBlue});
    titanscript.definitions.push_back({"vec4", Color::LightBlue});
    titanscript.definitions.push_back({"mat4", Color::LightBlue});
}

SyntaxMaster::~SyntaxMaster() {}

SyntaxMaster* SyntaxMaster::get_singleton() { return singleton; }

void SyntaxMaster::init() { singleton = new SyntaxMaster; }

SyntaxHighlighter* SyntaxMaster::get_highlighter(
    SyntaxHighlighter::Language p_language) {
    switch (p_language) {
        case SyntaxHighlighter::C_PLUS_PLUS:
            return &get_singleton()->c_plus_plus;

        case SyntaxHighlighter::GLSL:
            return &get_singleton()->glsl;

        case SyntaxHighlighter::XML:
            return &get_singleton()->xml;

        case SyntaxHighlighter::TITANSCRIPT:
            return &get_singleton()->titanscript;

        default:
            return nullptr;
    }
}
