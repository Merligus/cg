#include "MeshSweeper.h"
#include "P5.h"
#include <stack>

MeshMap P5::_defaultMeshes;

inline Ray
P5::makeRay(int x, int y) const
{
	auto p = _renderer->unproject(vec3f{ float(x), float(height() - y), 0 });
	auto c = _renderer->camera();
	Ray r{ c->position(), c->directionOfProjection() };

	if (c->projectionType() == Camera::Perspective)
		r.direction = (p - r.origin).versor();
	else
		r.origin = p - r.direction * c->nearPlane();
	return r;
}

inline void
P5::buildDefaultMeshes()
{
	_defaultMeshes["None"] = nullptr;
	_defaultMeshes["Box"] = MeshSweeper::makeBox();
	_defaultMeshes["Cone"] = GLRenderer::cone();
}

inline Primitive*
makePrimitive(MeshMapIterator mit)
{
	return new Primitive(mit->second, mit->first);
}

inline Scene*
P5::buildScene()
{
	Scene* scene;
	_current = scene = new cg::Scene{ "Scene 1" };

	cg::Scene *currentScene = scene;
	cg::SceneObject *currentBox;
	std::list<cg::SceneObject>::iterator it;
	std::list<Reference<Component>>::iterator p;

	auto& meshes = Assets::meshes();

	cg::SceneObject obj("Empty Object", *currentScene);
	it = currentScene->append(obj);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
	(*p)->setMyIterator(p);

	currentBox = &(*it);
	cg::SceneObject box("Box", *currentScene);
	cg::SceneObject jato("Jato", *currentScene);
	cg::SceneObject coelho("Coelho", *currentScene);
	box.setParent(currentBox);
	jato.setParent(currentBox);
	coelho.setParent(currentBox);

	it = currentBox->appendChildren(box);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.0f, -7.0f, 0.0f));
	p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
	(*p)->setMyIterator(p);

	it = currentBox->appendChildren(jato);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	it->transform()->setLocalScale(0.5f);
	it->transform()->setLocalPosition(vec3f(0.0f, -5.0f, 0.0f));
	p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
	for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
		if (mit->first.compare("f-16.obj") == 0)
			it->primitive()->setMesh(Assets::loadMesh(mit), mit->first);
	(*p)->setMyIterator(p);

	it = currentBox->appendChildren(coelho);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
	for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
		if (mit->first.compare("bunny.obj") == 0)
			it->primitive()->setMesh(Assets::loadMesh(mit), mit->first);
	(*p)->setMyIterator(p);

	cg::SceneObject dirLight("Luz Direcional Branca", *currentScene);
	it = currentScene->append(dirLight);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->light()->setColor((Color)vec4f((float)165 / 255, (float)165 / 255, (float)165 / 255, 0));
	it->transform()->rotate(vec3f(0, 0, 90.0f));
	it->light()->setType(Light::Type::Directional);

	cg::SceneObject pointLight("Luz Pontual Amarela", *currentScene);
	it = currentScene->append(pointLight);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.9f, -1.0f, 0.0f));
	it->light()->setColor((Color)vec4f((float)170 / 255, (float)181 / 255, 0, 0));

	cg::SceneObject redSpot("Luz Spot Vermelha no Jato", *currentScene);
	it = currentScene->append(redSpot);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.0f, -0.4f, 0.0f));
	it->light()->setColor((Color)vec4f(1, 0, 0, 1));
	it->transform()->rotate(vec3f(10.0f, 0, -9.0f));
	it->light()->setType(Light::Type::Spot);

	cg::SceneObject blueSpot("Luz Spot Azul no Coelho", *currentScene);
	it = currentScene->append(blueSpot);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.0f, 2.7f, 0.0f));
	it->light()->setColor((Color)vec4f(0, (float)89 / 255, (float)230 / 255, 0));
	it->transform()->rotate(vec3f(16.8f, 3.7f, 6.1f));
	it->light()->setInnerCutOff(16.1);
	it->light()->setOuterCutOff(24.5);
	it->light()->setType(Light::Type::Spot);

	return scene;
}

void
P5::initialize()
{
	Application::loadShaders(_program[0], "shaders/p5.vs", "shaders/p5.fs");
	Application::loadShaders(_program[1], "shaders/p5-smooth.vs", "shaders/p5-smooth.fs");
	Application::loadShaders(_program[2], "shaders/p5-none.vs", "shaders/p5-none.fs");
	Application::loadShaders(_program[3], "shaders/p5-flat.vs", "shaders/p5-flat.fs");
	Assets::initialize();
	buildDefaultMeshes();
	_renderer = new GLRenderer{ *buildScene() };
	_renderer->setImageSize(width(), height());
	_rayTracer = new RayTracer{ *_renderer->scene(), _renderer->camera() };
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	_program[2].use();
}

