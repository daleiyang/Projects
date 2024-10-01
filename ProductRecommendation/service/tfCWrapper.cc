#include "tfCWrapper.h"

void free_buffer(void* data, size_t length) { free(data); }

TF_Buffer* read_file(const char* file) {
  FILE *f = fopen(file, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);  //same as rewind(f);

  void* data = malloc(fsize);
  fread(data, fsize, 1, f);
  fclose(f);

  TF_Buffer* buf = TF_NewBuffer();
  buf->data = data;
  buf->length = fsize;
  buf->data_deallocator = free_buffer;
  return buf;
}

static void Deallocator(void* data, size_t length, void* arg) {
  free((void*)data);
}

TF_Tensor* CreateTensor(TF_DataType data_type,
                        const int64_t* dims, size_t num_dims,
                        const void* data, size_t len) {
  if (dims == NULL || data == NULL) {
    return NULL;
  }

  TF_Tensor* tensor = TF_AllocateTensor(data_type, dims, static_cast<int>(num_dims), len);
  if (tensor == NULL) {
    return NULL;
  }

  void* tensor_data = TF_TensorData(tensor);
  if (tensor_data == NULL) {
    TF_DeleteTensor(tensor);
    return NULL;
  }

  memcpy(tensor_data, data, min(len, TF_TensorByteSize(tensor)));

  return tensor;
}

void DeleteTensor(TF_Tensor* tensor) {
  if (tensor == NULL) {
    return;
  }
  TF_DeleteTensor(tensor);
}

TFCWrapper::TFCWrapper() {
  graph_ = NULL;
  sess_ = NULL;
}

TFCWrapper::~TFCWrapper() {
  if (sess_) {
    TF_Status* status = TF_NewStatus();
    TF_CloseSession(sess_, status);
    if (TF_GetCode(status) != TF_OK) {
      cerr << "close session failed " << TF_Message(status);
    }
    TF_DeleteSession(sess_, status);
    if (TF_GetCode(status) != TF_OK) {
      cerr << "delete session failed " << TF_Message(status);
    }
    TF_DeleteStatus(status);
  }
  if (graph_) {
    TF_DeleteGraph(graph_);
  }
}

bool TFCWrapper::Init(const string& pb_file,
                      const vector<string>& inputs,
                      const vector<string>& outputs) {

  TF_Status* status = TF_NewStatus();

  // Import graph_def into graph
  TF_Graph* graph = TF_NewGraph();
  TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();
  TF_Buffer* graph_def = read_file(pb_file.c_str());
  TF_GraphImportGraphDef(graph, graph_def, opts, status);
  TF_DeleteBuffer(graph_def);
  TF_DeleteImportGraphDefOptions(opts);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "import graphdef failed " << TF_Message(status);
    TF_DeleteStatus(status);
    return false;
  }

  TF_SessionOptions* opt = TF_NewSessionOptions();
  uint8_t sess_config[2] = {0x38, 0x01};
  TF_SetConfig(opt, (void*)sess_config, 2, status);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "Unable to parse ConfigProto " << TF_Message(status);
    TF_DeleteSessionOptions(opt);
    TF_DeleteStatus(status);
    return false;
  }
  TF_Session* session = TF_NewSession(graph, opt, status);
  TF_DeleteSessionOptions(opt);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "new session failed " << TF_Message(status);
    TF_DeleteStatus(status);
    return false;
  }
  graph_ = graph;
  sess_ = session;
  inputs_ = inputs;
  outputs_ = outputs;
  TF_DeleteStatus(status);
  return true;
}

