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

#include <glog/logging.h>

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
                btree() : root_(new node()) { }

            public:
                iterator begin();
                iterator end() { return iterator(); }
                iterator find(const _TpKey& key);

                void insert(const _TpKey& key, const _TpValue& value);
                const bool empty() const { return root_->empty(); }

            private:
                node* root_;
        };

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree<_TpKey, _TpValue, _order>::insert(const _TpKey& key,
                const _TpValue& value) {
            if (root_->is_leaf()) {
                if (root_->num_items_ < node::max_num_items) {
                    root_->insert(key, value);
                } else {
                    std::pair<_TpKey, _TpValue> item = std::make_pair(key, value);

                    for (uint8_t idx = 0; idx < _order; idx++) {
                        if (item.first < root_->items_[idx].first) {
                            std::pair<_TpKey, _TpValue> tmp = item;
                            item = root_->items_[idx];
                            root_->items_[idx] = tmp;
                        }
                    }

                    for (uint8_t idx = node::max_num_items-1; idx >= _order; idx--) {
                        if (item.first > root_->items_[idx].first) {
                            std::pair<_TpKey, _TpValue> tmp = item;
                            item = root_->items_[idx];
                            root_->items_[idx] = tmp;
                        }
                    }

                    node* p_new_right_node = new node();

                    for (uint8_t idx = _order; idx < root_->num_items_; idx++) {
                        p_new_right_node->insert(root_->items_[idx].first, root_->items_[idx].second);
                    }

                    root_->num_items_ = _order;

                    node* new_root = new node();
                    new_root->insert(item.first, item.second);
                    new_root->nodes_[0] = root_;
                    new_root->nodes_[1] = p_new_right_node;

                    root_->set_parent(new_root);
                    p_new_right_node->set_parent(new_root);
                    root_ = new_root;
                }
            } else {
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey,
                 _TpValue, _order>::begin() {
            if (!root_->empty()) {
                node* p_node = root_;
                std::stack<uint8_t> idx_stack;

                while (p_node->nodes_[0]) {
                    idx_stack.push(0);
                    p_node = p_node->nodes_[0];
                }

                uint8_t idx = 0;
                return iterator(p_node, idx, idx_stack);
            } else {
                return iterator();
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey,
                 _TpValue, _order>::find(const _TpKey& key) {
            btree_node<_TpKey, _TpValue, _order>* p_node = root_;

            uint8_t idx = 0;

            while (idx < p_node->num_items_ && p_node->items_[idx].first < key)
                idx++;

            if (idx < p_node->num_items_ && p_node->items_[idx].first == key)
                return iterator(p_node, idx, std::stack<uint8_t>());
            else
                return iterator();
        }
}

#endif  // CBTL_CBT_BTREE_H_
