#include "InvalidEnvException.h"

InvalidEnvException::InvalidEnvException(const std::string &env,
                                         const std::string &reason)
    : EnvException(env,
                   "THE ENV VAR: " + env +
                       " IS NOT IN THE EXPECTED FORMAT! REASON: " + reason) {}
