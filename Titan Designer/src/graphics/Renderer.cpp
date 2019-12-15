#include "Renderer.h"

#include "core/WindowManager.h"
#include "View.h"

#include "world/Sky.h"
#include "world/Terrain.h"
#include "world/Environment.h"
#include "world\Light.h"

MasterRenderer* MasterRenderer::singleton;

//=========================================================================
//MasterRenderer
//=========================================================================

void MasterRenderer::init()
{
	FBOMANAGER->bind_default_fbo();
	glClearColor(0.3f, 0.3f, 0.3f, 1);

	singleton = new MasterRenderer;

	MeshHandler::get_singleton()->Init();
	Sprite::Init();
	Primitives::Init();
}

void MasterRenderer::set_active_renderer(Renderer* p_renderer)
{
	active_renderer = p_renderer;
}

Renderer* MasterRenderer::get_active_renderer() const
{
	return active_renderer;
}

MasterRenderer* MasterRenderer::get_singleton()
{
	return singleton;
}

//=========================================================================
//Renderer
//=========================================================================

Renderer::Renderer()
{
	init();
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	//use_depth_test(0.5f, 1000.0f);
	//use_culling();
	use_blending();

	draw_on_screen = true;
}

void Renderer::resized()
{
	vec2 size = viewport->get_size();

	canvas_matrix = mat4::Scale(vec3(1.0f / size.x, 1.0f / size.y, 0.0f));
}

void Renderer::check_error()
{
	GLenum err = GL_NO_ERROR;
	while ((err = glGetError()) != GL_NO_ERROR)
		T_ERROR("OpenGL error: " + std::to_string(err));
}

void Renderer::prepare()
{
	check_error();

	FBOMANAGER->clear_all();
	//render_buffer->bind();
}

void Renderer::finish()
{

}

void Renderer::set_camera(Camera* p_camera)
{
	camera = p_camera;

	if (camera)
	{
		projection_matrix = p_camera->get_projection_matrix();
		view_matrix = p_camera->get_view_matrix();
		final_matrix = p_camera->get_final_matrix();
	}
}

void Renderer::set_viewport(Viewport* p_viewport)
{
	viewport = p_viewport;
}

Viewport* Renderer::get_viewport() const
{
	return viewport;
}

const mat4& Renderer::get_projection_matrix() const
{
	return projection_matrix;
}

const mat4& Renderer::get_view_matrix() const
{
	return view_matrix;
}

const mat4& Renderer::get_final_matrix() const
{
	return final_matrix;
}

void Renderer::use_scissor(const rect2 &area)
{
	glEnable(GL_SCISSOR_TEST);

	glScissor(
		WINDOWSIZE.x / 2 + (int)area.get_bottom_left().x,
		WINDOWSIZE.y / 2 + (int)area.get_bottom_left().y,
		(int)area.size.x * 2,
		(int)area.size.y * 2);
}
void Renderer::stop_scissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void Renderer::use_depth_test(float p_near, float p_far)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthRange(p_near, p_far);
}

void Renderer::stop_depth_test()
{
	glDisable(GL_DEPTH_TEST);
}

void Renderer::use_culling()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Renderer::stop_culling()
{
	glDisable(GL_CULL_FACE);
}

void Renderer::use_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::use_additive_blending()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
}

void Renderer::stop_blending()
{
	glDisable(GL_BLEND);
}

void Renderer::set_draw_on_screen(bool p_draw_on_screen)
{
	draw_on_screen = p_draw_on_screen;
}

bool Renderer::get_draw_on_screen() const
{
	return draw_on_screen;
}

