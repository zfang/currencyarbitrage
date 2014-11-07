#ifndef CYCLE_H
#define CYCLE_H

#include <algorithm>
#include <deque>
#include <unordered_set>

using namespace std;

template <typename Type>
class Cycle;

template <typename Type>
struct cycle_hash {
   size_t operator() (const Cycle<Type>& cycle) const {
      size_t hashValue = 0;
      for (auto &t : cycle)
         hashValue ^= hash<Type>()(t);

      return hashValue;
   }
};

template<typename Type>
class Cycle {
   using cycle_list = deque<Type>;
   private:
      cycle_list cycle;
      unordered_set<Type> vertex_set;
      static const Type empty_node;
   public:
      int size() const;
      void push_back(const Type&);
      void pop_front();
      Type& front();
      const Type& front() const;
      void adjust_by_source(const Type&);
      bool contains(const Type&);
      void reverse();
      typename cycle_list::iterator begin() noexcept;
      typename cycle_list::const_iterator begin() const noexcept;
      typename cycle_list::iterator end() noexcept;
      typename cycle_list::const_iterator end() const noexcept;
      // Friends
      template <typename T>
      friend ostream &operator<<( ostream &, Directed_weighted_graph<T> const & );
};

template <typename Type>
const Type Cycle<Type>::empty_node = Type();

template<typename Type>
bool operator==(const Cycle<Type>& left, const Cycle<Type>& right) {
   cycle_hash<Type> hash_fn;
   return hash_fn(left) == hash_fn(right);
}

template<typename Type>
typename Cycle<Type>::cycle_list::iterator Cycle<Type>::begin() noexcept {
   return cycle.begin();
}

template<typename Type>
typename Cycle<Type>::cycle_list::const_iterator Cycle<Type>::begin() const noexcept {
   return cycle.begin();
}

template<typename Type>
typename Cycle<Type>::cycle_list::iterator Cycle<Type>::end() noexcept {
   return cycle.end();
}

template<typename Type>
typename Cycle<Type>::cycle_list::const_iterator Cycle<Type>::end() const noexcept {
   return cycle.end();
}

template<typename Type>
int Cycle<Type>::size() const {
   return cycle.size();
}

template<typename Type>
void Cycle<Type>::push_back(const Type& element) {
   cycle.push_back(element);
   vertex_set.insert(element);
}

template<typename Type>
void Cycle<Type>::pop_front() {
   cycle.pop_front();
}

template<typename Type>
Type& Cycle<Type>::front() {
   return cycle.front();
}

template<typename Type>
const Type& Cycle<Type>::front() const {
   return cycle.front();
}

template<typename Type>
void Cycle<Type>::adjust_by_source(const Type& source) {
   if (find(cycle.begin(), cycle.end(), source) != cycle.end())
      while (cycle.front() != source) {
         cycle.push_back(cycle.front());
         cycle.pop_front();
      }
}

template<typename Type>
bool Cycle<Type>::contains(const Type& element) {
   return vertex_set.count(element) > 0;
}

template<typename Type>
void Cycle<Type>::reverse() {
   std::reverse(cycle.begin(), cycle.end());
}

template <typename T>
ostream &operator << ( ostream &out, Cycle<T> const &cycle ) {
   for (auto &i : cycle)
      out << i << " ";
   return out;
}
#endif
