#pragma once

// for now it holds int as keys, however
// later will refactor to allow using any keys which can be compared

#include "aii/functional.hpp"
#include "aii/allocator.hpp"
#include "aii/expected.hpp"
#include "aii/error.hpp"

namespace Aii{

namespace Details::RbTree{
  enum class Colour{
    Black, Red
  };

  template<typename K, typename T, typename C>
  struct Node{
    Node* parent;
    Node* left;
    Node* right;
    Colour colour;
    T val;
    K key;
  };
} // namespace Details::RbTree

template<
  typename K, 
  typename T, 
  typename C = Aii::LessThan<K>,
  typename Allocator = Aii::Allocator<Details::RbTree::Node<K, T, C>>
> class RbTree{
  public:
    using KeyType = K;
    using ValueType = T;
    using ComparisonType = C;
    using Colour = Details::RbTree::Colour;
    using Node = Details::RbTree::Node<K, T, C>;

  public:
    RbTree() noexcept;
    RbTree(KeyType rootKey) noexcept;
  
    constexpr Node* Root() const noexcept{ return m_root;}
    constexpr C& Comparer() const noexcept{ return m_cmp;}

    Node* Min() const noexcept{
      Node* indexer = m_root;
      while(indexer->left){
        indexer = indexer->left;
      }
      return indexer;
    }

    Node* Successor(Node* node) const noexcept{
      // successor is either node itself or the node with 
      // the minimal increased
      Node* indexer = node;
      while(indexer->left){
        indexer = indexer->left;
      }
      return indexer;
    }

    Node* Max() const noexcept{
      Node* indexer = m_root;
      while(indexer->right){
        indexer = indexer->right;
      }
      return indexer;
    }

    Aii::Expected<void, Error> Insert(Node* node) noexcept;
    Aii::Expected<void, Error> Delete(Node* node) noexcept;

    T& operator[](const K& key) noexcept;
    T& operator[](K&& key) noexcept;

  private:
    Aii::Expected<void, Error> LeftRotate(Node* node) noexcept;
    Aii::Expected<void, Error> RightRotate(Node* node) noexcept;

    Aii::Expected<void, Error> InsertFixups(Node* node) noexcept;
    Aii::Expected<void, Error> DeleteFixups(Node* node) noexcept;

    Aii::Expected<void, Error> Transplant(Node* old, Node* graft) noexcept;

    constexpr bool Compare(const KeyType& a, const KeyType& b) const noexcept
    { return m_cmp(a, b);}

  private:
    Node* m_root;
    ComparisonType m_cmp;
};

} // namespace Aii

template<typename K, typename T, typename C, typename A>
Aii::RbTree<K, T, C, A>::RbTree() noexcept
  :
    m_root{nullptr}
{

}

