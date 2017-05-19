/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/debug.hpp>

#include <ege/Component.hpp>
#include <etk/math/Transform3D.hpp>
#include <esignal/Signal.hpp>
#include <ephysics/reactphysics3d.h>
#include <ege/resource/Mesh.hpp>

namespace ege {
	class Environement;
	namespace physics {
		class Engine;
		class Component : public ege::Component {
			public:
				esignal::Signal<etk::Transform3D> signalPosition;
			protected:
				ememory::SharedPtr<ege::physics::Engine> m_engine;
				rp3d::RigidBody* m_rigidBody;
			public:
				/**
				 * @brief Create a basic position component (no orientation and position (0,0,0))
				 */
				Component(ememory::SharedPtr<ege::Environement> _env);
				/**
				 * @brief Create a basic position component
				 * @param[in] _transform transformation of the position
				 */
				Component(ememory::SharedPtr<ege::Environement> _env, const etk::Transform3D& _transform);
				~Component();
			public:
				virtual const std::string& getType() const;
				/**
				 * @brief set a new transformation
				 * @param[in] _transform transformation of the position
				 */
				void setTransform(const etk::Transform3D& _transform);
				/**
				 * @brief set a new transformation
				 * @return Transformation of the position
				 */
				etk::Transform3D getTransform() const;
			protected:
				std::vector<ememory::SharedPtr<ege::PhysicsShape>> m_shape; //!< collision shape module ... (independent of bullet lib)
			public:
				const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& getShape() const;
				void setShape(const std::vector<ememory::SharedPtr<ege::PhysicsShape>>& _prop);
				void addShape(const ememory::SharedPtr<ege::PhysicsShape>& _shape);
				void generate();
		};
	}
}