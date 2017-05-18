/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#include <ege/render/Component.hpp>
#include <ege/position/Component.hpp>
#include <ege/physics/Component.hpp>

const std::string& ege::render::Component::getType() const {
	static std::string tmp("render");
	return tmp;
}

ege::render::Component::Component() {
	
}

ege::render::Component::Component(const std::string& _fileName) {
	loadMesh(_fileName);
}

ege::render::Component::Component(ememory::SharedPtr<ege::resource::Mesh> _mesh) :
  m_mesh(_mesh) {
	
}

void ege::render::Component::addFriendComponent(const ememory::SharedPtr<ege::Component>& _component) {
	if (_component->getType() == "physics") {
		// get the real usable pointer
		ememory::SharedPtr<ege::physics::Component> component = ememory::staticPointerCast<ege::physics::Component>(_component);
		// get the current transform
		m_transform = component->getTransform();
		// connnect a signal on it to keep all change
		component->signalPosition.connect(sharedFromThis(), &ege::render::Component::onSignalPositionChange);
	} else if (_component->getType() == "position") {
		// get the real usable pointer
		ememory::SharedPtr<ege::position::Component> component = ememory::staticPointerCast<ege::position::Component>(_component);
		// get the current transform
		m_transform = component->getTransform();
		// connnect a signal on it to keep all change
		component->signalPosition.connect(sharedFromThis(), &ege::render::Component::onSignalPositionChange);
	}
}

void ege::render::Component::onSignalPositionChange(const etk::Transform3D& _transform) {
	m_transform = _transform;
}

bool ege::render::Component::loadMesh(const std::string& _meshFileName) {
	ememory::SharedPtr<ege::resource::Mesh> tmpMesh = ege::resource::Mesh::create(_meshFileName);
	if(tmpMesh == nullptr) {
		EGE_ERROR("can not load the resources : " << _meshFileName);
		return false;
	}
	return setMesh(tmpMesh);
}

bool ege::render::Component::setMesh(ememory::SharedPtr<ege::resource::Mesh> _mesh) {
	if (m_mesh != nullptr) {
		m_mesh.reset();
	}
	m_mesh = _mesh;
	// auto load the shape :
	if (m_mesh == nullptr) {
		return true;
	}
	return true;
}