#include "singleton.h"

Singleton & Singleton::instance()
{
   static Singleton instance;
   return instance;
}

