#ifndef BUILDERTERRAIN_H
#define BUILDERTERRAIN_H

#include <vector>
#include <sstream>
#include <QByteArray>

#include "primitives/point.h"
#include "terrain.h"
#include "unordered_map"

class BuilderTerrain
{
    public:
        BuilderTerrain();
        void getAllPoints(std::vector<runnel::Point*> &points_terran, std::stringstream& vertexstream);
        void getTriangle(Terrain* ter, QByteArray& result);
        void runTriangulation(Terrain* ter);
        glm::vec3 calculateNormal(runnel::Point* p1, runnel::Point* p2, runnel::Point* p3);
        void buildStruct(Terrain* ter, runnel::Triangle *trian, int pto1, int pto2, int pto3);
        void buildNeighbourhood(Terrain* ter, std::unordered_map<int, std::vector< runnel::Triangle*> >& neigh, runnel::Triangle *trian, std::vector< int> pto );
        void buildNeighbourhoodByEdges(Terrain* ter, runnel::Triangle* trian, std::unordered_map< int, std::unordered_map<int, runnel::Edge*> > & edges_neigh, int pto0, int pto1);
};

#endif // BUILDERTERRAIN_H
