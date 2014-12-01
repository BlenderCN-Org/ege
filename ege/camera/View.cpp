/**
 * @author Edouard DUPIN
 * 
 * @copyright 2013, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */


#include <ege/camera/View.h>
#include <ege/debug.h>
#include <etk/math/Vector3D.h>

#undef __class__
#define __class__ "camera::View"

void ege::camera::View::update() {
	//m_matrix = etk::matLookAt(m_eye, m_target, m_up);
	//m_matrix.translate(m_eye);
	m_matrix.identity();
	m_matrix.rotate(vec3(0,0,1), -m_angle);
	vec3 pos = -getViewVector();
	vec2 angles = tansformPositionToAngle(pos);
	float distance = pos.length();
	
	m_matrix.translate(vec3(0,0,-distance));
	m_matrix.rotate(vec3(1,0,0), -M_PI*0.5f + angles.y());
	m_matrix.rotate(vec3(0,0,1), -angles.x()-M_PI/2.0f);
	m_matrix.translate(-m_target);
	
	EGE_DEBUG("Camera properties : distance=" << distance );
	EGE_DEBUG("                         psy=" << angles.y());
	EGE_DEBUG("                       Tetha=" << angles.x());
	EGE_DEBUG("                       m_eye=" << etk::to_string(m_eye));
}

ege::camera::View::View(const vec3& _eye, const vec3& _target, float _angle) :
  m_eye(_eye),
  m_target(_target),
  m_angle(_angle) {
	update();
}

void ege::camera::View::setEye(const vec3& _eye) {
	m_eye = _eye;
	update();
}

void ege::camera::View::setTarget(const vec3& _target) {
	m_target = _target;
	update();
}

void ege::camera::View::setAngle(float _angle) {
	m_angle = _angle;
	update();
}

vec3 ege::camera::View::getViewVector() const {
	return m_target - m_eye;
}


ege::Ray ege::camera::View::getRayFromScreen(const vec2& _offset) {
	vec2 cameraAngleOffset(m_angleView*0.5f*_offset.x(), _offset.y()*0.5f*m_angleView/m_aspectRatio);
	#if 1
		// It is not the best way to create the ray but it work . (My knowlege is not enought now ...)
		mat4 inverse = m_matrix.invert();
		vec3 screenOffset(0,0,-1);
		screenOffset = screenOffset.rotate(vec3(0,1,0), cameraAngleOffset.x());
		screenOffset = screenOffset.rotate(vec3(1,0,0), -cameraAngleOffset.y());
		vec2 angles = tansformPositionToAngle(-getViewVector());
		screenOffset = screenOffset.rotate(vec3(1,0,0), -M_PI*0.5f + angles.y());
		screenOffset = screenOffset.rotate(vec3(0,0,1), angles.x() - M_PI/2.0f);
		vec3 direction = screenOffset;
	#else
		// lA PROJECTION TOURNE EN FONCTION DE L'ANGLE
		mat4 inverse = m_matrix.invert();
		vec3 screenOffset(0,0,-1);
		//screenOffset = getViewVector();
		screenOffset = screenOffset.rotate(vec3(0,1,0), cameraAngleOffset.x());
		screenOffset = screenOffset.rotate(vec3(1,0,0), cameraAngleOffset.y());
		vec3 direction = inverse*screenOffset;
	#endif
	direction.safeNormalize();
	ege::Ray out(m_eye, direction);
	EGE_VERBOSE("return ray : " << out);
	return out;
}

void ege::camera::View::drawDebug(const std::shared_ptr<ewol::resource::Colored3DObject>& _draw, const std::shared_ptr<ege::Camera>& _camera) {
	mat4 mat;
	mat.identity();
	vec2 angles = tansformPositionToAngle(-getViewVector());
	mat.rotate(vec3(1,0,0), -M_PI*0.5f + angles.y());
	mat.rotate(vec3(0,0,1), angles.x() - M_PI/2.0f);
	mat.translate(m_eye);
	_draw->drawSquare(vec3(5,5,5), mat, etk::Color<float>(0.0f, 0.0f, 1.0f, 1.0f));
}
