#ifndef SWAP_HPP
#define SWAP_HPP

#include <string>

namespace kvstore {

// Swap ABC with pure virtual functions.
// Use Curiously Recurring Template Pattern (CRTP) in derived
// classes for providing compile-time polymorphism.
template <typename SwapType>
class SwapBase {

protected:
  // Try to save the given key/value into secondary storage, which may be persistent.
  // Keys are unique, so an existing key will update the value only.
  // Return: True if key/value was successfully put into secondary storage
  bool swap_save(const std::string& key, const std::string& value) { 
    return static_cast<SwapType *>(this)->swap_save_impl(key, value);
  }

  // Try to get the given key/value from secondary storage
  // Return: bool: True if key/value was found
  //         string: Value (undefined if bool is false)
  const std::pair<bool, std::string> swap_get(const std::string& key) {
    return static_cast<SwapType *>(this)->swap_get_impl(key);
  }
  
  // Test if key is present in secondary storage.
  bool swap_has(const std::string& key) { 
    return static_cast<SwapType *>(this)->swap_has_impl(key);
  }
  
  // Try to delete key/value in secondary storage.
  // Return: True if key/value was present before deletion.
  bool swap_delete(const std::string& key) {
    return static_cast<SwapType *>(this)->swap_delete_impl(key);
  }
  
  // Current number of key/value pairs in secondary storage.
  size_t swap_size() {
    return static_cast<SwapType *>(this)->swap_size_impl();
  }

 private:
  virtual bool swap_save_impl(const std::string& key, const std::string& value) = 0; 
  virtual const std::pair<bool, std::string> swap_get_impl(const std::string& key) = 0;
  virtual bool swap_has_impl(const std::string& key) = 0;
  virtual bool swap_delete_impl(const std::string& key) = 0;
  virtual size_t swap_size_impl() = 0;
};

// The ever empty swap which saves nothing.
class EmptySwap : public SwapBase<EmptySwap> {
  // Allow our base class to call our private member functions
  friend class SwapBase<EmptySwap>;

private:
  bool swap_save_impl(const std::string& /*key*/, const std::string& /*value*/) override {
    return false; 
  }

  const std::pair<bool, std::string> swap_get_impl(const std::string& /*key*/) override {
    return {false, ""};
  }

  bool swap_has_impl(const std::string& /*key*/) override { return false; }
  bool swap_delete_impl(const std::string& /*key*/) override { return false; }
  size_t swap_size_impl() override { return 0u; }
};

}  // namespace kvstore

#endif  // SWAP_HPP
