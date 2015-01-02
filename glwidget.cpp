#include "glwidget.h"
#include <QSlider>

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(parent)
{
    // make this widget accept input
    this->setFocusPolicy(Qt::StrongFocus);
    this->focusWidget();

    // set up timer to automatically call updateGL, which controls when the widget is repainted
    // 16ms = ~62fps
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer.start(16 * 2);
}

GLWidget::~GLWidget()
{
    delete terrainGenerator;

    for (int t = textures.length()-1; t >= 0; t--)
        delete textures.takeAt(t);
}

void GLWidget::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
    glClearDepth(1);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
    GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat LightPosition[]= { 0.0f, 20.0f, 20.0f, 1.0f };
    glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);

    loadTextures();

    terrainGenerator = new TerrainGenerator(2 /*TILE RADIUS*/, &camera);

    TerrainPatchSettings& settings = terrainGenerator->getPatchSettings();

    settings.heightMap(textures[1]);
    settings.heightMapScale(0.277778f);
    settings.maxHeight(8192);
    settings.noiseMap(textures[2]);
    settings.size(48);
    settings.smoothingIterations(0);
    settings.smoothingWeight(0.2f);
    settings.spacing(2);
    settings.texture(NULL);
    settings.textureScale(1.0f);
    settings.heightMapOffset(1024.0f, 924.0f);

    terrainGenerator->generate();

    camera.distance = 400;
    camera.rotate(90, 30);
    camera.saveStateAsDefaults();

    emit glInitialized();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    transformCamera();

    terrainGenerator->update();
}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45, (GLfloat)w/(GLfloat)h, .001, 10000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::loadTextures() {
    textures.append(new Texture(":/images/checker.jpg"));
    textures.append(new Texture(":/images/heightmap.png"));
    textures.append(new Texture(":/images/uniformclouds.jpg"));
    textures.append(new Texture(":/images/terrain.jpg"));
    textures.append(new Texture(":/images/heightmap2.jpg"));
}

void GLWidget::transformCamera()
{
    QMatrix4x4 mCamera;

    //translate to the camera's positions offset by the lookat vector so that
    //all rotations appear at the center of the screen
    mCamera.translate(-camera.lookAt.x(), -camera.lookAt.y(), -camera.distance - camera.lookAt.z());

    //now translate to the camera's lookat and rotate around that point
    mCamera.translate(camera.lookAt.x(), camera.lookAt.y(), camera.lookAt.z());
    mCamera.rotate(camera.rotationY);
    mCamera.rotate(camera.rotationX);

    //translate back to the camera's position or the view will be offset
    mCamera.translate(-camera.lookAt.x(), -camera.lookAt.y(), -camera.lookAt.z());

    //apply the transformation matrix
    glMultMatrixd(mCamera.constData());
}
