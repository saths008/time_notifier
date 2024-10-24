#ifndef ENV_EXCEPTION_H
#define ENV_EXCEPTION_H

#include <string>

class EnvException : public std::exception {
protected:
  std::string m_env;
  std::string m_err_msg;

public:
  EnvException(const std::string &env, const std::string &err_msg);

  const char *what();
};

#endif
