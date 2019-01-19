#include <iostream>
#include <stdlib.h>
#include "parseropt.hpp"

void show_help(const char **argv) {
    std::cout << " [-h|--help|-?] [-v|--version] [-u name|--user name|--user-name name] [-p password|--pass password]" << std::endl;
    exit(0);
}

int main(int argc, const char **argv) {

    // Simple functional api. No initialization required.
    bool help = parseropt(false, "-h", "--help", "-?");
    int version = parseropt(0, "-v", "--version", "--show-version");
    std::string name = parseropt("", "-u", "--user", "--user-name");
    std::string pass = parseropt("", "-p", "--pass", "--passwprd");

    if(help || argc <= 1) {
        show_help(argv);
    }

    if(version) {
        std::cout << argv[0] << " | demo v1.0 Compiled on " << __DATE__ << std::endl;
    }

    if(!name.empty()) {
        std::cout << "provided name: " << name << std::endl;
    }

    if(!pass.empty()) {
        std::cout << "provided pass: " << pass << std::endl;
    }

    struct parseropt args(argc, argv);

    if(args.has("-h") || args.has("--help") || args.has("-?") || args.size() == 1) {
        show_help(argv);
    }

    if(args.has("-v") || args.has("--version")) {
        std::cout << "demo v1.0 Compiled on " << __DATE__ << std::endl;
    }

    if(args.has("-u") || args.has("--user") || args.has("--user-name")) {
        std::string arg = args["-u"];
        if(arg.empty()) arg = args["--user"];
        if(arg.empty()) arg = args["--user-name"];
        std::string name = arg;
        std::cout << "provided user: " << name << std::endl;
    }

    if(args.has("-p") || args.has("--pass") || args.has("--password")) {
        std::string arg = args["-p"];
        if(arg.empty()) arg = args["--pass"];
        if(arg.empty()) arg = args["--password"];
        std::string pass = arg;
        std::cout << "provided pass: " << pass << std::endl;
    }

    std::cout << "--------" << std::endl;
    std::cout << args.str() << std::endl;
}
