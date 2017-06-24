#include "Variant.h"
#include "Sprite.h"
#include "Event.h"
#include "Primitives.h"

#if 0

void Variant::set(const Variant& key, const Variant& val)
{
	string skey = *key.s;

	switch (type.)
	{
		case UNDEF:
		case BOOL:
		case REAL:
			break;

		case STRING:
			if (key.type == REAL)
				s[(int)key.r] = val.s[0];
			break;

		case VEC2:
		case VEC3:
		case VEC4:
			if (key.type == REAL && (int)key.r >= 0 && (int)key.r <= 3) //Index: 0, 1, 2, 3
				v4.v[key] = val.r;
			else if (key.type == STRING)
			{
				if (skey == "x" || skey == "r")
					v4.x = val.r;
				else if (skey == "y" || skey == "g")
					v4.y = val.r;
				else if (skey == "z" || skey == "b")
					v4.z = val.r;
				else if (skey == "w" || skey == "a")
					v4.w = val.r;
			}
			break;

		case OBJECT:
			switch (((Object*)o)->type.basetype)
			{
				case ObjectType::WORLDOBJECT:
					if (skey == "transform")
						dynamic_cast<WorldObject*>(o)->GetTransform().update(val.t->pos, val.t->size, val.t->rotation);
					else if (skey == "visible")
						dynamic_cast<WorldObject*>(o)->SetVisible(val.b);
					else if (skey == "name")
						dynamic_cast<WorldObject*>(o)->name = (string) *val.s;
					else if (skey == "color")
						dynamic_cast<WorldObject*>(o)->SetColor(val.v4);
					break;
			}
			switch (((Object*)o)->type.derivedtype)
			{
				case ObjectType::SPRITE:
					if (skey == "bounds")
						dynamic_cast<Sprite*>(o)->bounds = val.v4;
					else if (skey == "shader")
						dynamic_cast<Sprite*>(o)->shader = (Shader*)val.o;
					else if (skey == "tilesize")
						dynamic_cast<Sprite*>(o)->tilesize = val.v2;
					else if (skey == "tilecount")
						dynamic_cast<Sprite*>(o)->tilecount = val.v2;
					break;

				case ObjectType::BOX:
					if (skey == "shader")
						((Box*)o)->shader = (Shader*)val.o;
					break;

				case ObjectType::CIRCLE:
					if (skey == "shader")
						((Disk*)o)->shader = (Shader*)val.o;
					break;
			}
			break;

		case TRANSFORM:
			if (skey == "pos")
				t->pos = val;
			else if (skey == "size")
				t->size = val;
			else if (skey == "rot")
				t->rotation = val;
			break;
	}
}
Variant Variant::get(const Variant& key)
{
	string skey = *key.s;
	switch (type)
	{
		case UNDEF:
		case BOOL:
		case REAL:
			break;

		case STRING:
			return Variant(s[(int)key]);
			break;

		case VEC2:
		case VEC3:
		case VEC4:
			if (key.type == REAL && (int)key.r >= 0 && (int)key.r <= 3) //Index: 0, 1, 2, 3
				return v4.v[key];
			else if (key.type == STRING)
			{
				if (skey == "x" || skey == "r")
					return Variant(v4.x);
				else if (skey == "y" || skey == "g")
					return Variant(v4.y);
				else if (skey == "z" || skey == "b")
					return Variant(v4.z);
				else if (skey == "w" || skey == "a")
					return Variant(v4.w);
			}
			break;

		case OBJECT:
			switch (((Object*)o)->type.basetype)
			{
				case ObjectType::WORLDOBJECT:
					if (skey == "transform")
						return Variant(&dynamic_cast<WorldObject*>(o)->GetTransform());
					else if (skey == "visible")
						return Variant(dynamic_cast<WorldObject*>(o)->GetVisible());
					else if (skey == "name")
						return Variant((String)dynamic_cast<WorldObject*>(o)->name);
					else if (skey == "color")
						return Variant(dynamic_cast<WorldObject*>(o)->GetColor());
					break;
			}
			switch (((Object*)o)->type.derivedtype)
			{
				case ObjectType::SPRITE:
					if (skey == "bounds")
						return Variant(dynamic_cast<Sprite*>(o)->bounds);
					else if (skey == "shader")
						return Variant(dynamic_cast<Sprite*>(o)->shader);
					else if (skey == "tilesize")
						return Variant(dynamic_cast<Sprite*>(o)->tilesize);
					else if (skey == "tilecount")
						return Variant(dynamic_cast<Sprite*>(o)->tilecount);
					break;

				case ObjectType::INPUTEVENT:
					if (skey == "pos")
						return dynamic_cast<InputEvent*>(o)->pos;
					else if (skey == "index")
						return dynamic_cast<InputEvent*>(o)->index;
					else if (skey == "type")
						return dynamic_cast<InputEvent*>(o)->type;
					else if (skey == "presstype")
						return dynamic_cast<InputEvent*>(o)->press_type;
					else if (skey == "key")
						return &dynamic_cast<InputEvent*>(o)->key;
					break;

				case ObjectType::COLLISIONEVENT:
					if (skey == "contact")
						return dynamic_cast<CollisionEvent*>(o)->contact;
					else if (skey == "object")
						return dynamic_cast<CollisionEvent*>(o)->object;
					break;
			}
			break;

		case TRANSFORM:
			if (skey == "pos")
				return Variant(t->pos);
			else if (skey == "size")
				return Variant(t->size);
			else if (skey == "rot")
				return Variant(t->rotation);
			break;
	}
	return Variant();
}

Array<string> Variant::ListMemberObjectVarsForType(const ObjectType &type)
{
	Array<string> res;

	switch (type.basetype)
	{
		case ObjectType::WORLDOBJECT:
			res.push_back("transform");
			res.push_back("visible");
			res.push_back("name");
			res.push_back("color");
			break;
	}

	switch (type.derivedtype)
	{
		case ObjectType::SPRITE:
			res.push_back("bounds");
			res.push_back("shader");
			res.push_back("tilesize");
			res.push_back("tilecount");
			break;

		case ObjectType::BOX:
		case ObjectType::CIRCLE:
			res.push_back("shader");
			break;

		case ObjectType::INPUTEVENT:
			res.push_back("pos");
			res.push_back("index");
			res.push_back("type");
			res.push_back("presstype");
			res.push_back("key");
			break;

		case ObjectType::COLLISIONEVENT:
			res.push_back("contact");
			res.push_back("object");
			break;
	}
	return res;
}

Array<string> Variant::ListMemberVars()
{
	Array<string> res;
	switch (type)
	{
		default:
			return res;
		case VEC2:
		case VEC3:
		case VEC4:
			res.push_back("x");
			res.push_back("y");
			res.push_back("z");
			res.push_back("w");
			res.push_back("r");
			res.push_back("g");
			res.push_back("b");
			res.push_back("a");
			return res;

		case OBJECT:
			res.push_back(ListMemberObjectVarsForType(o->type));
			return res;
		case TRANSFORM:
			res.push_back("pos");
			res.push_back("size");
			res.push_back("rot");
			return res;
	}
}

bool Variant::HasMember(const string &name)
{
	return ListMemberVars().contains(name);
}

#endif