/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ege/resource/tools/Face.hpp>
#include <ege/resource/tools/FaceIndexing.hpp>
#include <etk/Hash.hpp>
#include <ege/Material.hpp>

namespace ege {
	namespace isoSphere {
		void create(etk::Hash<ememory::SharedPtr<ege::Material>>& _materials, etk::Hash<FaceIndexing>& _listFaces, etk::Vector<vec3>& _listVertex, etk::Vector<vec2>& _listUV,
		            const etk::String& _materialName, int32_t _recursionLevel);
	}
}

