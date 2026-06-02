#version 330 core

out vec4 FragColor;
in vec2 vUV;

// agregar el box.obj 
struct Triangle
{
    vec3 v0;
    vec3 v1;
    vec3 v2;
};

#define MAX_TRIANGLES 32

uniform int uNumTriangles;
uniform Triangle uTriangles[MAX_TRIANGLES];

uniform bool uBlueLightEnabled; // se agrega esto apra encender y apagar las luces

uniform int uCameraMode; // cambiar orientaciones

uniform bool uGlassEnabled; //activar o desactivar transparencia
uniform bool uMetalEnabled; // activar desactivar el metal

float intersectSphere(vec3 ro, vec3 rd, vec3 center, float radius)
{
    vec3 oc = ro - center;

    float a = dot(rd, rd);
    float b = 2.0 * dot(oc, rd);
    float c = dot(oc, oc) - radius * radius;

    float disc = b * b - 4.0 * a * c;

    if (disc < 0.0)
        return -1.0;

    float t1 = (-b - sqrt(disc)) / (2.0 * a);
    float t2 = (-b + sqrt(disc)) / (2.0 * a);

    if (t1 > 0.001) return t1;
    if (t2 > 0.001) return t2;

    return -1.0;
}

float intersectTriangle(vec3 ro, vec3 rd, vec3 v0, vec3 v1, vec3 v2)
{
    vec3 e1 = v1 - v0;
    vec3 e2 = v2 - v0;

    vec3 pvec = cross(rd, e2);
    float det = dot(e1, pvec);

    if(abs(det) < 0.000001)
        return -1.0;

    float invDet = 1.0 / det;

    vec3 tvec = ro - v0;
    float u = dot(tvec, pvec) * invDet;

    if(u < 0.0 || u > 1.0)
        return -1.0;

    vec3 qvec = cross(tvec, e1);
    float v = dot(rd, qvec) * invDet;

    if(v < 0.0 || u + v > 1.0)
        return -1.0;

    float t = dot(e2, qvec) * invDet;

    if(t > 0.001)
        return t;

    return -1.0;
}

float intersectPlane(vec3 ro, vec3 rd, vec3 planePoint, vec3 planeNormal)
{
    float denom = dot(rd, planeNormal);

    if(abs(denom) < 0.0001)
        return -1.0;

    float t = dot(planePoint - ro, planeNormal) / denom;

    if(t > 0.001)
        return t;

    return -1.0;
}

// sombras 
// No uso el OBJ para sombras porque genera manchas grandes
// en la escena tipo Cornell Box.
bool isInShadow(vec3 P, vec3 N, vec3 lightPos)
{
    vec3 shadowOrigin = P + N * 0.1;
    vec3 shadowDir = normalize(lightPos - shadowOrigin);
    float lightDist = length(lightPos - shadowOrigin);

    // Esfera izquierda
    float s1 = intersectSphere(
        shadowOrigin,
        shadowDir,
        vec3(-0.55, 0.0, 0.0),
        0.45
    );

    if(s1 > 0.0 && s1 < lightDist)
        return true;

    // Esfera derecha
    float s2 = intersectSphere(
        shadowOrigin,
        shadowDir,
        vec3(0.55, 0.0, 0.0),
        0.45
    );

    if(s2 > 0.0 && s2 < lightDist)
        return true;

    // OBJ
    //for(int i = 0; i < MAX_TRIANGLES; i++)
    //{
        //if(i >= uNumTriangles)
            //break;

        //float st = intersectTriangle(
            //shadowOrigin,
            //shadowDir,
            //uTriangles[i].v0,
            //uTriangles[i].v1,
            //uTriangles[i].v2
        //);

        //if(st > 0.0 && st < lightDist)
            //return true;
    //}

    return false;
}

vec3 shade(vec3 P, vec3 N, vec3 baseColor)
{
    vec3 whiteLightPos = vec3(-3.0, 3.0, 2.0);
    vec3 L1 = normalize(whiteLightPos - P);
    float diff1 = max(dot(N, L1), 0.0);

    if(isInShadow(P, N, whiteLightPos))
        diff1 = 0.0;

    vec3 whiteContribution = vec3(1.0) * diff1 * 0.35;

    vec3 blueLightPos = vec3(2.0, 2.0, 2.0);
    vec3 L2 = normalize(blueLightPos - P);
    float diff2 = max(dot(N, L2), 0.0);

    if(isInShadow(P, N, blueLightPos))
        diff2 = 0.0;

    vec3 blueContribution = vec3(0.0);

    if(uBlueLightEnabled)
    {
        blueContribution = vec3(0.4, 0.6, 1.0) * diff2;
    }

    vec3 ambient = baseColor * 0.20;

    return ambient
         + baseColor * whiteContribution
         + baseColor * blueContribution;
}

