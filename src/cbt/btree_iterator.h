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
                btree_iterator(btree_node<_TpKey, _TpValue, _order>* ptr, uint8_t idx) : ptr_(ptr), idx_(idx) { }

            private:
                void _incr() {
                    if (idx_ < node::max_num_nodes && ptr_->nodes_[idx_+1]) {
                        throw "not implemented";
                    } else if (idx_+1 < ptr_->num_items_) {
                        idx_++;
                    } else if (ptr_->parent_) {
                        throw "not implemented";
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

                const btree_iterator operator++(int) {
                    btree_iterator it = *this;
                    _incr();
                    return it;
                }

            private:
                btree_node<_TpKey, _TpValue, _order>* ptr_;
                uint8_t idx_;
        };
}

#endif  // CBTL_CBT_BTREE_ITERATOR_H_
