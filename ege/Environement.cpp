/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/Environement.hpp>
#include <ege/elements/Element.hpp>
#include <ewol/object/Manager.hpp>

#include <gale/renderer/openGL/openGL.hpp>
#include <etk/math/Matrix4.hpp>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <LinearMath/btDefaultMotionState.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/CollisionShapes/btCollisionShape.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionShapes/btConvexPolyhedron.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <LinearMath/btTransformUtil.h>
#include <LinearMath/btIDebugDraw.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btCollisionObject.h>

ememory::SharedPtr<ege::Element> ege::Environement::getElementNearest(ememory::SharedPtr<ege::Element> _sourceRequest, float& _distance) {
	if (_sourceRequest == nullptr) {
		return nullptr;
	}
	vec3 sourcePosition = _sourceRequest->getPosition();
	ememory::SharedPtr<ege::Element> result = nullptr;
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		// chack nullptr  pointer
		if (m_listElement[iii] == nullptr) {
			continue;
		}
		if (m_listElement[iii]->getGroup() <= 0) {
			continue;
		}
		// check if they are in the same group:
		if (m_listElement[iii]->getGroup() == _sourceRequest->getGroup()) {
			continue;
		}
		// check distance ...
		vec3 destPosition = m_listElement[iii]->getPosition();
		float distance = btDistance(sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distance>distance) {
			_distance = distance;
			result = m_listElement[iii];
		}
	}
	return result;
}


void ege::Environement::getElementNearest(const vec3& _sourcePosition,
                                          float _distanceMax,
                                          std::vector<ege::Environement::ResultNearestElement>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		// chack nullptr  pointer
		result.element = m_listElement[iii];
		if (result.element == nullptr) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->getPosition();
		if (_sourcePosition == destPosition) {
			continue;
		}
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax>result.dist) {
			_resultList.push_back(result);
		}
	}
}

void ege::Environement::getElementNearestFixed(const vec3& _sourcePosition,
                                               float _distanceMax,
                                               std::vector<ege::Environement::ResultNearestElement>& _resultList) {
	_resultList.clear();
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		// chack nullptr  pointer
		result.element = m_listElement[iii];
		if (result.element == nullptr) {
			continue;
		}
		if (result.element->isFixed() == false) {
			continue;
		}
		// check distance ...
		vec3 destPosition = result.element->getPositionTheoric();
		result.dist = btDistance(_sourcePosition, destPosition);
		//EGE_DEBUG("Distance : " << _distance << " >? " << distance << " id=" << iii);
		if (_distanceMax <= result.dist) {
			continue;
		}
		// try to add the element at the best positions:
		size_t jjj;
		for (jjj=0; jjj<_resultList.size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.insert(_resultList.begin()+jjj, result);
				break;
			}
		}
		// add element at the end :
		if (jjj >= _resultList.size()) {
			_resultList.push_back(result);
		}
	}
}

static etk::Hash<ege::createElement_tf>& getHachTableCreating() {
	static etk::Hash<ege::createElement_tf> s_table;
	return s_table;
}

void ege::Environement::addCreator(const std::string& _type, ege::createElement_tf _creator) {
	if (_creator == nullptr) {
		EGE_ERROR("Try to add an empty CREATOR ...");
		return;
	}
	EGE_DEBUG("Add creator: " << _type);
	getHachTableCreating().add(_type, _creator);
	EGE_DEBUG("Add creator: " << _type << " (done)");
}


ememory::SharedPtr<ege::Element> ege::Environement::createElement(const std::string& _type, const std::string& _description, bool _autoAddElement) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	ememory::SharedPtr<ege::Element> tmpElement = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (tmpElement->initString(_description) == false) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElement(tmpElement);
	}
	return tmpElement;
}

ememory::SharedPtr<ege::Element> ege::Environement::createElement(const std::string& _type, const ejson::Value& _value, bool _autoAddElement) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	ememory::SharedPtr<ege::Element> tmpElement = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (tmpElement->initJSON(_value) == false) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElement(tmpElement);
	}
	return tmpElement;
}

ememory::SharedPtr<ege::Element> ege::Environement::createElement(const std::string& _type, const exml::Node& _node, bool _autoAddElement) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	ememory::SharedPtr<ege::Element> tmpElement = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (tmpElement->initXML(_node) == false) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElement(tmpElement);
	}
	return tmpElement;
}