vec3 skyColor(vec3 rd)
{
    float t = 0.5 * (rd.y + 1.0);

    return mix(
        vec3(0.05, 0.05, 0.08),
        vec3(0.4, 0.5, 0.8),
        t
    );
}

// funcion para kimitar los planos

// funcion para un brilo especualr Phong (arreglar estilo metalico de la esfera)

vec3 metallicShade(vec3 P, vec3 N, vec3 baseColor, vec3 ro)
{
    vec3 lightPos = vec3(-3.0, 3.0, 2.0);
    vec3 L = normalize(lightPos - P);
    vec3 V = normalize(ro - P);
    vec3 R = reflect(-L, N);

    float diff = max(dot(N, L), 0.0);
    float spec = pow(max(dot(V, R), 0.0), 80.0);

    vec3 ambient = 0.08 * baseColor;
    vec3 diffuse = 0.35 * diff * baseColor;
    vec3 specular = 1.2 * spec * vec3(1.0, 0.95, 0.75);

    vec3 reflection = 0.45 * skyColor(reflect(normalize(P - ro), N));

    return ambient + diffuse + specular + reflection;
}

// textura de ladrillo

vec3 brickTexture(vec3 P)
{
    float scaleX = 5.0;
    float scaleY = 8.0;

    vec2 uv = vec2(P.z * scaleX, P.y * scaleY);

    // Desfase por filas
    float row = floor(uv.y);
    uv.x += mod(row, 2.0) * 0.5;

    vec2 f = fract(uv);

    float mortar = 0.06;

    float brickMask =
        step(mortar, f.x) *
        step(mortar, f.y);

    vec3 mortarColor = vec3(0.12, 0.10, 0.10);
    vec3 brickColor  = vec3(0.75, 0.18, 0.12);

    return mix(mortarColor, brickColor, brickMask);
}

// aqui se pone el piso de ajedrex limitado al cornell box
vec3 checkerTexture(vec3 P)
{
    float checker = mod(floor(P.x * 4.0) + floor(P.z * 4.0), 2.0);

    if(checker < 1.0)
        return vec3(0.75);
    else
        return vec3(0.35);
}

