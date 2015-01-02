#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QTimer>
#include <QElapsedTimer>

#include <qmatrix4x4.h>

#include "texture.h"
#include "camera.h"

#include "Terrain/terraingenerator.h"

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    // input
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

signals:
    void glInitialized();

public slots:
    void spinboxChanged(int value);
    void spinboxChanged(double value);
    void checkboxChanged(bool value);

private:
    void loadTextures();
    void transformCamera();

public:
    TerrainGenerator* terrainGenerator;

private:
    QTimer timer;
    Camera camera;

    QPoint prevMousePos;
    std::map<int, bool> key, mouse;

    QList<Texture*> textures;
};

#endif // GLWIDGET_H
