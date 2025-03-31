#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <lmdb.h>
#include <chrono>
#include <thread>
#include "./json.hpp"
#include "./interface.hpp"
#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <map>
#include <sys/resource.h>
#include <filesystem>
#include <atomic>

#include "./loginAndJoin.hpp"
#include "./parteMedica.hpp"
#include "./parteSistema.hpp"

#include <crypt.h>
#include <crypto++/aes.h>
#include <crypto++/filters.h>
#include <crypto++/modes.h>
#include <crypto++/hex.h>

using json = nlohmann::json;
using namespace std;
using namespace chrono;

#endif