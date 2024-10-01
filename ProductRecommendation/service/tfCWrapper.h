#ifndef ALGORITHM_TENSORFLOWC_SRC_TFCWRAPPER
#define ALGORITHM_TENSORFLOWC_SRC_TFCWRAPPER

//tensorflow
#include "Common/ThirdParty/tf_c_api/include/tensorflow/c/c_api.h"

//c
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//c++
#include <iostream>
#include <vector>
using namespace std;

class TFCWrapper {
private:
    TF_Graph* graph_;
    TF_Session* sess_;
    vector<string> inputs_;
    vector<string> outputs_;

public:
    struct InputValue {
        TF_DataType type_;
        int64_t* dims_;
        int num_dims_;
        void* data;
        size_t len;
    };
    struct OutputValue {
        TF_DataType type_;
        int64_t* dims_;
        int num_dims_;
        void* data;
        size_t len;
    };

public:
    TFCWrapper();
    ~TFCWrapper();
    bool Init(const string& pb_file, const vector<string>& inputs, const vector<string>& outputs);
    bool InitWithCKPT(const string& pb_file, const string& ckpt_file_path, const vector<string>& inputs, const vector<string>& outputs);
    bool Run(const vector<InputValue>& inputs, vector<OutputValue>& outputs);

};
#endif  // ALGORITHM_TENSORFLOWC_SRC_TFCWRAPPER