void Renderer::use_wireframe(bool p_wireframe)
{
	if (p_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

Texture2D* Renderer::get_texture(int p_type) const
{
	return textures[p_type];
}

FBO2D* Renderer::get_fbo(int p_type) const
{
	return buffers[p_type];
}

void Renderer::activate()
{
	MASTER_RENDERER->set_active_renderer(this);
	//set_viewport();
}

void Renderer::deactivate()
{
	MASTER_RENDERER->set_active_renderer(NULL);
	glViewport(0, 0, WINDOWSIZE.x, WINDOWSIZE.y);
}

void Renderer::set_viewport()
{
	vec2 bottom_left = WINDOWSIZE_F / 2.0f;

	glViewport(
		to_int(bottom_left.x + viewport->renderarea.get_left()),
		to_int(bottom_left.y + viewport->renderarea.get_bottom()),
		to_int(viewport->renderarea.size.x * 4.0f),
		to_int(viewport->renderarea.size.y * 4.0f));
}

void Renderer::activate_world_transform()
{
	set_camera(viewport->world->get_active_camera());
}

void Renderer::deactivate_world_transform()
{
	set_camera(NULL);
}

void Renderer::activate_canvas_transform()
{
	projection_matrix = canvas_matrix;
	view_matrix = mat4();
	update();
}

void Renderer::deactivate_canvas_transform()
{
	projection_matrix = mat4();
	view_matrix = mat4();
	update();
}

void Renderer::render()
{
}

void Renderer::draw_plane()
{
	MeshHandler::get_singleton()->get_plane()->bind();
	MeshHandler::get_singleton()->get_plane()->draw();
	MeshHandler::get_singleton()->get_plane()->unbind();
}

void Renderer::draw_line(const vec3& p_start, const vec3& p_end, const Color& p_color)
{
	SimpleMesh* mesh = MeshHandler::get_singleton()->get_line();
	Shader* shader = CanvasData::get_singleton()->get_default_shader();

	// center-like coordinates
	//vec3 delta = p_end - p_start;
	//vec3 center = (p_end + p_start) / 2.0f;

	//float angle = delta.angle(vec2(0, -1.0f));
	//float length = delta.length() / 2.0f;

	Transform transform;

	shader->bind();
	shader->set_uniform("color", p_color);
	shader->set_uniform("texture_enabled", false);
	shader->set_uniform("model", get_final_matrix() * transform.get_model());
	mesh->bind();
	mesh->draw();

	mesh = MeshHandler::get_singleton()->get_plane();
	mesh->bind();
}

void Renderer::update()
{
	final_matrix = projection_matrix * view_matrix;
}

#undef CLASSNAME
#define CLASSNAME Renderer

void Renderer::bind_methods()
{
}

//=========================================================================
//ForwardRenderer
//=========================================================================

ForwardRenderer::ForwardRenderer() : Renderer()
{
	render_buffer = new FBO2D(WINDOWSIZE);
	render_buffer->add_color_texture();
	render_buffer->add_depth_texture();
	render_buffer->init();

	shadow_buffer = new FBO2D(1024);
	shadow_buffer->add_depth_texture();
	shadow_buffer->init();

	reflection_buffer = new FBO2D(WINDOWSIZE);
	reflection_buffer->add_color_texture();
	reflection_buffer->add_depth_texture();
	reflection_buffer->init();
}

ForwardRenderer::ForwardRenderer(Viewport* p_viewport) : ForwardRenderer()
{
	viewport = p_viewport;
}

ForwardRenderer::~ForwardRenderer()
{
	
}

void ForwardRenderer::set_light_matrix(const mat4& p_light_matrix)
{
	light_matrix = p_light_matrix;
}

const mat4& ForwardRenderer::get_light_matrix() const
{
	return light_matrix;
}

FBO2D* ForwardRenderer::get_shadow_buffer() const
{
	return shadow_buffer;
}

FBO2D* ForwardRenderer::get_render_buffer() const
{
	return render_buffer;
}

FBO2D* ForwardRenderer::get_reflection_buffer() const
{
	return reflection_buffer;
}

void ForwardRenderer::render()
{
	set_viewport();
	
	FBOMANAGER->bind_default_fbo();

	if ((viewport->mode == Viewport::FRAMEBUFFER || viewport->mode == Viewport::POSTPROCESS) && viewport->fbo)
		viewport->fbo->bind();

	activate();

	if (viewport->world)
	{
		Camera* c = viewport->world->get_active_camera();
		c->update_matrices();

		RENDERER->use_depth_test(c->get_near(), c->get_far());

		activate_world_transform();
		viewport->world->draw();
		deactivate_world_transform();
	}

	RENDERER->stop_depth_test();

	if (viewport->canvas)
	{
		MeshHandler::get_singleton()->get_plane()->bind();
		activate_canvas_transform();
		viewport->canvas->draw();
		viewport->post_draw_canvas();
		deactivate_canvas_transform();
	}

	deactivate();

	if (viewport->mode == Viewport::POSTPROCESS && viewport->postprocess)
		viewport->postprocess->post_process();

	if (viewport->fbo)
		viewport->fbo->unbind();
}

#undef CLASSNAME
#define CLASSNAME ForwardRenderer

void ForwardRenderer::bind_methods()
{

}

//=========================================================================
//DefferedRenderer
//=========================================================================

DeferredRenderer::DeferredRenderer()
{
	deferred_buffer = new FBO2D(WINDOWSIZE);
	deferred_buffer->add_float_color_texture(); // albedo
	deferred_buffer->add_float_color_texture(); // position
	deferred_buffer->add_float_color_texture(); // normal
	deferred_buffer->add_float_color_texture(); // specular
	deferred_buffer->add_color_texture(); // material
	//deferred_buffer->add_color_texture(WINDOWSIZE / 8); // mipmap
	deferred_buffer->add_depth_texture(); // depth
	deferred_buffer->init();

	render_buffer = new FBO2D(WINDOWSIZE);
	render_buffer->add_float_color_texture();
	render_buffer->add_depth_texture();
	render_buffer->init();

	final_buffer = new FBO2D(WINDOWSIZE);
	final_buffer->add_color_texture();
	final_buffer->add_depth_texture();
	final_buffer->init();
	
	shadow_buffer = new FBO2D(1024);
	shadow_buffer->add_depth_texture();
	shadow_buffer->init();

	ssao_buffer = new FBO2D(WINDOWSIZE);
	ssao_buffer->add_color_texture();
	ssao_buffer->init();

	ssao_blur_buffer = new FBO2D(WINDOWSIZE);
	ssao_blur_buffer->add_color_texture();
	ssao_blur_buffer->init();

	reflection_buffer = new FBO2D(512);
	reflection_buffer->add_color_texture();
	reflection_buffer->add_depth_texture();
	reflection_buffer->init();

	godray_buffer = new FBO2D(WINDOWSIZE / 2);
	godray_buffer->add_color_texture();
	godray_buffer->init();
	godray_buffer->clear_color = Color(0, 0, 0);

	bloom_buffer = new FBO2D(WINDOWSIZE / 2);
	bloom_buffer->add_color_texture();
	bloom_buffer->init();
	bloom_buffer->clear_color = Color(0, 0, 0);

	virtual_tex_buffer = new FBO2D(1024);
	virtual_tex_buffer->add_color_texture();
	virtual_tex_buffer->init();
	virtual_tex_buffer->clear_color = Color(0, 0, 0);
	virtual_tex_buffer->cleared_every_frame = false;
	virtual_tex_buffer->clear();

	indirection_buffer = new FBO2D(2048);
	indirection_buffer->add_color_texture();
	indirection_buffer->init();
	indirection_buffer->clear_color = Color(0, 0, 0);
	indirection_buffer->cleared_every_frame = false;
	indirection_buffer->clear();

	save_buffer = new FBO2D(1024);
	save_buffer->add_color_texture();
	save_buffer->init();
	save_buffer->clear_color = Color(0, 0, 0);

	buffers.push_back(deferred_buffer);
	buffers.push_back(render_buffer);
	buffers.push_back(final_buffer);
	buffers.push_back(shadow_buffer);
	buffers.push_back(reflection_buffer);
	buffers.push_back(bloom_buffer);

	textures.set(DEFERRED_ALBEDO, deferred_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(DEFERRED_POSITION, deferred_buffer->color_textures[1]->cast_to_type<Texture2D*>());
	textures.set(DEFERRED_NORMAL, deferred_buffer->color_textures[2]->cast_to_type<Texture2D*>());
	textures.set(DEFERRED_MATERIAL, deferred_buffer->color_textures[3]->cast_to_type<Texture2D*>());
	textures.set(DEFERRED_SPECULAR, deferred_buffer->color_textures[4]->cast_to_type<Texture2D*>());
	textures.set(DEFERRED_DEPTH, deferred_buffer->depth_tex->cast_to_type<Texture2D*>());
	textures.set(SHADOW, shadow_buffer->depth_tex->cast_to_type<Texture2D*>());
	textures.set(FINAL_COLOR, final_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(RENDER_COLOR, render_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(RENDER_DEPTH, render_buffer->depth_tex->cast_to_type<Texture2D*>());
	textures.set(REFLECTION, reflection_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(SSAO, ssao_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(SSAO_BLUR, ssao_blur_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(GODRAY, godray_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(BLOOM, bloom_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	//textures.set(DOF, dof_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	//textures.set(LIGHTING, lighting_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(VIRTUALTEX, virtual_tex_buffer->color_textures[0]->cast_to_type<Texture2D*>());
	textures.set(INDIRECTION, indirection_buffer->color_textures[0]->cast_to_type<Texture2D*>());

	first_pass = CONTENT->LoadShader("EngineCore/Shaders/FirstPass");
	second_pass = CONTENT->LoadShader("EngineCore/Shaders/SecondPass");
	godray = CONTENT->LoadShader("EngineCore/Shaders/Godray");
	shader_2d = CanvasData::get_singleton()->get_default_shader();
	ssao = CONTENT->LoadShader("EngineCore/Shaders/SSAO");
	bloom = CONTENT->LoadShader("EngineCore/Shaders/Bloom");
	tex_shader = CONTENT->LoadShader("EngineCore/Shaders/TextureShader");

	reflection_camera = new Camera;
	light_camera = new Camera;

	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex0.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex1.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex2.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex3.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex4.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex5.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex6.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex7.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex8.png"));
	flare_textures.push_back(CONTENT->LoadTexture("Textures/oreon/lens_flare/tex9.png"));

	grid_texture = CONTENT->LoadTexture("Textures/tile.png");

	generate_ssao_kernel();
}

DeferredRenderer::~DeferredRenderer()
{

}

vec3 DeferredRenderer::get_position_at_pixel(const vec2& p_pos) const
{
	return deferred_buffer->read_pixel(p_pos, 1).get_rgb();
}

vec3 DeferredRenderer::get_material_at_pixel(const vec2& p_pos) const
{
	return deferred_buffer->read_pixel(p_pos, 3).get_rgb();
}

void DeferredRenderer::generate_ssao_kernel()
{
	vec3 kernel_buffer[64];

	for (unsigned int i = 0; i < 64; ++i)
	{
		vec3 sample = vec3(Math::random() * 2.0f - 1.0f,	Math::random() * 2.0f - 1.0f,	Math::random());
		sample = sample.normalize();

		float scale = i / 64.0f;
		scale = Math::lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;

		kernel_buffer[i] = sample;
	}

	glGenTextures(1, &kernel_id);
	glBindTexture(GL_TEXTURE_2D, kernel_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 8, 8, 0, GL_RGB, GL_FLOAT, &kernel_buffer[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	vec3 noise_buffer[16];

	for (unsigned int i = 0; i < 16; ++i)
	{
		vec3 sample = vec3(Math::random() * 2.0f - 1.0f, Math::random() * 2.0f - 1.0f, 0.0f);
		noise_buffer[i] = sample;
	}

	glGenTextures(1, &noise_id);
	glBindTexture(GL_TEXTURE_2D, noise_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise_buffer[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void DeferredRenderer::render_bloom()
{
	bloom->bind();
	bloom_buffer->bind();

	textures[FINAL_COLOR]->bind(0);
	bloom->set_uniform("input", 0);
}

void DeferredRenderer::render_ssao()
{
	ssao->bind();
	ssao_buffer->bind();

	textures[DEFERRED_POSITION]->bind(0);
	textures[DEFERRED_NORMAL]->bind(1);

	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, noise_id);

	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, kernel_id);

	ssao->set_uniform("g_position", 0);
	ssao->set_uniform("g_normal", 1);
	ssao->set_uniform("noise", 2);
	ssao->set_uniform("kernel", 3);

	ssao->set_uniform("window_size", WINDOWSIZE_F);
	ssao->set_uniform("kernel_size", 64);
	ssao->set_uniform("radius", 0.5f);

	activate_world_transform();
	ssao->set_uniform("projection", final_matrix);
	deactivate_world_transform();

	draw_plane();
}

void DeferredRenderer::render_shadowmap()
{
	Camera* c = viewport->world->get_active_camera();
	DirectionalLight* light = ACTIVE_WORLD->get_active_light();

	light_camera->set_pos(light->get_pos());
	light_camera->set_rotation(light->get_rotation());
	light_camera->set_projection(30.0f, 1.0f, 100.0f);

	light_camera->update_matrices();

	shadow_buffer->bind();
	set_camera(light_camera);
	//set_camera(c);

	light_matrix = light_camera->get_final_matrix(); 

	RENDERER->use_depth_test(c->get_near(), c->get_far());
	
	for (int c = 0; c < viewport->world->get_child_count(); c++)
	{
		Node* n = viewport->world->get_child_by_index(c);
		WorldObject* wo = n->cast_to_type<WorldObject*>();
		wo->notificate(WorldObject::NOTIFICATION_DRAW);
	}
	
	RENDERER->stop_depth_test();

	set_camera(NULL);
}

void DeferredRenderer::render_reflection()
{
	Camera* c = ACTIVE_WORLD->get_active_camera();

	vec3 p = c->get_pos();
	vec3 r = c->get_rotation();

	reflection_camera->set_pos(vec3(p.x, p.y, p.z));
	reflection_camera->set_rotation(vec3(r.x, r.y, r.z));
	reflection_camera->set_projection(30.0f, 0.5f, 1000.0f);
	reflection_camera->update();

	reflection_buffer->bind();
	set_camera(reflection_camera);

	RENDERER->use_depth_test(c->get_near(), c->get_far());

	for (int c = 0; c < ACTIVE_WORLD->get_child_count(); c++)
	{
		Node* n = ACTIVE_WORLD->get_child_by_index(c);
		WorldObject* wo = n->cast_to_type<WorldObject*>();
		wo->notificate(WorldObject::NOTIFICATION_DRAW);
	}

	RENDERER->stop_depth_test();

	set_camera(NULL);
}

void DeferredRenderer::render_flare()
{
	int count = 10;

	Camera* camera = ACTIVE_WORLD->get_active_camera();
	Sky* sky = ACTIVE_WORLD->get_child_by_type<Sky*>();
	DirectionalLight* light = ACTIVE_WORLD->get_child_by_type<DirectionalLight*>();

	if (!camera || !sky || !light)
		return;

	vec3 light_dir = light->get_transform().get_quat().get_axis();
	light_dir = vec3(0, 0, 10);

	vec3 view_pos = camera->get_pos();
	vec3 light_position = light_dir + view_pos;

	activate_world_transform();
	vec4 s = (final_matrix * vec4(light_position, 1.0f));

	if (s.w < 0)
		return;

	vec2 sun_on_screen = s.get_xy() / s.w;
	deactivate_world_transform();

	sun_on_screen *= viewport->get_size();

	stop_depth_test();
	use_additive_blending();

	shader_2d->bind();
	shader_2d->set_uniform("texture_enabled", true);
	shader_2d->set_uniform("tex", 0);

	final_buffer->bind();

	activate_canvas_transform();

	float length = sun_on_screen.length();
	float stepsize = length * 2.0f / (count - 1);
	vec2 dir = sun_on_screen.normalize();

	float max_length = viewport->get_size().x;
	shader_2d->set_uniform("color", vec4(1.0f, 1.0f, 1.0f, 1.0f - length / max_length));
	shader_2d->set_uniform("texbounds", vec4(0.0f, 1.0f, 0.0f, 1.0f));

	for (int c = 0; c < count; c++)
	{
		Transform t;

		vec2 size = vec2(100.0f);

		t.set_size(vec3(size, 1.0f));
		t.set_pos(vec3(sun_on_screen + dir * to_float(c) * stepsize, 0.0f));
		t.update();

		flare_textures[c]->bind(0);

		shader_2d->set_uniform("model", get_final_matrix() * t.get_model());

		draw_plane();
	}

	deactivate_canvas_transform();

	use_blending();
}

void DeferredRenderer::render_godray()
{
	Camera* camera = ACTIVE_WORLD->get_active_camera();
	Sky* sky = ACTIVE_WORLD->get_child_by_type<Sky*>();
	DirectionalLight* light = ACTIVE_WORLD->get_child_by_type<DirectionalLight*>();

	if (!camera || !sky || !light)
		return;

	vec3 light_dir = light->get_transform().get_quat().get_axis();
	light_dir = vec3(0, 0, 1);
	vec3 view_pos = camera->get_pos();
	vec3 light_position = light_dir + view_pos;

	activate_world_transform();
	vec4 s = (final_matrix * vec4(light_position, 1.0f));
	vec2 sun_on_screen = s.get_xy() / s.w;
	deactivate_world_transform();
	
	godray_buffer->bind();

	textures[DEFERRED_MATERIAL]->bind(0);

	godray->bind();
	godray->set_uniform("g_material", 0);
	godray->set_uniform("sun_color", sky->get_sun_color().get_rgb());
	godray->set_uniform("sun_pos", sun_on_screen);
	
	draw_plane();
}

void DeferredRenderer::render_first_pass()
{
	textures[DEFERRED_ALBEDO]->bind(0);
	textures[DEFERRED_POSITION]->bind(1);
	textures[DEFERRED_NORMAL]->bind(2);
	textures[DEFERRED_MATERIAL]->bind(3);
	textures[SHADOW]->bind(4);
	textures[GODRAY]->bind(5);
	textures[BLOOM]->bind(6);

	Color sky_color;
	Color light_color;
	vec3 light_dir = vec3(0, 0, -1);
	Sky* sky = ACTIVE_WORLD->get_child_by_type<Sky*>();
	vec3 view_pos = ACTIVE_WORLD->get_active_camera()->get_pos();
	DirectionalLight* light = ACTIVE_WORLD->get_active_light();
	bool lighting_enabled = false;

	if (sky)
	{
		vec3 light_position = sky->get_sun_direction() + view_pos;
		Color sky_color = sky->get_sky_color();
	}

	if (light)
	{
		light_dir = light->get_transform().get_quat().get_axis();
		lighting_enabled = true;
		light_color = light->get_color();
	}

	first_pass->bind();
	first_pass->set_uniform("clouds_enabled", false);
	first_pass->set_uniform("godray_enabled", true);
	first_pass->set_uniform("g_albedo", 0);
	first_pass->set_uniform("g_position", 1);
	first_pass->set_uniform("g_normal", 2);
	first_pass->set_uniform("g_material", 3);
	first_pass->set_uniform("shadow_map", 4);
	first_pass->set_uniform("godray_tex", 5);

	first_pass->set_uniform("light_matrix", light_matrix);
	first_pass->set_uniform("lighting_enabled", lighting_enabled);
	first_pass->set_uniform("light_dir", light_dir);
	first_pass->set_uniform("light_color", vec3(1.0));


	if (environment)
	{
		first_pass->set_uniform("ambient", environment->get_ambient_color().get_rgb());
		first_pass->set_uniform("fog_enabled", environment->get_fog_enabled());
		first_pass->set_uniform("fog_density", environment->get_fog_density());
		first_pass->set_uniform("fog_gradient", environment->get_fog_gradient());
	}
	else
	{
		first_pass->set_uniform("ambient", vec3(0.4f));
		first_pass->set_uniform("fog_enabled", false);
	}

	first_pass->set_uniform("view_pos", view_pos);
	first_pass->set_uniform("light_color", vec3(1.0f));
	first_pass->set_uniform("specular_strength", .1f);
	first_pass->set_uniform("specular_power", 32.0f);
	first_pass->set_uniform("sky_color", sky_color.rgb);

	final_buffer->bind();

	draw_plane();
}

void DeferredRenderer::render_second_pass()
{
	second_pass->bind();
	second_pass->set_uniform("gamma", 1.0f);
	second_pass->set_uniform("render_buffer", 0);
	second_pass->set_uniform("exposure", 1.0f);

	textures[FINAL_COLOR]->bind(0);

	if (draw_on_screen)
		render_buffer->unbind();
	else
		render_buffer->bind();

	draw_plane();
}

void DeferredRenderer::render_physical_tile(const vec2& p_pos, const vec2& p_size)
{
	vec2 texsize = virtual_tex_buffer->color_textures[0]->cast_to_type<Texture2D*>()->get_size();

	float left = -1.0f + 2.0f * p_pos.x / texsize.x;
	float right = -1.0f + 2.0f * (p_pos.x + p_size.x) / texsize.x;
	float bottom = -1.0f + 2.0f * p_pos.y / texsize.y;
	float top = -1.0f + 2.0f * (p_pos.y + p_size.y) / texsize.y;

	Transform t = Transform(rect2(left, right, top, bottom));
	tex_shader->set_uniform("model", t.get_model());

	draw_plane();
}

void DeferredRenderer::render_virtual_tex()
{
	// render to physics texture
	virtual_tex_buffer->bind();
	grid_texture->bind(0);
	tex_shader->bind();

	render_physical_tile(vec2(), vec2(128, 128));
	render_physical_tile(vec2(128, 0), vec2(64, 64));
	render_physical_tile(vec2(128 + 64, 0), vec2(64, 64));
	render_physical_tile(vec2(128, 64), vec2(64, 64));
	render_physical_tile(vec2(128 + 64, 64), vec2(64, 64));

	// render to indirection texture
	float offset = 128.0f / 1024.0f;
	indirection_buffer->bind();
	shader_2d->bind();
	shader_2d->set_uniform("texture_enabled", false);
	shader_2d->set_uniform("model", mat4());
	shader_2d->set_uniform("color", Color(offset, 0, 5.0f/255));

	draw_plane();
}

void DeferredRenderer::render()
{
	environment = ACTIVE_WORLD->get_child_by_type<Environment*>();

	set_viewport();

	FBOMANAGER->bind_default_fbo();

	deferred_buffer->bind();

	activate();

	if (viewport->world)
	{
		Camera* c = viewport->world->get_active_camera();
		c->update_matrices();

		use_depth_test(c->get_near(), c->get_far());
		use_wireframe(viewport->get_wireframe_enabled());

		activate_world_transform();
		viewport->world->draw();
		viewport->post_draw_world();
		deactivate_world_transform();

		use_wireframe(false);

		stop_depth_test();
	}

	if (viewport->canvas)
	{
		MeshHandler::get_singleton()->get_plane()->bind();
		activate_canvas_transform(); 
		viewport->canvas->draw();
		viewport->post_draw_canvas();
		deactivate_canvas_transform();
	}

	render_virtual_tex();
	render_shadowmap();
	render_reflection();
	render_godray();
	render_first_pass();
	render_flare();
	//render_ssao();
	render_second_pass();
	//save_fbo(final_buffer, "test.tga", 0);

	deactivate();

	if (viewport->mode == Viewport::POSTPROCESS && viewport->postprocess)
		viewport->postprocess->post_process();

	if (viewport->fbo)
		viewport->fbo->unbind();	
}

void DeferredRenderer::save_tex(Ref<Texture2D> p_tex)
{
	save_buffer->bind();
	save_fbo(save_buffer, p_tex->get_file(), 0);
}

// http://www.david-amador.com/2012/09/how-to-take-screenshot-in-opengl/
void DeferredRenderer::save_fbo(FBO2D* p_fbo, const String& p_filename, int attachment)
{
	p_fbo->bind();
	glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	int nSize = p_fbo->size.x * p_fbo->size.y * 3;

	SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, p_fbo->size.x, p_fbo->size.y, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
	char* pixels = new char[3 * p_fbo->size.x * p_fbo->size.y];

	glReadPixels(0, 0, p_fbo->size.x, p_fbo->size.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	
	for (int i = 0; i < p_fbo->size.y; i++)
		std::memcpy(((char*)surface->pixels) + surface->pitch * i, pixels + 3 * p_fbo->size.x * (p_fbo->size.y - i - 1), p_fbo->size.x * 3);
	
	delete[] pixels;

	SDL_SaveBMP(surface, "pic.bmp");

	//FILE* filePtr;
	//fopen_s(&filePtr, p_filename.c_str(), "wb");
	//if (!filePtr)
	//	return;

	//unsigned char TGAheader[12] = { 0,0,2,0,0,0,0,0,0,0,0,0 };
	//unsigned char header[6] = { p_fbo->size.y % 256,p_fbo->size.y / 256,
	//	p_fbo->size.y % 256, p_fbo->size.y / 256,
	//	24,0 };
	//// We write the headers
	//fwrite(TGAheader, sizeof(unsigned char), 12, filePtr);
	//fwrite(header, sizeof(unsigned char), 6, filePtr);
	//// And finally our image data
	//fwrite(dataBuffer, sizeof(GLubyte), nSize, filePtr);
	//fclose(filePtr);

	//free(dataBuffer);
}

#undef CASE
#define CASE(X)\
	case X:\
		return #X

#undef ELSEIF
#define ELSEIF(X)\
	if (p_typename == #X)\
		return X;

String DeferredRenderer::get_texture_typename(int p_type) const
{
	switch (p_type)
	{
		CASE(FINAL_COLOR);
		CASE(RENDER_COLOR);
		CASE(RENDER_DEPTH);
		CASE(DEFERRED_ALBEDO);
		CASE(DEFERRED_POSITION);
		CASE(DEFERRED_NORMAL);
		CASE(DEFERRED_MATERIAL);
		CASE(DEFERRED_DEPTH);
		CASE(REFLECTION);
		CASE(SHADOW);
		CASE(SSAO);
		CASE(SSAO_BLUR);
		CASE(GODRAY);
		CASE(BLOOM);
		CASE(DOF);
		CASE(LIGHTING);
		CASE(VIRTUALTEX);
		CASE(INDIRECTION);
	}
}

int DeferredRenderer::get_texture_type(const String& p_typename) const
{
	if (p_typename == "FINAL_COLOR")
		return FINAL_COLOR;
	ELSEIF(RENDER_COLOR)
	ELSEIF(RENDER_DEPTH)
	ELSEIF(DEFERRED_ALBEDO)
	ELSEIF(DEFERRED_POSITION)
	ELSEIF(DEFERRED_NORMAL)
	ELSEIF(DEFERRED_MATERIAL)
	ELSEIF(DEFERRED_DEPTH)
	ELSEIF(REFLECTION)
	ELSEIF(SHADOW)
	ELSEIF(SSAO)
	ELSEIF(SSAO_BLUR)
	ELSEIF(GODRAY)
	ELSEIF(BLOOM)
	ELSEIF(DOF)
	ELSEIF(LIGHTING)
	ELSEIF(VIRTUALTEX)
	ELSEIF(INDIRECTION)
}

#undef CLASSNAME
#define CLASSNAME DeferredRenderer

void DeferredRenderer::bind_methods()
{

}