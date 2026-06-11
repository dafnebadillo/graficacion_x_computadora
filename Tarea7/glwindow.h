#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include "mesh.h"
#include <QMatrix4x4>
#include <QKeyEvent>

class GLWindow : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    
public:
    GLWindow(QWidget *parent = nullptr);
    ~GLWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QOpenGLShaderProgram shader;

    unsigned int VAO;
    unsigned int VBO;

    // importar dragon obj
    Mesh dragon;
    bool blueLightEnabled;

    // oreintaciones de la camara
    int cameraMode;

    // material y texturas
    bool useMaterialB;
    bool textureEnabled;
    bool useBlinn;
    unsigned int textureID;


};

#endif