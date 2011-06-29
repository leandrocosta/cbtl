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

namespace cbt {
    template<typename _TpKey, typename _TpValue, uint8_t order>
        class btree_node {
            public:
                btree_node() : num_keys_(0) { }

            private:
                const bool is_leaf();
                void insert_here(const _TpKey& key, const _TpValue& value);

            public:
                void insert(const _TpKey& key, const _TpValue& value);
                const _TpValue& find(const _TpKey& key) const;

            private:
                btree_node<_TpKey, _TpValue, order>* parent_;
                btree_node<_TpKey, _TpValue, order>* nodes_[2*order+1];

                _TpKey keys_[2*order];
                _TpValue values_[2*order+1];

                uint8_t num_keys_;
        };

    template<typename _TpKey, typename _TpValue, uint8_t order>
        const bool btree_node<_TpKey, _TpValue, order>::is_leaf() {
            return (num_keys_ == 0);
        }

    template<typename _TpKey, typename _TpValue, uint8_t order>
        void btree_node<_TpKey, _TpValue, order>::insert_here(const _TpKey& key, const _TpValue& value) {
            uint8_t i = num_keys_;

            while (i > 0 && keys_[i-1] > key) {
                keys_[i] = keys_[i-1];
                values_[i] = values_[i-1];
            }

            keys_[i] = key;
            values_[i] = value;
            num_keys_++;
        }

    template<typename _TpKey, typename _TpValue, uint8_t order>
        void btree_node<_TpKey, _TpValue, order>::insert(const _TpKey& key, const _TpValue& value) {
            if (is_leaf()) {
                if (num_keys_ < 2*order) {
                    insert_here(key, value);
                } else {
                }
            } else {
            }
        }

    template<typename _TpKey, typename _TpValue, unsigned char order>
        const _TpValue& btree_node<_TpKey, _TpValue, order>::find(const _TpKey& key) const {
            uint8_t i = 0;

            while (i < num_keys_ && keys_[i] < key)
                i++;
            if (i < num_keys_ && key == keys_[i])
                return values_[i];
            else
                throw("error");
        }
}

#endif  // CBTL_CBT_BTREE_NODE_H_
