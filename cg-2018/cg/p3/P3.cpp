#include "MeshSweeper.h"
#include "P3.h"

MeshMap P3::_defaultMeshes;

inline void
P3::buildDefaultMeshes()
{
	_defaultMeshes["None"] = nullptr;
	_defaultMeshes["Box"] = MeshSweeper::makeBox();
}

inline Primitive*
makePrimitive(MeshMapIterator mit)
{
	return new Primitive(mit->second, mit->first);
}

inline void
P3::buildScene()
{
	std::cout << "Cria cena inicial\n";
	_current = _scene = new cg::Scene{ "Scene 1" };

	cg::Scene *currentScene = _scene;
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
	it->light()->setAmbient((Color)vec4f((float)35/255, (float)35/255, (float)35/255, 0));
	it->light()->setDiffuse((Color)vec4f((float)45/255, (float)45/255, (float)45/255, 0));
	it->light()->setSpecular((Color)vec4f((float)45/255, (float)45/255, (float)45/255, 0));
	it->light()->setDirection(vec3f(-1.0f, 0.0f, 0.0f));
	it->light()->setType(Light::Type::Directional);

	cg::SceneObject pointLight("Luz Pontual Amarela", *currentScene);
	it = currentScene->append(pointLight);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.9f, -1.0f, 0.0f));
	it->light()->setAmbient((Color)vec4f((float)80/255, (float)80/255, 0, 0));
	it->light()->setDiffuse((Color)vec4f((float)170/255, (float)181/255, 0, 0));
	it->light()->setSpecular((Color)vec4f((float)170/255, (float)193/255, 0, 0));

	cg::SceneObject redSpot("Luz Spot Vermelha no Jato", *currentScene);
	it = currentScene->append(redSpot);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.0f, -0.4f, 0.0f));
	it->light()->setAmbient((Color)vec4f(1, 0, 0, 1));
	it->light()->setDiffuse((Color)vec4f(1, 0, 0, 1));
	it->light()->setSpecular((Color)vec4f(1, 0, 0, 1));
	it->light()->setDirection(vec3f(-0.3f, -1.8f, -0.2f));
	it->light()->setType(Light::Type::Spot);

	cg::SceneObject blueSpot("Luz Spot Azul no Coelho", *currentScene);
	it = currentScene->append(blueSpot);
	it->setMyIterator(it);
	p = it->addComponent(new Transform());
	(*p)->setMyIterator(p);
	p = it->addComponent(new Light());
	(*p)->setMyIterator(p);
	it->transform()->setLocalPosition(vec3f(0.0f, 2.7f, 0.0f));
	it->light()->setAmbient((Color)vec4f(0, (float)22/255, (float)246/255, 0));
	it->light()->setDiffuse((Color)vec4f(0, (float)89/255, (float)230/255, 0));
	it->light()->setSpecular((Color)vec4f((float)3/255, (float)155/255, (float)170/255, 0));
	it->light()->setDirection(vec3f(0.1f, -0.6f, -0.2f));
	it->light()->setInnerCutOff(17.1);
	it->light()->setOuterCutOff(36.5);
	it->light()->setType(Light::Type::Spot);
}

void
P3::initialize()
{
	Application::loadShaders(_program[0], "shaders/p3.vs", "shaders/p3.fs");
	Application::loadShaders(_program[1], "shaders/p3-smooth.vs", "shaders/p3-smooth.fs");
	Assets::initialize();
	buildDefaultMeshes();
	buildScene();
	_camera = new Camera{};
	_camera->setDefaultView((float)width() / (float)height());
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);
	glEnable(GL_LINE_SMOOTH);
	_program[0].use();
}

namespace ImGui
{
	void ShowDemoWindow(bool*);
}