inline void
P5::hierarchyWindow()
{
	ImGui::Begin("Hierarchy");
	if (ImGui::Button("Options###object"))
		ImGui::OpenPopup("OptionsObjectPopup");
	if (ImGui::BeginPopup("OptionsObjectPopup"))
	{
		if (ImGui::MenuItem("New Empty Object"))
		{
			cg::Scene *currentScene = _renderer->scene();

			std::string objectIndex("Object ");
			static int i = 1;
			objectIndex += std::to_string(i++);

			if (_current == nullptr || _current == _renderer->scene())
			{
				cg::SceneObject newBox(objectIndex.c_str(), *currentScene);
				std::list<cg::SceneObject>::iterator it = currentScene->append(newBox);
				it->setMyIterator(it); // criando nova raíz
				std::list<Reference<Component>>::iterator p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
				(*p)->setMyIterator(p);
			}
			else
			{
				cg::SceneObject *currentBox = (cg::SceneObject*)(_current);
				cg::SceneObject newBox(objectIndex.c_str(), *currentScene);
				newBox.setParent(currentBox);
				std::list<cg::SceneObject>::iterator it = currentBox->appendChildren(newBox);
				it->setMyIterator(it); // criando child
				std::list<Reference<Component>>::iterator p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
				(*p)->setMyIterator(p);
			}
		}
		if (ImGui::BeginMenu("New 3D Object"))
		{
			if (ImGui::MenuItem("Box"))
			{
				cg::Scene *currentScene = _renderer->scene();
				std::list<Reference<Component>>::iterator p;

				std::string boxIndex("Box ");
				static int index = 1;
				boxIndex += std::to_string(index++);

				if (_current == nullptr || _current == _renderer->scene())
				{
					cg::SceneObject newBox(boxIndex.c_str(), *currentScene);
					std::list<cg::SceneObject>::iterator it = currentScene->append(newBox);
					it->setMyIterator(it); // criando nova raíz
					p = it->addComponent(new Transform());
					(*p)->setMyIterator(p);
					p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
					(*p)->setMyIterator(p);
				}
				else
				{
					cg::SceneObject *currentBox = (cg::SceneObject*)(_current);
					cg::SceneObject newBox(boxIndex.c_str(), *currentScene);
					newBox.setParent(currentBox);
					std::list<cg::SceneObject>::iterator it = currentBox->appendChildren(newBox);
					it->setMyIterator(it); // criando child
					p = it->addComponent(new Transform());
					(*p)->setMyIterator(p);
					p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
					(*p)->setMyIterator(p);
				}
			}
			if (ImGui::MenuItem("Capsule"))
			{
				// TODO: create a new capsule.
			}
			if (ImGui::MenuItem("Cylinder"))
			{
				// TODO: create a new cylinder.
			}
			if (ImGui::MenuItem("Sphere"))
			{
				// TODO: create a new sphere.
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("New Light Object"))
		{
			cg::Scene *currentScene = _renderer->scene();

			std::string lightIndex("Light ");
			static int i = 1;
			lightIndex += std::to_string(i++);

			if (_current == nullptr || _current == _renderer->scene())
			{
				cg::SceneObject newBox(lightIndex.c_str(), *currentScene);
				std::list<cg::SceneObject>::iterator it = currentScene->append(newBox);
				it->setMyIterator(it); // criando nova raíz

				std::list<Reference<Component>>::iterator p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);

				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
			}
			else
			{
				cg::SceneObject *currentBox = (cg::SceneObject*)(_current);
				cg::SceneObject newBox(lightIndex.c_str(), *currentScene);
				newBox.setParent(currentBox);
				std::list<cg::SceneObject>::iterator it = currentBox->appendChildren(newBox);
				it->setMyIterator(it); // criando child

				std::list<Reference<Component>>::iterator p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);

				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
			}
		}
		if (ImGui::MenuItem("Delete", "Ctrl+Del"))
		{
			if (_current != nullptr && _current != _renderer->scene())
			{
				std::cout << "Apaga objeto e hierarquia do objeto selecionado\n";
				cg::SceneObject *currentObject = (cg::SceneObject*)_current;
				_current = currentObject->autoDelete();
			}
		}
		ImGui::EndPopup();
	}
	ImGui::Separator();

	ImGuiTreeNodeFlags flag{ ImGuiTreeNodeFlags_OpenOnArrow };
	const auto scene = _renderer->scene();
	auto open = ImGui::TreeNodeEx(scene,
		_current == scene ? flag | ImGuiTreeNodeFlags_Selected : flag,
		scene->name());

	if (ImGui::IsItemClicked())
		_current = scene;
	if (ImGui::BeginDragDropTarget())
	{
		if (auto* payload = ImGui::AcceptDragDropPayload("Objeto"))
		{
			auto mit = *(std::list<SceneObject>::iterator*)payload->Data;
			mit->setParent(nullptr);
			auto it2 = --scene->containerEnd();
			mit->autoDelete();
			_current = &(*it2);
		}
		ImGui::EndDragDropTarget();
	}
	if (open)
	{
		for (std::list<cg::SceneObject>::iterator it = scene->containerBegin(); it != scene->containerEnd(); ++it)
			_current = it->show(flag, _current);

		ImGui::TreePop(); // fecha a cena
	}
	ImGui::End();
}

namespace ImGui
{ // begin namespace ImGui

