#pragma once

// Linked List Impl

#include <type_traits>
#include <cstdint>
#include <cassert>

#include "aii/allocator.hpp"
#include "aii/optional.hpp"

namespace Aii{

namespace Crtp{

template<typename D>
class ListNode{
  ListNode*& Next() noexcept{
    static_cast<D*>(this)->Next();
  }

  ListNode*& Head() noexcept{
    return this;
  };

  void InsertNext(ListNode* node) noexcept;
  void Append(ListNode* node) noexcept;
  void Remove(ListNode* node) noexcept;
  auto Extract(ListNode* node) noexcept -> ListNode*;
};

} // namespace Crtp

// Container version of a singly linked list
template<typename T, 
         typename A = Aii::Allocator<T>> 
class List{
  class Node: Crtp::ListNode<Node>{
    Node*& Next(){ return m_next;}
    T& Val(){ return m_val;}

    auto operator=(const Node*& src) noexcept -> Node&;
    auto operator=(Node*&& src) noexcept -> Node&;
    private:
      Node* m_next;
      T m_val;

  };

  using NodeAllocType = typename A::template Rebind<Node>::other;
  
  public:
    List() noexcept;
    List(std::size_t count) noexcept;
    List(std::size_t count, T val) noexcept;
    List(const List& src) noexcept;
    List(List&& other) noexcept;
    ~List() noexcept;
    List& operator=(const List& src) noexcept;
    List& operator=(List&& src) noexcept;

    constexpr Node*& Head() const noexcept{ return m_head;}

    bool Empty() const noexcept{ return m_head == nullptr;}

    void PushFront(Node* node) noexcept;
    void Append(Node* node) noexcept;
    void Remove(Node* node) noexcept;
    void Extract(Node* node) noexcept;

  private:
    NodeAllocType& NodeAllocator() noexcept{ return m_allocator;}
    void DeallocateElements() noexcept;

  private:
    Node* m_head;
    NodeAllocType m_allocator;
};

} // namespace Aii

// Crtp base class for list nodes impl

template<typename D>
void Aii::Crtp::ListNode<D>::InsertNext(ListNode* node) noexcept{
  // Theta(1)
  auto parentNode = this;
  auto afterNode = parentNode->Next();
  parentNode->Next() = node;
  if(node){
    node->Next(afterNode);
  }
}

template<typename D>
void Aii::Crtp::ListNode<D>::Append(ListNode* node) noexcept{
  // Theta(n)
  auto parentNode = this;
  auto indexNode = parentNode->Next();
  while(indexNode != nullptr){
    parentNode = indexNode;
    indexNode = parentNode->Next();
  }
  parentNode->Next() = node;
}

template<typename D>
void Aii::Crtp::ListNode<D>::Remove(ListNode* node) noexcept{
  // Theta(n)
  auto parentNode = this;
  auto indexNode = parentNode->Next();
  while(indexNode != nullptr || indexNode != node){
    parentNode = indexNode;
    indexNode = parentNode->Next();
  }
  if(indexNode == node){
    parentNode->Next() = indexNode->Next();
  }
}

template<typename D>
auto Aii::Crtp::ListNode<D>::Extract(ListNode* node) noexcept -> ListNode*{
  // Theta(n)
  auto parentNode = this;
  auto indexNode = parentNode->Next();
  while(indexNode != nullptr || indexNode != node){
    parentNode = indexNode;
    indexNode = parentNode->Next();
  }
  if(indexNode == node){
    parentNode->Next() = indexNode->Next();
    return node;
  }
  else{
    return nullptr;
  }
}

// Container version of linked list impl

template<typename T, typename A>
auto Aii::List<T, A>::Node::operator=(const Node*& src) noexcept -> Node&{
  Next() = src->Next();
  Val() = src->Val();
}

template<typename T, typename A>
auto Aii::List<T, A>::Node::operator=(Node*&& src) noexcept -> Node&{
  Next() = src->Next();
  Val() = src->Val();
  src->Next() = nullptr;
}

