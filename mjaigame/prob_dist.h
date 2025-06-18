#ifndef mjaiC___prob_dist_h
#define mjaiC___prob_dist_h

#include <map>
#include <sstream>
#include <vector>

class ProbDist {
public:
    int randNum;
    std::map<std::vector<double>, double> dist;
    void setRandNum() { randNum = rand(); }
    ProbDist() {
        setRandNum();
        dist.clear();
    };
    ProbDist(std::map<std::vector<double>, double> d) {
        setRandNum();
        dist = d;
    };
    ProbDist(std::vector<double> v, double d = 0) {
        setRandNum();
        dist[v] = d;
    };
    
    std::string toString() {
        std::string ret;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            
            std::vector<double> v = it->first;
            char s[512];
            sprintf(s,"[%.0f,%.0f,%.0f,%.0f] %.4f\n",v[0],v[1],v[2],v[3],it->second);
            ret += s;
        }
        return ret;
    }
    
    bool isTotalValueEqual1() {
        double ret = 0;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            ret += it->second;
        }
        return ret == 1;
    }
    
    bool isTotalValueAlmostEqual1(double acc) {
        
        double ret = 0;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            ret += it->second;
        }
        
        return ret >= 1 - acc && ret <= 1 + acc;
    }
    
    double expectedDouble() {
        double result = 0;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            double weighted = ProbDist::mult((double)it->first[0], it->second);
            if (!result) {
                result = weighted;
            } else {
                result = ProbDist::add(result, weighted);
            }
        }
        return result;
    }
    
    std::vector<double> expectedVector() {
        std::vector<double> result;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            std::vector<double> weighted;
            for (int i = 0; i < it->first.size(); i++) {
                double tmp = it->second * (double)it->first[i];
                weighted.push_back(tmp);
            }
            if (result.size() == 0) {
                result = weighted;
            } else {
                result = ProbDist::add(result, weighted);
            }
        }
        return result;
    }
    ProbDist add(std::vector<double> key, double value) {
        return add(*this, ProbDist(key, value));
    }
    static ProbDist add(ProbDist lhs, ProbDist rhs) {
        std::map<std::vector<double>, double> localDist;
        for (std::map<std::vector<double>, double>::iterator itL = lhs.dist.begin();
             itL != lhs.dist.end(); ++itL) {
            for (std::map<std::vector<double>, double>::iterator itR =
                 rhs.dist.begin();
                 itR != rhs.dist.end(); ++itR) {
                std::vector<double> v = ProbDist::add(itL->first, itR->first);
                if (localDist.find(v) == localDist.end()) {
                    localDist[v] = 0;
                }
                localDist[v] = localDist[v] + itL->second * itR->second;
            }
        }
        ProbDist a = ProbDist(localDist);
        if (!a.isTotalValueAlmostEqual1(0.1)) {
            throw "prob dist err(add)";
        }
        
        return a;
    }
    static std::vector<double> add(std::vector<double> lhs,
                                   std::vector<double> rhs) {
        std::vector<double> ret;
        for (int i = 0; i < lhs.size(); i++)
            ret.push_back(lhs[i] + rhs[i]);
        return ret;
    }
    static double add(double lhs, double rhs) { return lhs + rhs; }
    
    ProbDist mult(std::vector<double> key, double value) {
        return mult(*this, ProbDist(key, value));
    }
    static ProbDist mult(ProbDist lhs, ProbDist rhs) {
        std::map<std::vector<double>, double> localDist;
        for (std::map<std::vector<double>, double>::iterator itL = lhs.dist.begin();
             itL != lhs.dist.end(); ++itL) {
            for (std::map<std::vector<double>, double>::iterator itR =
                 rhs.dist.begin();
                 itR != rhs.dist.end(); ++itR) {
                std::vector<double> v = ProbDist::mult(itL->first, itR->first);
                if (localDist.find(v) == localDist.end()) {
                    localDist[v] = 0;
                }
                localDist[v] = localDist[v] + itL->second * itR->second;
            }
        }
        
        ProbDist a = ProbDist(localDist);
        if (!a.isTotalValueAlmostEqual1(0.1)) {
            throw "prob dist err(mult)";
        }
        return a;
    }
    static std::vector<double> mult(std::vector<double> lhs,
                                    std::vector<double> rhs) {
        if (lhs.size() != 1) {
            throw "probdist mult format error";
        }
        std::vector<double> ret;
        for (int i = 0; i < rhs.size(); i++)
            ret.push_back(lhs[0] * rhs[i]);
        return ret;
    }
    static double mult(double lhs, double rhs) { return lhs * rhs; }
    
    static ProbDist merge(std::map<ProbDist, double> distribution) {
        std::map<std::vector<double>, double> localDist;
        for (std::map<ProbDist, double>::iterator it = distribution.begin();
             it != distribution.end(); ++it) {
            ProbDist tmp = it->first;
            for (std::map<std::vector<double>, double>::iterator itd =
                 tmp.dist.begin();
                 itd != tmp.dist.end(); ++itd) {
                localDist[itd->first] =
                localDist[itd->first] + it->second * itd->second;
            }
        }
        ProbDist a = ProbDist(localDist);
        if (!a.isTotalValueAlmostEqual1(0.1)) {
            double total = 0;
            for (std::map<std::vector<double>, double>::iterator it = a.dist.begin();
                 it != a.dist.end(); ++it) {
                total += it->second;
            }
            
            std::ostringstream fstr;
            fstr << "total = " << total << " ";
            std::string th = fstr.str();
            th += "prob dist err(merge)";
#ifdef __LINE__
            char s[256];
            sprintf(s, "%d", __LINE__);
            th = th + s;
#else
#endif
            char *cstr = new char[256];
            std::strcpy(cstr, th.c_str());
            throw cstr;
        }
        return a;
    }
    
    static bool equal(ProbDist lhs, ProbDist rhs) {
        return lhs.dist.size() == rhs.dist.size() &&
        std::equal(lhs.dist.begin(), lhs.dist.end(), rhs.dist.begin());
    }
    void set(std::vector<double> v, double _default = 0) { dist[v] = (_default); }
    
    double get(std::vector<double> v, double _default = 0) {
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            if (it->first == (v)) {
                return it->second;
            }
        }
        return _default;
    }
    void upsert(std::vector<double> v, double m) { dist[v] = m; }
    
    ProbDist mapValue4GetAverageRank() {
        ProbDist ret;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            int cnt = 4;
            for (int j = 0; j < 4; j++) {
                if (it->first[j] == 1) {
                    cnt--;
                }
            }
            std::vector<double> newValue{(double)cnt};
            ret.dist[newValue] += it->second;
        }
        return ret;
    }
    
    ProbDist mapValue4tenpaisToRyukyokuPoints() {
        std::map<std::vector<double>, double> localDist;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            if (!it->second) {
                continue;
            }
            double cnt = 0;
            for (int j = 0; j < 4; j++) {
                if (it->first[j] > 0) {
                    cnt++;
                }
            }
            std::vector<double> newValue;
            if (cnt == 0 || cnt == 4) {
                for (int k = 0; k < 4; k++) {
                    newValue.push_back(0);
                }
            } else {
                for (int k = 0; k < 4; k++) {
                    if (it->first[k] > 0) {
                        newValue.push_back(3000.0 / cnt);
                    } else {
                        newValue.push_back(-3000.0 / (4 - cnt));
                    }
                }
            }
            localDist[newValue] += it->second;
        }
        return ProbDist(localDist);
    }
    
    ProbDist replace(std::vector<double> oldValue, ProbDist newPb) {
        std::map<std::vector<double>, double> localDist;
        double localprob = 0;
        for (std::map<std::vector<double>, double>::iterator it = dist.begin();
             it != dist.end(); ++it) {
            if (isEqualVector(it->first, oldValue)) {
                localprob = it->second;
            } else {
                localDist[it->first] = it->second;
            }
        }
        for (std::map<std::vector<double>, double>::iterator it =
             newPb.dist.begin();
             it != newPb.dist.end(); ++it) {
            localDist[it->first] =
            localDist[it->first] + ((double)it->second * localprob);
        }
        ProbDist a = ProbDist(localDist);
        if (!a.isTotalValueAlmostEqual1(0.1)) {
            double total = 0;
            for (std::map<std::vector<double>, double>::iterator it = a.dist.begin();
                 it != a.dist.end(); ++it) {
                total += it->second;
            }
            
            std::ostringstream fstr;
            fstr << "total = " << total << " ";
            std::string th = fstr.str();
            th += "prob dist err(replace)";
#ifdef __LINE__
            char s[256];
            sprintf(s, "%d", __LINE__);
            th = th + s;
#else
#endif
            char *cstr = new char[256];
            std::strcpy(cstr, th.c_str());
            throw cstr;
        }
        return a;
    }
    
    bool find(std::vector<double> v) { return dist.find(v) != dist.end(); }
    double findAndGetProb(std::vector<double> v) { return dist[v]; }
    
    static bool isEqualVector(std::vector<double> a, std::vector<double> b) {
        if (a.size() != b.size())
            return false;
        for (int i = 0; i < a.size(); i++) {
            if (a[i] != b[i])
                return false;
        }
        return true;
    }
    bool operator<(const ProbDist &obj) const {
        if (randNum != obj.randNum) {
            return randNum < obj.randNum;
        }
        return dist.size() < obj.dist.size();
    }
};
#endif