void main()
{
    vec2 uv = vUV * 2.0 - 1.0;
    uv.x *= 960.0 / 496.0;

    vec3 ro;
    vec3 rd;

    if(uCameraMode == 0)
    {
        ro = vec3(0.0, 0.0, 2.5);
        rd = normalize(vec3(uv.x, uv.y, -1.8));
    }
    else if(uCameraMode == 1)
    {
        ro = vec3(-2.5, 0.2, 1.5);
        rd = normalize(vec3(uv.x + 0.8, uv.y, -1.8));
    }
    else
    {
        ro = vec3(2.5, 0.2, 1.5);
        rd = normalize(vec3(uv.x - 0.8, uv.y, -1.8));
    }

    int hitObject = -1;
    float closestT = 999999.0;

    vec3 hitCenter = vec3(0.0);
    vec3 hitColor = vec3(0.0);

    bool hit = false;

    // Esfera izquierda: metal
    vec3 center1 = vec3(-0.55, 0.0, 0.0);
    float radius1 = 0.45;
    float t1 = intersectSphere(ro, rd, center1, radius1);

    if(t1 > 0.0 && t1 < closestT)
    {
        closestT = t1;
        hitCenter = center1;
        hitColor = vec3(0.9, 0.75, 0.35);
        hitObject = 0;
        hit = true;
    }

    // Esfera derecha: vidrio 
    vec3 center2 = vec3(0.55, 0.0, 0.0);
    float radius2 = 0.45;
    float t2 = intersectSphere(ro, rd, center2, radius2);

    if(t2 > 0.0 && t2 < closestT)
    {
        closestT = t2;
        hitCenter = center2;
        hitColor = vec3(0.35, 0.75, 1.0);
        hitObject = 1;
        hit = true;
    }

    // obj
    for(int i = 0; i < MAX_TRIANGLES; i++)
    {
        if(i >= uNumTriangles)
            break;

        float tobj = intersectTriangle(
            ro,
            rd,
            uTriangles[i].v0,
            uTriangles[i].v1,
            uTriangles[i].v2
        );

        if(tobj > 0.0 && tobj < closestT)
        {
            closestT = tobj;
            hitColor = vec3(0.8, 0.25, 0.25);
            hitObject = 3;
            hit = true;
        }
    }

    // Piso ajedrez
    //vec3 planePoint = vec3(0.0, -0.5, 0.0);
    //vec3 planeNormal = vec3(0.0, 1.0, 0.0);

    //float tp = intersectPlane(ro, rd, planePoint, planeNormal);

    //if(tp > 0.0 && tp < closestT)
    //{
       // closestT = tp;
        //hitColor = vec3(0.7);
        //hitObject = 2;
        //hit = true;
    //}


    // CONSTRUIR EL CORNELL BOX ANALITICAMENTE

    // Piso

    float tFloor = intersectPlane(ro, rd, vec3(0.0, -0.5, 0.0), vec3(0.0, 1.0, 0.0));
    //vec3 pFloor = ro + tFloor * rd;

    if(tFloor > 0.0 && tFloor < closestT)
    {
        closestT = tFloor;

        vec3 pFloor = ro + tFloor * rd;
        hitColor = checkerTexture(pFloor);

        hitObject = 2;
        hit = true;
    }

    // Techo
    float tCeil = intersectPlane(ro, rd, vec3(0.0, 1.25, 0.0), vec3(0.0, -1.0, 0.0));
    //vec3 pTecho = ro + tCeil * rd;
    if(tCeil > 0.0 && tCeil < closestT)
    {
        closestT = tCeil;
        hitColor = vec3(0.70);
        hitObject = 4;
        hit = true;
    }

    // Pared fondo
    float tBack = intersectPlane(ro, rd, vec3(0.0, 0.0, -1.6), vec3(0.0, 0.0, 1.0));
    //vec3 pFondo = ro + tBack * rd;
    if(tBack > 0.0 && tBack < closestT)
    {
        closestT = tBack;
        hitColor = vec3(0.70);
        hitObject = 5;
        hit = true;
    }

    // Pared izquierda roja
    float tLeft = intersectPlane(ro, rd, vec3(-1.4, 0.0, 0.0), vec3(1.0, 0.0, 0.0));
    //vec3 pLeft = ro + tLeft * rd;
    if(tLeft > 0.0 && tLeft < closestT)
    {
        closestT = tLeft;
        //hitColor = vec3(0.8, 0.25, 0.25);
        vec3 pLeft = ro + tLeft * rd;
        hitColor = brickTexture(pLeft);

        hitObject = 6;
        hit = true;
    }

    // Pared derecha azul
    float tRight = intersectPlane(ro, rd, vec3(1.4, 0.0, 0.0), vec3(-1.0, 0.0, 0.0));
    //vec3 pRight = ro + tRight * rd;
    if(tRight > 0.0 && tRight < closestT)
    {
        closestT = tRight;
        hitColor = vec3(0.25, 0.25, 0.8);
        hitObject = 7;
        hit = true;
    }
    
    if(hit)
    {
        vec3 P = ro + closestT * rd;

        vec3 N;

        if(hitObject == 2)
            N = vec3(0.0, 1.0, 0.0);
        else if(hitObject == 4)
            N = vec3(0.0, -1.0, 0.0);
        else if(hitObject == 5)
            N = vec3(0.0, 0.0, 1.0);
        else if(hitObject == 6)
            N = vec3(1.0, 0.0, 0.0);
        else if(hitObject == 7)
            N = vec3(-1.0, 0.0, 0.0);
        else if(hitObject == 3)
            N = vec3(0.0, 1.0, 0.0);
        else
            N = normalize(P - hitCenter);

        vec3 color;

        if(hitObject == 0)
        {
            if(uMetalEnabled)
            {
                color = metallicShade(P, N, hitColor, ro);
            }else
            {
                color = shade(P, N, hitColor);
            }
            
        }
        else if(hitObject == 1)
        {
            if(uGlassEnabled)
            {
                vec3 T = refract(rd, N, 1.0 / 1.5);
                vec3 glassColor = skyColor(T);
                color = 0.25 * shade(P, N, hitColor) + 0.75 * glassColor;

            }else
            {
                color = shade(P, N, hitColor);
            }
        }
        else
        {
            color = shade(P, N, hitColor);
        }

        FragColor = vec4(color, 1.0);
    }
    else
    {
        FragColor = vec4(0.02, 0.02, 0.03, 1.0);
    }
}