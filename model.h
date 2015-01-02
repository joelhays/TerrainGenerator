#ifndef MODEL_H
#define MODEL_H

#include <QVector2D>
#include <QVector3D>
#include <QList>

struct Vertex
{
    QVector3D position;
    QVector2D uv;
    QVector3D normal;
};

struct Mesh
{
    QVector3D position;
    QList<Vertex> vertices;
    QList<int> indices;
};

#endif // MODEL_H
