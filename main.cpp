#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <string>
#include <thread>

using std::cout;
using std::endl;

const std::string TIME_NOTIFY_ENV_NAME = "TIME_NOTIFY_DURATION";
const std::string NOTIFY_SOUND = "./bell_sound.wav";
const std::string NOTIF_URGENCY = "\"critical\"";
const std::string NOTIF_BODY = "\"Step away from the screen\"";

class EnvException : public std::exception {
protected:
  std::string m_env;
  std::string m_err_msg;

public:
  EnvException(const std::string &env, const std::string &err_msg)
      : m_env(env), m_err_msg(err_msg) {}

  const char *what() {
    // print for daemon logging
    cout << m_err_msg << endl;
    return m_err_msg.c_str();
  };
};

class MissingEnvException : public EnvException {
public:
  MissingEnvException(const std::string &env)
      : EnvException(env, "THE ENV VAR: " + env + " COULD NOT BE FOUND!") {}
};

class InvalidEnvException : public EnvException {
private:
  // reason why Env value is invalid
  std::string reason;

public:
  InvalidEnvException(const std::string &env, const std::string &reason)
      : EnvException(env,
                     "THE ENV VAR: " + env +
                         " IS NOT IN THE EXPECTED FORMAT! REASON: " + reason) {}
};

int main() {
  if (!std::getenv(TIME_NOTIFY_ENV_NAME.c_str()))
    throw MissingEnvException(TIME_NOTIFY_ENV_NAME);

  const std::string time_notify_dur_s(
      std::getenv(TIME_NOTIFY_ENV_NAME.c_str()));

  int64_t time_notify_dur = 0;

  try {
    time_notify_dur = std::stoi(time_notify_dur_s);
  } catch (const std::exception &e) {
    throw InvalidEnvException(TIME_NOTIFY_ENV_NAME, e.what());
  }

  cout << "Got $" << TIME_NOTIFY_ENV_NAME << ": " << time_notify_dur << endl;

  const auto duration = std::chrono::minutes(time_notify_dur);
  const std::string notify_sound_cmd("aplay " + NOTIFY_SOUND);
  const std::string notify_visual_cmd("notify-send -u " + NOTIF_URGENCY +
                                      " \"" + std::to_string(time_notify_dur) +
                                      " mins reached!\" " + NOTIF_BODY);

  while (1) {
    std::this_thread::sleep_for(duration);
    std::system(notify_visual_cmd.c_str());
    std::system(notify_sound_cmd.c_str());
  }

  return 0;
}
