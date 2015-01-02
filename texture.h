#ifndef TEXTURE_H
#define TEXTURE_H

#include <qgl.h>
#include <QString>
#include <QImage>

class Texture
{
public:
    Texture(QString filename);

    QColor getPixel(int x, int y) const;
    int width() const;
    int height() const;
    QString filename() const;

public:
    GLuint m_id;
    QImage m_img;
    QString m_filename;
};

#endif // TEXTURE_H
