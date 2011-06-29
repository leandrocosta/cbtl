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

namespace cbt {
    template<typename _TpKey, typename _TpValue, unsigned char order = 1>
        class btree {
            public:
                void insert(const _TpKey& key, const _TpValue& value);
                const _TpValue& find(const _TpKey& key) const;

            private:
                btree_node<_TpKey, _TpValue, order> root_;
        };

    template<typename _TpKey, typename _TpValue, unsigned char order>
        void btree<_TpKey, _TpValue, order>::insert(const _TpKey& key, const _TpValue& value) {
            root_.insert(key, value);
        }

    template<typename _TpKey, typename _TpValue, unsigned char order>
        const _TpValue& btree<_TpKey, _TpValue, order>::find(const _TpKey& key) const {
            return root_.find(key);
        }
}

#endif  // CBTL_CBT_BTREE_H_