	void
		ObjectNameInput(NameableObject* object)
	{
		const int bufferSize{ 128 };
		static NameableObject* current;
		static char buffer[bufferSize];

		if (object != current)
		{
			strcpy_s(buffer, bufferSize, object->name());
			current = object;
		}
		if (ImGui::InputText("Name", buffer, bufferSize))
			object->setName(buffer);
	}

	inline bool
		ColorEdit3(const char* label, Color& color)
	{
		return ImGui::ColorEdit3(label, (float*)&color);
	}

	inline bool
		DragVec3(const char* label, vec3f& v)
	{
		return DragFloat3(label, (float*)&v, 0.1f, 0.0f, 0.0f, "%.2f");
	}

	void
		TransformEdit(Transform* transform)
	{
		vec3f temp;

		temp = transform->localPosition();
		if (ImGui::DragVec3("Position", temp))
			transform->setLocalPosition(temp);
		temp = transform->localEulerAngles();
		if (ImGui::DragVec3("Rotation", temp))
			transform->setLocalEulerAngles(temp);
		temp = transform->localScale();
		if (ImGui::DragVec3("Scale", temp))
			transform->setLocalScale(temp);
	}

} // end namespace ImGui

inline void
P5::sceneGui()
{
	auto scene = (Scene*)_current;

	ImGui::ObjectNameInput(_current);
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Colors"))
	{
		ImGui::ColorEdit3("Background", scene->backgroundColor);
		ImGui::ColorEdit3("Ambient Light", scene->ambientLight);
	}
}

