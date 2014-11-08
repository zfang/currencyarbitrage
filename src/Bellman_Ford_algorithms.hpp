#ifndef BELLMAN_FORD_ALGORITHMS_HPP
#define BELLMAN_FORD_ALGORITHMS_HPP

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>
#include <future>
#include "Cycle.hpp"
#include "common.hpp"

using namespace std;

template <typename Type>
class Bellman_Ford_algorithms {
   private:
      unordered_map<Type, Type> predecessors;
      unordered_map<Type, double> distances;
   public:
      void find_shortest_paths_and_negative_cycles( Directed_weighted_graph<Type> const &, vector<Cycle<Type> > & cycles );
      template <typename T>
      friend ostream &operator<<( ostream &, Bellman_Ford_algorithms<T> const & );
};

template <typename Type>
void Bellman_Ford_algorithms<Type>::find_shortest_paths_and_negative_cycles( Directed_weighted_graph<Type> const &graph, vector<Cycle<Type> > & cycles ) {
   unordered_set<Cycle<Type>, cycle_hash<Type> > negative_cycles;

   for (auto &i : graph)
      // Add an auxiliary edge to each vertex
      distances[i.first] = 0;
   predecessors.clear();

   for (ssize_t i(graph.size()-2); i >= 0; --i) {
      bool relaxed = false;
      for (auto &u : graph)
         for (auto &v : u.second) {
            double d = distances[u.first] + graph.weight(u.first, v.first);
            if (d < distances[v.first]) {
               relaxed = true;
               distances[v.first] = d;
               predecessors[v.first] = u.first;
            }
         }
      if (!relaxed)
         break;
   }

   vector<future<shared_ptr<Cycle<Type> > > > futures;

   for (const auto &u : graph)
      for (const auto &v : u.second)
         if (distances[u.first] + graph.weight(u.first, v.first) < distances[v.first])
            futures.push_back(async(
#ifdef GET_CYCLES_ASYNC
                     launch::async,
#else
                     launch::deferred,
#endif
                     [&] () -> shared_ptr<Cycle<Type>> {
                        Type vertex = v.first;
                        shared_ptr<Cycle<Type>> ptr (new Cycle<Type>);
                        for (; !ptr->contains(vertex); ptr->push_back(vertex), vertex = predecessors[vertex]);
                        for (; *ptr->begin() != vertex; ptr->pop_front());

                        return ptr;
                     }));

   for (auto &f : futures)
      negative_cycles.insert(*f.get());

   for (const auto &c : negative_cycles)
      cycles.push_back(c);
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
