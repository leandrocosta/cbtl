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
                typedef _BTreeNode<_TpKey, _TpValue, _order> _Node;

            public:
                typedef btree_iterator<_TpKey, _TpValue, _order> iterator;

            public:
                btree() : root_(new _Node()) { }

            private:
                _Node* _get_node_to_insert(const _TpKey& key) const;
                void _insert_into_this_node(_Node* p_node, const typename _Node::_TpItem& item, _Node* p_node_next_to_item);

            public:
                iterator begin();
                iterator end() { return iterator(); }
                iterator find(const _TpKey& key);

                void insert(const _TpKey& key, const _TpValue& value);
                const bool empty() const { return root_->empty(); }

            private:
                _Node* root_;
        };

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        _BTreeNode<_TpKey, _TpValue, _order>* btree<_TpKey, _TpValue, _order>::_get_node_to_insert(const _TpKey& key) const {
            _Node* p_node = root_;

            while (! p_node->is_leaf()) {
                uint8_t idx = 0;

                while (idx < p_node->num_items() && p_node->item(idx).first < key)
                    idx++;

                p_node = p_node->node(idx);
            }

            return p_node;

        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree<_TpKey, _TpValue, _order>::_insert_into_this_node(_Node* p_node, const typename _Node::_TpItem& item, _Node* p_node_next_to_item) {
            if (p_node->num_items() < _Node::MAX_NUM_ITEMS) {
                p_node->insert(item, p_node_next_to_item);
            } else { // we need to split this one to insert item
                typename _Node::_TpItem item_to_rise = item;
                p_node->get_median_item_rnode(item_to_rise, p_node_next_to_item);

                _Node* p_new_node_right = p_node->split(p_node_next_to_item);

                if (root_ == p_node) { // if this is root, create new root and update with item to rise
                    _Node* new_root = new _Node();
                    new_root->insert(item_to_rise);
                    new_root->set_node(0, p_node);
                    new_root->set_node(1, p_new_node_right);

                    p_node->set_parent(new_root);
                    p_new_node_right->set_parent(new_root);
                    root_ = new_root;
                } else { // updating parent with item to rise
                    _Node* p_parent = p_node->parent();
                    p_new_node_right->set_parent(p_parent);

                    _insert_into_this_node(p_parent, item_to_rise, p_new_node_right);
                }
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        void btree<_TpKey, _TpValue, _order>::insert(const _TpKey& key,
                const _TpValue& value) {
            _insert_into_this_node(_get_node_to_insert(key), std::make_pair(key, value), NULL);
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey,
                 _TpValue, _order>::begin() {
            if (!root_->empty()) {
                _Node* p_node = root_;

                while (p_node->node(0))
                    p_node = p_node->node(0);

                return iterator(p_node);
            } else {
                return iterator();
            }
        }

    template<typename _TpKey, typename _TpValue, uint8_t _order>
        typename btree<_TpKey, _TpValue, _order>::iterator btree<_TpKey,
                 _TpValue, _order>::find(const _TpKey& key) {
            _Node* p_node = root_;

            uint8_t idx = 0;

            while (idx < p_node->num_items() && p_node->item(idx).first < key)
                idx++;

            if (idx < p_node->num_items() && p_node->item(idx).first == key)
                return iterator(p_node, idx);
            else
                return iterator();
        }
}

#endif  // CBTL_CBT_BTREE_H_
