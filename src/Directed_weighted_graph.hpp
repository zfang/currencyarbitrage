#ifndef DIRECTED_WEIGHTED_GRAPH_H
#define DIRECTED_WEIGHTED_GRAPH_H

#include <unordered_map>
#include <iostream>
#include <limits>
#include <iterator>

using namespace std;


template <typename Type>
class Bellman_Ford_algorithms;

template <typename Type>
class Directed_weighted_graph {
   using graph_map = unordered_map<Type, unordered_map<Type, double>>;
   private:
      graph_map graph;
   public:
      int size() const;
      int degree( const Type & ) const;
      bool add_edge( const Type &, const Type &, double );
      bool add_vertex( const Type & );
      double weight( const Type &, const Type & ) const;

      typename graph_map::iterator begin() noexcept;
      typename graph_map::const_iterator begin() const noexcept;
      typename graph_map::iterator end() noexcept;
      typename graph_map::const_iterator end() const noexcept;

      // Friends
      template <typename T>
      friend ostream &operator<<( ostream &, Directed_weighted_graph<T> const & );
      friend class Bellman_Ford_algorithms<Type>;
};

template <typename Type>
typename Directed_weighted_graph<Type>::graph_map::iterator Directed_weighted_graph<Type>::begin() noexcept {
   return graph.begin();
}

template <typename Type>
typename Directed_weighted_graph<Type>::graph_map::const_iterator Directed_weighted_graph<Type>::begin() const noexcept {
   return graph.begin();
}

template <typename Type>
typename Directed_weighted_graph<Type>::graph_map::iterator Directed_weighted_graph<Type>::end() noexcept {
   return graph.end();
}

template <typename Type>
typename Directed_weighted_graph<Type>::graph_map::const_iterator Directed_weighted_graph<Type>::end() const noexcept {
   return graph.end();
}

template <typename Type>
int Directed_weighted_graph<Type>::size() const {
   return graph.size();
}

template <typename Type>
int Directed_weighted_graph<Type>::degree( const Type &n ) const {
   auto iter = graph.find(n);
   return iter == graph.end() ? -1 : iter->second.size();
}

template <typename Type>
bool Directed_weighted_graph<Type>::add_vertex( const Type &n ) {
   if (graph.count(n))
      return false;
   graph[n] = unordered_map<Type, double>();
   return true;
}

template <typename Type>
bool Directed_weighted_graph<Type>::add_edge( const Type &n, const Type &m, double w ) {
   if (!graph.count(n) || !graph.count(m))
      return false;
   graph[n][m] = w;
   return true;
}

template <typename Type>
double Directed_weighted_graph<Type>::weight( const Type &n, const Type &m ) const {
   typename graph_map::const_iterator iter = graph.find(n);
   if (iter == graph.end())
      return numeric_limits<double>::infinity();
   typename unordered_map<Type, double>::const_iterator iter2 = iter->second.find(m);
   if (iter2 == iter->second.end())
      return numeric_limits<double>::infinity();

   return iter2->second;
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
