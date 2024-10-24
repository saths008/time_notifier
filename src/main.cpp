#include "InvalidEnvException.h"
#include "MissingEnvException.h"
#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>

using std::cout;
using std::endl;

const std::string TIME_NOTIFY_ENV_NAME = "TIME_NOTIFY_DURATION";
const std::string NOTIFY_SOUND = "./media/bell_sound.wav";
const std::string NOTIF_URGENCY = "\"critical\"";
const std::string NOTIF_BODY = "\"Step away from the screen\"";

// Attempts to turn a string into the type T
template <typename T> T process_env_value(const std::string &env_val) {
  if (!std::is_integral_v<T>)
    throw std::invalid_argument("This type has not been implemented yet");

  int64_t time_notify_dur = 0;

  try {
    time_notify_dur = std::stoi(env_val);
  } catch (const std::exception &e) {
    throw InvalidEnvException(TIME_NOTIFY_ENV_NAME, e.what());
  }

  return time_notify_dur;
}

int main() {
  if (!std::getenv(TIME_NOTIFY_ENV_NAME.c_str()))
    throw MissingEnvException(TIME_NOTIFY_ENV_NAME);

  const std::string time_notify_dur_s(
      std::getenv(TIME_NOTIFY_ENV_NAME.c_str()));

  auto time_notify_dur = process_env_value<int64_t>(time_notify_dur_s);

  cout << "Got $" << TIME_NOTIFY_ENV_NAME << ": " << time_notify_dur << endl;

  const auto duration = std::chrono::minutes(time_notify_dur);
  const std::string notify_visual_cmd("notify-send -u " + NOTIF_URGENCY +
                                      " \"" + std::to_string(time_notify_dur) +
                                      " mins reached!\" " + NOTIF_BODY);
  const std::string notify_sound_cmd("paplay " + NOTIFY_SOUND);

  cout << "notify_visual_cmd: " << notify_visual_cmd << "\n";
  cout << "notify_sound_cmd: " << notify_sound_cmd << "\n";

  while (1) {
    std::this_thread::sleep_for(duration);
    cout << "Sending a notification" << endl;
    if (std::system(notify_visual_cmd.c_str()) == -1)
      throw std::runtime_error("Failed to visually send notification.");
    if (std::system(notify_sound_cmd.c_str()) == -1)
      throw std::runtime_error("Failed to send notification sound.");
  }

  return 0;
}