template<typename T, typename A>
Aii::List<T, A>::List() noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{

}

template<typename T, typename A>
Aii::List<T, A>::List(std::size_t count) noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{
  // allocates count elements using default initialisation
  if(count > 0){
    m_head = NodeAllocator().Allocate();
    count--;
  }
  while(count > 0){
    Head()->InsertNext(NodeAllocator().Allocate(T()));
    count--;
  }
}

template<typename T, typename A>
Aii::List<T, A>::List(std::size_t count, T val) noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{
  // allocates count elements using default initialisation
  if(count > 0){
    m_head = NodeAllocator().Allocate();
    count--;
  }
  while(count > 0){
    Head()->InsertNext(NodeAllocator().Allocate(val));
    count--;
  }
}

template<typename T, typename A>
Aii::List<T, A>::List(const List& other) noexcept
  :
    m_head{other.m_head},
    m_allocator{other.m_allocator}
{
}

template<typename T, typename A>
Aii::List<T, A>::List(List&& other) noexcept
  :
    m_head{std::move(other.m_head)},
    m_allocator{std::move(other.m_allocator)}
{
}

template<typename T, typename A>
void Aii::List<T, A>::DeallocateElements() noexcept{
  // O(n) where n is # elements in the list
  Node* parentNode = m_head;
  Node* indexNode = nullptr;
  if(parentNode){
    indexNode = parentNode->Next();
    NodeAllocator().Deallocate(parentNode);
  }
  while(indexNode){
    parentNode = indexNode;
    indexNode = parentNode->Next();
    NodeAllocator().Deallocate(parentNode);
  }
}

template<typename T, typename A>
Aii::List<T, A>::~List() noexcept{
  // O(n) where n is # elements in the list
  DeallocateElements();
}

template<typename T, typename A>
auto Aii::List<T, A>::operator=(const List& src) noexcept -> List&{
  // O(max{# elements in this, # elements in src})
  // loops through the caller's elements O(this) for deallocation and then loop through the 
  // source's elements O(other) to copy the elements into the caller
  DeallocateElements();
  Head() = src.Head();
  NodeAllocator() = src.NodeAllocator();
  Node* indexer = Head();
  Node* srcIndexer = src.Head();
  while(srcIndexer){
    Node* allocNode = NodeAllocator().Allocate(srcIndexer->Val());
    indexer->InsertNext(allocNode);
    srcIndexer = srcIndexer->Next();
  }
}

template<typename T, typename A>
auto Aii::List<T, A>::operator=(List&& src) noexcept -> List&{
  Head() = std::move(src.Head());
  NodeAllocator() = std::move(src.NodeAllocator());
}

template<typename T, typename A>
void Aii::List<T, A>::PushFront(Node* node) noexcept{
  Node* prevHead = Head();
  Head() = node;
  node->Next() = prevHead;
}

template<typename T, typename A>
void Aii::List<T, A>::Append(Node* node) noexcept{
  // Theta(n)
  Node* indexer = Head();
  if(!indexer){
    Head() = node;
  }
  while(indexer->Next()){
    indexer = indexer->Next();
  }
  indexer->Next() = node;
}

template<typename T, typename A>
void Aii::List<T, A>::Remove(Node* node) noexcept{
  // Theta(n)
  Node* indexer = Head();
  if(!indexer){
    return;
  }
  while(indexer){
    if(indexer->Next() == node){
      if(indexer->Next()){
        indexer->Next() = indexer->Next()->Next();
      }
      else{
        return;
      }
    }
    indexer = indexer->Next();
  }
}

template<typename T, typename A>
void Aii::List<T, A>::Extract(Node* node) noexcept{
  // Theta(n)
  Node* indexer = Head();
  if(!indexer){
    return;
  }
  while(indexer){
    if(indexer->Next() == node){
      if(indexer->Next()){
        indexer->Next() = indexer->Next()->Next();
      }
      return node;
    }
    indexer = indexer->Next();
  }
}

