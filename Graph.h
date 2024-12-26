#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <ostream>
#include "ArrayList.h"

template <class T> struct Edge;

template <class T>
struct Vertex {
    T data;
    ArrayList<Edge<T>*> edgeList;

    Vertex(T data){
        this->data = data;
    }
};

template <class T>
inline std::ostream& operator<<(std::ostream& os, Vertex<T>* v){
    os << v->data;

    return os;
}

template <class T>
struct Edge{
    Vertex<T>* from;
    Vertex<T>* to;
    int weight;

    Edge(Vertex<T>* from, Vertex<T>* to, int weight){
        this->from = from;
        this->to = to;
        this->weight = weight;
    }
};

template <class T>
inline std::ostream& operator<<(std::ostream& os, Edge<T>* e){
    os << "(" << e->from << ", " << e->to << ") - " << e->weight;

    return os;
}

template <class T>
struct Graph {
    ArrayList<Vertex<T>*> vertices;

    void addVertex(Vertex<T>* v){
        vertices.append(v);
    }

    void addEdge(Vertex<T>* x, Vertex<T>* y, int w){
        x->edgeList.append(new Edge<T>(x, y, w));
        y->edgeList.append(new Edge<T>(y, x, w));
    }

    void addDirectedEdge(Vertex<T>* x, Vertex<T>* y, int w){
        x->edgeList.append(new Edge<T>(x, y, w));
    }

};

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Graph<T>& g){
    for (int i = 0; i < g.vertices.size(); i++){
        os << g.vertices[i]->edgeList << std::endl;
    }

    return os;
}

//Waypoints aren't being used
// template <class T>
// struct Waypoint{
//     Waypoint* parent;
//     Vertex<T>* vertex;
//     ArrayList<Waypoint*> children;
//     int partialCost;
//     int weight;

//     Waypoint(Vertex<T>* v){
//         parent = nullptr;
//         vertex = v;
//         weight = 0;
//         partialCost = 0;
//     }


//     void expand(){
//         for (int i = 0; i < vertex->edgeList.size(); i++){
//             Waypoint* temp = new Waypoint(vertex->edgeList[i]->to);
//             temp->parent = this;
//             temp->weight = vertex->edgeList[i]->weight;
//             temp->partialCost = partialCost + vertex->edgeList[i]->weight;
//             children.append(temp);
//         }
//     }
// };

// template <class T>
// inline std::ostream& operator<<(std::ostream& os, Waypoint<T>* wp){
//     std::string p = "null";
//     if (wp->parent != nullptr){
//         p = wp->parent->vertex->data;
//     }

//     os << p << " -> " << wp->vertex->data;

//     return os;
// }

#endif
