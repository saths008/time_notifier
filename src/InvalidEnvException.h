#ifndef INVALID_ENV_EXCEPTION_H
#define INVALID_ENV_EXCEPTION_H

#include "EnvException.h"

class InvalidEnvException : public EnvException {
private:
  // reason why Env value is invalid
  std::string reason;

public:
  InvalidEnvException(const std::string &env, const std::string &reason);
};

#endif
