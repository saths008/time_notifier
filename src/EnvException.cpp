#include "EnvException.h"
#include <iostream>

EnvException::EnvException(const std::string &env, const std::string &err_msg)
    : m_env(env), m_err_msg(err_msg) {}

const char *EnvException::what() {
  // print for daemon logging
  std::cout << m_err_msg << std::endl;
  return m_err_msg.c_str();
};
