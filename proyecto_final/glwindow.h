#ifndef GLWINDOW_H
#define GLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include <QVector3D>
#include <QVector>
#include <QString>

struct Triangle
{
    QVector3D v0;
    QVector3D v1;
    QVector3D v2;
};

class GLWindow : public QOpenGLWidget,
                 protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

public:
    explicit GLWindow(QWidget *parent = nullptr);
    ~GLWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QOpenGLShaderProgram shaderProgram;
    unsigned int VAO;
    unsigned int VBO;

    bool blueLightEnabled;
    bool glassEnabled; // shader sofisticado
    bool metalEnabled; // ek otro shader sofisticado pero para la otra esfera

    QVector<Triangle> objTriangles;

    bool loadOBJ(const QString& path);
    int cameraMode;
};

#endif