bool TFCWrapper::InitWithCKPT(const string& pb_file,
                      const string& ckpt_file_path,
                      const vector<string>& inputs,
                      const vector<string>& outputs) {
  TF_Status* status = TF_NewStatus();

  // Import graph_def into graph
  TF_Graph* graph = TF_NewGraph();
  TF_ImportGraphDefOptions* opts = TF_NewImportGraphDefOptions();
  TF_Buffer* graph_def = read_file(pb_file.c_str());
  TF_GraphImportGraphDef(graph, graph_def, opts, status);
  TF_DeleteBuffer(graph_def);
  TF_DeleteImportGraphDefOptions(opts);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "import graphdef failed " << TF_Message(status);
    TF_DeleteStatus(status);
    return false;
  }

  TF_SessionOptions* opt = TF_NewSessionOptions();
  uint8_t sess_config[2] = {0x38, 0x01};
  TF_SetConfig(opt, (void*)sess_config, 2, status);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "Unable to parse ConfigProto " << TF_Message(status);
    TF_DeleteSessionOptions(opt);
    TF_DeleteStatus(status);
    return false;
  }
  TF_Session* session = TF_NewSession(graph, opt, status);
  TF_DeleteSessionOptions(opt);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "new session failed " << TF_Message(status);
    TF_DeleteStatus(status);
    return false;
  }

  TF_Operation* restore_op = TF_GraphOperationByName(graph, "save/restore_all");

  size_t checkpoint_path_str_len = ckpt_file_path.size();
  size_t encoded_size = TF_StringEncodedSize(checkpoint_path_str_len);

  size_t total_size = sizeof(int64_t) + encoded_size;
  char* input_encoded = (char*)malloc(total_size);
  memset(input_encoded, 0, total_size);
  TF_StringEncode(ckpt_file_path.data(), checkpoint_path_str_len,
                  input_encoded + sizeof(int64_t), encoded_size, status);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "something wrong with encoding: " << TF_Message(status);
    if (input_encoded != NULL) {
      free(input_encoded);
      input_encoded = NULL;
    }
    TF_DeleteStatus(status);
    return false;
  }

  TF_Tensor* path_tensor = TF_NewTensor(TF_STRING, NULL, 0, input_encoded,
                                        total_size, &Deallocator, 0);

  TF_Output run_path = {TF_GraphOperationByName(graph, "save/Const"), 0};

  TF_SessionRun(session,
                // RunOptions 
                NULL,
                // Input tensors 
                &run_path, &path_tensor, 1,
                // Output tensors 
                NULL, NULL, 0,
                // Target operations 
                &restore_op, 1,
                // RunMetadata 
                NULL,
                // Output status 
                status);
  if (TF_GetCode(status) != TF_OK) {
    if (input_encoded != NULL) {
        free(input_encoded);
        input_encoded = NULL;
    }
    cerr << "Unable to run restore_op: " << TF_Message(status);
    DeleteTensor(path_tensor);
    TF_DeleteStatus(status);
    return false;
  }

  graph_ = graph;
  sess_ = session;
  inputs_ = inputs;
  outputs_ = outputs;
  if (input_encoded != NULL) {
    free(input_encoded);
    input_encoded = NULL;
  }
  TF_DeleteStatus(status);
  return true;
}

bool TFCWrapper::Run(const vector<InputValue>& ivalues,
                     vector<OutputValue>& ovalues) {
  vector<TF_Output> inputs;
  for (size_t i = 0; i < inputs_.size(); i++) {
    inputs.push_back({TF_GraphOperationByName(graph_, inputs_[i].c_str()), 0});
  }

  vector<TF_Output> outputs;
  for (size_t i = 0; i < outputs_.size(); i++) {
    outputs.push_back({TF_GraphOperationByName(graph_, outputs_[i].c_str()), 0});
  }

  vector<TF_Tensor*> input_values;
  for (size_t i = 0; i < ivalues.size(); i++) {
    const InputValue& in = ivalues[i];
    input_values.push_back(
      CreateTensor(in.type_, in.dims_, in.num_dims_, in.data, in.len));
  }

  vector<TF_Tensor*> output_values;
  output_values.resize(ovalues.size(), NULL);

  TF_Buffer* run_options = TF_NewBufferFromString("", 0);
  TF_Status* status = TF_NewStatus();
  TF_SessionRun(sess_, run_options,
                &inputs[0], &input_values[0], input_values.size(),
                &outputs[0], &output_values[0], output_values.size(),
                NULL, 0, NULL, status);
  if (TF_GetCode(status) != TF_OK) {
    cerr << "session fun failed: " << TF_Message(status);
    return false;
  }
  TF_DeleteBuffer(run_options);
  TF_DeleteStatus(status);

  for (size_t i = 0; i < ovalues.size(); i++) {
    const OutputValue& out = ovalues[i];
    const TF_Tensor* outvalue = output_values[i];
    int output_dims = TF_NumDims(outvalue);
    if (output_dims != out.num_dims_) {
      cerr << "output dim error " << output_dims << ", " << out.num_dims_;
    } else {
      for (int d = 0; d < output_dims; d++) {
        if (out.dims_[d] != TF_Dim(outvalue, d)) {
          cerr << "output dim size error " << d;
        }
      }
    }
    void* output_data = TF_TensorData(outvalue);
    size_t output_size = TF_TensorByteSize(outvalue);
    if (output_size != out.len) {
      cerr << "output data size error " << output_size << ", " << out.len;
    }
    memcpy(out.data, output_data, min(out.len, output_size));
  }
  for (size_t i = 0; i < input_values.size(); i++) {
    DeleteTensor(input_values[i]);
  }
  for (size_t i = 0; i < output_values.size(); i++) {
    DeleteTensor(output_values[i]);
  }
  return true;
}

