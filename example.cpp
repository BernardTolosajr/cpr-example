#include "example.h"
#include <iostream>
#include <cpr/cpr.h>
#include <json.hpp>

std::string Example::get(std::string url) {
  auto response = cpr::Get(cpr::Url{url});
  auto json = nlohmann::json::parse(response.text);
  return json.dump(4);
}
