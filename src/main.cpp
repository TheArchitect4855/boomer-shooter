#define GLFW_INCLUDE_NONE

#include <glm/ext/quaternion_trigonometric.hpp>
#include "common/array.hpp"
#include "common/ecs/world.hpp"
#include "common/environment.hpp"
#include "common/log.hpp"
#include "common/gltf_loader.hpp"
#include "engine/context.hpp"
#include "engine/engine.hpp"
#include "game/player.hpp"

int main(int argc, char** argv) {
	ContextConfiguration cfg = ContextConfiguration{"Boomer Shooter", Size{640, 480}};
	Context context = Context(cfg);
	context.enableVSync();

	World world;
	gltf::GltfLoader loader = gltf::GltfLoader::loadBinary("assets/level.glb");
	loader.Scenes[0].load(world);

	TRACE("loading skybox");
	Array<const char*> envFiles = Array<const char*>(6);
	envFiles[0] = "assets/environment.png";
	envFiles[1] = "assets/environment.png";
	envFiles[2] = "assets/environment.png";
	envFiles[3] = "assets/environment.png";
	envFiles[4] = "assets/environment.png";
	envFiles[5] = "assets/environment.png";
	Array<const char*> skyboxFiles = Array<const char*>(6);
	skyboxFiles[0] = "assets/skybox.png";
	skyboxFiles[1] = "assets/skybox.png";
	skyboxFiles[2] = "assets/skybox.png";
	skyboxFiles[3] = "assets/skybox.png";
	skyboxFiles[4] = "assets/skybox.png";
	skyboxFiles[5] = "assets/skybox.png";
	world.Environment = Environment(envFiles, skyboxFiles, 2);
	TRACE("skybox loaded");

	world.Camera.Position = glm::vec3(5, 2, 5);
	world.Camera.Rotation = glm::angleAxis(2.36f, glm::vec3(0, 1, 0));
	Engine engine(context, world);
	Player player(world);
	engine.addSystem(SystemSchedule::render, player);
	engine.run();
}
