#ifndef DIRECTED_WEIGHTED_GRAPH_H
#define DIRECTED_WEIGHTED_GRAPH_H

#include <unordered_map>
#include <iostream>
#include <limits>

using namespace std;


template <typename Type>
class Bellman_Ford_algorithms;

template <typename Type>
class Directed_weighted_graph {
   private:
      unordered_map<Type, unordered_map<Type, double>> graph;
   public:
      int size() const;
      int degree( Type & ) const;
      bool add_edge( Type &, Type &, double );
      bool add_vertex( Type & );
      // Friends
      template <typename T>
      friend ostream &operator<<( ostream &, Directed_weighted_graph<T> const & );
      friend class Bellman_Ford_algorithms<Type>;
};

template <typename Type>
int Directed_weighted_graph<Type>::size() const {
   return graph.size();
}

template <typename Type>
int Directed_weighted_graph<Type>::degree( Type &n ) const {
   auto iter = graph.find(n);
   return iter == graph.end() ? -1 : iter->second.size();
}

template <typename Type>
bool Directed_weighted_graph<Type>::add_vertex( Type &n ) {
   if (graph.count(n))
      return false;
   graph[n] = unordered_map<Type, double>();
   return true;
}

template <typename Type>
bool Directed_weighted_graph<Type>::add_edge( Type &n, Type &m, double w ) {
   if (!graph.count(n) || !graph.count(m))
      return false;
   graph[n][m] = w;
   return true;
}

template <typename T>
ostream &operator << ( ostream &out, Directed_weighted_graph<T> const &dwg ) {
   for (const auto &u : dwg.graph) {
      out << u.first << endl;
      for (const auto &v : u.second) {
         out << "   " << v.first << "(" << v.second << ") ";
      }
      out << endl;
   }
   return out;
}
#endif
