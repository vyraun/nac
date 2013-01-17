#ifndef _NAC_H
#define _NAC_H

#include <iostream>
#include <fstream>
#include <random>
#include "opencv2/core/core.hpp"
#include <time.h>

#ifndef TRACKER_H
#include "tracker.h"
#endif

#ifndef TLNN_H
#include "tlnn.h"
#endif

class nac{
protected:
    cv::Mat va, vz, vy, vyp, vv, vw, dvv, dvw; // value network
    cv::Mat pa, pz, pv, pw, py, pd, dlpv, dlpw, ngpv, ngpw, dngpw;
    cv::Mat gIv, gIw, gPsiV, gPsiW, psiGPsiV, psiGPsiW, psiW, psiV, dpv, dpw; // policy network
    cv::Mat action, cov, invCov, alpha_ng;
    int nvI, npI, nvH, nvY, npH, npY;
    bool updateFlag;
    float alpha, beta, gamma, wr, tde;
    std::random_device rd;
public:
    nac();
    ~nac();
    // network functions
    void init(int _nvI, int _nvY, int _npI, int _npY, float _alpha, float _beta, float _gamma, float _wr);
    void update(const cv::Mat& ps, const cv::Mat& vs, float reward);
    void updateFisher(const cv::Mat& ps, const cv::Mat& vs, float reward, bool fisherFlag);
    void evalPol(const cv::Mat& ps);
    // high level exploring functions
    void train(MatL& miu, MatL& pmiu, MatL& cov, cv::Mat& evarBinary, cv::Mat& mask, cv::Mat& dispImg, 
        cv::Mat& kernel, cv::Mat& lrw, cv::Mat& action, cv::Mat& ps, cv::Mat& vs, MatL& kpData, int tidx, 
        int nFrame, int exploreIter, int kDim, int yDim, int maskVal, float epsilon, tracker& t, 
        gmem& gm, fio& fileIO);
    void test(MatL& miu, MatL& pmiu, MatL& cov, cv::Mat& evarBinary, cv::Mat& mask, cv::Mat& dispImg, 
        cv::Mat& kernel, cv::Mat& lrw, cv::Mat& action, cv::Mat& ps, MatL& kpData, int tidx, int nFrame, 
        int maskVal, int xDim, tracker& t, gmem& gm, fio& fileIO);
    // low level exploring functions
    void genAction(bool exploreFlag, std::mt19937& engine);
    void act(MatL& miuL, MatL& pmiuL, MatL& covL, cv::Mat& _action, int tidx, bool exploreFlag, 
            tracker& t, cv::Mat& lrw, cv::Mat& x, bool actionFlag, std::mt19937& engine);
    void setCov(float xr, float yr, float lxr, float sxr, float ar);
    void copyPw(cv::Mat& lrw);
    void resetEllipse(MatL& miuL, MatL& covL, const cv::Mat& lrw, const cv::Mat& x, tracker& t, int tidx);
    void setUpdateFlag(bool _updateFlag);
    void preProcess(cv::Mat& action);
    void postProcess(cv::Mat& action, bool yFlag);
    void recordData(cv::Mat& rwd, cv::Mat& pwn, cv::Mat& vwn, cv::Mat& ngwn, cv::Mat& val, cv::Mat& td, 
            float r, int count);
    void recordDataByElement(cv::Mat& rwd, cv::Mat& pwn, cv::Mat& vwn, cv::Mat& ngwn, cv::Mat& val, 
            cv::Mat& td, float r, int count);
    // unused nonlinear functions
    void initNonLinear(int _nvI, int _npI, int _nvH, int _nvY, int _npH, int _npY, float _alpha,
            float _beta, float _gamma, float _wr);
    void updateNonLinear(const cv::Mat& policyState, const cv::Mat& valueState, float reward);
    void updateFisherNonLinear(const cv::Mat& ps, const cv::Mat& vs, float reward, bool fisherFlag);
    void evalPolNonLinear(const cv::Mat& s);
    void resetEllipseNonLinear(MatL& miuL, MatL& covL, const cv::Mat& ps, tlnn& net, tracker& t, int tidx);
    void recordDiagnosisNonLinear(cv::Mat& rwd, cv::Mat& pwn, cv::Mat& pvn, cv::Mat& vwn, cv::Mat& vvn, 
            cv::Mat& ngwn, cv::Mat& ngvn, cv::Mat& val, cv::Mat& td, float r, int count);
    void copyPwNonLinear(tlnn& net);
    void trainNonLinear(MatL& miu, MatL& pmiu, MatL& cov, cv::Mat& evarBinary, cv::Mat& mask, cv::Mat& dispImg, 
        cv::Mat& prop, cv::Mat& action, cv::Mat& ps, cv::Mat& vs, MatL& kpData, int tidx, int nFrame,
        int exploreIter, int xDim, int tDim, int maskVal, float epsilon, tracker& t, gmem& gm, fio& fileIO,
        tlnn& net);
    void testNonLinear(MatL& miu, MatL& pmiu, MatL& cov, cv::Mat& evarBinary, cv::Mat& mask, cv::Mat& dispImg, 
        cv::Mat& prop, cv::Mat& action, cv::Mat& ps, MatL& kpData, int tidx, int nFrame, int maskVal, int xDim,  
        tracker& t, gmem& gm, fio& fileIO, tlnn& net);
    
};

#endif

// void recordDiagnosis(float* reward, float* pwn, float* pvn, float* vwn, float* vvn, 
//         float* ngwn, float* ngvn, float* value, float* td, float r, int count);
// void recordDiagnosisLinear(cv::Mat& reward, cv::Mat& pwn, cv::Mat& vwn, cv::Mat& ngwn, 
//         cv::Mat& value, cv::Mat& td, float r, int count); 
// void policyForwardPropagateLinear(const cv::Mat& state);
// void updateLinearNetwork(const cv::Mat& policyState, const cv::Mat& valueState, float reward);
// void initializeLinearNetwork(int _nvI, int _npI, int _nvY, int _npY, float _alpha, float _beta, 
//         float _gamma, float _wr, float _tMax, float _tMin, float _t0, float _sigma);
// float generateActionRelative(bool trainFlag, float tp);
