#include "P1.h"

namespace cg
{ // begin namespace cg

inline Primitive*
makeBoxMesh()
{
  const vec4f p1{-0.5, -0.5, -0.5, 1};
  const vec4f p2{+0.5, -0.5, -0.5, 1};
  const vec4f p3{+0.5, +0.5, -0.5, 1};
  const vec4f p4{-0.5, +0.5, -0.5, 1};
  const vec4f p5{-0.5, -0.5, +0.5, 1};
  const vec4f p6{+0.5, -0.5, +0.5, 1};
  const vec4f p7{+0.5, +0.5, +0.5, 1};
  const vec4f p8{-0.5, +0.5, +0.5, 1};
  const Color c1{Color::black};
  const Color c2{Color::red};
  const Color c3{Color::yellow};
  const Color c4{Color::green};
  const Color c5{Color::blue};
  const Color c6{Color::magenta};
  const Color c7{Color::cyan};
  const Color c8{Color::white};

  // Box vertices
  static const vec4f v[]
  {
    p1, p5, p8, p4, // x = -0.5
    p2, p3, p7, p6, // x = +0.5
    p1, p2, p6, p5, // y = -0.5
    p4, p8, p7, p3, // y = +0.5
    p1, p4, p3, p2, // z = -0.5
    p5, p6, p7, p8  // z = +0.5
  };

  // Box vertex colors
  static const Color c[]
  {
    c1, c5, c8, c4, // x = -0.5
    c2, c3, c7, c6, // x = +0.5
    c1, c2, c6, c5, // y = -0.5
    c4, c8, c7, c3, // y = +0.5
    c1, c4, c3, c2, // z = -0.5
    c5, c6, c7, c8  // z = +0.5
  };

  // Box triangles
  static const GLMeshArray::Triangle t[]
  {
    { 0,  1,  2}, { 2,  3,  0},
    { 4,  5,  7}, { 5,  6,  7},
    { 8,  9, 11}, { 9, 10, 11},
    {12, 13, 14}, {14, 15, 12},
    {16, 17, 19}, {17, 18, 19},
    {20, 21, 22}, {22, 23, 20}
  };

  return new Primitive{new GLMeshArray{24, {v, 0}, {c, 1}, 12, t}};
}

} // end namespace cg

inline void
P1::buildScene()
{
  std::cout << "Cria cena inicial\n";
  _current = _scene = new cg::Scene{"Scene 1"};

  cg::Scene *currentScene = _scene;
  cg::SceneObject *newBox, *currentBox;
  std::list<cg::SceneObject>::iterator it;
  
  newBox = new cg::SceneObject{ "Box 1", currentScene, cg::makeBoxMesh()}; // criando box nivel 1  
  it = currentScene->append(*newBox);
  it->setMyIterator(it);

  currentBox = it->mySelf(); // criando box nivel 2
  newBox = new cg::SceneObject{ "Box 1.1", currentScene, cg::makeBoxMesh() };
  newBox->setParent(currentBox);
  it = currentBox->appendChildren(*newBox);
  it->setMyIterator(it);

  newBox = new cg::SceneObject{ "Box 2", currentScene, cg::makeBoxMesh() }; // criando box nivel 1
  it = currentScene->append(*newBox);
  it->setMyIterator(it);

  currentBox = it->mySelf(); // criando box nivel 2
  newBox = new cg::SceneObject{ "Box 2.1", currentScene, cg::makeBoxMesh() };
  newBox->setParent(currentBox);
  it = currentBox->appendChildren(*newBox);
  it->setMyIterator(it);
}

void
P1::initialize()
{
  cg::Application::loadShaders(_program, "p1.vs", "p1.fs");
  buildScene();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0f, 1.0f);
  glLineWidth(2.0f);
  glEnable(GL_LINE_SMOOTH);
  _program.use();
}

namespace ImGui
{
  void ShowDemoWindow(bool*);
}

