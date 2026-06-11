
    #version 330 core

    in vec3 FragPos;
    in vec3 Normal;
    in vec2 TexCoord;

    out vec4 FragColor;

    uniform vec3 viewPos;

    uniform vec3 whiteLightPos;
    uniform vec3 blueLightPos;
    uniform bool blueEnabled;

    uniform vec3 matAmbient;
    uniform vec3 matDiffuse;
    uniform vec3 matSpecular;
    uniform float shininess;

    uniform bool textureEnabled;
    uniform bool useBlinn;
    uniform sampler2D texture1;

    vec3 computeLight(vec3 lightPos, vec3 lightColor)
    {
        vec3 N = normalize(Normal);
        vec3 L = normalize(lightPos - FragPos);
        vec3 V = normalize(viewPos - FragPos);

        float diff = max(dot(N, L), 0.0);

        float spec = 0.0;

        if(useBlinn)
        {
            vec3 H = normalize(L + V);
            spec = pow(max(dot(N, H), 0.0), shininess);
        }
        else
        {
            vec3 R = reflect(-L, N);
            spec = pow(max(dot(V, R), 0.0), shininess);
        }

        vec3 ambient  = matAmbient * lightColor;
        vec3 diffuse  = matDiffuse * diff * lightColor;
        vec3 specular = matSpecular * spec * lightColor;

        return ambient + diffuse + specular;
    }

    void main()
    {
        vec3 color = vec3(0.0);

        color += computeLight(
            whiteLightPos,
            vec3(1.0, 1.0, 1.0)
        );

        if(blueEnabled)
        {
            color += computeLight(
                blueLightPos,
                vec3(0.0, 0.25, 1.0)
            );
        }

        if(textureEnabled)
        {
            vec3 texColor = texture(texture1, TexCoord).rgb;
            color *= texColor;
        }

        FragColor = vec4(color, 1.0);
    }
