#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <tiny_gltf.h>
#include "array.hpp"
#include "ecs/world.hpp"
#include "../engine/graphics/mesh.hpp"
#include "../engine/graphics/texture.hpp"

namespace gltf {
	class Node {
		Array<Mesh> m_Mesh;
		glm::mat4 m_Transform;
		// TODO: Camera
		// TODO: Skinning
		// TODO: Lights
		// TODO: Audio emitters
		// TODO: Children
		// TODO: Morph weights
		bool m_HasMesh, m_HasTransform;
	public:
		void setMesh(const Array<Mesh>& mesh);
		void setTransform(const glm::mat4& transform);
		bool tryGetMesh(Array<Mesh>& mesh) const;
		bool tryGetTransform(glm::mat4& transform) const;
	};

	struct Scene {
		Array<Node> Nodes;
		// TODO: Audio emitters

		void load(World& world);
	};

	class GltfLoader {
		Array<unsigned int> m_Buffers;
		tinygltf::Model& m_Model;
		void loadAttribute(unsigned int attributeIndex, unsigned int attributeSize, unsigned int accessorIndex);
		Texture loadTexture(unsigned int textureIndex, unsigned int internalFormat);
	public:
		Array<Mesh> Meshes;
		Array<Scene> Scenes;
		GltfLoader(tinygltf::Model& model);
		GltfLoader(const GltfLoader& copy) = delete;
		~GltfLoader();

		GltfLoader& operator=(const GltfLoader& copy) = delete;

		static GltfLoader loadBinary(const char* path);
	};
};
