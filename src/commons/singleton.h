// Copyright (c) 2014-2019 winking324
//

#pragma once  // NOLINT(build/header_guard)


namespace gump {


template <class T>
class Singleton {
 public:
  static T* Instance() {
    static T inst;
    return &inst;
  }

  Singleton() = delete;
  ~Singleton() = delete;

  Singleton(const Singleton &) = delete;
  Singleton(Singleton &&) = delete;

  Singleton& operator=(const Singleton &) = delete;
  Singleton& operator=(Singleton &&) = delete;
};


}  // namespace gump
