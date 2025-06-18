#ifndef mjaiC___pai_h
#define mjaiC___pai_h

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include "util.h"
#define NUM_IDS 34

const std::vector<std::string> TYPE_STRS{
    "m",
    "p",
    "s",
    "t"};
const std::vector<std::string> TSUPAI_STRS{
    "",
    "E",
    "S",
    "W",
    "N",
    "P",
    "F",
    "C"};

class Pai
{
public:
    int identifier;
    std::string type;
    int number;
    bool red;
    Pai()
    {
    }
    Pai(int _id)
    {
        identifier = _id;
        red = false;
        type = getType();
        number = getNumber();
    }
    Pai(std::string _type, int _number, bool _red = false)
    {
        initConst();
        type = _type;
        number = _number;
        red = _red;
        setIdentifier();
    }
    Pai(std::string s)
    {
        initConst();
        bool checked = false;
        if (!checked) {
            if (s ==
                "?") {
                type = number = NULL;
                red = false;
                checked = true;
                identifier = -1;
                return;
            }
        }
        if (!checked) {
            std::regex re(
                          "^([1-9])([mps])(r)?$");
            std::smatch sm;
            if (std::regex_match(s, sm, re)) {
                type = sm[2];
                number = std::stoi(sm[1]);
                red = sm[3] == "r" ? true : false;
                checked = true;
                setIdentifier();
                return;
            }
        }
        if (!checked) {
            int jiIdx = getIndexJi(s);
            if (jiIdx >= 1) {
                type =
                "t";
                number = jiIdx;
                red = false;
                checked = true;
                setIdentifier();
                return;
            }
        }
        if (!checked) {
            throw
            "unknown hai strings";
        }
    }
    void setIdentifier()
    {
        //    if (identifier >= 0 && identifier <= NUM_IDS) {
        //      return;
        //    }
        // if (type == "" || !number) {
        int type_index = getIndexType(type);
        
        if (type_index == -1) {
            throw
            "bad type";
        }
        if (number < 1 || number > 9) {
            throw
            "number out of range";
        }
        identifier = type_index * 9 + (number - 1);
        //}
    }
    static int getIdentifier(std::string _type, int _number)
    {
        int type_index = getIndexType(_type);
        
        if (type_index == -1) {
            throw
            "bad type";
        }
        if (_number < 1 || _number > 9) {
            throw
            "number out of range";
        }
        return type_index * 9 + (_number - 1);
    }
    
    int getIndexJi(std::string ji)
    {
        for (int i = 0; i < TSUPAI_STRS.size(); i++) {
            if (TSUPAI_STRS[i] == ji) {
                return i;
            }
        }
        return -1;
    }
    static int getIndexType(std::string t)
    {
        for (int i = 0; i < TYPE_STRS.size(); i++) {
            if (TYPE_STRS[i] == t) {
                return i;
            }
        }
        return -1;
    }
    
    void initConst()
    {
        type =
        "";
        number = 0;
        red = false;
    }
    std::string getType()
    {
        if (identifier < 0 || identifier > NUM_IDS) {
            return "";
        }
        return TYPE_STRS[identifier / 9];
    }
    int getNumber()
    {
        if (identifier < 0 || identifier > NUM_IDS) {
            return 0;
        }
        return identifier % 9 + 1;
    }
    bool hasSameSymbol(Pai other)
    {
        return identifier == other.identifier;
    }
    bool equal(Pai other)
    {
        return other.identifier == identifier && other.red == red;
    }
    bool isIn(std::vector<Pai> pais)
    {
        for (int i = 0; i < pais.size(); i++) {
            if (equal(pais[i])) {
                return true;
            }
        }
        return false;
    }
    Pai nextForDora()
    {
        int nextNumber = 0;
        if ((type ==
             "t" &&
             number == 4) ||
            (type !=
             "t" &&
             number == 9)) {
                nextNumber = 1;
            } else if (type ==
                       "t" &&
                       number == 7) {
                nextNumber = 5;
            } else {
                nextNumber = number + 1;
            }
        return Pai(type, nextNumber, false);
    }
    Pai originForDora()
    {
        int originNumber = 0;
        if (type ==
            "t" &&
            number == 1) {
            originNumber = 4;
        }
        else if (type !=
                 "t" &&
                 number == 1) {
            originNumber = 9;
        }
        else if (type ==
                 "t" &&
                 number == 5) {
            originNumber = 7;
        }
        else {
            originNumber = number - 1;
        }
        return Pai(type, originNumber, false);
    }
    bool isYaochu()
    {
        return type ==
        "t" ||
        number == 1 || number == 9;
    }
    Pai removeRed()
    {
        return Pai(identifier);
    }
    bool hasNext(int n)
    {
        int tmpNumber = number + n;
        if (type !=
            "t" &&
            tmpNumber >= 1 && tmpNumber <= 9) {
            return true;
        }
        return false;
    }
    Pai next(int n)
    {
        int tmpNumber = number + n;
        if (type !=
            "t" &&
            tmpNumber >= 1 && tmpNumber <= 9) {
            return Pai(type, tmpNumber, false);
        }
        // throw "illegal argument Pai::next";
        return Pai(
                   "?");
    }
    std::string toString()
    {
        if (identifier ==
            -1) {
            return "?";
        }
        if (type ==
            "t") {
            return TSUPAI_STRS[number];
        }
        std::string ret = std::to_string(number);
        ret += type;
        ret += red ? "r" : "";
        return ret;
    }
    static std::string paisToStr(std::vector<Pai> pais)
    {
        std::string ret =
        "";
        for (int i = 0; i < pais.size(); i++) {
            ret += pais[i].toString() +
            " ";
        }
        return ret;
    }
    static std::vector<Pai> strToPais(std::string str)
    {
        std::vector<std::string> separetedStrings = Util::split(str, ' ');
        std::vector<Pai> ret;
        for (int i = 0; i < separetedStrings.size(); i++) {
            ret.push_back(Pai(separetedStrings[i]));
        }
        return ret;
    }
    static int compare(Pai lhs, Pai rhs)
    {
        return lhs.identifier - rhs.identifier;
    }
    
    static std::vector<std::string> split(const std::string s, char delim)
    {
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
    bool operator<(const Pai& right) const
    {
        if (identifier < right.identifier) {
            return true;
        } else if (red < right.red) {
            return true;
        }
        return false;
    }
};

#endif