template<typename K, typename T, typename C, typename A>
Aii::RbTree<K, T, C, A>::RbTree(KeyType rootKey) noexcept
  :
    m_root{
      .parent = nullptr, 
      .left = nullptr, 
      .right = nullptr, 
      .colour = Colour::Black,
      .key = rootKey
    }
{

}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error> 
Aii::RbTree<K, T, C, A>::LeftRotate(Node* node) noexcept{
  // Possible Errors:
  //  InvalidArgument
  //  
  // On error, the tree is left in the state before the function call
  if(!node){
    return {Aii::Error::InvalidArgument};
  }
  Node* pivot = node->right;
  node->right = pivot->left;
  if(node->right){
    node->right->parent = node;
  }
  pivot->parent = node->parent;
  // case on pivot's new parent
  // we need to replace node by pivot in the parent chain
  if(!pivot->parent){
    m_root = pivot;
  }
  else if(pivot->parent->left == node){
    pivot->parent->left = pivot;
  }
  else{
    pivot->parent->right = pivot;
  }
  pivot->left = node;
  node->parent = pivot;
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error> 
Aii::RbTree<K, T, C, A>::RightRotate(Node* node) noexcept{
  // Possible Errors:
  //  InvalidArgument
  //  
  // On error, the tree is left in the state before the function call
  if(!node){
    return {Aii::Error::InvalidArgument};
  }
  Node* pivot = node->left;
  node->left = pivot->right;
  if(node->left){
    node->left->parent = node;
  }
  pivot->parent = node->parent;
  // case on pivot's new parent
  // we need to replace node by pivot in the parent chain
  if(!pivot->parent){
    m_root = pivot;
  }
  else if(pivot->parent->left == node){
    pivot->parent->left = pivot;
  }
  else{
    pivot->parent->right = pivot;
  }
  pivot->right = node;
  node->parent = pivot;
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error>
Aii::RbTree<K, T, C, A>::Insert(Node* node) noexcept{
  //  Time Complexity:
  //    * O(logn)
  //
  //  Possible Errors:
  //    * InvalidArgument
  //
  //  Description:
  //    * First iterate to the bottom of the tree, and find the parent
  //      to insert into. 
  //    * Then Perform InsertFixups(node) in order to perserve the properties of 
  //      Rb Trees on exit.
  if(!node){
    return Error::InvalidArgument;
  }
  Node* indexer = Root();
  Node* prev = indexer ? indexer->parent : nullptr;
  while(indexer){
    prev = indexer;
    if(Comparer()(node->key, indexer->key)){
      indexer = indexer->left;
    }
    else{
      indexer = indexer->right;
    }
  }
  if(!prev){
    m_root = node;
    m_root->colour = Colour::Black;
    return {};
  }
  else if(Comparer()(node->key, prev->key)){
    prev->left = node;
  }
  else{
    prev->right = node;
  }
  node->parent = prev;
  node->left = nullptr;
  node->right = nullptr;
  node->colour = Colour::Red;
  InsertFixups(node);
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error>
Aii::RbTree<K, T, C, A>::InsertFixups(Node* node) noexcept{
  while(node->parent->colour == Colour::Red){
    if(node->parent == node->parent->parent->left){
      Node* uncle = node->parent->parent->right;
      if(uncle->colour == Colour::Red){
        uncle->colour = Colour::Black;
        node->parent->colour = Colour::Black;
        node->parent->parent->colour = Colour::Red;
        node = node->parent->parent;
      }
      else{
        if(node == node->parent->right){
          node = node->parent;
          LeftRotate(node);
        }
        node->parent->colour = Colour::Black;
        node->parent->parent->colour = Colour::Red;
        RightRotate(node->parent->parent);
      }
    }
    else{
      Node* uncle = node->parent->parent->left;
      if(uncle->colour == Colour::Red){
        uncle->colour = Colour::Black;
        node->parent->colour = Colour::Black;
        node->parent->parent->colour = Colour::Red;
        node = node->parent->parent;
      }
      else{
        if(node == node->parent->left){
          node = node->parent;
          RightRotate(node);
        }
        node->parent->colour = Colour::Black;
        node->parent->parent->colour = Colour::Red;
        LeftRotate(node->parent->parent);
      }
    }
  }
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error> 
Aii::RbTree<K, T, C, A>::Transplant(Node* old, Node* graft) noexcept{
  //  Time Complexity:
  //    * Theta(1)
  //
  //  Possible Errors:
  //    * InvalidArgument
  //
  //  Description:
  //    Replaces the sub tree rooted at old by the sub tree rooted at graft.
  if(!old || !graft){
    return Aii::Error::InvalidArgument;
  }
  if(old->parent == nullptr){
    m_root = graft;
  }
  else if(old == old->parent->left){
    old->parent->left = graft;
  }
  else{
    old->parent->right = graft;
  }
  graft->parent = old->parent;
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error> 
Aii::RbTree<K, T, C, A>::Delete(Node* node) noexcept{
  Node* y = node;
  Node* x = nullptr;
  Colour originalColour = y->colour;
  // branch depending on whether the node has children
  if(!node->left){
    x = node->right;
    Transplant(node, node->right);  // BUT node->right MAYBE nullptr
  }
  else if(!node->right){
    x = node->left;
    Transplant(node, node->left);   // BUT node->left MAYBE nullptr
  }
  else{
    y = Successor(node);
    originalColour = y->colour;
    x = y->right;                   // MAYBE nullptr
    if(y != node->right){
      Transplant(y, y->right);      // y->right MAYBE nullptr
      y->right = node->right;
      y->right->parent = y;
    }
    else{
      x->parent = y;
    }
    Transplant(node, y);
    y->left = node->left;
    y->left->parent = y;
    y->colour = node->colour;
  }
  // end of case analysis
  if(originalColour == Colour::Black){
    DeleteFixups(x);

  }
}

template<typename K, typename T, typename C, typename A>
Aii::Expected<void, Aii::Error> 
Aii::RbTree<K, T, C, A>::DeleteFixups(Node* node) noexcept{

}
