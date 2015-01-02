#ifndef TERRAINPATCH_H
#define TERRAINPATCH_H

#include <Qlist>
#include <QVector3D>
#include <QVector2D>
#include <qgl.h>
#include <qmath.h>

#include <QMap>

#include "texture.h"
#include "utils.h"

#include "terrainpatchsettings.h"
#include "model.h"


class TerrainPatch
{
public:
    TerrainPatch(QVector3D position, const TerrainPatchSettings& settings);
    ~TerrainPatch();

    void Update();

    QVector3D Offset() const;
private:
    void createVertices();
    void createMesh();
    void calculateNormals();
    QVector3D calculateNormal(Vertex& vertex);

    float getSmoothHeight(float x, float z);
    float getHeight(float x, float z);
    float sampleMap(float x, float z, float scale, const Texture* map);
private:
    QVector3D m_offset;

    Mesh m_mesh;
    const TerrainPatchSettings& m_settings;
};

#endif // TERRAINPATCH_H