inline void
P5::inspectShape(Primitive& primitive)
{
	char buffer[16];

	snprintf(buffer, 16, "%s", primitive.meshName());
	ImGui::InputText("Mesh", buffer, 16, ImGuiInputTextFlags_ReadOnly);
	if (ImGui::BeginDragDropTarget())
	{
		if (auto* payload = ImGui::AcceptDragDropPayload("PrimitiveMesh"))
		{
			auto mit = *(MeshMapIterator*)payload->Data;
			primitive.setMesh(mit->second, mit->first);
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::SameLine();
	if (ImGui::Button("...###PrimitiveMesh"))
		ImGui::OpenPopup("PrimitiveMeshPopup");
	if (ImGui::BeginPopup("PrimitiveMeshPopup"))
	{
		auto& meshes = Assets::meshes();

		if (!meshes.empty())
		{
			for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
				if (ImGui::Selectable(mit->first.c_str()))
					primitive.setMesh(Assets::loadMesh(mit), mit->first);
			ImGui::Separator();
		}
		for (auto mit = _defaultMeshes.begin(); mit != _defaultMeshes.end(); ++mit)
			if (ImGui::Selectable(mit->first.c_str()))
				primitive.setMesh(mit->second, mit->first);
		ImGui::EndPopup();
	}
}

inline void
P5::inspectMaterial(Material& material)
{
	ImGui::ColorEdit3("Ambient", material.ambient);
	ImGui::ColorEdit3("Diffuse", material.diffuse);
	ImGui::ColorEdit3("Spot", material.spot);
	ImGui::DragFloat("Shine", &material.shine, 1, 1, 1000.0f);
	ImGui::ColorEdit3("Specular", material.specular);
}

inline void
P5::inspectPrimitive(Primitive& primitive)
{
	//const auto flag = ImGuiTreeNodeFlags_NoTreePushOnOpen;

	//if (ImGui::TreeNodeEx("Shape", flag))
	inspectShape(primitive);
	//if (ImGui::TreeNodeEx("Material", flag))
	inspectMaterial(primitive.material);
}

inline void
P5::inspectLight(Light& light)
{
	static const char* lightTypes[]{ "Directional", "Point", "Spot" };
	auto lt = light.type();

	if (ImGui::BeginCombo("Type", lightTypes[lt]))
	{
		for (auto i = 0; i < IM_ARRAYSIZE(lightTypes); ++i)
		{
			bool selected = lt == i;

			if (ImGui::Selectable(lightTypes[i], selected))
				lt = (Light::Type)i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	light.setType(lt);
	Color temp;
	vec3f temp2;
	int falloff;
	float innerCutOff, outerCutOff;

	temp = light.color();
	if (ImGui::ColorEdit3("Light Color", temp))
		light.setColor(temp);
	if (lt == Light::Type::Point)
	{
		falloff = light.falloff();
		if (ImGui::SliderInt("Falloff", &falloff, 0, 2))
			light.setFalloff(falloff);
	}
	if (lt == Light::Type::Spot)
	{
		falloff = light.falloff();
		if (ImGui::SliderInt("Falloff", &falloff, 0, 2))
			light.setFalloff(falloff);
		innerCutOff = light.innerCutOff();
		outerCutOff = light.outerCutOff();
		if (ImGui::DragFloat("Inner Cutoff", &innerCutOff, 0.5, 0.1, 89.0))
			light.setInnerCutOff(innerCutOff > outerCutOff ? outerCutOff - 0.1 : innerCutOff);
		if (ImGui::DragFloat("Outer Cutoff", &outerCutOff, 0.5, 0.1, 89.0))
			light.setOuterCutOff(outerCutOff < innerCutOff ? innerCutOff + 0.1 : outerCutOff);
	}
}

inline void
P5::addComponentButton(SceneObject& object)
{
	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponentPopup");
	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		if (ImGui::MenuItem("Primitive"))
		{
			std::list<Reference<Component>>::iterator p;
			cg::SceneObject *currentBox = (cg::SceneObject*)(_current);

			p = currentBox->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
			(*p)->setMyIterator(p);
		}
		if (ImGui::MenuItem("Light"))
		{
			std::list<Reference<Component>>::iterator l;
			cg::SceneObject *currentBox = (cg::SceneObject*)(_current);

			l = currentBox->addComponent(new Light());
			(*l)->setMyIterator(l);
		}
		ImGui::EndPopup();
	}
}

inline void
P5::sceneObjectGui()
{
	auto object = (SceneObject*)_current;
	auto transform = object->transform();
	auto primitive = object->primitive();
	auto light = object->light();

	addComponentButton(*object);
	ImGui::Separator();
	ImGui::ObjectNameInput(object);
	ImGui::SameLine();
	ImGui::Checkbox("###visible", &object->visible);
	ImGui::Separator();
	if (transform != nullptr)
		if (ImGui::CollapsingHeader(transform->typeName()))
			ImGui::TransformEdit(transform);
	if (primitive != nullptr)
		if (ImGui::CollapsingHeader(primitive->typeName()))
			inspectPrimitive(*primitive);
	if (light != nullptr)
		if (ImGui::CollapsingHeader(light->typeName()))
			inspectLight(*light);
}

inline void
P5::objectGui()
{
	if (_current == nullptr)
		return;
	if (dynamic_cast<SceneObject*>(_current))
	{
		sceneObjectGui();
		return;
	}
	if (dynamic_cast<Scene*>(_current))
		sceneGui();
}

inline void
P5::inspectorWindow()
{
	ImGui::Begin("Inspector");
	objectGui();
	ImGui::End();
}

inline void
P5::cameraGui()
{
	auto camera = _renderer->camera();

	if (ImGui::Button("Set Default View"))
		camera->setDefaultView(float(width()) / float(height()));
	ImGui::Separator();

	vec3f temp;

	ImGui::ObjectNameInput(camera);
	temp = camera->position();
	if (ImGui::DragVec3("Position", temp))
		camera->setPosition(temp);
	temp = camera->eulerAngles();
	if (ImGui::DragVec3("Rotation", temp))
		camera->setEulerAngles(temp);

	auto d = camera->distance();

	if (ImGui::DragFloat("Distance",
		&d,
		MIN_DISTANCE * 10.0f,
		MIN_DISTANCE,
		math::Limits<float>::inf()))
		camera->setDistance(d <= MIN_DISTANCE ? MIN_DISTANCE : d);

	static const char* projectionNames[]{ "Perspective", "Orthographic" };
	auto cp = camera->projectionType();

	if (ImGui::BeginCombo("Projection", projectionNames[cp]))
	{
		for (auto i = 0; i < IM_ARRAYSIZE(projectionNames); ++i)
		{
			bool selected = cp == i;

			if (ImGui::Selectable(projectionNames[i], selected))
				cp = (Camera::ProjectionType)i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	camera->setProjectionType(cp);
	if (cp == Camera::Perspective)
	{
		auto fov = camera->viewAngle();

		if (ImGui::SliderFloat("View Angle",
			&fov,
			MIN_ANGLE,
			MAX_ANGLE,
			"%.0f deg",
			1.0f))
			camera->setViewAngle(fov <= MIN_ANGLE ? MIN_ANGLE : (fov >= MAX_ANGLE ? MAX_ANGLE : fov));
	}
	else
	{
		auto h = camera->height();

		if (ImGui::DragFloat("Height",
			&h,
			MIN_HEIGHT * 10.0f,
			MIN_HEIGHT,
			math::Limits<float>::inf()))
			camera->setHeight(h <= MIN_HEIGHT ? MIN_HEIGHT : h);
	}

	float n;
	float f;

	camera->clippingPlanes(n, f);

	if (ImGui::DragFloatRange2("Clipping Planes",
		&n,
		&f,
		MIN_DEPTH,
		MIN_DEPTH,
		math::Limits<float>::inf(),
		"Near: %.2f",
		"Far: %.2f"))
	{
		if (n <= 0)
			n = MIN_DEPTH;
		if (f - n < MIN_DEPTH)
			f = n + MIN_DEPTH;
		camera->setClippingPlanes(n, f);
	}
}

inline void
P5::assetsWindow()
{
	ImGui::Begin("Assets");
	if (ImGui::CollapsingHeader("Meshes"))
	{
		auto& meshes = Assets::meshes();

		for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
		{
			auto meshName = mit->first.c_str();
			auto selected = false;

			ImGui::Selectable(meshName, &selected);
			if (ImGui::BeginDragDropSource())
			{
				Assets::loadMesh(mit);
				ImGui::Text(meshName);
				ImGui::SetDragDropPayload("PrimitiveMesh", &mit, sizeof(mit));
				ImGui::EndDragDropSource();
			}
		}
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Textures"))
	{
		// p4
	}
	ImGui::End();
}

inline void
P5::renderModeGui()
{
	static int sm;

	ImGui::Combo("Shading Mode", &sm, "None\0Flat\0Gouraud\0Phong\0\0");
	switch (sm)
	{
	case 0:
		if (_indexProgramaAtual != 2)
			_program[2].use();
		_indexProgramaAtual = 2;
		break;
	case 1:
		if (_indexProgramaAtual != 3)
			_program[3].use();
		_indexProgramaAtual = 3;
		break;
	case 2:
		if (_indexProgramaAtual != 0)
			_program[0].use();
		_indexProgramaAtual = 0;
		break;
	case 3:
		if (_indexProgramaAtual != 1)
			_program[1].use();
		_indexProgramaAtual = 1;
		break;
	default:
		break;
	}

	static bool showEdges;

	ImGui::ColorEdit3("Edges", _edgeColor);
	ImGui::SameLine();
	ImGui::Checkbox("###showEdges", &showEdges);
}

inline void
P5::rendererWindow()
{
	int rt = _traceFlag;

	ImGui::Begin("Renderer");
	ImGui::Combo("Type", &rt, "OpenGL\0Ray Tracer\0\0");
	if (!rt)
		renderModeGui();
	else
		_traceFlag = true;
	if (ImGui::Button("Atualizar"))
		_image = nullptr;
	if (ImGui::Button("BVH Mode On"))
		_renderer->scene()->bvhMode = true;
	ImGui::SameLine();
	if (ImGui::Button("BVH Mode Off"))
		_renderer->scene()->bvhMode = false;
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Camera"))
		cameraGui();
	ImGui::End();
}

inline void
P5::fileMenu()
{
	if (ImGui::MenuItem("New"))
	{
		// TODO
	}
	if (ImGui::MenuItem("Open...", "Ctrl+O"))
	{
		// TODO
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Save", "Ctrl+S"))
	{
		// TODO
	}
	if (ImGui::MenuItem("Save As..."))
	{
		// TODO
	}
	ImGui::Separator();
	if (ImGui::MenuItem("Exit", "Alt+F4"))
	{
		shutdown();
	}
}

inline bool
showStyleSelector(const char* label)
{
	static int style = 1;

	if (!ImGui::Combo(label, &style, "Classic\0Dark\0Light\0"))
		return false;
	switch (style)
	{
	case 0: ImGui::StyleColorsClassic();
		break;
	case 1: ImGui::StyleColorsDark();
		break;
	case 2: ImGui::StyleColorsLight();
		break;
	}
	return true;
}

inline void
P5::showOptions()
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
	showStyleSelector("Color Theme##Selector");
	ImGui::ColorEdit3("Selected Wireframe", _selectedWireframeColor);
	ImGui::PopItemWidth();
}

inline void
P5::mainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			fileMenu();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Renderer"))
		{
			ImGui::MenuItem("Ray Tracer", nullptr, &_traceFlag);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::BeginMenu("Options"))
			{
				showOptions();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Example"))
		{
			if (ImGui::MenuItem("Example #1"))
			{
				Scene* scene = _renderer->scene();
				cg::Scene *currentScene = scene;
				_renderer->setScene(*currentScene);
				auto camera = _renderer->camera();

				_current = nullptr;
				while (_renderer->scene()->containerSize() > 0)
				{
					auto currentObject = scene->containerBegin();
					_current = currentObject->autoDelete();
				}

				std::list<cg::SceneObject>::iterator it;
				std::list<Reference<Component>>::iterator p;

				cg::SceneObject obj("Box1", *currentScene);
				it = currentScene->append(obj);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.spot.setRGB(Color::white);
				it->primitive()->material.specular.setRGB(Color::white);
				it->primitive()->material.shine = 938.0f;

				cg::SceneObject box2("Box2", *currentScene);
				it = currentScene->append(box2);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, 1.6f));
				it->transform()->setLocalScale(0.2f);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.specular.setRGB(Color::white);
				it->primitive()->material.shine = 25.0f;

				cg::SceneObject cone("Cone", *currentScene);
				it = currentScene->append(cone);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, 7.0f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Cone")));
				(*p)->setMyIterator(p);

				cg::SceneObject pointLight("Luz Pontual Vermelha Invisivel", *currentScene);
				it = currentScene->append(pointLight);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, 2.7f));
				it->transform()->setLocalScale(0.1f);
				it->light()->setColor((Color)vec4f((float)255 / 255, 0, 0, 0));

				camera->setDefaultView(float(width()) / float(height()));
				camera->setPosition(vec3f(1.0f, 0.0f, 2.4f));
				camera->rotateYX(-20.0f, 0.0f);
			}
			if (ImGui::MenuItem("Example #2"))
			{
				Scene* scene = _renderer->scene();
				cg::Scene *currentScene = scene;
				_renderer->setScene(*currentScene);
				auto camera = _renderer->camera();

				_current = nullptr;
				while (_renderer->scene()->containerSize() > 0)
				{
					auto currentObject = scene->containerBegin();
					_current = currentObject->autoDelete();
				}

				std::list<cg::SceneObject>::iterator it;
				std::list<Reference<Component>>::iterator p;

				cg::SceneObject obj("Box1", *currentScene);
				it = currentScene->append(obj);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, -2.0f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.spot.setRGB(Color::white);
				it->primitive()->material.specular.setRGB(Color::white);
				it->primitive()->material.shine = 938.0f;

				cg::SceneObject box2("Box2", *currentScene);
				it = currentScene->append(box2);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, 2.0f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.specular.setRGB(Color::white);
				it->primitive()->material.shine = 25.0f;

				cg::SceneObject blueSpot1("Luz Spot Azul 1", *currentScene);
				it = currentScene->append(blueSpot1);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, 0.3f));
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->light()->setColor((Color)vec4f(0, (float)89 / 255, (float)230 / 255, 0));
				it->transform()->rotate(vec3f(-90.0f, 0.0, 0.0f));
				it->light()->setInnerCutOff(16.1);
				it->light()->setOuterCutOff(24.5);
				it->light()->setType(Light::Type::Spot);

				cg::SceneObject blueSpot2("Luz Spot Azul 2", *currentScene);
				it = currentScene->append(blueSpot2);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 0.0f, -0.3f));
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->light()->setColor((Color)vec4f(0, (float)89 / 255, (float)230 / 255, 0));
				it->transform()->rotate(vec3f(90.0f, 0.0, 0.0f));
				it->light()->setInnerCutOff(16.1);
				it->light()->setOuterCutOff(24.5);
				it->light()->setType(Light::Type::Spot);

				cg::SceneObject cone("Cone", *currentScene);
				it = currentScene->append(cone);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalScale(0.2f);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Cone")));
				(*p)->setMyIterator(p);

				auto currentBox = &(*it);
				cg::SceneObject whiteSpot("Luz Branca no Cone", *currentScene);
				whiteSpot.setParent(currentBox);

				it = currentBox->appendChildren(whiteSpot);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, 5.8f, 0.0f));
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->light()->setColor(Color::white);
				it->light()->setType(Light::Type::Spot);

				scene->backgroundColor = Color::black;
				camera->setDefaultView(float(width()) / float(height()));
				camera->setPosition(vec3f(0.53f, 0.31f, 0.51f));
				camera->rotateYX(-17.26f, 5.8f);
			}
			if (ImGui::MenuItem("Example #3"))
			{
				Scene* scene = _renderer->scene();
				cg::Scene *currentScene = scene;
				_renderer->setScene(*currentScene);
				auto camera = _renderer->camera();

				_current = nullptr;
				while (_renderer->scene()->containerSize() > 0)
				{
					auto currentObject = scene->containerBegin();
					_current = currentObject->autoDelete();
				}

				std::list<cg::SceneObject>::iterator it;
				std::list<Reference<Component>>::iterator p;
				auto& meshes = Assets::meshes();

				cg::SceneObject chao("Chao", *currentScene);
				it = currentScene->append(chao);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(0.0f, -0.3f, 0.0f));
				it->transform()->setLocalScale(vec3f(30.0f, 0.0f, 30.0f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.ambient.setRGB(0, 60, 7);
				it->primitive()->material.diffuse.setRGB(104, 104, 104);
				it->primitive()->material.spot.setRGB(Color::black);
				it->primitive()->material.specular.setRGB(46, 46, 46);

				cg::SceneObject casa("Casa", *currentScene);
				it = currentScene->append(casa);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalScale(0.05f);
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("None")));
				for (auto mit = meshes.begin(); mit != meshes.end(); ++mit)
					if (mit->first.compare("House15.obj") == 0)
						it->primitive()->setMesh(Assets::loadMesh(mit), mit->first);
				(*p)->setMyIterator(p);
				it->primitive()->material.ambient.setRGB(51, 51, 51);
				it->primitive()->material.diffuse.setRGB(204, 204, 204);
				it->primitive()->material.spot.setRGB(59, 59, 59);
				it->primitive()->material.specular.setRGB(0, 0, 0);

				cg::SceneObject cone("Cone", *currentScene);
				it = currentScene->append(cone);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(-13.9f, 0.0f, 0.0f));
				it->transform()->setLocalScale(vec3f(0.5f, 5.0f, 0.5f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Cone")));
				(*p)->setMyIterator(p);
				it->primitive()->material.ambient.setRGB(51, 51, 51);
				it->primitive()->material.diffuse.setRGB(204, 204, 204);
				it->primitive()->material.spot.setRGB(Color::black);
				it->primitive()->material.specular.setRGB(Color::black);

				cg::SceneObject espelho("Espelho", *currentScene);
				it = currentScene->append(espelho);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(-18.3f, 0.0f, 0.0f));
				it->transform()->setLocalScale(vec3f(0.0f, 5.0f, 5.0f));
				p = it->addComponent((cg::Primitive*)makePrimitive(_defaultMeshes.find("Box")));
				(*p)->setMyIterator(p);
				it->primitive()->material.ambient.setRGB(Color::black);
				it->primitive()->material.diffuse.setRGB(Color::black);
				it->primitive()->material.spot.setRGB(Color::black);
				it->primitive()->material.specular.setRGB(Color::white);

				cg::SceneObject luzVermelha("Luz Vermelha", *currentScene);
				it = currentScene->append(luzVermelha);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(-6.8f, 2.7f, -2.3f));
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->light()->setColor((Color)vec4f((float)168 / 255, 0, 0));
				it->light()->setFalloff(0);
				it->light()->setType(Light::Type::Point);

				cg::SceneObject luzAzul("Luz Azul", *currentScene);
				it = currentScene->append(luzAzul);
				it->setMyIterator(it);
				p = it->addComponent(new Transform());
				(*p)->setMyIterator(p);
				it->transform()->setLocalPosition(vec3f(-6.8f, 2.7f, 2.3f));
				p = it->addComponent(new Light());
				(*p)->setMyIterator(p);
				it->light()->setColor((Color)vec4f(0, (float)89 / 255, (float)230 / 255));
				it->light()->setFalloff(0);
				it->light()->setType(Light::Type::Point);

				scene->backgroundColor = Color::black;
				camera->setDefaultView(float(width()) / float(height()));
				camera->setPosition(vec3f(-17.29f, 1.23f, 0.17f));
				camera->rotateYX(-91.45f, 4.2f);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void
