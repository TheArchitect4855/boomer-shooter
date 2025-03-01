#include "gltf_loader.hpp"

#include <glad/gl.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "list.hpp"
#include "log.hpp"
#include "../engine/graphics/material.hpp"

using namespace gltf;

void Scene::load(World& world) {
	Model model;
	Transform transform;
	for (unsigned int i = 0; i < Nodes.length(); i += 1) {
		Entity entity = world.spawn();
		if (Nodes[i].tryGetMesh(model.Meshes)) world.addComponent(entity, model);

		if (Nodes[i].tryGetTransform(transform.Matrix)) world.addComponent(entity, transform);
		else world.addComponent(entity, Transform{glm::mat4(1)});
	}
}

void Node::setMesh(const Array<Mesh>& mesh) {
	m_HasMesh = true;
	m_Mesh = mesh;
}

void Node::setTransform(const glm::mat4& transform) {
	m_HasTransform = true;
	m_Transform = transform;
}

bool Node::tryGetMesh(Array<Mesh>& mesh) const {
	mesh = m_Mesh;
	return m_HasMesh;
}

bool Node::tryGetTransform(glm::mat4& transform) const {
	transform = m_Transform;
	return m_HasTransform;
}

GltfLoader::GltfLoader(tinygltf::Model& model) : m_Model{model} {
	// 1. Load buffer views
	m_Buffers = Array<unsigned int>(model.bufferViews.size());
	glGenBuffers(m_Buffers.length(), m_Buffers.elements());
	for (unsigned int i = 0; i < m_Buffers.length(); i += 1) {
		tinygltf::BufferView& view = model.bufferViews[i];
		if (view.target == 0) {
			WARN("skipping buffer %d because it has a target of 0", i);
			continue;
		}

		TRACE("loading buffer view %d (%s)", i, view.name.c_str());
		ASSERT(view.byteStride == 0); // Sparse m_Buffers not supported

		const unsigned char* buffer = model.buffers.at(view.buffer).data.data();
		glBindBuffer(view.target, m_Buffers[i]);
		glBufferData(view.target, view.byteLength, buffer + view.byteOffset, GL_STATIC_DRAW);
	}

	TRACE("loaded buffer views");

	// 2. Load materials
	// TODO: Optimize to remove possible texture duplication?
	Array<Material> materials(model.materials.size());
	for (unsigned int i = 0; i < materials.length(); i += 1) {
		tinygltf::Material& material = model.materials[i];
		TRACE("loading material %d (%s)", i, material.name.c_str());

		Material& mat = materials[i];
		tinygltf::PbrMetallicRoughness& pbr = material.pbrMetallicRoughness;
		ASSERT(pbr.baseColorTexture.texCoord == 0); // Multiple UV channels not supported

		if (pbr.baseColorTexture.index >= 0) mat.Albedo = loadTexture(pbr.baseColorTexture.index, GL_SRGB);
		mat.BaseColor = Color{(float)pbr.baseColorFactor.at(0), (float)pbr.baseColorFactor.at(1), (float)pbr.baseColorFactor.at(2)};
	}

	TRACE("loaded materials");

	// 3. Load meshes
	List<Mesh> meshList;
	Array<Array<Mesh>> meshes = Array<Array<Mesh>>(model.meshes.size());
	for (unsigned int i = 0; i < meshes.length(); i += 1) {
		tinygltf::Mesh& mesh = model.meshes[i];
		TRACE("loading mesh %d (%s)", i, mesh.name.c_str());
		ASSERT(mesh.weights.size() == 0); // Weights not supported

		Array<unsigned int> primitives = Array<unsigned int>(mesh.primitives.size());
		TRACE("loading %d primitives", primitives.length());
		glGenVertexArrays(primitives.length(), primitives.elements());

		Array<Mesh> primitiveMeshes = Array<Mesh>(primitives.length());
		for (unsigned int j = 0; j < primitives.length(); j += 1) {
			tinygltf::Primitive& p = mesh.primitives[j];
			ASSERT(p.targets.size() == 0); // Morph targets not supported

			glBindVertexArray(primitives[j]);
			loadAttribute(0, 3, p.attributes.at("POSITION"));
			if (p.attributes.contains("NORMAL")) loadAttribute(1, 3, p.attributes["NORMAL"]);
			if (p.attributes.contains("TEXCOORD_0")) loadAttribute(2, 2, p.attributes.at("TEXCOORD_0"));
			// TODO: Load other attributes

			// Bind element buffer
			tinygltf::Accessor& indexAccessor = model.accessors.at(p.indices);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[indexAccessor.bufferView]);

			Mesh mesh(primitives[j], indexAccessor.count, indexAccessor.byteOffset, indexAccessor.componentType, p.mode);
			if (p.material >= 0) mesh.Material = materials[p.material];
			primitiveMeshes[j] = mesh;
			meshList.push(mesh);
		}

		meshes[i] = primitiveMeshes;
	}

	Meshes = meshList.toArray();
	TRACE("loaded meshes");

	// 4. Load nodes
	Array<Node> nodes(model.nodes.size());
	for (unsigned int i = 0; i < nodes.length(); i += 1) {
		tinygltf::Node& node = model.nodes[i];
		TRACE("loading node %d (%s)", i, node.name.c_str());

		Node& n = nodes[i];
		if (node.mesh >= 0) n.setMesh(meshes[node.mesh]);
		if (node.matrix.size() > 0) {
			// Set transform from matrix
			glm::mat4 transform;
			for (unsigned int i = 0; i < 16; i += 1) transform[i / 4][i % 4] = node.matrix.at(i);
			n.setTransform(transform);
		} else if (node.rotation.size() > 0 || node.scale.size() > 0 || node.translation.size() > 0) {
			// Attempt to set transform from components
			glm::mat4 transform(1.0);
			std::vector<double>& v = node.translation;
			if (v.size() > 0) transform = glm::translate(transform, glm::vec3(v.at(0), v.at(1), v.at(2)));

			v = node.rotation;
			if (v.size() > 0) transform *= glm::mat4_cast(glm::quat(v.at(3), v.at(0), v.at(1), v.at(2)));

			v = node.scale;
			if (v.size() > 0) transform *= glm::scale(transform, glm::vec3(v.at(0), v.at(1), v.at(2)));

			n.setTransform(transform);
		}
	}

	TRACE("loaded nodes");

	// 5. Load scenes
	Scenes = Array<Scene>(model.scenes.size());
	for (unsigned int i = 0; i < Scenes.length(); i += 1) {
		tinygltf::Scene& scene = model.scenes[i];
		TRACE("loading scene %d (%s)", i, scene.name.c_str());

		Array<Node> sceneNodes(scene.nodes.size());
		for (unsigned int j = 0; j < sceneNodes.length(); j += 1) sceneNodes[j] = nodes[scene.nodes.at(j)];
		Scenes[i].Nodes = sceneNodes;
	}

	TRACE("loaded scenes");
}

