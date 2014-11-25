/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#include <ege/debug.h>
#include <ege/widget/Scene.h>

#include <math.h>
#include <ege/debug.h>
#include <ewol/ewol.h>
#include <ewol/openGL/openGL.h>
#include <etk/math/Matrix4.h>
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

#undef __class__
#define __class__ "Scene"
namespace etk {
	template<> std::string to_string<std::shared_ptr<ewol::resource::Colored3DObject> >(const std::shared_ptr<ewol::resource::Colored3DObject>& _value) {
		return "{{ERROR}}";
	}
};

ege::widget::Scene::Scene() :
  signalDisplayDebug(*this, "drawDebug", "Call to draw debug after all elements"),
  m_cameraName("default") {
	addObjectType("ege::widget::Scene");
}

void ege::widget::Scene::init(std::shared_ptr<ege::Environement> _env) {
	m_env = _env;
	ewol::Widget::init();
	setKeyboardRepeate(false);
	setCanHaveFocus(true);
	periodicCallEnable();
	m_debugDrawProperty = ewol::resource::Colored3DObject::create();
}

ege::widget::Scene::~Scene() {
	
}

void ege::widget::Scene::onRegenerateDisplay() {
	if (true == needRedraw()) {
		
	}
}

//#define SCENE_DISPLAY_SPEED

//////////////////////////////////////////////////////////////////////////////////////////////
#ifdef SCENE_DISPLAY_SPEED
	static int64_t g_startTime = 0;
	static int32_t g_counterNbTimeDisplay = 0;
#endif

#define NUMBER_OF_SUB_PASS  (0)


void ege::widget::Scene::onDraw() {
	#ifdef SCENE_DISPLAY_SPEED
		g_counterNbTimeDisplay++;
		g_startTime = ewol::getTime();
	#endif
	
	// draw constant object :
	{
		mat4 tmpMatrix;
		for (auto &it : m_env->getStaticMeshToDraw()) {
			if (it != nullptr) {
				it->draw(tmpMatrix);
			}
		}
	}
	
	// get camera :
	std::shared_ptr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera == nullptr) {
		EGE_ERROR(" can not get camera named: '" << m_cameraName << "'");
		return;
	}
	//EGE_DEBUG("Draw (start)");
	mat4 tmpMatrix;
	std::shared_ptr<btDynamicsWorld> world = m_env->getPhysicEngine().getDynamicWorld();
	if (world != nullptr) {
		
		m_env->getOrderedElementForDisplay(m_displayElementOrdered, camera->getEye(), camera->getViewVector());
		EGE_DEBUG("DRAW : " << m_displayElementOrdered.size() << "/" << m_env->getElement().size() << " elements");
		
		// TODO : remove this  == > no more needed ==> checked in the generate the list of the element ordered
		for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
			m_displayElementOrdered[iii].element->preCalculationDraw(*camera);
		}
		// note :  the first pass is done at the reverse way to prevent multiple display od the same point in the screen 
		//         (and we remember that the first pass is to display all the non transparent elements)
		for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
			m_displayElementOrdered[iii].element->draw(0);
		}
		// for the other pass the user can draw transparent elements ...
		for (int32_t pass=1; pass <= NUMBER_OF_SUB_PASS+1; pass++) {
			for (size_t iii=0; iii<m_displayElementOrdered.size(); iii++) {
				m_displayElementOrdered[iii].element->draw(pass);
			}
		}
		// Draw debug ...
		for (int32_t iii=m_displayElementOrdered.size()-1; iii >= 0; iii--) {
			m_displayElementOrdered[iii].element->drawDebug(m_debugDrawProperty, camera);
		}
		signalDisplayDebug.emit(m_debugDrawProperty);
	} else {
		EGE_WARNING("No Dynamic world ...");
	}
	if (camera != nullptr) {
		m_env->getParticuleEngine().draw(*camera);
	}
	#ifdef SCENE_DISPLAY_SPEED
		float localTime = (float)(ewol::getTime() - g_startTime) / 1000.0f;
		if (localTime>1) {
			EWOL_ERROR("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		} else {
			EWOL_DEBUG("      scene : " << localTime << "ms " << g_counterNbTimeDisplay);
		}
	#endif
}

// I really does not know what is this ...
btRigidBody& btActionInterface::getFixedBody() {
	static btRigidBody s_fixed(0, 0,0);
	s_fixed.setMassProps(btScalar(0.),btVector3(btScalar(0.),btScalar(0.),btScalar(0.)));
	return s_fixed;
}

void ege::widget::Scene::periodicCall(const ewol::event::Time& _event) {
	markToRedraw();
}

//#define SCENE_BRUT_PERFO_TEST

void ege::widget::Scene::systemDraw(const ewol::DrawProperty& _displayProp) {
	#ifdef SCENE_BRUT_PERFO_TEST
		int64_t tmp___startTime0 = ewol::getTime();
	#endif
	glBindBuffer(GL_ARRAY_BUFFER,0);
	ewol::openGL::push();
	// here we invert the reference of the standard openGl view because the reference in the common display is Top left and not buttom left
	glViewport( m_origin.x(),
	            m_origin.y(),
	            m_size.x(),
	            m_size.y());
	// configure render with the camera...
	std::shared_ptr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->configureOpenGL();
	}
	onDraw();
	ewol::openGL::pop();
	#ifdef SCENE_BRUT_PERFO_TEST
		float tmp___localTime1 = (float)(ewol::getTime() - tmp___startTime0) / 1000.0f;
		EWOL_DEBUG("      SCENE render  : " << tmp___localTime1 << "ms ");
	#endif
}

void ege::widget::Scene::onParameterChangeValue(const ewol::parameter::Ref& _paramPointer) {
	ewol::Widget::onParameterChangeValue(_paramPointer);
	/*
	if (_paramPointer == m_isRunning) {
		// nothing to do ...
	}
	*/
}


void ege::widget::Scene::setCamera(const std::string& _cameraName) {
	if (m_cameraName == _cameraName) {
		return;
	}
	m_cameraName = _cameraName;
	// Update camera aspect ratio:
	std::shared_ptr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->setSceenSize(m_size);
	}
}

void ege::widget::Scene::calculateSize(const vec2& _available) {
	ewol::Widget::calculateSize(_available);
	// Update camera aspect ratio:
	std::shared_ptr<ege::Camera> camera = m_env->getCamera(m_cameraName);
	if (camera != nullptr) {
		camera->setSceenSize(m_size);
	}
}


