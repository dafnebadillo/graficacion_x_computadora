#version 330 core

    layout(location = 0) in vec3 position;
    layout(location = 1) in vec3 normal;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    out vec3 FragPos;
    out vec3 Normal;
    out vec2 TexCoord;

    void main()
    {
        FragPos = vec3(model * vec4(position, 1.0));
        Normal = mat3(model) * normal;

        TexCoord = position.yz * 4.0;
        //TexCoord = vec2(atan(position.z, position.x) / 6.28318 + 0.5, position.y + 0.5);

        gl_Position = projection * view * vec4(FragPos, 1.0);
    }
