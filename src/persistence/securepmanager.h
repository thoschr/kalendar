#ifndef SECUREPMANAGER_H
#define SECUREPMANAGER_H

#include "pmanager.h"

/* Proxy class to encrypt data on disk */
//TODO: override PManager functions to encrypt/decrypt data. Use AES-256, implement it in util.
class SecurePManager : public PManager
{
public:
    SecurePManager(string database = DEFAULT_DATABASE_NAME);
};

#endif // SECUREPMANAGER_H
