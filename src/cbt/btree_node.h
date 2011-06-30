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

#ifndef CBTL_CBT_BTREE_NODE_H_
#define CBTL_CBT_BTREE_NODE_H_

#include <stdint.h>
#include <cstring>

#include <glog/logging.h>

namespace cbt {
    template<typename _TpKey, typename _TpValue, uint8_t _order>
        class btree;

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        class btree_iterator;

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        class btree_node {
            private:
                friend class btree<_TpKey, _TpValue, _order>;
                friend class btree_iterator<_TpKey, _TpValue, _order>;

                const static uint8_t max_num_items = 2*_order;
                const static uint8_t max_num_nodes = max_num_items+1;

            public:
                btree_node() : parent_(NULL), num_items_(0) {
                    memset(&nodes_, 0, max_num_nodes * sizeof(btree_node*));
                }

            private:
                const bool is_leaf() const { return (nodes_[0] == NULL); }
                const bool empty() const { return (num_items_ == 0); }
                void set_parent(btree_node* p_node) { parent_ = p_node; }

                void insert(const _TpKey& key, const _TpValue& value, btree_node* p_right_node = NULL);
                void insert(const std::pair<_TpKey, _TpValue>& item);
                void insert(const std::pair<_TpKey, _TpValue>& item, btree_node* p_right_node);

            private:
                btree_node* parent_;
                btree_node* nodes_[max_num_nodes];

                std::pair<_TpKey, _TpValue> items_[max_num_items];

                uint8_t num_items_;
        };

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree_node<_TpKey, _TpValue, _order>::insert(const _TpKey& key, const _TpValue& value, btree_node* p_right_node) {
            DLOG(INFO) << "btree_node with " << int(num_items_) << " items inserting item (" << key << ", " << value << ")";
            uint8_t i = num_items_;

            while (i > 0 && items_[i-1].first > key) {
                items_[i] = items_[i-1];
                nodes_[i+1] = nodes_[i];
                i--;
            }

            items_[i] = std::make_pair(key, value);
            nodes_[i+1] = p_right_node;
            num_items_++;
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree_node<_TpKey, _TpValue, _order>::insert(const std::pair<_TpKey, _TpValue>& item) {
            insert(item.first, item.second);
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree_node<_TpKey, _TpValue, _order>::insert(const std::pair<_TpKey, _TpValue>& item, btree_node* p_right_node) {
            insert(item.first, item.second, p_right_node);
        }
}

#endif  // CBTL_CBT_BTREE_NODE_H_
