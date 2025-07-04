#pragma once
// Double Linked List impl

#include "aii/allocator.hpp"

namespace Aii{

namespace Crtp{

template<typename D>
class DoubleListNode{
  // Semantics:
  //  Size one list => Head() && !Next() && !Prev() 
  //  Size two list => Head() && (Next() == Prev() != null)
  DoubleListNode*& Head() noexcept{
    return this;
  };

  DoubleListNode*& Next() noexcept{
    static_cast<D*>(this)->Next();
  }

  DoubleListNode*& Prev() noexcept{
    static_cast<D*>(this)->Next();
  }

  void InsertNext(DoubleListNode* node) noexcept;
  void Append(DoubleListNode* node) noexcept;
  void Remove(DoubleListNode* node) noexcept;
  auto Extract(DoubleListNode* node) noexcept -> DoubleListNode*;
};

} // namespace Crtp
  
// Composition type
template<typename T>
class DoubleListNode: Crtp::DoubleListNode<DoubleListNode<T>>{
  public:
    DoubleListNode(T val): m_next{nullptr}, m_prev{nullptr}, m_val{val}{}
    DoubleListNode*& Head() noexcept{ return this;}
    DoubleListNode*& Next() noexcept{ return m_next;}
    DoubleListNode*& Prev() noexcept{ return m_prev;}

  private:
    DoubleListNode* m_prev;
    DoubleListNode* m_next;
    T m_val;
};

// Container type
template<
  typename T, 
  typename A = Aii::Allocator<T>> 
class DoubleList{
  class Node: Crtp::DoubleListNode<Node>{

    Node(T val): m_next{nullptr}, m_prev{nullptr}, m_val{val}{}
    Node(const Node& src) noexcept;
    Node(Node&& src) noexcept;

    Node*& Next(){ return m_next;}
    Node*& Prev(){ return m_prev;}

    T& Val(){ return m_val;}
    private:
      Node* m_next;
      Node* m_prev;
      T m_val;
  };

  using NodeAllocType = 
    typename A::template Rebind<Node>::other;

  public:
    DoubleList() noexcept;
    DoubleList(std::size_t count) noexcept;
    DoubleList(std::size_t count, T val) noexcept;
    DoubleList(const DoubleList& src) noexcept;
    DoubleList(DoubleList&& other) noexcept;
    ~DoubleList() noexcept;
    void Swap(DoubleList& src) noexcept;
    DoubleList& operator=(const DoubleList& src) noexcept;
    DoubleList& operator=(DoubleList&& src) noexcept;

    constexpr Node*& Head() const noexcept{ return m_head;}

    bool Empty() const noexcept{ return m_head == nullptr;}

    void PushFront(Node* node) noexcept;
    void Append(Node* node) noexcept;
    void Remove(Node* node) noexcept;
    auto Extract(Node* node) noexcept -> Node*;

    template<typename ...Args>
    void EmplaceBack(Args ...args) noexcept;
    template<typename ...Args>
    void EmplaceFront(Args ...args) noexcept;

  private:
    NodeAllocType& NodeAllocator() noexcept{ return m_allocator;}
    void DeallocateElements() noexcept;

  private:
    Node* m_head;
    NodeAllocType m_allocator;
};

}

// Crtp Base Class

template<typename D>
void Aii::Crtp::DoubleListNode<D>::InsertNext(DoubleListNode* node) noexcept{
  // theta(1)
  if(!Next()){
    Next() = node;
    Prev() = node;
    node->Next() = this;
    node->Prev() = this;
  }
  else{
    DoubleListNode oldNext = Next();
    Next() = node;
    node->Next() = oldNext;
    node->Prev() = this;
    oldNext->Prev() = node;
  }
}

template<typename D>
void Aii::Crtp::DoubleListNode<D>::Append(DoubleListNode* node) noexcept{
  // theta(1)
  if(!Prev()){
    Next() = node;
    Prev() = node;
    node->Next() = this;
    node->Prev() = this;
  }
  else{
    DoubleListNode oldPrev = Prev();
    Prev() = node;
    node->Next() = this;
    node->Prev() = oldPrev;
    oldPrev->Next() = node;
  }
}

template<typename D>
void Aii::Crtp::DoubleListNode<D>::Remove(DoubleListNode* node) noexcept{
  // theta(1)
  if(!Prev()){
    return;
  }
  else{
    Extract(node);
    return;
  }
}

