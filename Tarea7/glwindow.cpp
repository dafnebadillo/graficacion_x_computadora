#include "glwindow.h"
#include "math3d.h"
#include <QDebug>

GLWindow::GLWindow(QWidget *parent)
    : QOpenGLWidget(parent),
      VAO(0),
      VBO(0),
      blueLightEnabled(true),
      cameraMode(1),
      useMaterialB(false),
      textureEnabled(false),
      useBlinn(false),
      textureID(0)
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

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.08f, 0.08f, 0.10f, 1.0f);

    // shaders import

    shader.addShaderFromSourceFile(QOpenGLShader::Vertex, "/Users/dafnebadillo/Tarea7/shaders/vertex.glsl");
    shader.addShaderFromSourceFile(QOpenGLShader::Fragment, "/Users/dafnebadillo/Tarea7/shaders/fragment.glsl");
    shader.link();

    dragon.loadOBJ("/Users/dafnebadillo/Tarea7/dragon.obj", this);

    // crear textura
    // rojo y azul
    //unsigned char textureData[] = {255, 255, 255,   80,  80,  80, 80,  80,  80,  255, 255, 255}; 
    unsigned char textureData[] = {200,200,200,   120,120,120, 120,120,120,   200,200,200};

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);


    float vertices[] = {0.0f,  0.8f, 0.0f, -0.8f, -0.8f, 0.0f, 0.8f, -0.8f, 0.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void GLWindow::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void GLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.bind();

    //rotacion 
    //static float angle = 0.0f;
    //angle += 0.5f;
    //Mat4 model = multiply(rotateY(angle), scale(2.0f));
    //update();

    //fijo
    Mat4 model = multiply(multiply(translate(0.0f, -0.1f, 0.0f), rotateY(90.0f)), scale(2.0f));
    //Mat4 view = lookAt(makeVec3(0.0f, 0.0f, 3.0f),makeVec3(0.0f, 0.0f, 0.0f),makeVec3(0.0f, 1.0f, 0.0f));
    Mat4 projection = perspective(45.0f, float(width()) / float(height()), 0.1f, 100.0f);

    // orientaciones de las camaras
    Mat4 view;

    if (cameraMode == 1)
    {
        view = lookAt(
            makeVec3(0.0f, 0.0f, 3.0f),
            makeVec3(0.0f, 0.0f, 0.0f),
            makeVec3(0.0f, 1.0f, 0.0f)
    );
    }
    else if (cameraMode == 2)
    {
        view = lookAt(
            makeVec3(3.0f, 0.0f, 0.0f),
            makeVec3(0.0f, 0.0f, 0.0f),
            makeVec3(0.0f, 1.0f, 0.0f)
    );
    }
    else
    {
        view = lookAt(
            makeVec3(0.0f, 3.0f, 0.01f),
            makeVec3(0.0f, 0.0f, 0.0f),
            makeVec3(0.0f, 0.0f, -1.0f)
        );
    }

    int modelLoc = glGetUniformLocation(shader.programId(), "model");
    int viewLoc = glGetUniformLocation(shader.programId(), "view");
    int projLoc = glGetUniformLocation(shader.programId(), "projection");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.m);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.m);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.m);

    //glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    //glBindVertexArray(0);

    // luces
    int whiteLoc = glGetUniformLocation(shader.programId(), "whiteLightPos");
    glUniform3f(whiteLoc, 2.0f, 2.0f, 2.0f);

    int blueLoc = glGetUniformLocation(shader.programId(), "blueLightPos");
    glUniform3f(blueLoc, -3.0f, 2.0f, 1.0f);

    int enabledLoc = glGetUniformLocation(shader.programId(), "blueEnabled");
    glUniform1i(enabledLoc, blueLightEnabled);

    // materiales

    if(useMaterialB)
    {
        glUniform3f(glGetUniformLocation(shader.programId(), "matAmbient"), 0.23125f, 0.23125f, 0.23125f);
        glUniform3f(glGetUniformLocation(shader.programId(), "matDiffuse"), 0.2775f, 0.2775f, 0.2775f);
        glUniform3f(glGetUniformLocation(shader.programId(), "matSpecular"), 0.773911f, 0.773911f, 0.773911f);
        glUniform1f(glGetUniformLocation(shader.programId(), "shininess"), 89.6f);
    }
    else
    {
        glUniform3f(glGetUniformLocation(shader.programId(), "matAmbient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(shader.programId(), "matDiffuse"), 0.50f, 0.50f, 0.50f);
        glUniform3f(glGetUniformLocation(shader.programId(), "matSpecular"), 0.70f, 0.70f, 0.70f);
        glUniform1f(glGetUniformLocation(shader.programId(), "shininess"), 32.0f);
    }

    glUniform3f(glGetUniformLocation(shader.programId(), "viewPos"), 0.0f, 0.0f, 3.0f);
    glUniform1i(glGetUniformLocation(shader.programId(), "textureEnabled"), textureEnabled ? 1 : 0);
    glUniform1i(glGetUniformLocation(shader.programId(), "useBlinn"), useBlinn ? 1 : 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniform1i(glGetUniformLocation(shader.programId(), "texture1"), 0);

    dragon.draw(this);

    shader.release();
}

void GLWindow::keyPressEvent(QKeyEvent *event)
{   
    // teclas de la luces B - azul
    if (event->key() == Qt::Key_B)
    {
        blueLightEnabled = !blueLightEnabled;
        qDebug() << "Luz azul:" << blueLightEnabled;
        update();
    }

    // teclas de las orientaciones de la cmara 
    if (event->key() == Qt::Key_1)
    {
        cameraMode = 1;
        qDebug() << "Camara frontal";
        update();
    }

    if (event->key() == Qt::Key_2)
    {
        cameraMode = 2;
        qDebug() << "Camara lateral";
        update();
    }

    if (event->key() == Qt::Key_3)
    {
        cameraMode = 3;
        qDebug() << "Camara superior";
        update();
    }

    // teclas de los materiales y tecturas
    if(event->key() == Qt::Key_M)
    {
        useMaterialB = !useMaterialB;
        qDebug() << "Material B:" << useMaterialB;
        update();
    }

    if(event->key() == Qt::Key_T)
    {
        textureEnabled = !textureEnabled;
        qDebug() << "Textura:" << textureEnabled;
        update();
    }

    if(event->key() == Qt::Key_P)
    {
        useBlinn = !useBlinn;
        qDebug() << "Blinn:" << useBlinn;
        update();
    }
}