P5::gui()
{
	mainMenu();
	if (_traceFlag)
		return;
	hierarchyWindow();
	inspectorWindow();
	assetsWindow();
	rendererWindow();


	/*static bool demo = true;
	ImGui::ShowDemoWindow(&demo);*/
}

inline void
renderMesh(GLMesh* mesh, GLuint mode)
{
	glPolygonMode(GL_FRONT_AND_BACK, mode);
	glDrawElements(GL_TRIANGLES, mesh->vertexCount(), GL_UNSIGNED_INT, 0);
}

constexpr auto CAMERA_RES = 0.01f;
constexpr auto ZOOM_SCALE = 1.01f;

void
P5::render()
{
	_renderer->update();
	if (_traceFlag)
	{
		if (_indexProgramaAtual != 1)
		{
			_program[1].use();
			_indexProgramaAtual = 1;
		}
		else
		{
			if (_image == nullptr)
			{
				_image = new GLImage{ width(), height() };
				_rayTracer->renderImage(*_image);
			}
			_image->draw();
			return;
		}
	}

	const auto& bc = _renderer->scene()->backgroundColor;

	_program[_indexProgramaAtual].use();
	glClearColor(bc.r, bc.g, bc.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_renderer->drawGround(10, 1);

	auto camera = _renderer->camera();

	if (_moveFlags)
	{
		const auto delta = camera->distance() * CAMERA_RES;
		auto d = vec3f::null();

		if (_moveFlags.isSet(MoveBits::Forward))
			d.z -= delta;
		if (_moveFlags.isSet(MoveBits::Back))
			d.z += delta;
		if (_moveFlags.isSet(MoveBits::Left))
			d.x -= delta;
		if (_moveFlags.isSet(MoveBits::Right))
			d.x += delta;
		if (_moveFlags.isSet(MoveBits::Up))
			d.y += delta;
		if (_moveFlags.isSet(MoveBits::Down))
			d.y -= delta;
		camera->translate(d);
	}
	_program[_indexProgramaAtual].setUniformMat4("vpMatrix", vpMatrix(camera));

	if (_current != _renderer->scene()) // desenha o selecionado
	{
		cg::SceneObject *object = (cg::SceneObject*)_current;
		if (object->primitive() != nullptr)
		{
			if (object->visible && object->primitive()->mesh() != nullptr)
			{
				cg::GLMesh * m;
				cg::Transform *t = object->transform();
				_program[_indexProgramaAtual].setUniformMat4("transform", t->localToWorldMatrix());

				m = glMesh((object->primitive())->mesh());
				m->bind();
				if (_indexProgramaAtual != 2)
				{
					_program[_indexProgramaAtual].setUniformVec4("material.diffuse", object->primitive()->material.diffuse);
					_program[_indexProgramaAtual].setUniform("flatMode", (int)1);
				}
				else
					_program[_indexProgramaAtual].setUniformVec4("color", object->primitive()->material.diffuse);
				auto normalMatrix = mat3f{ t->worldToLocalMatrix() }.transposed();
				_renderer->setVectorColor(Color::white);
				// _renderer->drawNormals(*(object->primitive())->mesh(), t->localToWorldMatrix(), normalMatrix);

				_renderer->setLineColor(_selectedWireframeColor);
				auto SIZE = object->primitive()->bvhSize();
				for (int i = 0; i < SIZE; i++)
				{
					auto b = object->primitive()->bvhAt(i);
					b.transform(t->localToWorldMatrix());
					_renderer->drawBounds(b);
				}
				_renderer->drawAxes(t->position(), mat3f{ t->rotation() });
				// renderMesh(m, GL_LINE);
			}
		}
	}

	if (_indexProgramaAtual == 2)
		_program[_indexProgramaAtual].setUniformVec4("color", _edgeColor);

	int luzPontualIndex = 0, luzDirecionalIndex = 0, luzSpotIndex = 0;
	for (std::list<cg::SceneObject>::iterator it = _renderer->scene()->containerBegin(); it != _renderer->scene()->containerEnd(); ++it)
		it->render(&_program[_indexProgramaAtual], &luzPontualIndex, &luzDirecionalIndex, &luzSpotIndex, _indexProgramaAtual);

	if (_indexProgramaAtual != 2)
	{
		_program[_indexProgramaAtual].setUniform("nLP", (int)luzPontualIndex);
		_program[_indexProgramaAtual].setUniform("nLD", (int)luzDirecionalIndex);
		_program[_indexProgramaAtual].setUniform("nLS", (int)luzSpotIndex);
		_program[_indexProgramaAtual].setUniformVec3("viewPos", camera->position());
	}
}

bool
P5::windowResizeEvent(int width, int height)
{
	_renderer->setImageSize(width, height);
	return true;
}

bool
P5::keyInputEvent(int key, int action, int mods)
{
	auto active = action != GLFW_RELEASE && mods == GLFW_MOD_ALT;

	switch (key)
	{
	case GLFW_KEY_W:
		_moveFlags.enable(MoveBits::Forward, active);
		break;
	case GLFW_KEY_S:
		_moveFlags.enable(MoveBits::Back, active);
		break;
	case GLFW_KEY_A:
		_moveFlags.enable(MoveBits::Left, active);
		break;
	case GLFW_KEY_D:
		_moveFlags.enable(MoveBits::Right, active);
		break;
	case GLFW_KEY_Q:
		_moveFlags.enable(MoveBits::Up, active);
		break;
	case GLFW_KEY_Z:
		_moveFlags.enable(MoveBits::Down, active);
		break;
	}

	if (action == GLFW_PRESS)
	{
		if (mods == GLFW_MOD_CONTROL && key == GLFW_KEY_DELETE)
		{
			std::cout << "Deleteando hierarquia do objeto selecionado\n";
			if (_current != nullptr && _current != _renderer->scene())
			{
				cg::SceneObject *currentObject = (cg::SceneObject*)_current;
				_current = currentObject->autoDelete();
			}
			return true;
		}
	}
	return false;
}

bool
P5::scrollEvent(double, double yOffset)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return false;
	_renderer->camera()->zoom(yOffset < 0 ? 1.0f / ZOOM_SCALE : ZOOM_SCALE);
	return true;
}

