#pragma once

#include "sequence_storage.h"

class Singleton
{
public:
  SqStorage mSequenceStorage;

  static Singleton& instance();
private:
    Singleton() = default;
};
