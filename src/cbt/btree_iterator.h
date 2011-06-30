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
 * \file cgt/btree_iterator.h
 * \brief Contains btree_iterator definition.
 * \author Leandro Costa
 * \date 2011
 */

#ifndef CBTL_CBT_BTREE_ITERATOR_H_
#define CBTL_CBT_BTREE_ITERATOR_H_

#include <utility>
#include <stack>

#include <glog/logging.h>

namespace cbt {
    template<typename _TpKey, typename _TpValue, uint8_t _order>
        class btree_node;

    /*! 
     * \class btree_iterator
     * \brief The btree_iterator class template.
     * \author Leandro Costa
     * \date 2011
     *
     * A btree_iterator that points to a tree_node and returns std::pair<_TpKey, _TpValue>.
     */

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        class btree_iterator {
            private:
                typedef btree_node<_TpKey, _TpValue, _order> node;

            public:
                btree_iterator() : ptr_(NULL), idx_(0) { }
                btree_iterator(node* ptr, uint8_t idx, std::stack<uint8_t>
                        idx_stack) : ptr_(ptr), idx_(idx), idx_stack_(idx_stack) { }

            private:
                void _incr() {
                    DLOG(INFO) << "old key (idx_: " << int(idx_) << "): " << ptr_->items_[idx_].first;
                    if (idx_+1 < node::max_num_nodes && ptr_->nodes_[idx_+1]) {
                        DLOG(INFO) << "node found";
                        idx_stack_.push(idx_+1);
                        ptr_ = ptr_->nodes_[idx_+1];
                        idx_ = 0;

                        while (ptr_->nodes_[0]) {
                            idx_stack_.push(idx_);
                            ptr_ = ptr_->nodes_[0];
                        }
                    } else if (idx_+1 < ptr_->num_items_) {
                        idx_++;
                    } else if (ptr_->parent_) {
                        ptr_ = ptr_->parent_;
                        idx_ = idx_stack_.top();
                        idx_stack_.pop();

                        while (ptr_ && idx_ >= ptr_->num_items_) {
                            ptr_ = ptr_->parent_;

                            if (ptr_) {
                                idx_ = idx_stack_.top();
                                idx_stack_.pop();
                            } else {
                                idx_ = 0;
                            }
                        }
                    } else {
                        ptr_ = NULL;
                        idx_ = 0;
                    }
                }

            public:
                const bool operator==(const btree_iterator& other) const {
                    return (ptr_ == other.ptr_ && idx_ == other.idx_);
                }

                const bool operator!=(const btree_iterator& other) const {
                    return !operator==(other);
                }

                std::pair<_TpKey, _TpValue>& operator*() const {
                    return ptr_->items_[idx_];
                }

                std::pair<_TpKey, _TpValue>* operator->() const {
                    return &(operator*());
                }

                btree_iterator& operator++() {
                    _incr();
                    return *this;
                }

                const btree_iterator operator++(int i) {
                    btree_iterator it = *this;
                    _incr();
                    return it;
                }

            private:
                btree_node<_TpKey, _TpValue, _order>* ptr_;
                uint8_t idx_;
                std::stack<uint8_t> idx_stack_;
        };
}

#endif  // CBTL_CBT_BTREE_ITERATOR_H_
