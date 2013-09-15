/**
 * @author Edouard DUPIN
 * 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * 
 * @license BSD v3 (see license file)
 */

#ifndef __EGE_RESOURCE_PARTICULE_MESH_H__
#define __EGE_RESOURCE_PARTICULE_MESH_H__

#include <ewol/resources/Mesh.h>

namespace ege
{
	namespace resource
	{
		class ParticuleMesh : public ewol::Mesh
		{
			protected:
				int32_t        m_GLMainColor;
			protected:
				ParticuleMesh(const etk::UString& _fileName, const etk::UString& _shaderName);
				virtual ~ParticuleMesh(void);
			public:
				virtual const char* GetType(void) { return "ege::resource::ParticuleMesh"; };
				virtual void Draw(mat4& _positionMatrix, const etk::Color<float>& _mainColor, bool _enableDepthTest=true, bool _enableDepthUpdate=true);
			public:
				/**
				 * @brief Keep the resource pointer.
				 * @note Never free this pointer by your own...
				 * @param[in] _filename Name of the ewol mesh file.
				 * @return pointer on the resource or NULL if an error occured.
				 */
				static ege::resource::ParticuleMesh* Keep(const etk::UString& _meshName, const etk::UString& _shaderName="DATA:ParticuleMesh.prog");
				/**
				 * @brief Release the keeped resources
				 * @param[in,out] reference on the object pointer
				 */
				static void Release(ege::resource::ParticuleMesh*& _object);
		};
	};
};


#endif