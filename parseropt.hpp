// Simple parseropt replacement class (C++11). 
//
// 1) Simple functional api `parseropt(...)`. 
//    - No initialization required: (argc, argv) pair automatically retrieved.
//    - First argument is default option value, then all option indentifiers follow.
//
// 2) Simple OOP map-based api `parseropt class`. Initialization (argc, argv) pair required.
//
//    This parseropt class is a std::map replacement where key/value are std::string types.
//    Given invokation './a.out --user nine --pass dreams'
//
//    Additional API:
//    - cmdline() for a print app invokation string
//    - str() for pretty map printing
//    - size() number of arguments (equivalent to argc), rather than std::map.size()
//    - parseropt(arg...) init the arg in the cmd
//

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

#define PARSER_OPT_VERSION "1.0"

namespace parseropt_np
{
    // string conversion
    template<typename T>
    inline T astr(const std::string &str) {
        T t;
        return (std::istringstream(str) >> t) ? t :
               (T)(str.size() && (str != "0") && (str != "false"));
    }

    template<>
    inline char astr(const std::string &str) {
        return str.size() == 1 ? (char)(str[0]) : (char)(astr<int>(str));
    }
    template<>
    inline signed char astr(const std::string &str) {
        return str.size() == 1 ? (signed char)(str[0]) : (signed char)(astr<int>(str));
    }
    template<>
    inline unsigned char astr(const std::string &str) {
        return str.size() == 1 ? (unsigned char)(str[0]) : (unsigned char)(astr<int>(str));
    }

    template<>
    inline const char *astr(const std::string &str) {
        return str.c_str();
    }
    template<>
    inline std::string astr(const std::string &str) {
        return str;
    }

    // token split
    inline size_t split(std::vector<std::string> &tokens, const std::string &str, const std::string &delimiters) {
        std::string tmp;
        tokens.clear();
        for(auto &ch : str) {
            if(delimiters.find_first_of(ch) != std::string::npos) {
                if(tmp.size()) tokens.push_back(tmp), tmp = "";
                tokens.push_back(std::string() + ch);
            } else tmp += ch;
        }
        return tmp.empty() ? tokens.size() : (tokens.push_back(tmp), tokens.size());
    };

    // portable cmdline 
    inline std::vector<std::string> cmdline() {
        std::vector<std::string> args;
        std::string arg;
        pid_t pid = getpid();

        char fname[32] = {};
        sprintf(fname, "/proc/%d/cmdline", pid);
        std::ifstream ifs(fname);
        if(ifs.good()) {
            std::stringstream ss;
            ifs >> ss.rdbuf();
            arg = ss.str();
        }
        for(auto end = arg.size(), i = end - end; i < end; ++i) {
            auto st = i;
            while (i < end && arg[i] != '\0') ++i;
            args.push_back(arg.substr(st, i - st));
        }
        return args;
    }
}

// main map class; explicit initialization
struct parseropt : public std::map<std::string, std::string>
{
    using super = std::map<std::string, std::string>;

    parseropt(int argc, const char **argv) : std::map<std::string, std::string>() {
        // reconstruct vector
        std::vector<std::string> args(argc, std::string());
        for(int i = 0; i < argc; ++i) {
           args[i] = argv[i];
        }
        for(auto i = 0; i < args.size(); ++i) {
            std::vector<std::string> tokens;
            if(args[i][0] != '-') continue;

            if (i < args.size() - 1 && args[i + 1][0] != '-') {
                (*this)[args[i]] = args[i + 1];
            } else if (i < args.size() - 1 && args[i + 1][0] == '-') {
                (*this)[args[i]] = true;
            } else {
                (*this)[args[i]] = true;
            }
        }
    }

    parseropt(const std::vector<std::string> &args) : super() {
        std::vector<const char *> argv;
        for(auto &it : args) {
            argv.push_back(it.c_str());
        }
        *this = parseropt(argv.size(), argv.data());
    }

    size_t size() const {
        unsigned i = 0;
        while(has(std::to_string(i))) ++i;
        return i;
    }

    std::string str() const {
        std::stringstream ss;
        std::string sep;
        for( auto &it : *this ) {
            ss << sep << it.first << " " << it.second;
            sep = ' ';
        }
        return ss.str();
    }

    bool has(const std::string &op) const {
        return this->find(op) != this->end();
    }
};

template<typename T>
inline T parseropt(const T &defaults, const char *argv) {
    static struct parseropt map(parseropt_np::cmdline());
    return map.has(argv) ? parseropt_np::astr<T>(map[argv]) : defaults;
}

template<typename T, typename... Args>
inline T parseropt(const T &defaults, const char *arg0, Args... argv) {
    T t = parseropt<T>(defaults, arg0);
    return t == defaults ? parseropt<T>(defaults, argv...) : t;
}

inline const char * parseropt(const char *defaults, const char *argv) {
    static struct parseropt map(parseropt_np::cmdline());
    return map.has(argv) ? parseropt_np::astr<const char *>(map[argv]) : defaults;
}

template<typename... Args>
inline const char * parseropt(const char *defaults, const char *arg0, Args... argv) {
    const char *t = parseropt(defaults, arg0);
    return t == defaults ? parseropt(defaults, argv...) : t;
}
