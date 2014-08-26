#ifndef BELLMAN_FORD_ALGORITHMS_H
#define BELLMAN_FORD_ALGORITHMS_H

#include <unordered_map>
#include <vector>
#include <deque>
#include <iostream>

using namespace std;

template <typename Type>
class Bellman_Ford_algorithms {
   private:
      Directed_weighted_graph<Type> const &graph;
      unordered_map<Type, Type> predecessors;
      unordered_map<Type, double> distances;
      static const Type empty_node;
   public:
      Bellman_Ford_algorithms( Directed_weighted_graph<Type> const & );
      vector<deque<Type> > find_shortest_paths_and_negative_cycles( Type const & = empty_node );
      template <typename T>
      friend ostream &operator<<( ostream &, Bellman_Ford_algorithms<T> const & );
};

template <typename Type>
const Type Bellman_Ford_algorithms<Type>::empty_node = Type();

template <typename Type>
Bellman_Ford_algorithms<Type>::Bellman_Ford_algorithms( Directed_weighted_graph<Type> const & g ) : graph(g) {
   // Nothing to do
}

template <typename Type>
vector<deque<Type> > Bellman_Ford_algorithms<Type>::find_shortest_paths_and_negative_cycles( Type const & source ) {
   vector<deque<Type> > negative_cycles;

   for (auto &i : graph)
      distances[i.first] = 0;
   predecessors.clear();

   for (ssize_t i(graph.size()-2); i >= 0; --i) {
      bool relaxed = false;
      for (auto &u : graph)
         for (auto &v : u.second) {
            double d = distances[u.first] + v.second;
            if (d < distances[v.first]) {
               relaxed = true;
               distances[v.first] = d;
               predecessors[v.first] = u.first;
            }
         }
      if (!relaxed)
         break;
   }

   for (const auto &u : graph)
      for (const auto &v : u.second)
         if (distances[u.first] + v.second < distances[v.first]) {
            deque<Type> cycle;
            cycle.push_back(u.first);

            if (source == empty_node) {
               for (Type vertex = u.first; predecessors[vertex] != u.first; vertex = predecessors[vertex], cycle.push_back(vertex));
               negative_cycles.push_back(cycle);
            }

            else {
               bool foundSource = u.first == source;
               for (Type vertex = u.first; predecessors[vertex] != u.first; vertex = predecessors[vertex], cycle.push_back(vertex))
                  if (predecessors[vertex] == source)
                     foundSource = true;
               if (foundSource) {
                  while (cycle.front() != source) {
                     cycle.push_back(cycle.front());
                     cycle.pop_front();
                  }
                  negative_cycles.push_back(cycle);
               }
            }
         }
   return negative_cycles;
}

template <typename T>
ostream &operator << ( ostream &out, Bellman_Ford_algorithms<T> const &bfa ) {
   out << "distance: ";
   for (const auto &i : bfa.distances)
      out << i.first << "(" << i.second << ") ";
   out << endl << "predecessors: ";
   for (const auto &i : bfa.predecessors)
      out << i.first << "(" << i.second << ") ";
   out << endl;
   return out;
}
#endif