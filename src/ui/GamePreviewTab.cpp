#include "GamePreviewTab.h"

#include "ImageButton.h"
#include "Seperator.h"
#include "Dock.h"

GamePreviewTab::GamePreviewTab() : GamePreviewTab(NULL)
{

}

GamePreviewTab::GamePreviewTab(World2D* p_world)
{
	ImageButton* run_button = new ImageButton("EngineCore/Run.png");
	run_button->set_tip_description("Run");

	ImageButton* move_button = new ImageButton("EngineCore/Move.png");
	move_button->set_tip_description("Move");

	ImageButton* rotate_button = new ImageButton("EngineCore/Rotate.png");
	rotate_button->set_tip_description("Rotate");

	ImageButton* scale_button = new ImageButton("EngineCore/Scale.png");
	scale_button->set_tip_description("Scale");

	world_view = new WorldView(p_world);

	buttons.add_child(run_button);
	buttons.add_child(new Seperator);
	buttons.add_child(move_button);
	buttons.add_child(rotate_button);
	buttons.add_child(scale_button);

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

void GamePreviewTab::world_changed()
{
	set_tab_title(world_view->get_world()->get_name());
}

WorldView * GamePreviewTab::get_world_view() const
{
	return world_view;
}

void GamePreviewTab::notification(int p_notification)
{
	switch (p_notification)
	{
	case NOTIFICATION_DRAW:

		break;

	case NOTIFICATION_RESIZED:
	case NOTIFICATION_TRANSLATED:

		break;
	}
}