ememory::SharedPtr<ege::Element> ege::Environement::createElement(const std::string& _type, void* _data, bool _autoAddElement) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	ememory::SharedPtr<ege::Element> tmpElement = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (tmpElement->initVoid(_data) == false) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElement(tmpElement);
	}
	return tmpElement;
}

ememory::SharedPtr<ege::Element> ege::Environement::createElement(const std::string& _type, bool _autoAddElement) {
	if (getHachTableCreating().exist(_type) == false) {
		EGE_ERROR("Request creating of an type that is not known '" << _type << "'");
		return nullptr;
	}
	ege::createElement_tf creatorPointer = getHachTableCreating()[_type];
	if (creatorPointer == nullptr) {
		EGE_ERROR("nullptr pointer creator  == > internal error... '" << _type << "'");
		return nullptr;
	}
	ememory::SharedPtr<ege::Element> tmpElement = creatorPointer(ememory::dynamicPointerCast<ege::Environement>(sharedFromThis()));
	if (tmpElement == nullptr) {
		EGE_ERROR("allocation error '" << _type << "'");
		return nullptr;
	}
	if (tmpElement->init() == false) {
		EGE_ERROR("Init error ... '" << _type << "'");
		return nullptr;
	}
	if (_autoAddElement == true) {
		addElement(tmpElement);
	}
	return tmpElement;
}

void ege::Environement::addElement(ememory::SharedPtr<ege::Element> _newElement) {
	// prevent memory allocation and un allocation ...
	if (_newElement == nullptr) {
		return;
	}
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		if (m_listElement[iii] == nullptr) {
			m_listElement[iii] = _newElement;
			m_listElement[iii]->dynamicEnable();
			return;
		}
	}
	m_listElement.push_back(_newElement);
	_newElement->dynamicEnable();
}

void ege::Environement::rmElement(ememory::SharedPtr<ege::Element> _removeElement) {
	if (_removeElement == nullptr) {
		return;
	}
	// inform the element that an element has been removed  == > this permit to keep pointer on elements ...
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		if (m_listElement[iii] != nullptr) {
			m_listElement[iii]->elementIsRemoved(_removeElement);
		}
	}
	// ream remove on the element :
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		if (_removeElement == m_listElement[iii]) {
			m_listElement[iii]->onDestroy();
			m_listElement[iii]->dynamicDisable();
			m_listElement[iii]->unInit();
			m_listElement[iii].reset();
		}
	}
}


void ege::Environement::getOrderedElementForDisplay(std::vector<ege::Environement::ResultNearestElement>& _resultList,
                                                    const vec3& _position,
                                                    const vec3& _direction) {
	// TODO : Set it back ... corrected...
	// remove all unneeded elements (old display...)
	_resultList.clear();
	// basic element result
	ege::Environement::ResultNearestElement result;
	result.dist = 99999999999.0f;
	result.element = nullptr;
	// for all element in the game we chek if it is needed to display it ...
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		// chack nullptr  pointer
		if (m_listElement[iii] == nullptr) {
			// no pointer null are set in the output list ...
			continue;
		}
		result.element = m_listElement[iii];
		// check distance ...
		vec3 destPosition = result.element->getPosition();
		vec3 angleView = (destPosition - _position);
		angleView.safeNormalize();
		float dotResult=angleView.dot(_direction);
		//EGE_DEBUG("Dot position : " << destPosition << "  == > dot=" << dotResult);
		/*
		if (dotResult <= 0.85f) {
			// they are not in the camera angle view ...  == > no need to process display
			continue;
		}
		*/
		result.dist = btDistance(_position, destPosition);
		/*
		if (result.dist>500.0f) {
			// The element is realy too far ...  == > no need to display
			continue;
		}
		*/
		// try to add the element at the best positions:
		size_t jjj;
		for (jjj=0; jjj<_resultList.size(); jjj++) {
			if (_resultList[jjj].dist>result.dist) {
				_resultList.insert(_resultList.begin()+jjj, result);
				break;
			}
		}
		// add element at the end :
		if (jjj >= _resultList.size()) {
			_resultList.push_back(result);
		}
	}
}


