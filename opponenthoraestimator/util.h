#ifndef mjaiC___util_h
#define mjaiC___util_h

#include <algorithm>
#include <string>
#include <time.h>
#include <vector>

#include <iostream>
#include <stdarg.h>

#define PRINT_LOG_UTIL 0

class Util {
public:
    static void writeFileToLogTxt(char *s) {
        if (!PRINT_LOG_UTIL) {
            return;
        }
        FILE *fp;
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
        std::random_shuffle(array.begin(), array.end());
    }
    static void shuffleWithSeed(int array[], std::vector<int> &ret, int seed) {
        srand((unsigned)seed);
        int num_ids = sizeof(array) / sizeof(array[0]);
        std::random_shuffle(&array[0], &array[num_ids]);
        for (int i = 0; i < num_ids; i++)
            ret.push_back(array[i]);
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
        char *buffer;
        vasprintf(&buffer, format, ap);
        va_end(ap);
        
        std::cout << buffer << std::endl;
        free(buffer);
#endif
    }
    static int convertMJAIPainumToMjscore(int num, bool red = false) {
        
        if (num < 0 || num >= 34) {
            //std::cout << "renge error convertMJAIPainumToMjscore" << std::endl;
            return 0;
        }
        
        if (red) {
            if (num == 4) {
                return 38;
            }
            if (num == 13) {
                return 39;
            }
            if (num == 22) {
                return 40;
            }
        }
        int idxMJAIToMjscore = 0;
        if (num < 9) {
            idxMJAIToMjscore = num + 1;
        } else if (num >= 9 && num < 18) {
            idxMJAIToMjscore = num + 2;
        } else if (num >= 18 && num < 27) {
            idxMJAIToMjscore = num + 3;
        } else {
            idxMJAIToMjscore = num + 4;
        }
        
        return idxMJAIToMjscore;
    }
    
    static int convertMjscorePainumToMjAI(int num, bool red = false) {
        
        if (num < 0 || num >= 41) {
            //std::cout << "renge error convertMjscorePainumToMjAI" << std::endl;
            return 0;
        }
        
        if (num == 38) {
            return 5;
        }
        if (num == 39) {
            return 15;
        }
        if (num == 40) {
            return 25;
        }
        
        int idx = 0;
        if (num / 10 == 0) {
            idx = num - 1;
        } else if (num / 10 == 1) {
            idx = num - 2;
        } else if (num / 10 == 2) {
            idx = num - 3;
        } else {
            idx = num - 4;
        }
        return idx;
    }
};

#endif
