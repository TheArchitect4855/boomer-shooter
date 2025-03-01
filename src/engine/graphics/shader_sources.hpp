#pragma once

#define GLSL(source) "#version 460 core\n" #source

static const char* const standardVertexSource = GLSL(
	layout (location = 0) in vec3 vertex;
	layout (location = 1) in vec3 normal;
	layout (location = 2) in vec2 uv;
	layout (std140, binding = 3) uniform Globals {
		mat4 viewProjection;
		float ambientBrightness;
	};

	uniform mat4 transform;

	out VS_OUT {
		vec3 normal;
		vec2 uv;
		float ambientBrightness;
	} vs_out;

	void main() {
		vs_out.normal = normal;
		vs_out.uv = uv;
		vs_out.ambientBrightness = ambientBrightness;
		gl_Position = viewProjection * transform * vec4(vertex, 1);
	}
);

static const char* const standardFragmentSource = GLSL(
	in VS_OUT {
		vec3 normal;
		vec2 uv;
		float ambientBrightness;
	} vs_out;

	uniform sampler2D albedo;
	uniform vec4 baseColor;
	uniform samplerCube environment;

	out vec4 color;

	void main() {
		vec4 env = texture(environment, vs_out.normal) * vs_out.ambientBrightness;
		if (env.rgb == vec3(0)) env = vec4(1); // Override black env to be white (i.e. fully lit)

		color = baseColor * texture(albedo, vs_out.uv) * env;
		color.a = 1;
	}
);

static const char* const skyboxVertexSource = GLSL(
	layout (location = 0) in vec2 vertex;

	uniform mat4 transform;

	out VS_OUT {
		vec3 direction;
	} vs_out;

	void main() {
		gl_Position = vec4(vertex, 1, 1);
		vec4 worldPos = transform * vec4(vertex, 1, 1);
		vs_out.direction = normalize(worldPos.xyz);
	}
);

static const char* const skyboxFragmentSource = GLSL(
	in VS_OUT {
		vec3 direction;
	} vs_out;

	uniform samplerCube skybox;

	out vec4 color;

	void main () {
		color = texture(skybox, vs_out.direction);
	}
);
