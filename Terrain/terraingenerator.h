#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "camera.h"
#include "texture.h"
#include "Terrain/terrainpatch.h"

class TerrainGenerator
{
public:
    TerrainGenerator(int tileRadius, const Camera* camera);
    ~TerrainGenerator();

    void generate();
    void update();

    TerrainPatchSettings& getPatchSettings();

private:
    void createPatch(QVector3D position);
    void destroyPatches();

private:
    const Camera* m_camera;
    QList<TerrainPatch*> m_patches;
    TerrainPatchSettings m_settings;
};

#endif // TERRAINGENERATOR_H
