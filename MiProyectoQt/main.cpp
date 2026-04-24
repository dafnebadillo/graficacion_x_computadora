#include <QGuiApplication>
#include <QColor>
#include <QKeyEvent>
#include <QVector3D>
#include <QUrl>

#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QDiffuseMapMaterial>

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include <Qt3DRender/QMesh>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>

#include <Qt3DRender/QTexture>

// bump mapping
#include <Qt3DExtras/QNormalDiffuseMapMaterial>



class My3DWindow : public Qt3DExtras::Qt3DWindow
{
public:
    My3DWindow() = default;

    void setBlueLight(Qt3DRender::QPointLight *light)
    {
        blueLight = light;
    }

    void setTexturedMaterials(Qt3DCore::QEntity *entity,
                              Qt3DExtras::QDiffuseMapMaterial *matA,
                              Qt3DExtras::QDiffuseMapMaterial *matB)
    {
        modelEntity = entity;
        materialA = matA;
        materialB = matB;
    }

    // camaras y orientaciones

    void setCamera(Qt3DRender::QCamera *cam)
    {
        mainCamera = cam;
    }

    // bump mapping

    void setBumpMaterial(Qt3DExtras::QNormalDiffuseMapMaterial *mat)
    {
        bumpMaterial = mat;
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        // Toggle luz azul
        if (event->key() == Qt::Key_B && blueLight) {
            blueOn = !blueOn;
            blueLight->setIntensity(blueOn ? 0.6f : 0.0f);
        }

        // Cambiar material
        if (event->key() == Qt::Key_M && modelEntity) {
            useMaterialA = !useMaterialA;

            if (useMaterialA) {
                modelEntity->removeComponent(materialB);
                modelEntity->addComponent(materialA);
            } else {
                modelEntity->removeComponent(materialA);
                modelEntity->addComponent(materialB);
            }
        }

        // aqui cambias la camara
        if (mainCamera) {
            // tecla 1 vista central
            if (event->key() == Qt::Key_1) {
                mainCamera->setPosition(QVector3D(0.0f, 2.0f, 12.0f));
                mainCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
            }
            // tecla 2 vista de atras
            if (event->key() == Qt::Key_2) {
                mainCamera->setPosition(QVector3D(12.0f, 2.0f, 0.0f));
                mainCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
            }
            // tecla 3 vista de arrib a
            if (event->key() == Qt::Key_3) {
                mainCamera->setPosition(QVector3D(0.0f, 10.0f, 8.0f));
                mainCamera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
            }
        }

        // bump mapping buttons

        //if (event->key() == Qt::Key_N && modelEntity) {
          //  useBump = !useBump;
            //modelEntity->removeComponent(materialA);
            //modelEntity->removeComponent(materialB);
            //modelEntity->removeComponent(bumpMaterial);
            
            //if (useBump) {
              //  modelEntity->addComponent(bumpMaterial);
            //} else {
              //  modelEntity->addComponent(useMaterialA ? materialA : materialB);
            //}
        //}

        Qt3DExtras::Qt3DWindow::keyPressEvent(event);
    }

private:
    Qt3DRender::QPointLight *blueLight = nullptr;
    bool blueOn = true;

    Qt3DCore::QEntity *modelEntity = nullptr;
    Qt3DExtras::QDiffuseMapMaterial *materialA = nullptr;
    Qt3DExtras::QDiffuseMapMaterial *materialB = nullptr;
    bool useMaterialA = true;

    // bump mapping

    Qt3DExtras::QNormalDiffuseMapMaterial *bumpMaterial;

