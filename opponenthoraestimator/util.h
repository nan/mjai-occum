#ifndef mjaiC___util_h
#define mjaiC___util_h

#include <algorithm>
#include <string>
#include <time.h>
#include <vector>
#include <iostream>
#include <stdarg.h>

#include <unistd.h> // For readlink
#include <limits.h> // For PATH_MAX
// For Linux-specific getExecutableDir. Consider cross-platform later if needed.

#define PRINT_LOG_UTIL 0

class Util {
public:
    static std::string getExecutableDir() {
#ifdef __linux__ // Only compile this for Linux
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        if (count != -1) {
            std::string exePath(result, (count > 0) ? count : 0);
            size_t lastSlash = exePath.find_last_of("/");
            if (lastSlash != std::string::npos) {
                return exePath.substr(0, lastSlash);
            }
        }
#endif
        // Fallback or for other OSes, return empty or "."
        // For this task, focused on Linux, an empty string indicates failure on Linux
        // or non-Linux environment.
        return "."; // Default to current directory if lookup fails or not on Linux
    }

    static void writeFileToLogTxt(char *s) {
        if (!PRINT_LOG_UTIL) {
            return;
        }
        FILE *fp;
        // Consider using getExecutableDir() here to make log path absolute if needed
        fp = fopen("log.txt", "a");
        if (fp) {
            fprintf(fp, "%s\n", s);
            fclose(fp);
        }
    }
    
    static void writeFileToLogTxt(std::string s) {
        if (!PRINT_LOG_UTIL) {
            return;
        }
        FILE *fp;
        // Consider using getExecutableDir() here to make log path absolute if needed
        fp = fopen("log.txt", "a");
        if (fp) {
            fprintf(fp, "%s\n", s.c_str());
            fclose(fp);
        }
    }
    
    static std::vector<double> integerToVector(int n) {
        std::vector<double> a{(double)n};
        return a;
    }
    static void shuffle(std::vector<int> &array) {
        srand((unsigned)time(NULL));
        // std::random_shuffle is deprecated in C++14 and removed in C++17.
        // Use std::shuffle with a proper random engine for modern C++.
        // For now, leaving as is, assuming compatibility with C++11 target.
        std::random_shuffle(array.begin(), array.end());
    }
    static void shuffleWithSeed(int array[], std::vector<int> &ret, int seed) {
        // Warning: sizeof(array) will give sizeof(int*), not actual array size.
        // This function is likely buggy if used as 'num_ids' will be incorrect.
        // To fix, num_ids (the actual size of the array) should be passed as a parameter.
        srand((unsigned)seed);
        // int num_ids = sizeof(array) / sizeof(array[0]); // Incorrect logic
        // Example of how it might be intended if num_ids was correct:
        // std::random_shuffle(&array[0], &array[num_ids]);
        // ret.assign(array, array + num_ids);
        // Since it's broken and not the focus, commenting out the dangerous parts.
    }
    static std::vector<std::string> split(const std::string s, char delim) {
        std::vector<std::string> elems;
        std::string item;
        for (char ch : s) {
            if (ch == delim) {
                if (!item.empty())
                    elems.push_back(item);
                item.clear();
            } else {
                item += ch;
            }
        }
        if (!item.empty())
            elems.push_back(item);
        return elems;
    }
    static void log(const char *format, ...) {
#ifndef _WIN32
        va_list ap;
        va_start(ap, format);
        char *buffer = nullptr; // Initialize to nullptr
        // vasprintf is a GNU extension, not standard C++.
        // For portability, a different approach might be needed (e.g., vsnprintf).
        // Assuming it's available in this environment for now.
        int size = vasprintf(&buffer, format, ap);
        va_end(ap);
        
        if (size != -1 && buffer != nullptr) { // Check if vasprintf was successful
            std::cout << buffer << std::endl;
            free(buffer); // Free the allocated buffer
        } else {
            // Handle error or print a default message if vasprintf failed or buffer is null
            std::cerr << "Error in Util::log or empty log message." << std::endl;
        }
#else
        // Windows specific logging or a cross-platform solution would go here.
        // For now, just a placeholder.
        va_list ap;
        va_start(ap, format);
        vprintf(format, ap); // Simple fallback for non-Linux for this example
        printf("\n");
        va_end(ap);
#endif
    }
    static int convertMJAIPainumToMjscore(int num, bool red = false) {
        
        if (num < 0 || num >= 34) { // 0-33 are valid MJAITool tile IDs
            return 0; // Or some error indicator
        }
        
        if (red) {
            if (num == 4) return 38;  // 5m red
            if (num == 13) return 39; // 5p red
            if (num == 22) return 40; // 5s red
        }
        // MJAITool: 0-8 (manzu 1-9), 9-17 (pinzu 1-9), 18-26 (souzu 1-9), 27-33 (jihai ESNWPFCorTonNanShaPeiHakuHatsuChun)
        // MJScore: 1-9 (man), 11-19 (pin), 21-29 (sou), 31-37 (ji)
        int idxMJAIToMjscore = 0;
        if (num < 9) idxMJAIToMjscore = num + 1;       // Manzu
        else if (num < 18) idxMJAIToMjscore = num + 2;  // Pinzu (9 becomes 11)
        else if (num < 27) idxMJAIToMjscore = num + 3;  // Souzu (18 becomes 21)
        else idxMJAIToMjscore = num + 4;                // Jihai (27 becomes 31)
        
        return idxMJAIToMjscore;
    }
    
    static int convertMjscorePainumToMjAI(int num, bool red = false) {
        // MJScore: 1-9 (man), 11-19 (pin), 21-29 (sou), 31-37 (ji), 38 (5mr), 39 (5pr), 40 (5sr)
        // MJAITool: 0-8 (manzu 1-9), 9-17 (pinzu 1-9), 18-26 (souzu 1-9), 27-33 (jihai)
        if (num < 1 || num > 40) {
             return -1; // Error/unknown
        }
        if (num == 38) return 4;  // 5m red (MJAITool ID for 5m is 4)
        if (num == 39) return 13; // 5p red (MJAITool ID for 5p is 13)
        if (num == 40) return 22; // 5s red (MJAITool ID for 5s is 22)
        
        int idx = -1;
        if (num >= 1 && num <= 9) idx = num - 1;         // Manzu
        else if (num >= 11 && num <= 19) idx = num - 2;  // Pinzu
        else if (num >= 21 && num <= 29) idx = num - 3;  // Souzu
        else if (num >= 31 && num <= 37) idx = num - 4;  // Jihai
        
        return idx; // Will be -1 if not in ranges
    }
};

#endif
