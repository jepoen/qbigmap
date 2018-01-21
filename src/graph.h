#ifndef GRAPH_H
#define GRAPH_H

#include <QList>

class Edge {
private:
    int myV0, myV1;
    double myWeight;
public:
    Edge(int v0, int v1, double weight);
    int v0() const { return myV0; }
    int v1() const { return myV1; }
    double weight() const { return myWeight; }
};

typedef QList<Edge> EdgeList;
class Graph
{
private:
    int myNodeCnt;
    QList<EdgeList> edges;
    int max(int v0, int v1) { return v0 > v1 ? v0 : v1; }
public:
    Graph(int nodeCnt);
    void addEdge(const Edge& e);
    QList<int> shortestPath(int v0, int v1);
};

#endif // GRAPH_H
