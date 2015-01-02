#ifndef TERRAINPATCHSETTINGS_H
#define TERRAINPATCHSETTINGS_H

#include <QVector2D>
#include "texture.h"

struct TerrainPatchSettings
{
    TerrainPatchSettings()
    {
        m_texture = m_heightMap = m_noiseMap = NULL;
        m_maxHeight = 0;
        m_size = 0;
        m_spacing = 0.0f;
        m_smoothingIterations = 0;
        m_smoothingWeight = 0.0f;
        m_heightMapScale = 1;
        m_textureScale = 1;
        m_tileRadius = 2;
        m_heightMapOffset = QVector2D(0, 0);
        m_noiseMapScale = 1;
        m_noiseMapWeight = 1;
        m_smoothingRadius = 1;
    }

    const Texture* texture() const { return m_texture; }
    const Texture* heightMap() const { return m_heightMap; }
    const Texture* noiseMap() const { return m_noiseMap; }
    int maxHeight() const { return m_maxHeight; }
    int size() const { return m_size; }
    float spacing() const { return m_spacing; }
    int smoothingIterations() const { return m_smoothingIterations; }
    float smoothingWeight() const { return m_smoothingWeight; }
    float heightMapScale() const { return m_heightMapScale; }
    float textureScale() const { return m_textureScale; }
    int tileRadius() const { return m_tileRadius; }
    QVector2D heightMapOffset() const { return m_heightMapOffset; }
    float noiseMapScale() const { return m_noiseMapScale; }
    float noiseMapWeight() const { return m_noiseMapWeight; }
    int smoothingRadius() const { return m_smoothingRadius; }

    void texture(Texture* value) { m_texture = value; }
    void heightMap(Texture* value) { m_heightMap = value; }
    void noiseMap(Texture* value) { m_noiseMap = value; }
    void maxHeight(int value) { m_maxHeight = value; }
    void size(int value) { m_size = value % 2 == 0 ? value + 1 : value; }
    void spacing(float value) { m_spacing = value; }
    void smoothingIterations(int value) { m_smoothingIterations = value; }
    void smoothingWeight(float value) { m_smoothingWeight = value; }
    void heightMapScale(float value) { m_heightMapScale = value; }
    void textureScale(float value) { m_textureScale = value; }
    void tileRadius(int value) { m_tileRadius = value <= 0 ? 0 : value - 1; }
    void heightMapOffset(float x, float y) { m_heightMapOffset = QVector2D(x, y); }
    void noiseMapScale(float value) { m_noiseMapScale = value; }
    void noiseMapWeight(float value) { m_noiseMapWeight = value; }
    void smoothingRadius(int value) { m_smoothingRadius = value; }

private:
    const Texture* m_texture;
    const Texture* m_heightMap;
    const Texture* m_noiseMap;
    int m_maxHeight;
    int m_size;
    float m_spacing;
    int m_smoothingIterations;
    float m_smoothingWeight;
    float m_heightMapScale;
    float m_textureScale;
    int m_tileRadius;
    QVector2D m_heightMapOffset;
    float m_noiseMapScale;
    float m_noiseMapWeight;
    int m_smoothingRadius;
};

#endif // TERRAINPATCHSETTINGS_H
