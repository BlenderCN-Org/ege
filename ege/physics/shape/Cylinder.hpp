/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physics/shape/Shape.hpp>


namespace ege {
	namespace physics {
		namespace shape {
			class Cylinder : public ege::physics::Shape {
				public:
					Cylinder() :
					  m_radius(1.0f),
					  m_size(1.0f) {};
					virtual ~Cylinder() {};
				public:
					virtual bool parse(const char* _line);
					virtual void display() {};
				public:
					virtual enum ege::physics::Shape::type getType() const {
						return ege::physics::Shape::type::cylinder;
					};
				private:
					float m_radius;
				public:
					float getRadius() const {
						return m_radius;
					}
					void setRadius(float _radius) {
						m_radius = _radius;
					}
				private:
					float m_size;
				public:
					float getSize() const {
						return m_size;
					}
					void setSize(float _size) {
						m_size = _size;
					}
				public:
					virtual const ege::physics::shape::Cylinder* toCylinder() const {
						return this;
					};
					virtual ege::physics::shape::Cylinder* toCylinder() {
						return this;
					};
			};
		}
	}
}

