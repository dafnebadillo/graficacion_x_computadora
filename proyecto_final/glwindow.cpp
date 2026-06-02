#include "glwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QFile>
#include <QTextStream>
#include <QStringList>

// constructor
GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent),
      VAO(0),
      VBO(0),
      blueLightEnabled(true),
      glassEnabled(true),
      metalEnabled(true),
      cameraMode(0)
{
    setFocusPolicy(Qt::StrongFocus);
}

GLWindow::~GLWindow()
{
    makeCurrent();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    doneCurrent();
}

void GLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,

        -1.0f,  1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        2 * sizeof(float),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    bool ok;

    ok = shaderProgram.addShaderFromSourceFile(
        QOpenGLShader::Vertex,
        "/Users/dafnebadillo/proyecto_final/shaders/raytrace.vert"
    );

    if(!ok)
        qDebug() << "Error vertex shader:" << shaderProgram.log();

    ok = shaderProgram.addShaderFromSourceFile(
        QOpenGLShader::Fragment,
        "/Users/dafnebadillo/proyecto_final/shaders/raytrace.frag"
    );

    if(!ok)
        qDebug() << "Error fragment shader:" << shaderProgram.log();

    ok = shaderProgram.link();

    if(!ok)
        qDebug() << "Error linking shaders:" << shaderProgram.log();
    
    loadOBJ("/Users/dafnebadillo/proyecto_final/models/Box.obj");
    
}

void GLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWindow::paintGL()
{

    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram.bind();

    // luz
    shaderProgram.setUniformValue("uBlueLightEnabled", blueLightEnabled);
    // camara
    shaderProgram.setUniformValue("uCameraMode",cameraMode);
    // leer obj
    shaderProgram.setUniformValue("uNumTriangles", static_cast<int>(objTriangles.size()));
    // shader sofisticado
    shaderProgram.setUniformValue("uGlassEnabled", glassEnabled);
    shaderProgram.setUniformValue("uMetalEnabled", metalEnabled);


    for(int i = 0; i < objTriangles.size(); i++)
    {
        shaderProgram.setUniformValue(
            QString("uTriangles[%1].v0").arg(i).toStdString().c_str(),
            objTriangles[i].v0
        );

        shaderProgram.setUniformValue(
            QString("uTriangles[%1].v1").arg(i).toStdString().c_str(),
            objTriangles[i].v1
        );

        shaderProgram.setUniformValue(
            QString("uTriangles[%1].v2").arg(i).toStdString().c_str(),
            objTriangles[i].v2
        );
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    shaderProgram.release();
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{
    // cambiar la luz
    if(event->key() == Qt::Key_L)
    {
        blueLightEnabled = !blueLightEnabled;
        qDebug() << "Luz azul:" << blueLightEnabled;
        update();
    }
    // cambiar camara orientaciones
    if(event->key() == Qt::Key_1)
    {
        cameraMode = 0;
        qDebug() << "Camara frontal";
        update();
    }

    if(event->key() == Qt::Key_2)
    {
        cameraMode = 1;
        qDebug() << "Camara izquierda";
        update();
    }

    if(event->key() == Qt::Key_3)
    {
        cameraMode = 2;
        qDebug() << "Camara derecha";
        update();
    }
    // activar o desactivar la transparencia de la esfera
    if(event->key() == Qt::Key_V)
    {
        glassEnabled = !glassEnabled;
        qDebug() << "Transparencia:" << glassEnabled;
        update();
    }
    // activar desactivar el efecto metal de la otra esfera
    if(event->key() == Qt::Key_M)
    {
        metalEnabled = !metalEnabled;
        qDebug() << "Metal:" << metalEnabled;
        update();
    }   
}

bool GLWindow::loadOBJ(const QString& path)
{
    QFile file(path);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No se pudo abrir el OBJ:" << path;
        return false;
    }

    QVector<QVector3D> vertices;

    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString line = in.readLine().trimmed();

        if(line.isEmpty() || line.startsWith("#"))
            continue;

        QStringList parts = line.split(" ", Qt::SkipEmptyParts);

        if(parts[0] == "v")
        {
            float x = parts[1].toFloat();
            float y = parts[2].toFloat();
            float z = parts[3].toFloat();

            vertices.append(QVector3D(x, y, z));
        }
        else if(parts[0] == "f")
        {
            if(parts.size() < 4)
                continue;

            auto getIndex = [](const QString& token)
            {
                QString first = token.split("/")[0];
                return first.toInt() - 1;
            };

            int i0 = getIndex(parts[1]);
            int i1 = getIndex(parts[2]);
            int i2 = getIndex(parts[3]);

            if(i0 >= 0 && i1 >= 0 && i2 >= 0 &&
               i0 < vertices.size() &&
               i1 < vertices.size() &&
               i2 < vertices.size())
            {
                Triangle tri;
                tri.v0 = vertices[i0];
                tri.v1 = vertices[i1];
                tri.v2 = vertices[i2];

                objTriangles.append(tri);
            }

            // Si la cara tiene 4 vértices, la dividimos en dos triángulos
            if(parts.size() == 5)
            {
                int i3 = getIndex(parts[4]);

                if(i0 >= 0 && i2 >= 0 && i3 >= 0 &&
                   i0 < vertices.size() &&
                   i2 < vertices.size() &&
                   i3 < vertices.size())
                {
                    Triangle tri;
                    tri.v0 = vertices[i0];
                    tri.v1 = vertices[i2];
                    tri.v2 = vertices[i3];

                    objTriangles.append(tri);
                }
            }
        }
    }

    qDebug() << "OBJ cargado. Triangulos:" << objTriangles.size();

    return true;
}