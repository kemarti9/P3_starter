// src/tools/tool_registry.cpp — YOU IMPLEMENT (spec §4.3).
//
// A hand-written hash table. No std::unordered_map / std::map / std::set.
// Every function below is a stub so the starter compiles. Replace each body.

#include "tools/tool_registry.h"
#include <cstdint>
#include <utility>

ToolRegistry::ToolRegistry() : buckets_(16) {}    // TODO: create your initial buckets
ToolRegistry::~ToolRegistry() = default;   // unique_ptrs in your buckets free the tools

std::size_t ToolRegistry::hash(std::string_view key) noexcept {
    std::uint64_t h = 14695981039346656037ULL; 
    for (unsigned char c : key) {
        h ^= static_cast<std::uint64_t>(c);
        h *= 1099511628211ULL;
    }
    return static_cast<std::size_t>(h);
}

void ToolRegistry::rehash(std::size_t new_bucket_count) {
    std::vector<std::vector<std::unique_ptr<Tool>>> new_buckets(new_bucket_count);
    for (auto& chain : buckets_) {
        for (auto& tool : chain) {
            if (!tool) continue;
            std::size_t idx = hash(tool->name()) % new_bucket_count;
            new_buckets[idx].push_back(std::move(tool));
        }
    }
    buckets_ = std::move(new_buckets);
}

Tool* ToolRegistry::find(std::string_view name) const { 
    //I switched the order of find and add because add uses find
    if (buckets_.empty()) return nullptr;

    std::size_t idx = hash(name) % buckets_.size();
    for (const auto& tool : buckets_[idx]) {
        if (tool && tool->name() ==name) return tool.get();
    }
    
    return nullptr;   // TODO
}

bool ToolRegistry::add(std::unique_ptr<Tool> tool) {
    if (!tool) return false; //null
    if (find(tool->name()) != nullptr) return false; //duplicate
    if (static_cast<double>(count_ + 1) / static_cast<double>(buckets_.size()) > .75) { //grow
        rehash(buckets_.size() *2);
    }
    std::size_t idx = hash(tool->name()) % buckets_.size(); //insert
    buckets_[idx].push_back(std::move(tool));
    ++count_;
    return true;
}

std::size_t ToolRegistry::size() const noexcept {
    return count_;   
}

void ToolRegistry::for_each(const std::function<void(const Tool&)>& fn) const {
    for (const auto& chain:buckets_) {
        for (const auto& tool:chain) {
            if (tool) fn(*tool);
        }
    }
}

std::size_t ToolRegistry::bucket_count() const noexcept {
    return buckets_.size();   // TODO
}

double ToolRegistry::load_factor() const noexcept {
    if (buckets_.empty()) return 0.0;
    return static_cast<double>(count_)/static_cast<double>(buckets_.size());
}

std::size_t ToolRegistry::max_probe_length() const {
    std::size_t best =0;
    for (const auto& chain : buckets_) {
        if (chain.size() > best) best = chain.size();
    }
    return best;
}
