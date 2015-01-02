#include "texture.h"

Texture::Texture(QString filename)
{
    QImage img;

    if (!img.load(filename)) {
        throw "Failed to load texture file " + filename;
    }

    m_filename = filename;

    m_img = QGLWidget::convertToGLFormat(img);

    //mirror the image vertically so that it is in the correct orientation
    m_img = m_img.mirrored(false /*horizontally*/, true /*vertically*/);

    //create the texture in OpenGL
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_img.width(), m_img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_img.bits());
}

QColor Texture::getPixel(int x, int y) const
{
    //correct the x and y positions so that they wrap around the texture
    while (x < 0)
        x = m_img.width() - abs(x);
    while (y < 0)
        y = m_img.height() - abs(y);

    x = x % m_img.width();
    y = y % m_img.height();

    return m_img.pixel(x, y);
}

int Texture::width() const
{
    return m_img.width();
}

int Texture::height() const
{
    return m_img.height();
}

QString Texture::filename() const
{
    return m_filename;
}
