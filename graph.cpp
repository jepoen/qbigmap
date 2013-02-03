#include <cassert>
#include <QtDebug>
#include "graph.h"
Edge::Edge(int v0, int v1, double weight) :
        myV0(v0), myV1(v1), myWeight(weight)
{}

Graph::Graph(int nodeCnt) :
        myNodeCnt(nodeCnt)
{
    for (int i = 0; i < nodeCnt; i++) {
        edges.push_back(EdgeList());
    }
}

void Graph::addEdge(const Edge &e) {
    edges[e.v0()].push_back(e);
}

QList<int> Graph::shortestPath(int v0, int v1) {
    QList<int> q;
    QList<int> dist;
    QList<int> prev;
    QList<int> mark;
    QList<int> weight;
    for (int i = 0; i < myNodeCnt; i++) {
        prev.push_back(-1);
        mark.push_back(0);
        weight.push_back(-1);
        dist.push_back(-1);
    }
    mark[v0] = 1;
    dist[v0] = 0;
    q.push_back(v0);
    while (q.size() > 0) {
        int v = q.takeFirst();
        qDebug()<<"handle "<<v<<" queue size: "<<q.size();
        foreach(Edge e, edges[v]) {
            int w = e.v1();
            int wnew = max(weight[v], e.weight());
            if (mark[w] == 0) {
                prev[w] = v;
                dist[w] = dist[v]+1;
                mark[w] = 1;
                weight[w] = wnew;
                q.push_back(w);
            }
            else if (mark[w] == 1 && dist[w] == dist[v]+1 && weight[w] > wnew) {
                prev[w] = v;
                weight[w] = wnew;
            }
        }
        mark[v] = 2;
    }
    QList<int> res;
    for (int v = v1; prev[v] >= 0; v = prev[v]) {
        res.push_front(v);
    }
    if (res.size() > 0) {
        assert(v0 == prev[res[0]]);
        res.push_front(v0);
    }
    qDebug()<<res;
    return res;
}