inline void
P3::hierarchyWindow()
{
	ImGui::Begin("Hierarchy");
	if (ImGui::Button("Options###object"))
		ImGui::OpenPopup("OptionsObjectPopup");
	if (ImGui::BeginPopup("OptionsObjectPopup"))
	{
		if (ImGui::MenuItem("New Empty Object"))
		{
			cg::Scene *currentScene = _scene;

			std::string objectIndex("Object ");
			static int i = 1;
			objectIndex += std::to_string(i++);

			if (_current == nullptr || _current == _scene)
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
				cg::Scene *currentScene = _scene;
				std::list<Reference<Component>>::iterator p;

				std::string boxIndex("Box ");
				static int index = 1;
				boxIndex += std::to_string(index++);

				if (_current == nullptr || _current == _scene)
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
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("New Light Object"))
		{
			cg::Scene *currentScene = _scene;

			std::string lightIndex("Light ");
			static int i = 1;
			lightIndex += std::to_string(i++);

			if (_current == nullptr || _current == _scene)
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
			if (_current != nullptr && _current != _scene)
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
	auto open = ImGui::TreeNodeEx(_scene,
		_current == _scene ? flag | ImGuiTreeNodeFlags_Selected : flag,
		_scene->name());

	if (ImGui::IsItemClicked())
		_current = _scene;
	if (ImGui::BeginDragDropTarget())
	{
		if (auto* payload = ImGui::AcceptDragDropPayload("Objeto"))
		{
			auto mit = *(std::list<SceneObject>::iterator*)payload->Data;
			mit->setParent(nullptr);
			auto it2 = --_scene->containerEnd();
			mit->autoDelete();
			_current = &(*it2);
		}
		ImGui::EndDragDropTarget();
	}
	if (open)
	{
		for (std::list<cg::SceneObject>::iterator it = _scene->containerBegin(); it != _scene->containerEnd(); ++it)
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
P3::sceneGui()
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
P3::inspectShape(Primitive& primitive)
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
P3::inspectMaterial(Material& material)
{
	ImGui::ColorEdit3("Ambient", material.ambient);
	ImGui::ColorEdit3("Diffuse", material.diffuse);
	ImGui::ColorEdit3("Spot", material.spot);
	ImGui::DragFloat("Shine", &material.shine, 1, 1, 1000.0f);
}

inline void
P3::inspectPrimitive(Primitive& primitive)
{
	//const auto flag = ImGuiTreeNodeFlags_NoTreePushOnOpen;

	//if (ImGui::TreeNodeEx("Shape", flag))
	inspectShape(primitive);
	//if (ImGui::TreeNodeEx("Material", flag))
	inspectMaterial(primitive.material);
}

inline void
P3::inspectLight(Light& light)
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

	temp = light.ambient();
	if (ImGui::ColorEdit3("Ambient Light", temp))
		light.setAmbient(temp);
	temp = light.diffuse();
	if (ImGui::ColorEdit3("Diffuse Light", temp))
		light.setDiffuse(temp);
	temp = light.specular();
	if (ImGui::ColorEdit3("Specular Light", temp))
		light.setSpecular(temp);
	if (lt == Light::Type::Directional)
	{
		temp2 = light.direction();
		if (ImGui::DragVec3("Direction", temp2))
			light.setDirection(temp2);
	}
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
		temp2 = light.direction();
		if (ImGui::DragVec3("Direction", temp2))
			light.setDirection(temp2);
		innerCutOff = light.innerCutOff();
		outerCutOff = light.outerCutOff();
		if (ImGui::DragFloat("Inner Cutoff", &innerCutOff, 0.5, 0.1, 89.0))
			light.setInnerCutOff(innerCutOff > outerCutOff ? outerCutOff - 0.1 : innerCutOff);
		if (ImGui::DragFloat("Outer Cutoff", &outerCutOff, 0.5, 0.1, 89.0))
			light.setOuterCutOff(outerCutOff < innerCutOff ? innerCutOff + 0.1 : outerCutOff);
	}
}

inline void
P3::addComponentButton(SceneObject& object)
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
P3::sceneObjectGui()
{
	auto object = (SceneObject*)_current;

	addComponentButton(*object);
	ImGui::Separator();
	ImGui::ObjectNameInput(object);
	ImGui::SameLine();
	ImGui::Checkbox("###visible", &object->visible);
	ImGui::Separator();
	if (object->transform() != nullptr)
		if (ImGui::CollapsingHeader(object->transform()->typeName()))
			ImGui::TransformEdit(object->transform());
	if (object->primitive() != nullptr)
		if (ImGui::CollapsingHeader(object->primitive()->typeName()))
			inspectPrimitive(*object->primitive());
	if(object->light() != nullptr)
		if (ImGui::CollapsingHeader(object->light()->typeName()))
			inspectLight(*object->light());
}

inline void
P3::objectGui()
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
P3::inspectorWindow()
{
	ImGui::Begin("Inspector");
	objectGui();
	ImGui::End();
}

inline void
P3::cameraGui()
{
	if (ImGui::Button("Set Default View"))
		_camera->setDefaultView(float(width()) / float(height()));
	ImGui::Separator();

	vec3f temp;

	ImGui::ObjectNameInput(_camera);
	temp = _camera->position();
	if (ImGui::DragVec3("Position", temp))
		_camera->setPosition(temp);
	temp = _camera->eulerAngles();
	if (ImGui::DragVec3("Rotation", temp))
		_camera->setEulerAngles(temp);

	auto d = _camera->distance();

	if (ImGui::DragFloat("Distance",
		&d,
		MIN_DISTANCE * 10.0f,
		MIN_DISTANCE,
		math::Limits<float>::inf()))
		_camera->setDistance(d <= MIN_DISTANCE ? MIN_DISTANCE : d);

	static const char* projectionNames[]{ "Perspective", "Orthographic" };
	auto cp = _camera->projectionType();

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
	_camera->setProjectionType(cp);
	if (cp == Camera::Perspective)
	{
		auto fov = _camera->viewAngle();

		if (ImGui::SliderFloat("View Angle",
			&fov,
			MIN_ANGLE,
			MAX_ANGLE,
			"%.0f deg",
			1.0f))
			_camera->setViewAngle(fov <= MIN_ANGLE ? MIN_ANGLE : (fov >= MAX_ANGLE ? MAX_ANGLE : fov));
	}
	else
	{
		auto h = _camera->height();

		if (ImGui::DragFloat("Height",
			&h,
			MIN_HEIGHT * 10.0f,
			MIN_HEIGHT,
			math::Limits<float>::inf()))
			_camera->setHeight(h <= MIN_HEIGHT ? MIN_HEIGHT : h);
	}

	float n;
	float f;

	_camera->clippingPlanes(n, f);

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
		_camera->setClippingPlanes(n, f);
	}
}

inline void
P3::assetsWindow()
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
P3::renderModeGui()
{
	static int sm;

	ImGui::Combo("Shading Mode", &sm, "None\0Flat\0Gouraud\0Phong\0\0");
	switch (sm)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		if(_indexProgramaAtual != 0)
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

	static Color edgeColor;
	static bool showEdges;

	ImGui::ColorEdit3("Edges", edgeColor);
	ImGui::SameLine();
	ImGui::Checkbox("###showEdges", &showEdges);
}

inline void
P3::rendererWindow()
{
	static int rt;

	ImGui::Begin("Renderer");
	ImGui::Combo("Type", &rt, "OpenGL\0Ray Tracer\0\0");
	if (!rt)
		renderModeGui();
	else
	{
		// TODO (part 5)
	}
	ImGui::Separator();
	if (ImGui::CollapsingHeader("Camera"))
		cameraGui();
	ImGui::End();
}

inline void
P3::fileMenu()
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
P3::showOptions()
{
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.6f);
	showStyleSelector("Color Theme##Selector");
	ImGui::ColorEdit3("Selected Wireframe", _selectedWireframeColor);
	ImGui::PopItemWidth();
}

