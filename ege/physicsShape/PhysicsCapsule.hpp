/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once


#include <etk/types.hpp>
#include <ege/physicsShape/PhysicsShape.hpp>


namespace ege {
	class PhysicsCapsule : public ege::PhysicsShape {
		public:
			PhysicsCapsule() {};
			virtual ~PhysicsCapsule() {};
		public:
			virtual bool parse(const char* _line);
			virtual void display() {};
		public:
			virtual enum ege::PhysicsShape::type getType() const {
				return ege::PhysicsShape::capsule;
			};
		private:
			float m_radius;
		public:
			float getRadius() const {
				return m_radius;
			};
		private:
			float m_height;
		public:
			float getHeight() const {
				return m_height;
			};
		public:
			virtual const ege::PhysicsCapsule* toCapsule() const {
				return this;
			};
			virtual ege::PhysicsCapsule* toCapsule() {
				return this;
			};
	};
}

