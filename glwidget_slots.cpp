#include "glwidget.h"

void GLWidget::spinboxChanged(int value)
{
    QString s = sender()->objectName();

    if (s == "patchTileRadius")
        terrainGenerator->getPatchSettings().tileRadius(value);
    else if (s == "maxHeight")
        terrainGenerator->getPatchSettings().maxHeight(value);
    else if (s == "patchSize")
        terrainGenerator->getPatchSettings().size(value);
    else if (s == "patchSpacing")
        terrainGenerator->getPatchSettings().spacing(value);
    else if (s == "smoothingIterations")
        terrainGenerator->getPatchSettings().smoothingIterations(value);
    else if (s == "smoothingRadius")
        terrainGenerator->getPatchSettings().smoothingRadius(value);

    terrainGenerator->generate();

}

void GLWidget::spinboxChanged(double value)
{
    QString s = sender()->objectName();
    TerrainPatchSettings& settings = terrainGenerator->getPatchSettings();

    if (s == "patchSpacing")
        settings.spacing(value);
    else if (s == "smoothingWeight")
        settings.smoothingWeight(value);
    else if (s == "heightMapScale")
        settings.heightMapScale(value);
    else if (s == "textureScale")
    {
        settings.textureScale(value);
        return;
    }
    else if (s == "heightMapOffsetX")
        settings.heightMapOffset(value, settings.heightMapOffset().y());
    else if (s == "heightMapOffsetY")
        settings.heightMapOffset(settings.heightMapOffset().x(), value);
    else if (s == "noiseMapScale")
        settings.noiseMapScale(value);
    else if (s == "noiseMapWeight")
        settings.noiseMapWeight(value);

    terrainGenerator->generate();
}

void GLWidget::checkboxChanged(bool value)
{
    QString s = sender()->objectName();

    if (s == "wireframe")
    {
        if (value == true)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        return;
    }
    else if (s == "textured")
    {
        if (value == true)
            terrainGenerator->getPatchSettings().texture(textures[3]);
        else
            terrainGenerator->getPatchSettings().texture(NULL);
        return;
    }

    terrainGenerator->generate();
}