inline void
P3::mainMenu()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			fileMenu();
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
		ImGui::EndMainMenuBar();
	}
}

void
P3::gui()
{
	mainMenu();
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
P3::render()
{
	const auto& bc = _scene->backgroundColor;

	glClearColor(bc.r, bc.g, bc.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (_moveFlags)
	{
		const auto delta = _camera->distance() * CAMERA_RES;
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
		_camera->translate(d);
	}
	_program[_indexProgramaAtual].setUniformMat4("vpMatrix", vpMatrix(_camera));
	_program[_indexProgramaAtual].setUniformVec3("viewPos", _camera->position());

	int luzPontualIndex = 0, luzDirecionalIndex = 0, luzSpotIndex = 0;
	for (std::list<cg::SceneObject>::iterator it = _scene->containerBegin(); it != _scene->containerEnd(); ++it)
		it->render(&_program[_indexProgramaAtual], &luzPontualIndex, &luzDirecionalIndex, &luzSpotIndex);

	_program[_indexProgramaAtual].setUniform("nLP", (int)luzPontualIndex);
	_program[_indexProgramaAtual].setUniform("nLD", (int)luzDirecionalIndex);
	_program[_indexProgramaAtual].setUniform("nLS", (int)luzSpotIndex);
	if (_current != _scene) // desenha o selecionado
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
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);

				_program[_indexProgramaAtual].setUniformVec4("material.diffuse", _selectedWireframeColor);
				_program[_indexProgramaAtual].setUniform("flatMode", (int)1);
				renderMesh(m, GL_LINE);
			}
		}
	}
}

bool
P3::windowResizeEvent(int width, int height)
{
	_camera->setAspectRatio(float(width) / float(height));
	return true;
}

bool
P3::keyInputEvent(int key, int action, int mods)
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
			if (_current != nullptr && _current != _scene)
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
P3::scrollEvent(double, double yOffset)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return false;
	_camera->zoom(yOffset < 0 ? 1.0f / ZOOM_SCALE : ZOOM_SCALE);
	return true;
}

bool
P3::mouseButtonInputEvent(int button, int actions, int mods)
{
	if (ImGui::GetIO().WantCaptureMouse)
		return false;
	(void)mods;

	auto active = actions == GLFW_PRESS;

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
		_dragFlags.enable(DragBits::Rotate, active);
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
		_dragFlags.enable(DragBits::Pan, active);
	if (_dragFlags)
		cursorPosition(_pivotX, _pivotY);
	return true;
}

bool
P3::mouseMoveEvent(double xPos, double yPos)
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
		if (_dragFlags.isSet(DragBits::Rotate))
		{
			const auto da = -_camera->viewAngle() * CAMERA_RES;
			_camera->rotateYX(dx * da, dy * da, isKeyPressed(GLFW_KEY_LEFT_ALT));
		}
		if (_dragFlags.isSet(DragBits::Pan))
		{
			const auto dt = -_camera->distance() * CAMERA_RES;
			_camera->translate(-dt * math::sign(dx), dt * math::sign(dy), 0);
		}
	}
	return true;
}
