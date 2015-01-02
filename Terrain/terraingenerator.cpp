#include "terraingenerator.h"

TerrainGenerator::TerrainGenerator(int tileRadius, const Camera* camera)
    : m_camera(camera)
{
    m_settings.tileRadius(tileRadius);
}

TerrainGenerator::~TerrainGenerator()
{
    destroyPatches();
}

void TerrainGenerator::destroyPatches()
{
    if (m_patches.length() == 0) return;

    for (int t = m_patches.length()-1; t >= 0; t--)
        delete m_patches.takeAt(t);
}

void TerrainGenerator::createPatch(QVector3D offset)
{
    TerrainPatch* patch = new TerrainPatch(offset, m_settings);
    m_patches.append(patch);
}

 TerrainPatchSettings& TerrainGenerator::getPatchSettings()
 {
     return m_settings;
 }

void TerrainGenerator::generate()
{
    destroyPatches();

    int m_tileRadius = m_settings.tileRadius();

    for (int x = -m_tileRadius; x <= m_tileRadius; x++)
        for (int z = -m_tileRadius; z <= m_tileRadius; z++)
            createPatch(QVector3D(x, 0, z));
}

void TerrainGenerator::update()
{
    foreach(TerrainPatch *patch, m_patches)
            patch->Update();
}