template<typename D>
auto Aii::Crtp::DoubleListNode<D>::Extract(DoubleListNode* node) 
noexcept -> DoubleListNode*
{
  // theta(1)
  DoubleListNode* indexer = Head();
  if(!indexer){
    return nullptr;
  }
  while(indexer->Next()){
    if(indexer->Next() == node){
      indexer->Next() = indexer->Next()->Next();
      node->Next() = nullptr;
      node->Prev() = nullptr;
      return node;
    }
    indexer = indexer->Next();
  }
  return nullptr;
}

// Container Impl

template<typename T, typename A>
Aii::DoubleList<T, A>::Node::Node(const Node& src) noexcept
  :
    m_next{src.Next()},
    m_prev{src.m_prev()},
    m_val{src.Val()}
{

}

template<typename T, typename A>
Aii::DoubleList<T, A>::Node::Node(Node&& src) noexcept
  :
    m_next{src.Next()},
    m_prev{src.m_prev()},
    m_val{std::move(src.Val())}
{
  src.Next() = nullptr;
  src.Prev() = nullptr;
}

template<typename T, typename A>
Aii::DoubleList<T,A>::DoubleList() noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{

}

template<typename T, typename A>
Aii::DoubleList<T,A>::DoubleList(std::size_t count) noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{
  Head() = NodeAllocator().Allocate();
  for(std::size_t i = 1; i < count; i++){
    Head().Append(NodeAllocator().Allocate());
  }
}

template<typename T, typename A>
Aii::DoubleList<T,A>::DoubleList(std::size_t count, T val) noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{
  Head() = NodeAllocator().Allocate(val);
  for(std::size_t i = 1; i < count; i++){
    Head().Append(NodeAllocator().Allocate(val));
  }
}

template<typename T, typename A>
Aii::DoubleList<T,A>::DoubleList(const DoubleList& src) noexcept
  :
    m_head{nullptr},
    m_allocator{NodeAllocType()}
{
  Head() = NodeAllocator().Allocate(src.Head().Val());
  Node* srcIndexer = src.Head();
  while(srcIndexer->Next()){
    Head().Append(NodeAllocator().Allocate(srcIndexer->Next()));
    srcIndexer = srcIndexer->Next();
  }
}

template<typename T, typename A>
Aii::DoubleList<T,A>::DoubleList(DoubleList&& src) noexcept
  :
    m_head{src.Head()},
    m_allocator{NodeAllocType()}
{
  src.Head() = nullptr;
}

template<typename T, typename A>
void Aii::DoubleList<T, A>::DeallocateElements() noexcept{
  Node* indexer = Head();
  Node* prev = indexer;
  while(indexer){
    indexer = indexer->Next();
    NodeAllocType().Deallocate(prev);
    prev = indexer;
  }
}

template<typename T, typename A>
Aii::DoubleList<T,A>::~DoubleList() noexcept{
  DeallocateElements();
}

template<typename T, typename A>
void Aii::DoubleList<T,A>::Swap(DoubleList& src) noexcept{
  Node prevHead = Head();
  NodeAllocType prevAlloc = NodeAllocator();
  Head() = src.Head();
  NodeAllocator() = src.NodeAllocator();
  src.Head() = prevHead;
  src.NodeAllocator() = prevAlloc;
}

template<typename T, typename A>
auto Aii::DoubleList<T, A>::operator=(const DoubleList& src)
noexcept -> DoubleList&{
  DoubleList tmp{src};
  Swap(tmp);
}

template<typename T, typename A>
auto Aii::DoubleList<T, A>::operator=(DoubleList&& src)
noexcept -> DoubleList&{
  DoubleList tmp{std::move(src)};
  Swap(tmp);
}

template<typename T, typename A>
void Aii::DoubleList<T, A>::PushFront(Node* node) noexcept{
  Head()->InsertNext(node);
}

template<typename T, typename A>
void Aii::DoubleList<T, A>::Append(Node* node) noexcept{
  Head()->Append(node);
}

template<typename T, typename A>
void Aii::DoubleList<T, A>::Remove(Node* node) noexcept{
  Head()->Remove(node);
}

template<typename T, typename A>
auto Aii::DoubleList<T, A>::Extract(Node* node) noexcept -> Node*{
  return Head()->Extract(node);
}

template<typename T, typename A> template<typename ...Args>
void Aii::DoubleList<T, A>::EmplaceBack(Args ...args) noexcept{
  Head()->Append(NodeAllocator().Allocate(args...));
}

template<typename T, typename A> template<typename ...Args>
void Aii::DoubleList<T, A>::EmplaceFront(Args ...args) noexcept{
  Head()->InsertNext(NodeAllocator().Allocate(args...));
}
