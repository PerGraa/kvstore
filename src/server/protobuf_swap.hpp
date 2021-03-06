#ifndef PROTOBUF_SWAP_HPP
#define PROTOBUF_SWAP_HPP

#include "swap.hpp"
#include "proto/keyvalue.pb.h"
#include <string>

// Silence Clang compiler warnings
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#include "recordio/recordio_range.h"
#pragma clang diagnostic pop

namespace kvstore {

// A protobuf swap.
// It uses swap file at disk, the old file is deleted at start-up in
// ctor.
// The current implementation is silly, and much work can be done on
// the file data structure and related algorithms.
// Note: Data file is shared between servers, but that should be OK
// since we can only run one server at a time.
// Note: recordio::ReaderRange is an InputIterator, which means it
// will "only guarantee validity for single pass algorithms"
class ProtobufSwap : public SwapBase<ProtobufSwap> {
  // Allow our base class to call our private member functions
  friend class SwapBase<ProtobufSwap>;

 public:
  ProtobufSwap() {
    // Delete possible swap file from earlier server run
    std::remove(m_file_name.c_str());
  }

 private:
  bool swap_save_impl(const std::string& key, const std::string& value) override {
    // If key is not currently present, then update size
    if (!swap_has_impl(key)) {
      ++m_current_size;
    }

    // Add element and set valid marker
    std::ofstream ofs(m_file_name, std::ios::binary | std::ofstream::app);
    recordio::RecordWriter writer(&ofs);
    KeyValue kv;
    // Add valid marker
    kv.set_valid(true);
    kv.set_key(key);
    kv.set_value(value);
    writer.WriteProtocolMessage(kv);

    return true;
  }

  const std::pair<bool, std::string> swap_get_impl(const std::string& key) override {
    auto valid = false;
    std::string value;

    // Iterate through file and see if last element with correct key is valid
    for (const auto& kv : recordio::ReaderRange<KeyValue>(m_file_name)) {
      if (kv.key() == key) {
        valid = kv.valid();
        value = kv.value();
      }
    }

    return {valid, (valid ? value : "")};
  }

  bool swap_has_impl(const std::string& key) override {
    auto valid = false;

    // Iterate through file and see if last element with correct key is valid
    for (const auto& kv : recordio::ReaderRange<KeyValue>(m_file_name)) {
      if (kv.key() == key) {
        valid = kv.valid();
      }
    }

    return valid;
  }

  bool swap_delete_impl(const std::string& key) override {
    if (swap_has_impl(key)) {
      // Key found and valid
      std::ofstream ofs(m_file_name, std::ios::binary | std::ofstream::app);
      recordio::RecordWriter writer(&ofs);

      KeyValue kv;
      // Add invalid marker
      kv.set_valid(false);
      kv.set_key(key);
      kv.set_value("");
      writer.WriteProtocolMessage(kv);
      // Decrement count
      --m_current_size;
      return true;
    }

    // Key not valid
    return false;
  }

  size_t swap_size_impl() override { return m_current_size; }

  // Hardcoded filename. Should be OK since we can only run one server at a time.
  // Find this file in kvstore/build/ if normal out-of-source build procedure is used.
  const std::string m_file_name = "kvstore.protobuf.data";
  size_t m_current_size         = 0;
};

}  // namespace kvstore

#endif  // PROTOBUF_SWAP_HPP
