#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
using namespace std::chrono;
namespace fs = std::filesystem;

template <typename... Strings> std::string make_command(Strings... args) {
  return (... + (std::string(args) + " "));
}
template <typename... Types> void run(Types... args) {
  std::string cmd = make_command(args...);
  std::cout << cmd << std::endl;
  std::system(cmd.c_str());
}

void run(std::string run_commmand, bool timecheck) {
  if (!timecheck) {
    system(run_commmand.c_str());
    return;
  }
  auto st = system_clock::now();
  system(run_commmand.c_str());
  auto et = system_clock::now();
  std::cout << "took " << std::fixed << (et - st).count() / std::pow(10, 9)
            << "sec" << std::endl;
}

void run_commmand(std::string fname, std::string ext) {
  std::cout << "now running: " << fname << ext << std::endl;
  if (ext == ".cpp") {
    run("g++", "-o", fname, fname + ext, "-Wall", "-std=c++17");
    std::cout << "Compile compelete\n" << std::endl;
    run("./" + fname, true);
    exit(0);
  }
  if (ext == ".c") {
    run("gcc", "-o", fname, fname + ext, "&&", "./" + fname);
    exit(0);
  }
  if (ext == ".py") {
    run("python3", "./" + fname + ext);
    exit(0);
  }
  std::cout << "Sorry, I can't run " << ext << " filetype for now."
            << std::endl;
  exit(0);
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    std::cout << "(WIP) parameter not propvided; tyring compiling and running "
                 "most recently edited file"
              << std::endl;
    return 0;
  }
  fs::path path_arg = argv[1];

  if (path_arg.has_extension()) {
    run_commmand(path_arg.stem(), path_arg.extension());
  } else {
    auto cwd = fs::current_path();
    for (const auto &entry : fs::directory_iterator(cwd)) {
      auto p = entry.path();
      if (p.has_extension() && p.stem() == path_arg.stem()) {
        run_commmand(p.stem(), p.extension());
      }
    }
  }
}
