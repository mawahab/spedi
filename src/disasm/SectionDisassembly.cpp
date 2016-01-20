//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015 University of Kaiserslautern.

#include "SectionDisassembly.h"
#include <binutils/elf/elf++.hh>
#include <cassert>

namespace disasm {

SectionDisassembly::SectionDisassembly(const elf::section *section) :
    m_valid{false},
    m_section{section} {
}

const std::string
SectionDisassembly::sectionName() const {
    return m_section->get_name();
}

const addr_t &
SectionDisassembly::startAddr() const {
    return m_section->get_hdr().addr;
}

const size_t
SectionDisassembly::size() const {
    return m_section->size();
}

const uint8_t *
SectionDisassembly::data() const {
    return static_cast<const uint8_t *>(m_section->data());
}

void SectionDisassembly::add(const MaximalBlock &max_block) {
    assert(m_max_blocks.size() == max_block.getId()
               && "invalid index of maximal block");
    m_max_blocks.push_back(max_block);
}

void SectionDisassembly::add(MaximalBlock &&max_block) {
    assert(m_max_blocks.size() == max_block.getId()
               && "invalid index of maximal block");
    m_max_blocks.emplace_back(max_block);
}
const MaximalBlock &
SectionDisassembly::back() const {
    return m_max_blocks.back();
}
addr_t
SectionDisassembly::virtualAddrOf(const uint8_t *ptr) const {
    assert(data() <= ptr
               && ptr < data() + size()
               && "Invalid pointer !!!");
    return startAddr() + (ptr - data());
}

const uint8_t *
SectionDisassembly::physicalAddrOf(const addr_t virtual_addr) const {
    assert(startAddr() <= virtual_addr
               && virtual_addr < startAddr() + size()
               && "Invalid virtual address !!!");
    return data() + (virtual_addr - startAddr());
}

std::vector<MaximalBlock> &
SectionDisassembly::getMaximalBlocks() {
    return m_max_blocks;
}
bool SectionDisassembly::isLast(const MaximalBlock &max_block) const {
    return max_block.getId() == m_max_blocks.size() - 1;
}
bool SectionDisassembly::isFirst(const MaximalBlock &max_block) const {
    return max_block.getId() == 0;
}

const MaximalBlock &SectionDisassembly::maximalBlockAt(const size_t &index) const {
    return m_max_blocks[index];
}
MaximalBlock *SectionDisassembly::ptrToMaximalBlockAt(const size_t &index) {
    return &(*(m_max_blocks.begin() + index));
}
std::vector<MaximalBlock>::const_iterator SectionDisassembly::cbegin() const {
    return m_max_blocks.cbegin();
}
std::vector<MaximalBlock>::const_iterator SectionDisassembly::cend() const {
    return m_max_blocks.cend();
}
}