GltfLoader::~GltfLoader() {}

GltfLoader GltfLoader::loadBinary(const char *path) {
	using namespace gltf;

	TRACE("loading model");
	tinygltf::Model gltfModel;
	tinygltf::TinyGLTF tinyGltf;
	std::string err, warn;
	bool ok = tinyGltf.LoadBinaryFromFile(&gltfModel, &err, &warn, "assets/level.glb");
	TRACE("model loaded. OK: %d", ok);
	if (!warn.empty()) WARN("while loading GLTF: %s", warn.c_str());
	if (!err.empty()) ERROR("while loading GLTF: %s", err.c_str());
	ASSERT(ok);

	return GltfLoader(gltfModel);
}

void GltfLoader::loadAttribute(unsigned int attributeIndex, unsigned int attributeSize, unsigned int accessorIndex) {
	tinygltf::Accessor& accessor = m_Model.accessors.at(accessorIndex);
	TRACE("loading accessor %d (%s)", accessorIndex, accessor.name.c_str());

	tinygltf::BufferView& bufferView = m_Model.bufferViews.at(accessor.bufferView);
	glBindBuffer(bufferView.target, m_Buffers[accessor.bufferView]);
	glVertexAttribPointer(attributeIndex, attributeSize, accessor.componentType, accessor.normalized, bufferView.byteStride, (char*)accessor.byteOffset);
	glEnableVertexAttribArray(attributeIndex);
}

Texture GltfLoader::loadTexture(unsigned int textureIndex, unsigned int internalFormat) {
	tinygltf::Texture& texture = m_Model.textures.at(textureIndex);
	TRACE("loading texture %d (%s)", textureIndex, texture.name.c_str());

	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	TRACE("\tbound texture");

	int minFilter = GL_NEAREST_MIPMAP_LINEAR;
	int magFilter = GL_NEAREST;
	if (texture.sampler >= 0) {
		tinygltf::Sampler& sampler = m_Model.samplers.at(texture.sampler);
		if (sampler.minFilter > 0) minFilter = sampler.minFilter;
		if (sampler.magFilter > 0) magFilter = sampler.magFilter;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sampler.wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, sampler.wrapT);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	TRACE("\tset texture params");

	tinygltf::Image& source = m_Model.images.at(texture.source);
	ASSERT(source.width * source.height * 4 * (source.bits / 8) == source.image.size());
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, source.width, source.height, 0, GL_RGBA, source.pixel_type, source.image.data());
	TRACE("\tuploaded texture");

	glGenerateMipmap(GL_TEXTURE_2D);
	TRACE("\tgenerated mipmaps");
	return Texture(id);
}
