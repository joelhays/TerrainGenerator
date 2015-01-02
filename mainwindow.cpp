#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //clear margins from the main layout
    ui->horizontalLayout->setContentsMargins(0,0,0,0);

    //Notify this UI that the QGLWidget has been initialized and it's safe start using.
    connect(ui->glView, SIGNAL(glInitialized()), this, SLOT(glInitialized()));

    //TODO: smoothing is slow, so hiding options for now until I can make it real-time.
    ui->smoothingIterations->hide();
    ui->smoothingRadius->hide();
    ui->smoothingWeight->hide();
    ui->lblSmoothingIterations->hide();
    ui->lblSmoothingRadius->hide();
    ui->lblSmoothingWeight->hide();
    ui->line_smoothing->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::glInitialized()
{
    /*OpenGL and terrain properties in the GLWidget have been initialized,
      so now update the UI controls with the terrain generator values*/

    TerrainPatchSettings s = ui->glView->terrainGenerator->getPatchSettings();
    ui->patchTileRadius->setValue(s.tileRadius() + 1);
    ui->patchSize->setValue(s.size() - 1);
    ui->patchSpacing->setValue(s.spacing());
    ui->maxHeight->setValue(s.maxHeight());
    ui->smoothingIterations->setValue(s.smoothingIterations());
    ui->smoothingWeight->setValue(s.smoothingWeight());
    ui->heightMapScale->setValue(s.heightMapScale());
    ui->textureScale->setValue(s.textureScale());
    ui->heightMapOffsetX->setValue(s.heightMapOffset().x());
    ui->heightMapOffsetY->setValue(s.heightMapOffset().y());
    ui->noiseMapScale->setValue(s.noiseMapScale());
    ui->noiseMapWeight->setValue(s.noiseMapWeight());
    ui->smoothingRadius->setValue(s.smoothingRadius());

    //connect all controls to the GLWidget so that it can update terrain generator settings
    connect(ui->patchTileRadius, SIGNAL(valueChanged(int)), ui->glView, SLOT(spinboxChanged(int)));
    connect(ui->maxHeight, SIGNAL(valueChanged(int)), ui->glView, SLOT(spinboxChanged(int)));
    connect(ui->patchSize, SIGNAL(valueChanged(int)), ui->glView, SLOT(spinboxChanged(int)));
    connect(ui->smoothingIterations, SIGNAL(valueChanged(int)), ui->glView, SLOT(spinboxChanged(int)));
    connect(ui->smoothingRadius, SIGNAL(valueChanged(int)), ui->glView, SLOT(spinboxChanged(int)));

    connect(ui->patchSpacing, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->smoothingWeight, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->heightMapScale, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->textureScale, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->heightMapOffsetX, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->heightMapOffsetY, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->noiseMapScale, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));
    connect(ui->noiseMapWeight, SIGNAL(valueChanged(double)), ui->glView, SLOT(spinboxChanged(double)));

    connect(ui->wireframe, SIGNAL(toggled(bool)), ui->glView, SLOT(checkboxChanged(bool)));
    connect(ui->textured, SIGNAL(toggled(bool)), ui->glView, SLOT(checkboxChanged(bool)));
}
