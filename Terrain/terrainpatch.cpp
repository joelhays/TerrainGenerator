#include "terrainpatch.h"

TerrainPatch::TerrainPatch(QVector3D offset, const TerrainPatchSettings& settings)
    : m_offset(offset), m_settings(settings)
{
    //calculate the actual position of patch based on the size and spacing of the patch
    float patchScale = (settings.size() - 1) * settings.spacing();

    m_mesh.position = QVector3D(offset.x()*patchScale, offset.y(), offset.z()*patchScale);

    createVertices();
    createMesh();
    calculateNormals();
}

TerrainPatch::~TerrainPatch()
{
}

void TerrainPatch::createVertices()
{
    int m_size = m_settings.size();
    float m_spacing = m_settings.spacing();

    float offset = (m_size - 1) * m_spacing / 2;

    int rowZ = 0;
    for (int v = 0, totalVerts = m_size * m_size; v < totalVerts; v++)
    {
        int colX = v % m_size;
        if (v != 0 && colX == 0) rowZ++;

        float posX = colX * m_spacing,
            posZ = rowZ * m_spacing;

        //for alternating rows, offset the X position by half the vertex spacing
        if (rowZ % 2 == 0)
            posX -= m_spacing / 2;

        float height = getSmoothHeight(posX - offset, posZ - offset);

        //calculate texture coordinates
        float u = posX / ((m_size - 1) * m_spacing);
        float v = posZ / ((m_size - 1) * m_spacing);

        Vertex vtx;
        vtx.position = QVector3D(posX - offset, height, posZ - offset);
        vtx.uv = QVector2D(u, v);
        m_mesh.vertices.append(vtx);

        //for alternating rows, add an extra vertex to the end of this row
        if (rowZ % 2 == 0 && colX == m_size - 1)
        {
            posX = colX * m_spacing - m_spacing / 2;
            height = getSmoothHeight(posX - offset, posZ - offset);
            vtx.position = QVector3D(posX - offset, height, posZ - offset);
            vtx.uv = QVector2D(posX / ((m_size - 1) * m_spacing), v);
            m_mesh.vertices.append(vtx);
        }
    }
}

void TerrainPatch::createMesh()
{
    int m_size = m_settings.size();

    int currentRow = 0;
    int count = 0;
    for (int v = 0, totalVerts = m_size * m_size - m_size / 2 - 1; v < totalVerts; v++, count++)
    {
        int v1 = v, v2 = v + 1, v3 = v + m_size + 1;

        bool currentRowHasExtraVertex = currentRow % 2 == 0;

        //current vertex is the last vertex in the row, so skip it so that we don't draw a skewed triangle
        if (currentRowHasExtraVertex && count == m_size)
            continue;

        //current vertex is the first vertex in this row, so increment current row
        if ((currentRowHasExtraVertex && count == m_size + 1) || (!currentRowHasExtraVertex && count == m_size))
        {
            currentRow++;
            count = 0;
            currentRowHasExtraVertex = currentRow % 2 == 0;
        }

        if (!currentRowHasExtraVertex)
        {
            v1 = v + m_size;
            v2 = v;
        }

        m_mesh.indices.append(v3);
        m_mesh.indices.append(v2);
        m_mesh.indices.append(v1);

        Vertex* vtx1 = &m_mesh.vertices[v1],
            *vtx2 = &m_mesh.vertices[v2],
            *vtx3 = &m_mesh.vertices[v3];

        //if not the last vertex, add to triangle
        if (count != m_size-1)
        {
            int vTemp = 0;

            if (currentRowHasExtraVertex)
                vTemp = v3 + 1;
            else
                vTemp = v2 + 1;

            m_mesh.indices.append(vTemp);
            m_mesh.indices.append(v2);
            m_mesh.indices.append(v3);

            vtx1 = &m_mesh.vertices[vTemp],
            vtx2 = &m_mesh.vertices[v2],
            vtx3 = &m_mesh.vertices[v3];
        }
    }
}

void TerrainPatch::calculateNormals()
{
    for (int v = 0; v < m_mesh.vertices.count(); v++)
    {
        Vertex& current = m_mesh.vertices[v];
        calculateNormal(current);
    }
}

float TerrainPatch::getSmoothHeight(float x, float z)
{
    float height = getHeight(x, z);

    /*smooth only if iterations and radius are specified*/
    if (m_settings.smoothingIterations() > 0 && m_settings.smoothingRadius() > 0)
    {
        float fullSpace = m_settings.spacing();
        float halfSpace = fullSpace / 2;

        float avgHeight = 0;
        /*get values of all surrounding vertices based on the smoothing radius
          and calculate the average height*/
        for (int r = 1, rMax = m_settings.smoothingRadius(); r <= rMax; r++)
        {
            float fs = fullSpace * r;
            float hs = halfSpace * r;

            float ul = getHeight(x - hs, z + fs); /*upper-left*/
            float ur = getHeight(x + hs, z + fs); /*upper-right*/
            float lt = getHeight(x - fs, z); /*left*/
            float rt = getHeight(x + fs, z); /*right*/
            float ll = getHeight(x - hs, z - fs); /*lower-left*/
            float lr = getHeight(x + hs, z - fs); /*lower-right*/

            avgHeight += (ul + ur + lt + rt + ll + lr) / 6;
        }
        avgHeight /= m_settings.smoothingRadius();

        /*increase iterations based on the difference between the height of this pixel
          and the average height. adaptive iterations will not exceed the number of
          iterations specified*/
        int adaptiveIterations = abs(height - avgHeight);
        if (adaptiveIterations > m_settings.smoothingIterations())
            adaptiveIterations = m_settings.smoothingIterations();

        /*now interpolate between the pixel height and the average height*/
        for (int i = 0; i < m_settings.smoothingIterations() + adaptiveIterations; i++)
            height = Utils::lerp(height, avgHeight, m_settings.smoothingWeight());
    }

    return height;
}

