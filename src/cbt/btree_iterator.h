/*
 * CBTL - A btree template library for C++
 * ---------------------------------------
 * Copyright (C) 2011 Leandro Costa
 *
 * This file is part of CBTL.
 *
 * CBTL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * CBTL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with CBTL. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file cgt/_BTreeIterator.h
 * \brief Contains _BTreeIterator definition.
 * \author Leandro Costa
 * \date 2011
 */

#ifndef CBTL_CBT_BTREE_ITERATOR_H_
#define CBTL_CBT_BTREE_ITERATOR_H_

#include <utility>

#include "glog/logging.h"

namespace cbt {
  template<typename _TpKey, typename _TpValue, uint8_t _order>
    class _BTreeNode;

  /*! 
   * \class _BTreeIterator
   * \brief The _BTreeIterator class template.
   * \author Leandro Costa
   * \date 2011
   *
   * A _BTreeIterator that points to a tree_node and returns std::pair<_TpKey, _TpValue>.
   */

  template<typename _TpKey, typename _TpValue, uint8_t _order>
    class _BTreeIterator {
      private:
        typedef _BTreeNode<_TpKey, _TpValue, _order> _Node;

      public:
        _BTreeIterator() : ptr_(NULL), idx_(0) { }
        _BTreeIterator(_Node* ptr, uint8_t idx = 0) : ptr_(ptr), idx_(idx) { }

      private:
        void _incr();

      public:
        const bool operator==(const _BTreeIterator& other) const {
          return (ptr_ == other.ptr_ && idx_ == other.idx_);
        }

        const bool operator!=(const _BTreeIterator& other) const {
          return !operator==(other);
        }

        std::pair<_TpKey, _TpValue>& operator*() const {
          return ptr_->item(idx_);
        }

        std::pair<_TpKey, _TpValue>* operator->() const {
          return &(operator*());
        }

        _BTreeIterator& operator++() {
          _incr();
          return *this;
        }

        const _BTreeIterator operator++(int i) {
          _BTreeIterator it = *this;
          _incr();
          return it;
        }

      private:
        _Node* ptr_;
        uint8_t idx_;
    };

  template<typename _TpKey, typename _TpValue, uint8_t _order>
    void _BTreeIterator<_TpKey, _TpValue, _order>::_incr() {
      if (idx_+1 < _Node::MAX_NUM_NODES && ptr_->node(idx_+1)) {
        ptr_ = ptr_->node(idx_+1);
        idx_ = 0;

        while (ptr_->node(0)) {
          ptr_ = ptr_->node(0);
        }
      } else if (idx_+1 < ptr_->num_items()) {
        idx_++;
      } else {
        _TpKey k_old = ptr_->item(idx_).first;

        ptr_ = ptr_->parent();
        idx_ = 0;

        while (ptr_ && ptr_->item(idx_).first < k_old) {
          while (idx_ < ptr_->num_items() && ptr_->item(idx_).first < k_old)
            idx_++;

          if (idx_ == ptr_->num_items()) {
            ptr_ = ptr_->parent();
            idx_ = 0;
          }
        }
      }
    }
}

#endif  // CBTL_CBT_BTREE_ITERATOR_H_
