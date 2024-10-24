#ifndef MISSING_ENV_EXCEPTION_H
#define MISSING_ENV_EXCEPTION_H
#include "EnvException.h"

class MissingEnvException : public EnvException {
public:
  MissingEnvException(const std::string &env);
};

#endif
