/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

namespace ege {
	class FaceIndexing {
		public:
			std::vector<Face> m_faces;
			std::vector<uint32_t> m_index;
	};
}