inline void
P1::hierarchyWindow()
{
  ImGui::Begin("Hierarchy");
  if (ImGui::Button("Create###object"))
    ImGui::OpenPopup("CreateObjectPopup");
  if (ImGui::BeginPopup("CreateObjectPopup"))
  {
	if (ImGui::MenuItem("Empty Object"))
	{
		cg::SceneObject *newBox;
		cg::Scene *currentScene = _scene;

		std::string objectIndex("Object ");
		static int i = 1;
		objectIndex += std::to_string(i++);

		std::cout << "Create new Empty Box\n";
		if (_current == nullptr || _current == _scene)
		{
			std::cout << "\tCria raiz\n";
			newBox = new cg::SceneObject{ objectIndex.c_str(), currentScene};
			std::list<cg::SceneObject>::iterator it = currentScene->append(*newBox);
			it->setMyIterator(it); // criando nova raíz
		}
		else
		{
			cg::SceneObject *currentBox = (cg::SceneObject*)(_current);
			std::cout << "\tCria com pai\n";
			newBox = new cg::SceneObject{ objectIndex.c_str(), currentScene};
			newBox->setParent(currentBox);
			std::list<cg::SceneObject>::iterator it = currentBox->appendChildren(*newBox);
			it->setMyIterator(it); // criando child
		}
	}
    if (ImGui::BeginMenu("3D Object"))
    {
      if (ImGui::MenuItem("Box"))
      {
		  cg::SceneObject *newBox;
		  cg::Scene *currentScene = _scene;

		  std::string boxIndex("Box ");
		  static int index = 1;
		  boxIndex += std::to_string(index++);

		  std::cout << "Create new 3D Box\n";
		  if (_current == nullptr || _current == _scene)
		  {
			  std::cout << "\tCria raiz\n";
			  newBox = new cg::SceneObject{ boxIndex.c_str(), currentScene, cg::makeBoxMesh() };
			  std::list<cg::SceneObject>::iterator it = currentScene->append(*newBox);
			  it->setMyIterator(it); // criando nova raíz
		  }
		  else
		  {
			  cg::SceneObject *currentBox = (cg::SceneObject*)(_current);
			  std::cout << "\tCria com pai\n";
			  newBox = new cg::SceneObject{ boxIndex.c_str(), currentScene, cg::makeBoxMesh() };
			  newBox->setParent(currentBox);
			  std::list<cg::SceneObject>::iterator it = currentBox->appendChildren(*newBox);
			  it->setMyIterator(it); // criando child
		  }
      }
      ImGui::EndMenu();
    }
    ImGui::EndPopup();
  }
  ImGui::Separator();

  ImGuiTreeNodeFlags flag{ImGuiTreeNodeFlags_OpenOnArrow};
  auto open = ImGui::TreeNodeEx(_scene,
    _current == _scene ? flag | ImGuiTreeNodeFlags_Selected : flag,
    _scene->name());
  
  if (ImGui::IsItemClicked())
    _current = _scene;
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
ObjectNameInput(cg::SceneNode* object)
{
  const int bufferSize{128};
  static cg::SceneNode* current;
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
ColorEdit3(const char* label, cg::Color& color)
{
  return ImGui::ColorEdit3(label, (float*)&color);
}

inline bool
DragVec3(const char* label, vec3f& v)
{
  return DragFloat3(label, (float*)&v, 0.1f, 0.0f, 0.0f, "%.2g");
}

void
TransformEdit(cg::Transform* transform)
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
P1::sceneGui()
{
  auto scene = (cg::Scene*)_current;

  ImGui::ObjectNameInput(_current);
  ImGui::Separator();
  if (ImGui::CollapsingHeader("Colors"))
  {
    ImGui::ColorEdit3("Background", backgroundColor);
    ImGui::ColorEdit3("Selected Wireframe", selectedWireframeColor);
  }
}

inline void
P1::sceneObjectGui()
{
	auto object = (cg::SceneObject*)_current;

	ImGui::ObjectNameInput(object);
	ImGui::SameLine();
	ImGui::Checkbox("###visible", &object->visible);
	ImGui::Separator();

	if (ImGui::CollapsingHeader(object->transform()->typeName()))
	{
		auto t = object->transform();

		ImGui::TransformEdit(t);
		_transform = t->localToWorldMatrix();
	}
	if (object->primitive() != nullptr)
	{
		if (ImGui::CollapsingHeader(object->primitive()->typeName()))
		{
			// TODO: show primitive properties.
			auto m = object->primitive()->mesh();
			static bool v_borders = false;
			ImGui::Columns(4, NULL, v_borders);
			for (int j = 0; j < m->numeroDeTriangulos; j++)
			{
				ImGui::Separator();
				ImGui::Text("Triangle %d", j+1);
				ImGui::NextColumn();
				cg::GLMeshArray::Triangle v = m->triangles[j];
				for (int i = 0; i < 3; i++)
				{
					ImGui::Text("x %.1f", m->pos[v.v[i]].x);
					ImGui::Text("y %.1f", m->pos[v.v[i]].y);
					ImGui::Text("z %.1f", m->pos[v.v[i]].z);
					ImGui::PushID(i);
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(m->colors[v.v[i]].r, m->colors[v.v[i]].g, m->colors[v.v[i]].b, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(m->colors[v.v[i]].r, m->colors[v.v[i]].g + 0.1f, m->colors[v.v[i]].b + 0.1f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(m->colors[v.v[i]].r, m->colors[v.v[i]].g + 0.2f, m->colors[v.v[i]].b + 0.2f, 1.0f));
					ImGui::Button(" ");
					ImGui::PopStyleColor(3);
					ImGui::PopID();

					ImGui::NextColumn();
				}
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
	}
}

inline void
P1::objectGui()
{
  if (_current == nullptr)
    return;
  if (dynamic_cast<cg::SceneObject*>(_current))
  {
    sceneObjectGui();
    return;
  }
  if (dynamic_cast<cg::Scene*>(_current))
    sceneGui();
}

inline void
P1::inspectorWindow()
{
  ImGui::Begin("Inspector");
  objectGui();
  ImGui::End();
}

void
P1::gui()
{
  hierarchyWindow();
  inspectorWindow();
  
  static bool demo = true;
  ImGui::ShowDemoWindow(&demo);
  
}

void
P1::render()
{
	Base::render();
	for(std::list<cg::SceneObject>::iterator it = _scene->containerBegin(); it != _scene->containerEnd(); ++it)
		it->render(&_program);

	if (_current != _scene) // desenha o selecionado
	{
		cg::SceneObject *object = (cg::SceneObject*)_current;
		if (object->visible && object->primitive() != nullptr)
		{
			cg::GLMeshArray * m;
			cg::Transform *t = object->transform();
			_program.setUniformMat4("transform", t->localToWorldMatrix());

			m = (object->primitive())->mesh();
			m->bind();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);

			m = (object->primitive())->mesh();
			m->setVertexColor(selectedWireframeColor);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDrawElements(GL_TRIANGLES, m->vertexCount(), GL_UNSIGNED_INT, 0);
			m->useVertexColors();
		}
	}

}
