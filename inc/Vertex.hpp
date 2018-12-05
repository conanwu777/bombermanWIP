//
// Created by Remi DAVID on 10/17/18.
//

#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "../inc/Constant.hpp"

struct Vertex
{
    int x;
    int y;
    int z;

    Vertex() {}

    Vertex(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vertex(Vertex const &other)
    {
        this->x = other.x;
        this->y = other.y;
        this->z = other.z;
    }

    void set(int const &x, int const &y, int const &z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void set(Vertex const &v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    bool equals(int const &x, int const &y, int const &z)
    {
        return this->x == x && this->y == y && this->z == z;
    }

    bool equals(Vertex const &v)
    {
        return x == v.x && y == v.y && z == v.z;
    }
};

struct VertexComparator
{
public:
    bool operator()(const Vertex &v1, const Vertex &v2) const
    {
        return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
    }
};

#endif //VERTEX_HPP
