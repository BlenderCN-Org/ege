Ege
====

`Ege` (Ewol Game Engine) is a FREE software.

Install blender exporter emf file
=================================

get blender version

		blender --version

create user addon directory: (replace 2.xx with the blender version)

	mkdir -p ~/.config/blender/2.xx/scripts/addons/
	cd ~/.config/blender/2.xx/scripts/addons/
	ln -s EGE_FOLDER/blender/io_scene_emf .

- Launch Blender
- File -> User Preferences
- Select "Addons" tab
- Enable "Import-export: Ewol Mesh file format emf"


License (APACHE v2.0)
=====================
Copyright ege Edouard DUPIN

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.