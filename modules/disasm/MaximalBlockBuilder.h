//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015 Technical University of Kaiserslautern.
// Created by M. Ammar Ben Khadra.


#pragma once

#include "MCInstSmall.h"
#include "MaximalBlock.h"
#include <vector>

namespace disasm {
/**
 * MaximalBlockBuilder
 */
class MaximalBlockBuilder {
public:
    /**
     * Construct a MaximalBlockBuilder that is initially not valid.  Calling
     * methods other than operator= and valid on this results in
     * undefined behavior.
     */
    MaximalBlockBuilder() ;
    virtual ~MaximalBlockBuilder() = default;
    MaximalBlockBuilder(const MaximalBlockBuilder &src) = default;
    MaximalBlockBuilder &operator=(const MaximalBlockBuilder &src) = default;
    MaximalBlockBuilder(MaximalBlockBuilder &&src) = default;

    /*
     * Checks if an instruction could be appendable at given address to already
     * existing basic blocks. If successful, return ids of basic blocks
     * otherwise returns empty vector.
     */
    std::vector<unsigned int> appendableBasicBlocksAt
        (const addr_t addr) const;

    /*
     * Adds a new block with a single fragment containing the instruction. Used
     * when the given instruction is not appendable.
     */
    void createBasicBlockWith
        (const MCInstSmall &inst);

    /**
     * precondition: inst is appendable to basic block.
     */
    void append
        (const unsigned int bb_id, const MCInstSmall &inst);

    /**
     * precondition: inst is appendable to basic block.
     * MaximalBlock is buildable after this operation.
     */
    void append
        (const unsigned int bb_id,
         const MCInstSmall &inst,
         const BranchInstType br_type,
         const addr_t br_target);

    /**
     * precondition: inst is appendable to all given basic blocks.
     */
    void append
        (const std::vector<unsigned int> &bb_ids, const MCInstSmall &inst);

    /**
     * precondition: inst is appendable to all given basic blocks.
     * MaximalBlock is buildable after this operation.
     */
    void append
        (const std::vector<unsigned int> &bb_ids,
         const MCInstSmall &inst,
         const BranchInstType br_type,
         const addr_t br_target);

    /**
     * precondition: maximal block is buildable.
     */
    MaximalBlock build();

    /*
     * Reset the builder to its original state except in the case of MB overlap.
     * There, partial results will be kept to build the next MB.
     * Return true on clean reset, false otherwise.
     */
    bool reset();

    /*
     * remove basic blocks given by their Ids.
     */
    void remove
        (const std::vector<unsigned int> &bb_ids);


private:
    // Attention: pointer to a vector element will get invalidated in case
    // of reallocation.
    Fragment* findFragment
        (const unsigned int frag_id) const;
    BasicBlock* findBasicBlock
        (const unsigned int bb_id) const;

private:
    bool m_buildable;
    unsigned int m_bb_idx;
    unsigned int m_frag_idx;
    std::vector<BasicBlock> m_bblocks;
    std::vector<Fragment> m_frags;
};
}