float TerrainPatch::getHeight(float x, float z)
{
    /*offset x and y by the patch's position so that we sample the
      correct part of the textures*/
    x += m_mesh.position.x();
    z += m_mesh.position.z();

    float heightScale = m_settings.heightMapScale();
    float noiseScale = m_settings.noiseMapScale();
    float noiseWeight = m_settings.noiseMapWeight();

    /*scales for the noise map samples*/
    float lowNoiseScale = 0.1f * noiseScale;
    float midNoiseScale = 0.2f * noiseScale;
    float highNoiseScale = 0.5f * noiseScale;
    float extremeNoiseScale = 2.0f * noiseScale;

    /*interpolation weights for the noise map samples*/
    float lowNoiseWeight = 0.8f * noiseWeight;
    float midNoiseWeight = 0.75f * noiseWeight;
    float highNoiseWeight = 0.65f * noiseWeight;
    float extremeNoiseWeight = 0.75f * noiseWeight;

    /*sample the height and noise maps for this position*/
    float heightSample = sampleMap(x, z, heightScale, m_settings.heightMap());
    float lowNoiseSample = sampleMap(x, z, lowNoiseScale, m_settings.noiseMap());
    float midNoiseSample = sampleMap(x, z, midNoiseScale, m_settings.noiseMap());
    float highNoiseSample = sampleMap(x, z, highNoiseScale, m_settings.noiseMap());
    float extremeNoiseSample = sampleMap(x, z, extremeNoiseScale, m_settings.noiseMap());

    /*interpolate the samples*/
    float b1 = Utils::lerp(lowNoiseSample, midNoiseSample, 1-lowNoiseWeight);
    float b2 = Utils::lerp(b1, highNoiseSample, 1-midNoiseWeight);
    float b3 = Utils::lerp(b2, extremeNoiseSample, 1-highNoiseWeight);
    float height = Utils::lerp(heightSample, b3, 1-extremeNoiseWeight);

    /*force the height into an s-curve so that mid-range is softened*/
    height = pow(height * 2 - 1, 3) * m_settings.maxHeight();

    return height;
}

float TerrainPatch::sampleMap(float x, float z, float scale, const Texture* map)
{
    float heightMapOffsetX = m_settings.heightMapOffset().x();
    float heightMapOffsetY = m_settings.heightMapOffset().y();

    /*scale x and offset in the heightmap*/
    float xScaled = x * scale + heightMapOffsetX;
    float zScaled = z * scale + heightMapOffsetY;

    /*since the x and z positions may be in-between pixels, sample all
      pixels around this point so that the height values can be averaged
      and interpolated*/
    int fx = floor(xScaled);
    int fz = floor(zScaled);
    int cx = ceil(xScaled);
    int cz = ceil(zScaled);

    /*determine the ratio between the x and z values for interpolation*/
    float rx = xScaled - (int)xScaled;
    float rz = zScaled - (int)zScaled;

    float h1 = map->getPixel(fx, fz).valueF();
    float h2 = map->getPixel(cx, fz).valueF();
    float h3 = map->getPixel(fx, cz).valueF();
    float h4 = map->getPixel(cx, cz).valueF();

    /*interpolate between the height of the surrounding pixels based on the ration*/
    float b1 = Utils::lerp(h1, h2, rx);
    float b2 = Utils::lerp(h3, h4, rx);
    float height = Utils::lerp(b1, b2, rz);

    return height;
}

void TerrainPatch::Update()
{
    glPushMatrix();

    glColor3f(1, 1, 1);
    glEnable(GL_DEPTH_TEST);

    glTranslatef(m_mesh.position.x(), m_mesh.position.y(), m_mesh.position.z());

    if (m_settings.texture() != NULL)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_settings.texture()->m_id);
    }
    else
    {
        glDisable(GL_TEXTURE_2D);
    }

    float uvScale = m_settings.textureScale();

    glBegin(GL_TRIANGLES);
    foreach(int i, m_mesh.indices)
    {
        Vertex& v = m_mesh.vertices[i];
        glTexCoord2f(v.uv.x()*uvScale, v.uv.y()*uvScale);
        glNormal3f(v.normal.x(), v.normal.y(), v.normal.z());
        glVertex3f(v.position.x(), v.position.y(), v.position.z());
    }
    glEnd();

    glPopMatrix();
}

QVector3D TerrainPatch::Offset() const
{
    return m_offset;
}


QVector3D TerrainPatch::calculateNormal(Vertex &vertex)
{
    /*calculate the normal for this vertex based on the slope of the
      surrounding pixels int he height map*/

    QVector3D pos = vertex.position;

    float spacing = m_settings.spacing();
    float u = getSmoothHeight(pos.x(), pos.z() + spacing);
    float d = getSmoothHeight(pos.x(), pos.z() - spacing);
    float l = getSmoothHeight(pos.x() - spacing, pos.z());
    float r = getSmoothHeight(pos.x() + spacing, pos.z());

    QVector3D normal = QVector3D(l-r, spacing, d-u);
    normal.normalize();

    vertex.normal = normal;

    return normal;
}
