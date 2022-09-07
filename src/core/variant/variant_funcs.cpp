#include "variant.h"

#include "resources/font.h"
#include "game/scene.h"

#include "input/event.h"

#include "world/worldobject.h"
#include "core/object.h"

#if 0

Array<string> Variant::ListFuncsForObject(ObjectType type)
{
	Array<string> result;

	switch (type.basetype)
	{
		case ObjectType::WORLDOBJECT:
			result.push_back("AddComponent");
			result.push_back("IsInBox");
			break;
	}

	switch (type.derivedtype)
	{
		case ObjectType::SPRITE:
			result.push_back("IsInBox");
			result.push_back("FlipHorizontally");
			result.push_back("FlipVertically");
			break;

		case ObjectType::FONT:
			result.push_back("DrawToTexture");
			break;

		case ObjectType::INPUTEVENT:
			result.push_back("ToString");
			break;

		case ObjectType::KEY:
			result.push_back("ToString");
			break;

		case ObjectType::SHADER:
			result.push_back("SetUniform");
			result.push_back("UnBind");
			result.push_back("Bind");
			result.push_back("Start");
			break;

		case ObjectType::SCENE:
			result.push_back("Init");
			result.push_back("Start");
			result.push_back("AddWorldObject");
			break;

		case ObjectType::SCRIPTCOMPONENT:
			result.push_back("LoadScript");
			break;

		case ObjectType::PHYSICSCOMPONENT:
			result.push_back("SetAsBox");
			result.push_back("SetAsCircle");
			break;
	}

	return result;
}

Array<string> Variant::ListFuncs(Variant::BaseType type, ObjectType objtype = ObjectType())
{
	Array<string> result;

	switch (type)
	{
		default:
			return result;

		case STRING:
			result.push_back("");
			return result;

		case VEC2:
		case VEC3:
		case VEC4:
			result.push_back("length");
			result.push_back("normalize");
			result.push_back("dot");
			result.push_back("angle");
			result.push_back("tostring");
			return result;

		case MAT4:
			result.push_back("");
			return result;

		case TRANSFORM:
			result.push_back("");
			return result;

		case OBJECT:
			result.push_back(ListFuncsForObject(objtype));
			return result;
	}
	return result;
}

Array<string> Variant::ListMemberFuncNamesForType(const string &name)
{
	BaseType t = GetType(name);

	if (t == OBJECT)
	{
		DerivedType ot = ObjectType::GetType(name);
		return ListFuncs(t, ot);
	}

	return ListFuncs(t);
}

Array<string> Variant::ListMemberFuncNames()
{
	return ListFuncs(type, o->type);
}

bool Variant::FuncExists(const string &name)
{
	return ListMemberFuncNames().contains(name);
}

Variant Variant::RunFunc(const string &name, Array<Variant> paras)
{
	if (!FuncExists(name))
	{
		cout << FuncExists(name);
		T_ERROR("Member func: " + name + " does not exist for type: " + GetTypeName());
		return Variant();
	}

	switch (type)
	{
		case VEC2:
			if (name == "length")
				return v2.length();
			else if (name == "normalize")
				v2.normalize();
			else if (name == "dot")
				return v2.dot(paras[0]);
			else if (name == "angle")
				return v2.angle(paras[0]);
			else if (name == "ToString")
				return v2.ToString();
			break;
		case VEC3:
			if (name == "length")
				return v3.length();
			else if (name == "normalize")
				v3.normalize();
			else if (name == "dot")
				return v3.dot(paras[0]);
			else if (name == "angle")
				return v3.angle(paras[0]);
			else if (name == "ToString")
				return v3.ToString();
			break;
		case VEC4:
			if (name == "length")
				return v4.length();
			else if (name == "normalize")
				v4.normalize();
			else if (name == "dot")
				return v4.dot(paras[0]);
			else if (name == "angle")
				return v4.angle(paras[0]);
			else if (name == "ToString")
				return v4.ToString();
			break;

		//Start the real fun
		case OBJECT:
			switch (o->type.basetype)
			{
				case ObjectType::WORLDOBJECT:
					if (name == "IsInBox")
						return dynamic_cast<WorldObject*>(o)->CheckOverlap(paras[0]);
					else if (name == "AddComponent")
						dynamic_cast<WorldObject*>(o)->AddComponent((Component*)paras[0].o);
			}
			switch (o->type.derivedtype)
			{
				case ObjectType::SCENE:
					if (name == "Start")
						dynamic_cast<Scene*>(o)->Start();
					if (name == "Init")
						dynamic_cast<Scene*>(o)->Init();
					else if (name == "LoadContent")
						dynamic_cast<Scene*>(o)->LoadContent();
					else if (name == "AddWorldObject")
						dynamic_cast<Scene*>(o)->view.world->AddWorldObject(dynamic_cast<WorldObject*>(paras[0].o));

					break;

				case ObjectType::SPRITE:
					if (name == "FlipHorizontally")
						dynamic_cast<Sprite*>(o)->FlipHorizontally();
					if (name == "FlipVertically")
						dynamic_cast<Sprite*>(o)->FlipVertically();
					break;

				case ObjectType::FONT:
					if (name == "DrawToTexture")
						return dynamic_cast<Font*>(o)->DrawToTex(paras[0]);
					else if (name == "GetWidth")
						return dynamic_cast<Font*>(o)->get_width(paras[0]);
					break;

				case ObjectType::SHADER:
					if (name == "SetUniform")
					{
						switch (paras[1].type)
						{
							case Variant::BOOL:
								dynamic_cast<Shader*>(o)->setUniform(paras[0], (bool)paras[1]);
								break;

							case Variant::REAL:
								if (paras[1].r.t == Real::INT)
									dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].r.i);
								else
									dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].r.d);
								break;

							case Variant::VEC2:
								dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].v2);
								break;

							case Variant::VEC3:
								dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].v3);
								break;

							case Variant::VEC4:
								dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].v4);
								break;

							case Variant::MAT4:
								dynamic_cast<Shader*>(o)->setUniform(paras[0], paras[1].m4);
								break;
						}
					}
					else if (name == "Bind" || name == "Start")
						dynamic_cast<Shader*>(o)->Bind();
					else if (name == "UnBind")
						dynamic_cast<Shader*>(o)->UnBind();
					break;

				case ObjectType::INPUTEVENT:
					if (name == "ToString")
						return ((Event*)o)->to_string();
					break;

				case ObjectType::KEY:
					if (name == "ToString")
						return ((Key*)o)->ToString();
					break;

				case ObjectType::SCRIPTCOMPONENT:
					if (name == "LoadScript")
						((ScriptComponent*)o)->LoadScript();
					break;

				case ObjectType::PHYSICSCOMPONENT:
					if (name == "SetAsBox")
						((PhysicsComponent*)o)->SetAsBox();
					else if (name == "SetAsCircle")
						dynamic_cast<PhysicsComponent*>(o)->SetAsCircle();

					break;
			}
			break;
	}
	return Variant();
}

#endif