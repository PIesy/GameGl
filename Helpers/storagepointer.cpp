//
// Created by akann on 4/25/17.
//
#include "storagepointer.h"

const StoragePointer::Accessor StoragePointer::defaultAccessor = []() {return nullptr;};
const StoragePointer::Validator StoragePointer::defaultValidator = []() {return false;};
