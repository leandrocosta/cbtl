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

            private:
                node* get_node_to_insert(const _TpKey& key) const {
                    node* p_node = root_;

                    while (! p_node->is_leaf()) {
                        DLOG(INFO) << "node is not leaf";
                        uint8_t i = 0;

                        while (i < p_node->num_items_ && p_node->items_[i].first < key) {
                            DLOG(INFO) << "key " << p_node->items_[i].first << " is less than " << key;
                            i++;
                        }

                        p_node = p_node->nodes_[i];
                    }

                    return p_node;
                }

                /*
                void insert_into_this_node(node* p_node, const std::pair<_TpKey, _TpValue>& item, node* p_right_node) {
                    if (p_node->num_items_ < node::max_num_items) {
                        DLOG(INFO) << "we have space in this node";
                        p_node->insert(key, value, p_right_node);
                    } else { // we need to split this one
                        // selecting item to rise
                        std::pair<_TpKey, _TpValue> item_to_rise = item;

                        for (uint8_t idx = 0; idx < _order; idx++) {
                            if (item_to_rise.first < p_node->items_[idx].first) {
                                std::pair<_TpKey, _TpValue> tmp = item_to_rise;
                                item_to_rise = p_node->items_[idx];
                                p_node->items_[idx] = tmp;
                            }
                        }

                        for (uint8_t idx = node::max_num_items-1; idx >= _order; idx--) {
                            if (item_to_rise.first > p_node->items_[idx].first) {
                                std::pair<_TpKey, _TpValue> tmp = item_to_rise;
                                item_to_rise = p_node->items_[idx];
                                p_node->items_[idx] = tmp;
                            }
                        }

                        // creating new node to split
                        node* p_new_right_node = new node();

                        for (uint8_t idx = _order; idx < p_node->num_items_; idx++) {
                            p_new_right_node->insert(p_node->items_[idx].first, p_node->items_[idx].second);
                        }

                        p_node->num_items_ = _order;

                        if (root_ == p_node) { // if this is root, create new root and update with item to rise
                            node* new_root = new node();
                            new_root->insert(item_to_rise.first, item_to_rise.second);
                            new_root->nodes_[0] = p_node;
                            new_root->nodes_[1] = p_new_right_node;

                            p_node->set_parent(new_root);
                            p_new_right_node->set_parent(new_root);
                            root_ = new_root;
                        } else { // updating parent with item to rise
                            node* p_parent = p_node->parent_;

                            if (p_parent->num_items_ < node::max_num_items) {
                                p_parent->insert(item_to_rise, p_new_right_node);
                            } else { // we need to split this one to insert item_to_rise
                            }
                        }
                    }
                }
                */

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
            DLOG(INFO) << "inserting (" << key << ", " << value << ")";

            node* p_node = get_node_to_insert(key);

            DLOG(INFO) << "now node is leaf";

            if (p_node->num_items_ < node::max_num_items) {
                DLOG(INFO) << "we have space in this node";
                p_node->insert(key, value);
            } else { // we need to split this one
                // selecting item to rise
                std::pair<_TpKey, _TpValue> item_to_rise = std::make_pair(key, value);

                for (uint8_t idx = 0; idx < _order; idx++) {
                    if (item_to_rise.first < p_node->items_[idx].first) {
                        std::pair<_TpKey, _TpValue> tmp = item_to_rise;
                        item_to_rise = p_node->items_[idx];
                        p_node->items_[idx] = tmp;
                    }
                }

                for (uint8_t idx = node::max_num_items-1; idx >= _order; idx--) {
                    if (item_to_rise.first > p_node->items_[idx].first) {
                        std::pair<_TpKey, _TpValue> tmp = item_to_rise;
                        item_to_rise = p_node->items_[idx];
                        p_node->items_[idx] = tmp;
                    }
                }

                // creating new node to split
                node* p_new_right_node = new node();

                for (uint8_t idx = _order; idx < p_node->num_items_; idx++) {
                    p_new_right_node->insert(p_node->items_[idx].first, p_node->items_[idx].second);
                }

                p_node->num_items_ = _order;

                if (root_ == p_node) { // if this is root, create new root and update with item to rise
                    node* new_root = new node();
                    new_root->insert(item_to_rise.first, item_to_rise.second);
                    new_root->nodes_[0] = p_node;
                    new_root->nodes_[1] = p_new_right_node;

                    p_node->set_parent(new_root);
                    p_new_right_node->set_parent(new_root);
                    root_ = new_root;
                } else { // updating parent with item to rise
                    node* p_parent = p_node->parent_;

                    if (p_parent->num_items_ < node::max_num_items) {
                        p_parent->insert(item_to_rise, p_new_right_node);
                    } else { // we need to split this one to insert item_to_rise
                    }
                }
            }

            DLOG(INFO) << "insert done";
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey,
                 _TpValue, _order>::begin() {
            if (!root_->empty()) {
                node* p_node = root_;
                std::stack<uint8_t> idx_stack;

                while (p_node->nodes_[0]) {
                    //DLOG(INFO) << "passing by key " << p_node->items_[0].first;
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
