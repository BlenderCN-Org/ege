/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */
#include <ege/debug.h>
#include <ege/physicsShape/PhysicsSphere.h>



bool ege::PhysicsSphere::parse(const char* _line) {
	if (true == ege::PhysicsShape::parse(_line)) {
		return true;
	}
	if(0 == strncmp(_line, "radius : ", 9) ) {
		sscanf(&_line[9], "%f", &m_radius );
		EGE_VERBOSE("                radius=" << m_radius);
		return true;
	}
	return false;
}


