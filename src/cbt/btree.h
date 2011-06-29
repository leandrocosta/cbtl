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
 * \file cgt/btree.h
 * \brief Contains btree definition. The only file that needs to be included in source code.
 * \author Leandro Costa
 * \date 2011
 */

#ifndef CBTL_CBT_BTREE_H_
#define CBTL_CBT_BTREE_H_

#include "cbt/btree_node.h"
#include "cbt/btree_iterator.h"

namespace cbt {

    /*! 
     * \class btree
     * \brief The btree class template.
     * \author Leandro Costa
     * \date 2011
     *
     * A btree with keys of type \b _TpKey, and values of type \b _TpValue.
     */

    template<typename _TpKey, typename _TpValue, uint8_t _order = 1>
        class btree {
            private:
                typedef btree_node<_TpKey, _TpValue, _order> node;

            public:
                typedef btree_iterator<_TpKey, _TpValue, _order> iterator;

            public:
                iterator begin();
                iterator end() { return iterator(); }
                iterator find(const _TpKey& key);

                void insert(const _TpKey& key, const _TpValue& value);
                const bool empty() const { return root_.empty(); }

            private:
                node root_;
        };

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree<_TpKey, _TpValue, _order>::insert(const _TpKey& key, const _TpValue& value) {
            if (root_.is_leaf()) {
                if (root_.num_items_ < node::max_num_items) {
                    root_.insert(key, value);
                } else {
                }
            } else {
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey, _TpValue, _order>::begin() {
            if (!root_.empty()) {
                node& n = root_;
                uint8_t idx = 0;
                return iterator(&n, idx);
            } else {
                return iterator();
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey, _TpValue, _order>::find(const _TpKey& key) {
            btree_node<_TpKey, _TpValue, _order>& node = root_;

            uint8_t idx = 0;

            while (idx < node.num_items_ && node.items_[idx].first < key)
                idx++;

            if (idx < node.num_items_ && node.items_[idx].first == key)
                return iterator(&node, idx);
            else
                return iterator();
        }
}

#endif  // CBTL_CBT_BTREE_H_
