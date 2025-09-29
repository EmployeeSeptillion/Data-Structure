#pragma once
#include "DataModels.hpp"
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>

template<typename T>
class ArrayContainer {
private:
    std::vector<std::shared_ptr<T>> items;
    
public:
    // Add item to container
    void add(std::shared_ptr<T> item) { items.push_back(item); }
    void add(const T& item) { items.push_back(std::make_shared<T>(item)); }
    
    // Get item by index
    std::shared_ptr<T> get(size_t index) const { 
        if (index >= items.size()) return nullptr;
        return items[index]; 
    }
    
    // Get all items
    const std::vector<std::shared_ptr<T>>& getAll() const { return items; }
    
    // Size of container
    size_t size() const { return items.size(); }
    
    // Check if empty
    bool empty() const { return items.empty(); }
    
    // Clear all items
    void clear() { items.clear(); }
    
    // Iterator support
    typename std::vector<std::shared_ptr<T>>::iterator begin() { return items.begin(); }
    typename std::vector<std::shared_ptr<T>>::iterator end() { return items.end(); }
    typename std::vector<std::shared_ptr<T>>::const_iterator begin() const { return items.begin(); }
    typename std::vector<std::shared_ptr<T>>::const_iterator end() const { return items.end(); }
};

// Specialized versions for types with id field
template<>
class ArrayContainer<Resume> {
private:
    std::vector<std::shared_ptr<Resume>> items;
    
public:
    void add(std::shared_ptr<Resume> item) { items.push_back(item); }
    void add(const Resume& item) { items.push_back(std::make_shared<Resume>(item)); }
    
    std::shared_ptr<Resume> get(size_t index) const { 
        if (index >= items.size()) return nullptr;
        return items[index]; 
    }
    
    const std::vector<std::shared_ptr<Resume>>& getAll() const { return items; }
    size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    void clear() { items.clear(); }
    
    std::shared_ptr<Resume> findById(const std::string& id) const {
        auto it = std::find_if(items.begin(), items.end(),
            [&id](const std::shared_ptr<Resume>& item) {
                return item->id == id;
            });
        return (it != items.end()) ? *it : nullptr;
    }
    
    std::vector<std::shared_ptr<Resume>>::iterator begin() { return items.begin(); }
    std::vector<std::shared_ptr<Resume>>::iterator end() { return items.end(); }
    std::vector<std::shared_ptr<Resume>>::const_iterator begin() const { return items.begin(); }
    std::vector<std::shared_ptr<Resume>>::const_iterator end() const { return items.end(); }
};

template<>
class ArrayContainer<JobDescription> {
private:
    std::vector<std::shared_ptr<JobDescription>> items;
    
public:
    void add(std::shared_ptr<JobDescription> item) { items.push_back(item); }
    void add(const JobDescription& item) { items.push_back(std::make_shared<JobDescription>(item)); }
    
    std::shared_ptr<JobDescription> get(size_t index) const { 
        if (index >= items.size()) return nullptr;
        return items[index]; 
    }
    
    const std::vector<std::shared_ptr<JobDescription>>& getAll() const { return items; }
    size_t size() const { return items.size(); }
    bool empty() const { return items.empty(); }
    void clear() { items.clear(); }
    
    std::shared_ptr<JobDescription> findById(const std::string& id) const {
        auto it = std::find_if(items.begin(), items.end(),
            [&id](const std::shared_ptr<JobDescription>& item) {
                return item->id == id;
            });
        return (it != items.end()) ? *it : nullptr;
    }
    
    std::vector<std::shared_ptr<JobDescription>>::iterator begin() { return items.begin(); }
    std::vector<std::shared_ptr<JobDescription>>::iterator end() { return items.end(); }
    std::vector<std::shared_ptr<JobDescription>>::const_iterator begin() const { return items.begin(); }
    std::vector<std::shared_ptr<JobDescription>>::const_iterator end() const { return items.end(); }
};