#ifndef MESH_H
#define MESH_H

#include "math3d.h"
#include <QString>
#include <QOpenGLFunctions_3_3_Core>
#include <vector>

struct Vertex
{
    Vec3 position;
    Vec3 normal;
};

class Mesh
{
public:
    Mesh();

    bool loadOBJ(const QString &filename, QOpenGLFunctions_3_3_Core *gl);
    void draw(QOpenGLFunctions_3_3_Core *gl);

private:
    unsigned int VAO;
    unsigned int VBO;
    int vertexCount;

    int fixIndex(int index, int size);

    int cameraMode;
};

#endif