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
 * \file tests/cbt/btree_test.h
 * \brief Tests for btree class.
 * \author Leandro Costa
 * \date 2011
 */

#include <glog/logging.h>
#include "gtest/gtest.h"
#include "cbt/btree.h"

class EmptyBTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            p_btree_ = new cbt::btree<int, std::string>();
        }

        cbt::btree<int, std::string>* p_btree_;
};

TEST_F(EmptyBTree, ShouldBeEmptyWhenJustCreated) {
    EXPECT_TRUE(p_btree_->empty());
}

TEST_F(EmptyBTree, ShouldHaveBeginEqualToEnd) {
    EXPECT_EQ(p_btree_->end(), p_btree_->begin());
}

TEST_F(EmptyBTree, ShouldReturnEndWhenSearchedByAnyKey) {
    EXPECT_EQ(p_btree_->end(), p_btree_->find(1));
}


class OneItemBTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            p_btree_ = new cbt::btree<int, std::string>();
            p_btree_->insert(1, "A");
        }

        cbt::btree<int, std::string>* p_btree_;
};

TEST_F(OneItemBTree, ShouldNotBeEmptyWhenItemInserted) {
    EXPECT_FALSE(p_btree_->empty());
}

TEST_F(OneItemBTree, ShouldHaveBeginNotEqualToEnd) {
    EXPECT_NE(p_btree_->end(), p_btree_->begin());
}

TEST_F(OneItemBTree, ShouldHaveBeginPointingToItem) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(1, it->first);
    EXPECT_EQ("A", it->second);
}

TEST_F(OneItemBTree, ShouldHaveBeginPointingToEndAfterInc) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(p_btree_->end(), ++it);
}


class TwoItemsBTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            p_btree_ = new cbt::btree<int, std::string>();
            p_btree_->insert(1, "B");
            p_btree_->insert(2, "A");
        }

        cbt::btree<int, std::string>* p_btree_;
};

TEST_F(TwoItemsBTree, ShouldPermitIterateByItems) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(1, it->first);
    EXPECT_EQ("B", it->second);

    it++;
    EXPECT_EQ(2, it->first);
    EXPECT_EQ("A", it->second);

    ++it;
    EXPECT_EQ(p_btree_->end(), it);
}


class ThreeItemsBTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            p_btree_ = new cbt::btree<int, std::string>();
            p_btree_->insert(1, "B");
            p_btree_->insert(2, "A");
            p_btree_->insert(3, "C");
        }

        cbt::btree<int, std::string>* p_btree_;
};

TEST_F(ThreeItemsBTree, ShouldReturnLowerKeyAsFirstItem) {
    EXPECT_EQ(1, p_btree_->begin()->first);
}

TEST_F(ThreeItemsBTree, ShouldReturnSecondKeyAsSecondItem) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(2, (++it)->first);
}

TEST_F(ThreeItemsBTree, ShouldReturnThirdKeyAsThirdItem) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    ++it;
    EXPECT_EQ(3, (++it)->first);
}

TEST_F(ThreeItemsBTree, ShouldPermitIterateByItems) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(1, it->first);
    EXPECT_EQ("B", it->second);

    it++;
    EXPECT_EQ(2, it->first);
    EXPECT_EQ("A", it->second);

    it++;
    EXPECT_EQ(3, it->first);
    EXPECT_EQ("C", it->second);

    ++it;
    EXPECT_EQ(p_btree_->end(), it);
}


class SevenItemsBTree : public ::testing::Test {
    protected:
        virtual void SetUp() {
            p_btree_ = new cbt::btree<int, std::string>();
            p_btree_->insert(4, "A");
            p_btree_->insert(6, "D");
            p_btree_->insert(3, "C");
            p_btree_->insert(5, "E");
            p_btree_->insert(7, "B");
            p_btree_->insert(2, "F");
            p_btree_->insert(1, "G");
        }

        cbt::btree<int, std::string>* p_btree_;
};

TEST_F(SevenItemsBTree, ShouldReturnLowerKeyAsFirstItem) {
    EXPECT_EQ(1, p_btree_->begin()->first);
}

TEST_F(SevenItemsBTree, ShouldReturnSecondKeyAsSecondItem) {
    EXPECT_EQ(2, (++(p_btree_->begin()))->first);
}

TEST_F(SevenItemsBTree, ShouldReturnThirdKeyAsThirdItem) {
    EXPECT_EQ(3, (++(++(p_btree_->begin())))->first);
}

TEST_F(SevenItemsBTree, ShouldReturnFourthKeyAsFourthItem) {
    EXPECT_EQ(4, (++(++(++(p_btree_->begin()))))->first);
}

TEST_F(SevenItemsBTree, ShouldReturnFifthKeyAsFifthItem) {
    EXPECT_EQ(5, (++(++(++(++(p_btree_->begin())))))->first);
}

TEST_F(SevenItemsBTree, ShouldPermitIterateByItems) {
    cbt::btree<int, std::string>::iterator it = p_btree_->begin();
    EXPECT_EQ(1, it->first);
    EXPECT_EQ("G", it->second);

    it++;
    EXPECT_EQ(2, it->first);
    EXPECT_EQ("F", it->second);

    it++;
    EXPECT_EQ(3, it->first);
    EXPECT_EQ("C", it->second);

    it++;
    EXPECT_EQ(4, it->first);
    EXPECT_EQ("A", it->second);

    it++;
    EXPECT_EQ(5, it->first);
    EXPECT_EQ("E", it->second);

    it++;
    EXPECT_EQ(6, it->first);
    EXPECT_EQ("D", it->second);

    it++;
    EXPECT_EQ(7, it->first);
    EXPECT_EQ("B", it->second);

    it++;
    EXPECT_EQ(p_btree_->end(), it);
}

TEST_F(SevenItemsBTree, ShouldReturnCorrectNodeWithFind) {
  EXPECT_EQ(5, p_btree_->find(5)->first);
}

int main(int argc, char* argv[]) {
    ::google::InitGoogleLogging(argv[0]);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
