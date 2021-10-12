#pragma once

#include "sequence_storage.h"

class Singleton
{
public:
  SequenceStorage mSequenceStorage;

  static Singleton& instance();
private:
    Singleton() = default;
};
