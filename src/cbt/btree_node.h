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
#include <utility>

#include "glog/logging.h"

namespace cbt {
  template<typename _TpKey, typename _TpValue, uint8_t _order>
    class btree;

  template<typename _TpKey, typename _TpValue, uint8_t _order>
    class btree_iterator;

  template<typename _TpKey, typename _TpValue, uint8_t _order>
    class _BTreeNode {
      public:
        static const uint8_t MAX_NUM_ITEMS = 2*_order;
        static const uint8_t MAX_NUM_NODES = MAX_NUM_ITEMS+1;

      public:
        typedef std::pair<_TpKey, _TpValue> _TpItem;

      public:
        _BTreeNode() : parent_(NULL), num_items_(0) {
          memset(&nodes_, 0, MAX_NUM_NODES * sizeof(*nodes_));
        }

      public:
        _BTreeNode* parent() const { return parent_; }

        _BTreeNode* node(const uint8_t& idx) const { return nodes_[idx]; }
        void set_node(const uint8_t& idx, _BTreeNode* _ptr_node) {
          nodes_[idx] = _ptr_node;
        }

        _TpItem& item(const uint8_t& idx) { return items_[idx]; }
        inline const uint8_t num_items() const { return num_items_; }

        void set_parent(_BTreeNode* p_node) { parent_ = p_node; }
        const bool is_leaf() const { return (nodes_[0] == NULL); }

        void insert(const _TpItem& item, _BTreeNode* p_node_right = NULL) {
          if (num_items_ == MAX_NUM_ITEMS)
            throw std::exception();

          uint8_t i = num_items_;

          while (i > 0 && items_[i-1].first > item.first) {
            items_[i] = items_[i-1];
            nodes_[i+1] = nodes_[i];
            i--;
          }

          items_[i] = item;
          nodes_[i+1] = p_node_right;
          num_items_++;
        }

        _BTreeNode* split(_BTreeNode* p_node_left) {
          _BTreeNode* p_new_node_right = new _BTreeNode();

          for (uint8_t idx = _order; idx < num_items_; idx++) {
            p_new_node_right->insert(items_[idx], nodes_[idx+1]);
            nodes_[idx+1] = NULL;
          }

          if (p_node_left)
            p_node_left->parent_ = p_new_node_right;

          p_new_node_right->nodes_[0] = p_node_left;
          num_items_ = _order;

          return p_new_node_right;
        }

        void get_median_item_rnode(_TpItem* p_item, _BTreeNode** pp_node) {
          for (uint8_t idx = 0; idx < _order; idx++) {
            if (p_item->first < items_[idx].first) {
              _TpItem tmp = *p_item;
              _BTreeNode* p_node_tmp = *pp_node;

              *p_item = items_[idx];
              *pp_node = nodes_[idx+1];

              items_[idx] = tmp;
              nodes_[idx+1] = p_node_tmp;
            }
          }

          for (uint8_t idx = MAX_NUM_ITEMS-1; idx >= _order; idx--) {
            if (p_item->first > items_[idx].first) {
              _TpItem tmp = *p_item;
              _BTreeNode* p_node_tmp = *pp_node;

              *p_item = items_[idx];
              *pp_node = nodes_[idx+1];

              items_[idx] = tmp;
              nodes_[idx+1] = p_node_tmp;
            }
          }
        }


        const bool empty() const { return (num_items_ == 0); }

      private:
        _BTreeNode* parent_;
        _BTreeNode* nodes_[MAX_NUM_NODES];

        _TpItem items_[MAX_NUM_ITEMS];

        uint8_t num_items_;
    };
}

#endif  // CBTL_CBT_BTREE_NODE_H_
