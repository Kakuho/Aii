#pragma once

// for now it holds int as keys, however
// later will refactor to allow using any keys which can be compared

#include "aii/functional.hpp"
#include "aii/allocator.hpp"
#include "aii/expected.hpp"
#include "aii/error.hpp"

namespace Aii{

namespace Details{
  enum class Colour{
    Black, Red
  };

  template<typename K, typename T, typename C>
  struct RbNode{
    RbNode* parent;
    RbNode* left;
    RbNode* right;
    Colour colour;
    K key;
  };
} // namespace Details

template<
  typename K, 
  typename T, 
  typename C = Aii::LessThan<K>,
  typename Allocator = Aii::Allocator<Details::RbNode<K, T, C>>
> class RbTree{
  public:
    using KeyType = K;
    using ValueType = T;
    using ComparisonType = C;
    using Colour = Details::Colour;
    using Node = Details::RbNode<K, T, C>;

  public:
    RbTree() noexcept;
    RbTree(KeyType rootKey) noexcept;
  
    constexpr Node* Root() const noexcept{ return m_root;}
    constexpr C& Comparer() const noexcept{ return m_cmp;}

    Aii::Expected<void, Error> Insert(Node* node) noexcept;
    Aii::Expected<void, Error> Delete(Node* node) noexcept;

    T& operator[](const K& key) noexcept;
    T& operator[](K&& key) noexcept;

  private:
    Aii::Expected<void, Error> LeftRotate(Node* node) noexcept;
    Aii::Expected<void, Error> RightRotate(Node* node) noexcept;

    Aii::Expected<void, Error> InsertFixups(Node* node) noexcept;

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
  // first iterate to the bottom of the tree, and find the parent
  // to insert into
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
