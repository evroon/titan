#include "Optics.h"

#if 0

Shader* Optics::lightshader, *Optics::shadowshader;
Vector<Light> Optics::lights;

bool Optics::lightingenabled;

void Optics::Init()
{
	lightshader = ContentManager::LoadShader("engine/shaders/Light");
	shadowshader = ContentManager::ShadowShader;

	ContentManager::ShadowShader = shadowshader;
	shadowshader->Bind();

	lightingenabled = false;
}

void Optics::DrawLights()
{
	if (!lightingenabled)
		return;

	//Draw Lights
	FBOManager::BindFBO(FBOManager::LightFBO);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	for (int c = 0; c < lights.size(); c++)
		lights[c]->Draw();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Optics::DrawShadows(IndexedMap<WorldObject> * objects)
{
	if (!lightingenabled) return;
	FBOManager::BindFBO(FBOManager::ShadowFBO);
	FBOManager::ShadowFBO->tex->Bind(0);

	glBlendFunc(GL_ONE, GL_ONE);

	Optics::shadowshader->Bind();
	for (Light *light : lights )
	{
		float radius = (float)light->radius / Titan::width;
		vec2 lightpos = light->transform.pos / vec2(Titan::width, Titan::height);

		shadowshader->Bind();
		shadowshader->setUniform("lightpos", lightpos);
		shadowshader->setUniform("radius", radius);

		for (pair<int, WorldObject*> p : *objects)
			//if (p.second->castsshadows&& MathHelper::Distance(p.second->GetTransform().transform.pos, light->transform.pos) < 2 * light->radius)
				ShadowCaster::DrawBoxShadow(p.second->GetTransform(), light);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

Light* Optics::AddLight(vec2 pos, double radius)
{
	lightingenabled = true;
	Light *light = new Light(lightshader, NULL, pos, radius);
	lights.push_back(light);
	return light;
}

void Optics::GoDark()
{
	lights.clean();
}

#endif