void ege::Environement::generateInteraction(ege::ElementInteraction& _event) {
	// inform the element that an element has been removed  == > this permit to keep pointer on elements ...
	for (size_t iii=0; iii<m_listElement.size() ; iii++) {
		if (m_listElement[iii] == nullptr) {
			continue;
		}
		_event.applyEvent(*m_listElement[iii]);
		/*
		vec3 destPosition = m_listElement[iii]->getPosition();
		float dist = btDistance(sourcePosition, destPosition);
		if (dist == 0 || dist>decreasePower) {
			continue;
		}
		float inpact = (decreasePower-dist)/decreasePower * power;
		g_listElement[iii]->setFireOn(groupIdSource, type, -inpact, sourcePosition);
		*/
	}
}

ege::Environement::Environement() :
  signalPlayTimeChange(this, "time-change", ""),
  propertyStatus(this, "status",
                       gameStop,
                       "Satus of the activity of the Environement",
                       &ege::Environement::onChangePropertyStatus),
  propertyRatio(this, "ratio",
                      1.0f,
                      "game speed ratio"),
  m_listElement(),
  m_particuleEngine(this) {
	// nothing to do ...
	propertyStatus.add(gameStart, "start", "Scene is started");
	propertyStatus.add(gamePause, "pause", "Scene is paused");
	propertyStatus.add(gameStop, "stop", "Scene is stopped");
}

void ege::Environement::clear() {
	m_listElement.clear();
}


void ege::Environement::onCallbackPeriodicCall(const ewol::event::Time& _event) {
	float curentDelta = _event.getDeltaCall();
	EGE_VERBOSE("periodic call : " << _event);
	// small hack to change speed ...
	curentDelta *= *propertyRatio;
	// check if the processing is availlable
	if (propertyStatus.get() == gameStop) {
		return;
	}
	// update game time:
	int32_t lastGameTime = m_gameTime*0.000001f;
	m_gameTime += curentDelta;
	if (lastGameTime != (int32_t)(m_gameTime*0.000001f)) {
		EGE_VERBOSE("    Emit Signal");
		signalPlayTimeChange.emit(m_gameTime*0.000001f);
	}
	
	//EWOL_DEBUG("Time: m_lastCallTime=" << m_lastCallTime << " deltaTime=" << deltaTime);
	
	// update camera positions:
	for (auto &it : m_listCamera) {
		if (it.second != nullptr) {
			EGE_VERBOSE("    update camera : '" << it.first << "'");
			it.second->periodicCall(curentDelta);
		}
	}
	//EGE_DEBUG("stepSimulation (start)");
	///step the simulation
	if (m_physicEngine.getDynamicWorld() != nullptr) {
		EGE_VERBOSE("    step simulation : " << curentDelta);
		m_physicEngine.getDynamicWorld()->stepSimulation(curentDelta);
		//optional but useful: debug drawing
		m_physicEngine.getDynamicWorld()->debugDrawWorld();
	}
	EGE_VERBOSE("    Update particule engine");
	m_particuleEngine.update(curentDelta);
	// remove all element that requested it ...
	{
		int32_t numberEnnemyKilled=0;
		int32_t victoryPoint=0;
		auto it(m_listElement.begin());
		while (it != m_listElement.end()) {
			if(*it != nullptr) {
				if ((*it)->needToRemove() == true) {
					if ((*it)->getGroup() > 1) {
						numberEnnemyKilled++;
						victoryPoint++;
					}
					EGE_VERBOSE("[" << (*it)->getUID() << "] element Removing ... " << (*it)->getType());
					rmElement((*it));
					it = m_listElement.begin();
				} else {
					++it;
				}
			} else {
				++it;
			}
		}
		if (numberEnnemyKilled != 0) {
			//signalKillEnemy.emit(numberEnnemyKilled);
		}
	}
}


void ege::Environement::addCamera(const std::string& _name, const ememory::SharedPtr<ege::Camera>& _camera) {
	m_listCamera.insert(std::make_pair(_name, _camera));
}

ememory::SharedPtr<ege::Camera> ege::Environement::getCamera(const std::string& _name) {
	auto cameraIt = m_listCamera.find(_name);
	if (cameraIt != m_listCamera.end()) {
		return cameraIt->second;
	}
	return nullptr;
}


void ege::Environement::onChangePropertyStatus() {
	if (propertyStatus.get() == gameStart) {
		m_periodicCallConnection = getObjectManager().periodicCall.connect(this, &ege::Environement::onCallbackPeriodicCall);
	} else {
		m_periodicCallConnection.disconnect();
	}
}


