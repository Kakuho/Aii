#pragma once

// List node type to allow user defined data structures the semantics of 
// being part of multiple linked lists - composition.

namespace Aii{

template<typename T>
class ListNode{
  public:
    ListNode*& Next() const noexcept{ return m_next;}
    ListNode*& Next() noexcept{ return m_next;}
    constexpr ListNode*& Head() const noexcept{ return this;}
    constexpr ListNode*& Head() noexcept{ return this;}

    bool Empty() const noexcept{ return m_next == nullptr;}

    void PushFront(ListNode* node) noexcept;
    void Append(ListNode* node) noexcept;
    void Remove(ListNode* node) noexcept;
    void Extract(ListNode* node) noexcept;
  private:
    ListNode* m_next;
    T m_val;
};

} // namespace Aii

template<typename T>
void Aii::ListNode<T>::PushFront(ListNode* node) noexcept{
  ListNode* prevHead = Head();
  Head() = node;
  node->Next() = prevHead;
}

template<typename T>
void Aii::ListNode<T>::Append(ListNode* node) noexcept{
  // Theta(n)
  ListNode* indexer = Head();
  if(!indexer){
    Head() = node;
  }
  while(indexer->Next()){
    indexer = indexer->Next();
  }
  indexer->Next() = node;
}

template<typename T>
void Aii::ListNode<T>::Remove(ListNode* node) noexcept{
  // Theta(n)
  ListNode* indexer = Head();
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

template<typename T>
void Aii::ListNode<T>::Extract(ListNode* node) noexcept{
  // Theta(n)
  ListNode* indexer = Head();
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