    Qt3DRender::QCamera *mainCamera = nullptr;
};


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto *view = new My3DWindow();
    view->defaultFrameGraph()->setClearColor(QColor(30, 30, 30));

    auto *root = new Qt3DCore::QEntity();

    // usar y expoertar objeto
    auto *mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl::fromLocalFile("/Users/dafnebadillo/MiProyectoQt/bunny_texture.obj"));

    auto *transform = new Qt3DCore::QTransform();
    transform->setScale(1.0f);
    transform->setTranslation(QVector3D(0.0f, 0.0f, 0.0f));

    auto *entity = new Qt3DCore::QEntity(root);
    entity->addComponent(mesh);
    entity->addComponent(transform);

    // material A y textura peluchona
    auto *textureA = new Qt3DRender::QTextureLoader(root);
    textureA->setSource(QUrl::fromLocalFile("/Users/dafnebadillo/MiProyectoQt/textura_peluche.JPG"));

    auto *materialA = new Qt3DExtras::QDiffuseMapMaterial(root);
    materialA->setDiffuse(textureA);
    materialA->setAmbient(QColor::fromRgbF(0.00, 0.00, 0.00, 1.0));
    //materialA->setSpecular(QColor::fromRgbF(0.70, 0.70, 0.70, 1.0)); 
    // asi lo indica en las especifiaciones pero cbajando elspecular luce mejor la textura
    //materialA->setShininess(32.0f);
    materialA->setSpecular(QColor::fromRgbF(0.25, 0.25, 0.25, 1.0));
    materialA->setShininess(12.0f);

    //material B textura de piel de cocodrilo
    auto *textureB = new Qt3DRender::QTextureLoader(root);
    textureB->setSource(QUrl::fromLocalFile("/Users/dafnebadillo/MiProyectoQt/textura_cocodrilo.jpg"));
    //textureB->setSource(QUrl::fromLocalFile("/Users/dafnebadillo/MiProyectoQt/textura_galaxias.JPG"));
    // otra textura medio galactica
    // luce mejor la de cocodrilo pero tuve que vambiar valores de intensidad de luces y bajar el ambiente specular en ambas tecturas A y b
    

    auto *materialB = new Qt3DExtras::QDiffuseMapMaterial(root);
    materialB->setDiffuse(textureB);
    materialB->setAmbient(QColor::fromRgbF(0.23125, 0.23125, 0.23125, 1.0));
    //materialB->setSpecular(QColor::fromRgbF(0.773911, 0.773911, 0.773911, 1.0));
    materialB->setSpecular(QColor::fromRgbF(0.35, 0.35, 0.35, 1.0));
    // igual cambie el specular en esta
    //materialB->setShininess(89.6f);
    materialB->setShininess(20.0f);

    // Material inicial
    entity->addComponent(materialA);

    view->setTexturedMaterials(entity, materialA, materialB);

    // material de bump mapping

    //auto *normalTexture = new Qt3DRender::QTextureLoader(root);
    //normalTexture->setSource(QUrl::fromLocalFile("/Users/dafnebadillo/MiProyectoQt/normal_cocodrilo.jpg"));

    //auto *bumpMaterial = new Qt3DExtras::QNormalDiffuseMapMaterial(root);
    //bumpMaterial->setDiffuse(textureB);
    //bumpMaterial->setNormal(normalTexture);
    //bumpMaterial->setAmbient(QColor::fromRgbF(0.23, 0.23, 0.23, 1.0));
    //bumpMaterial->setSpecular(QColor::fromRgbF(0.35, 0.35, 0.35, 1.0));
    //bumpMaterial->setShininess(20.0f);

    // luzz blanca
    auto *whiteLightEntity = new Qt3DCore::QEntity(root);
    auto *whiteLight = new Qt3DRender::QPointLight(whiteLightEntity);
    whiteLight->setColor(QColor(255, 255, 255));
    whiteLight->setIntensity(0.7f);

    auto *whiteLightTransform = new Qt3DCore::QTransform(whiteLightEntity);
    whiteLightTransform->setTranslation(QVector3D(8.0f, 8.0f, 12.0f));

    whiteLightEntity->addComponent(whiteLight);
    whiteLightEntity->addComponent(whiteLightTransform);

    // luzz azul cambia con tecla B
    auto *blueLightEntity = new Qt3DCore::QEntity(root);
    auto *blueLight = new Qt3DRender::QPointLight(blueLightEntity);
    blueLight->setColor(QColor(0, 0, 255));
    blueLight->setIntensity(0.6f);

    auto *blueLightTransform = new Qt3DCore::QTransform(blueLightEntity);
    blueLightTransform->setTranslation(QVector3D(-8.0f, 4.0f, 10.0f));

    blueLightEntity->addComponent(blueLight);
    blueLightEntity->addComponent(blueLightTransform);

    view->setBlueLight(blueLight);

    // camara
    Qt3DRender::QCamera *camera = view->camera();
    view->setCamera(camera);
    camera->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    camera->setPosition(QVector3D(0.0f, 2.0f, 12.0f));
    camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));

    auto *camController = new Qt3DExtras::QOrbitCameraController(root);
    camController->setCamera(camera);

    view->setRootEntity(root);
    view->resize(800, 600);
    view->show();

    return app.exec();
}