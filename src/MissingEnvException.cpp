#include "MissingEnvException.h"

MissingEnvException::MissingEnvException(const std::string &env)
    : EnvException(env, "THE ENV VAR: " + env + " COULD NOT BE FOUND!") {}
