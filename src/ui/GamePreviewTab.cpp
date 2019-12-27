#include "GamePreviewTab.h"

#include "ImageButton.h"
#include "TextButton.h"
#include "Seperator.h"
#include "Dock.h"
#include "ComboBox.h"
#include "Toggle.h"

#include "graphics/Renderer.h"

GamePreviewTab::GamePreviewTab() : GamePreviewTab(NULL)
{

}

GamePreviewTab::GamePreviewTab(Scene* p_scene)
{
	auto toggle_run = [this]() { world_view->set_simulating(!world_view->get_simulating()); };

	Toggle* run_button = new Toggle(CONTENT->LoadFontAwesomeIcon("solid/play", vec2i(26)));
	run_button->set_tip_description("Run");
	run_button->connect("toggled", Connection::create_from_lambda(new V_Method_0(toggle_run)));
	
	ToggleStrip* strip = new ToggleStrip;
	strip->add_child("Move", CONTENT->LoadTexture("engine/ui/Move.png"));
	strip->add_child("Rotate", CONTENT->LoadTexture("engine/ui/Rotate.png"));
	strip->add_child("Scale", CONTENT->LoadTexture("engine/ui/Scale.png"));
	strip->connect("toggled", this, "transform_toggled");

	combo_box = new ComboBox;
	combo_box->connect("selected", this, "set_preview_type");

	TextButton* toggle_2d = new TextButton("3D");
	toggle_2d->connect("clicked",
		Connection::create_from_lambda(new V_Method_0([this, toggle_2d]() {
			bool v = world_view->get_handle_2d();
			world_view->set_handle_2d(!v);
			
			if (v)
				toggle_2d->set_text("3D");
			else
				toggle_2d->set_text("2D");
			})
		)
	);

	TextButton* toggle_wireframe = new TextButton("F");
	toggle_wireframe->connect("clicked",
		Connection::create_from_lambda(new V_Method_0([this, toggle_wireframe]() {
			bool v = world_view->get_viewport()->get_wireframe_enabled();
			world_view->get_viewport()->set_wireframe_enabled(!v);

			if (v)
				toggle_wireframe->set_text("F");
			else
				toggle_wireframe->set_text("W");
			})
		)
	);

	ImageButton* toggle_display_mode = new IconButton("solid/globe-europe");
	toggle_display_mode->connect("clicked",
		Connection::create_from_lambda(new V_Method_0([this, toggle_display_mode]() {
			int v = world_view->get_display_mode();

			if (v == WorldView::DISPLAY_CANVAS)
				world_view->set_display_mode(WorldView::DISPLAY_WORLD);
			else
				world_view->set_display_mode(WorldView::DISPLAY_CANVAS);

			if (v)
				toggle_display_mode->set_icon("solid/globe-europe");
			else
				toggle_display_mode->set_icon("solid/tv");
			})
		)
	);
	
	world_view = new WorldView(p_scene);

	buttons.add_child(run_button);
	buttons.add_child(new Seperator);
	buttons.add_child(strip);
	buttons.add_child(combo_box);
	buttons.add_child(toggle_2d);
	buttons.add_child(toggle_wireframe);
	buttons.add_child(toggle_display_mode);

	float s = buttons.get_required_size().y;

	world_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	world_view->set_margins(4, 4, 4, 12 + s);

	buttons.set_glue_vert(false);
	buttons.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons.set_margins(4, 4 + s, 4, 4);

	add_child(&buttons);
	add_child(world_view);

	world_view->connect_signal("world_changed", this, "world_changed");
	world_view->set_display_mode(WorldView::DISPLAY_WORLD);

	set_tab_title("Preview");

}

GamePreviewTab::~GamePreviewTab()
{
}

vec2 GamePreviewTab::get_required_size() const
{
	return vec2(200);
}

void GamePreviewTab::update_textures()
{
	combo_box->clear();

	Array<int> a;
	a.push_back(DeferredRenderer::DEFERRED_ALBEDO);
	a.push_back(DeferredRenderer::DEFERRED_POSITION);
	a.push_back(DeferredRenderer::DEFERRED_NORMAL);
	a.push_back(DeferredRenderer::DEFERRED_MATERIAL);
	a.push_back(DeferredRenderer::DEFERRED_DEPTH);
	a.push_back(DeferredRenderer::SHADOW);
	a.push_back(DeferredRenderer::FINAL_COLOR);
	a.push_back(DeferredRenderer::RENDER_COLOR);
	a.push_back(DeferredRenderer::RENDER_DEPTH);
	a.push_back(DeferredRenderer::REFLECTION);
	a.push_back(DeferredRenderer::SHADOW);
	a.push_back(DeferredRenderer::SSAO);
	a.push_back(DeferredRenderer::SSAO_BLUR);
	a.push_back(DeferredRenderer::GODRAY);
	a.push_back(DeferredRenderer::BLOOM);
	//a.push_back(DeferredRenderer::DOF);
	//a.push_back(DeferredRenderer::LIGHTING);
	a.push_back(DeferredRenderer::VIRTUALTEX);
	a.push_back(DeferredRenderer::INDIRECTION);
	a.push_back(DeferredRenderer::BLUR);


	for (int c = 0; c < a.size(); c++)
	{
		Texture2D* tex = DEFERRED_RENDERER->get_texture(a[c]);

		if (tex)
			combo_box->add_item(DEFERRED_RENDERER->get_texture_typename(a[c]));
	}

	combo_box->set_text(DEFERRED_RENDERER->get_texture_typename(DeferredRenderer::FINAL_COLOR));
}

void GamePreviewTab::transform_toggled(int p_index)
{
	if (p_index == 0)
		world_view->set_transform_type(WorldView::TRANSLATE);
	else if (p_index == 1)
		world_view->set_transform_type(WorldView::ROTATE);
	else if (p_index == 2)
		world_view->set_transform_type(WorldView::SCALE);

}

void GamePreviewTab::set_preview_type(const String& p_type)
{
	world_view->set_preview_type(DEFERRED_RENDERER->get_texture_type(p_type));
}

void GamePreviewTab::world_changed()
{
	set_tab_title(world_view->get_scene()->get_name());
}

WorldView* GamePreviewTab::get_world_view() const
{
	return world_view;
}

void GamePreviewTab::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_READY:

		update_textures();
		break;

	case NOTIFICATION_DRAW:

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		break;
	}
}

#undef CLASSNAME
#define CLASSNAME GamePreviewTab

void GamePreviewTab::bind_methods()
{
	REG_METHOD(set_preview_type);
	REG_METHOD(transform_toggled);
}