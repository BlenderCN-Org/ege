/** @file
 * @author Edouard DUPIN
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <ege/debug.hpp>
#include <ege/resource/Mesh.hpp>

ememory::SharedPtr<ege::resource::Mesh> ege::resource::Mesh::createCapsule(float _radius, float _size, const std::string& _materialName, const etk::Color<float>& _color, int32_t _lats, int32_t _longs) {
	EGE_VERBOSE(" create a capsule _size=" << _size << " _materialName=" << _materialName << " _color=" << _color);
	ememory::SharedPtr<ege::resource::Mesh> out = ege::resource::Mesh::create("---", "DATA:color3.prog");
	if (out != nullptr) {
		ememory::SharedPtr<ege::Material> material = ememory::makeShared<ege::Material>();
		// set the element material properties :
		material->setAmbientFactor(vec4(1,1,1,1));
		material->setDiffuseFactor(vec4(0,0,0,1));
		material->setSpecularFactor(vec4(0,0,0,1));
		material->setShininess(1);
		material->setRenderMode(gale::openGL::renderMode::triangle);
		out->addMaterial(_materialName, material);
		
		out->addFaceIndexing(_materialName);
		
		// center to border (TOP)
		float offset = _size*0.5f;
		for(int32_t iii=_lats/2+1; iii<=_lats; ++iii) {
			float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
			float y0  = _radius*sin(lat0);
			float yr0 = _radius*cos(lat0);
			
			float lat1 = M_PI * (-0.5f + float(iii) / _lats);
			float y1 = _radius*sin(lat1);
			float yr1 = _radius*cos(lat1);
			
			for(int32_t jjj=0; jjj<_longs; ++jjj) {
				float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
				float x = cos(lng);
				float z = sin(lng);
				vec3 v1 = vec3(x * yr1, y1+offset, z * yr1);
				vec3 v4 = vec3(x * yr0, y0+offset, z * yr0);
				
				lng = 2 * M_PI * float(jjj) / _longs;
				x = cos(lng);
				z = sin(lng);
				vec3 v2 = vec3(x * yr1, y1+offset, z * yr1);
				vec3 v3 = vec3(x * yr0, y0+offset, z * yr0);
				out->addQuad(_materialName, v1, v2, v3, v4, _color);
			}
		}
		// Cylinder
		for(int32_t jjj=0; jjj<_longs; ++jjj) {
			float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
			
			float y = _size*0.5f;
			
			float x = cos(lng)*_radius;
			float z = sin(lng)*_radius;
			vec3 v2  = vec3(x, y, z);
			vec3 v2b = vec3(x, -y, z);
			
			lng = 2.0f * M_PI * float(jjj) / _longs;
			x = cos(lng)*_radius;
			z = sin(lng)*_radius;
			vec3 v3  = vec3(x, y, z);
			vec3 v3b = vec3(x, -y, z);
			
			out->addQuad(_materialName, v2, v3, v3b, v2b, _color);
		}
		// center to border (BUTTOM)
		offset = -_size*0.5f;
		for(int32_t iii=0; iii<=_lats/2; ++iii) {
			float lat0 = M_PI * (-0.5f + float(iii - 1) / _lats);
			float y0  = _radius*sin(lat0);
			float yr0 = _radius*cos(lat0);
			
			float lat1 = M_PI * (-0.5f + float(iii) / _lats);
			float y1 = _radius*sin(lat1);
			float yr1 = _radius*cos(lat1);
			
			for(int32_t jjj=0; jjj<_longs; ++jjj) {
				float lng = 2.0f * M_PI * float(jjj - 1) / _longs;
				float x = cos(lng);
				float z = sin(lng);
				vec3 v1 = vec3(x * yr1, y1+offset, z * yr1);
				vec3 v4 = vec3(x * yr0, y0+offset, z * yr0);
				
				lng = 2 * M_PI * float(jjj) / _longs;
				x = cos(lng);
				z = sin(lng);
				vec3 v2 = vec3(x * yr1, y1+offset, z * yr1);
				vec3 v3 = vec3(x * yr0, y0+offset, z * yr0);
				out->addQuad(_materialName, v1, v2, v3, v4, _color);
			}
		}
		out->setNormalMode(ege::resource::Mesh::normalMode::face);
		out->calculateNormaleFace(_materialName);
		// generate the VBO
		out->generateVBO();
	} else {
		EGE_ERROR("can not create the basic mesh interface");
	}
	return out;
}