bool
P5::mouseButtonInputEvent(int button, int actions, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return false;
	(void)mods;

	auto active = actions == GLFW_PRESS;

	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (active)
		{
			cursorPosition(_pivotX, _pivotY);

			// TODO: scene intersection
			const auto ray = makeRay(_pivotX, _pivotY);
			float distance = ray.tMax;

			auto scene = _renderer->scene();
			std::stack<std::list<cg::SceneObject>::iterator> pilhaDeObjetos;
			for (std::list<cg::SceneObject>::iterator object = scene->containerBegin(); object != scene->containerEnd(); ++object)
				pilhaDeObjetos.push(object);
			while (!pilhaDeObjetos.empty())
			{
				auto object = pilhaDeObjetos.top();
				float d;
				vec3f position;
				vec3f normal;
				if (object->primitive() != nullptr)
				{
					bool iter = object->primitive()->intersect(ray, d, normal, position);
					if (d < distance && iter)
					{
						_current = &(*object);
						distance = d;
					}
				}
				pilhaDeObjetos.pop();
				for (std::list<cg::SceneObject>::iterator filho = object->childrenBegin(); filho != object->childrenEnd(); ++filho)
					pilhaDeObjetos.push(filho);
			}
		}
		return true;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		_dragFlags.enable(DragBits::Rotate, active);
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		_dragFlags.enable(DragBits::Pan, active);
	if (_dragFlags)
		cursorPosition(_pivotX, _pivotY);
	return true;
}

bool
P5::mouseMoveEvent(double xPos, double yPos)
{
	if (!_dragFlags)
		return false;
	_mouseX = (int)xPos;
	_mouseY = (int)yPos;

	const auto dx = (_pivotX - _mouseX);
	const auto dy = (_pivotY - _mouseY);

	_pivotX = _mouseX;
	_pivotY = _mouseY;
	if (dx != 0 || dy != 0)
	{
		auto camera = _renderer->camera();

		if (_dragFlags.isSet(DragBits::Rotate))
		{
			const auto da = -camera->viewAngle() * CAMERA_RES;
			camera->rotateYX(dx * da, dy * da, isKeyPressed(GLFW_KEY_LEFT_ALT));
		}
		if (_dragFlags.isSet(DragBits::Pan))
		{
			const auto dt = -camera->distance() * CAMERA_RES;
			camera->translate(-dt * math::sign(dx), dt * math::sign(dy), 0);
		}
	}
	return true;
}
