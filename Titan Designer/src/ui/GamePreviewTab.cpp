#include "GamePreviewTab.h"

#include "ImageButton.h"
#include "Seperator.h"
#include "Dock.h"
#include "ComboBox.h"
#include "Toggle.h"

#include "graphics/Renderer.h"

GamePreviewTab::GamePreviewTab() : GamePreviewTab(NULL)
{

}

GamePreviewTab::GamePreviewTab(World* p_world)
{
	ImageButton* run_button = new ImageButton("EngineCore/UI/Run.png");
	run_button->set_tip_description("Run");

	ImageButton* move_button = new ImageButton("EngineCore/UI/Move.png");
	move_button->set_tip_description("Move");

	ImageButton* rotate_button = new ImageButton("EngineCore/UI/Rotate.png");
	rotate_button->set_tip_description("Rotate");

	ImageButton* scale_button = new ImageButton("EngineCore/UI/Scale.png");
	scale_button->set_tip_description("Scale");
	
	ToggleStrip* strip = new ToggleStrip;
	strip->add_child("Move", CONTENT->LoadTexture("EngineCore/UI/Move.png"));
	strip->add_child("Rotate", CONTENT->LoadTexture("EngineCore/UI/Rotate.png"));
	strip->add_child("Scale", CONTENT->LoadTexture("EngineCore/UI/Scale.png"));
	strip->connect("toggled", this, "transform_toggled");

	combo_box = new ComboBox;
	combo_box->connect("selected", this, "set_preview_type");
	
	world_view = new WorldView(p_world);

	buttons.add_child(run_button);
	buttons.add_child(new Seperator);
	buttons.add_child(strip);
	buttons.add_child(combo_box);

	float s = buttons.get_child(0)->cast_to_type<Control*>()->get_required_size().y;

	world_view->set_anchors(ANCHOR_BEGIN, ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END);
	world_view->set_margins(4, 4, 4, 12 + s);

	buttons.set_glue_vert(false);
	buttons.set_anchors(ANCHOR_BEGIN, ANCHOR_END, ANCHOR_END, ANCHOR_END);
	buttons.set_margins(4, 4 + s, 4, 4);

	add_child(&buttons);
	add_child(world_view);

	world_view->connect_signal("world_changed", this, "world_changed");

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
	set_tab_title(world_view->get_world()->get_name());
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