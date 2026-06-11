#include "mesh.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

Mesh::Mesh()
    : VAO(0),
      VBO(0),
      vertexCount(0)
{
}

int Mesh::fixIndex(int index, int size)
{
    if (index > 0)
        return index - 1;

    if (index < 0)
        return size + index;

    return 0;
}

bool Mesh::loadOBJ(const QString &filename, QOpenGLFunctions_3_3_Core *gl)
{
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No se pudo abrir el OBJ:" << filename;
        return false;
    }

    QTextStream in(&file);

    std::vector<Vec3> positions;
    std::vector<Vec3> normals;
    std::vector<Vertex> vertices;

    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if (line.isEmpty() || line.startsWith("#"))
            continue;

        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if (parts[0] == "v")
        {
            positions.push_back({
                parts[1].toFloat(),
                parts[2].toFloat(),
                parts[3].toFloat()
            });
        }
        else if (parts[0] == "vn")
        {
            normals.push_back({
                parts[1].toFloat(),
                parts[2].toFloat(),
                parts[3].toFloat()
            });
        }
        else if (parts[0] == "f")
        {
            for (int i = 1; i <= 3; i++)
            {
                QStringList idx = parts[i].split("//");

                int vIndex = fixIndex(idx[0].toInt(), positions.size());
                int nIndex = fixIndex(idx[1].toInt(), normals.size());

                Vertex vertex;
                vertex.position = positions[vIndex];
                vertex.normal = normals[nIndex];

                vertices.push_back(vertex);
            }
        }
    }

    vertexCount = static_cast<int>(vertices.size());

    qDebug() << "Dragon cargado. Vertices:" << vertexCount;

    gl->glGenVertexArrays(1, &VAO);
    gl->glGenBuffers(1, &VBO);

    gl->glBindVertexArray(VAO);

    gl->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    gl->glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vertex),
        vertices.data(),
        GL_STATIC_DRAW
    );

    gl->glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void *>(0)
    );
    gl->glEnableVertexAttribArray(0);

    gl->glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void *>(sizeof(Vec3))
    );
    gl->glEnableVertexAttribArray(1);

    gl->glBindVertexArray(0);

    return true;
}

void Mesh::draw(QOpenGLFunctions_3_3_Core *gl)
{
    gl->glBindVertexArray(VAO);
    gl->glDrawArrays(GL_TRIANGLES, 0, vertexCount);
    gl->glBindVertexArray(0);
}