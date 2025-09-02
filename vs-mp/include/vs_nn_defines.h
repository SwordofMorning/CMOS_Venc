/**
* @file vs_nn_defines.h
* @brief Declaration of nn enumeration and struct
* @details Declaration of nn enumeration and struct
* @author VS npu group
* @date 2022-5-25
* @version v0.10
* @Copyright (C) 2022, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_NN_DEFINES_H__
#define __VS_NN_DEFINES_H__

#include "vs_comm.h"
#include "vs_errno.h"

#ifdef __cplusplus
extern "C"{
#endif

///< Define buffer alignment be using for store the model binary file
#define NN_MODEL_BUFFER_ALIGNMENT                64
///< Define the max length of input/output name
#define NN_MAX_NODE_NAME_LEN                     64
///< Ddefine the max length of model name
#define NN_MAX_MODEL_NAME_LEN                    64
///< Define the max dim count of each node
#define NN_MAX_DIM_COUNT                         6
///< The max number of inputs for network
#define NN_MAX_INPUT_NODES                       16
///< The max number of outputs for network
#define NN_MAX_OUTPUT_NODES                      16
///< The max number of ROI inputs/outputs support for network
#define NN_MAX_RPN_NODES                         4

typedef enum vs_nn_errno {
    E_NN_ERR_INVALID_FILE = 100,                           ///< the binary model file is invalid
    E_NN_ERR_INVALID_MODEL,                                ///< the model can't import to graph
    E_NN_ERR_INVALID_NODE,                                 ///< create the model node failed
    E_NN_ERR_INVALID_KERNEL,                               ///< create the kernel failed
    E_NN_ERR_INVALID_INPUT_NUM,                            ///< input num beyond the support
    E_NN_ERR_INVALID_OUTPUT_NUM,                           ///< output num beyond the support
    E_NN_ERR_VERIFIED_FAILED,                              ///< failed to verified model
    E_NN_ERR_MAX
} vs_nn_errno_e;

#define VS_ERR_NN_INVALID_PARAM                  VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_INVALID_PARAM)
#define VS_ERR_NN_INVALID_LEN                    VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_INVALID_LEN)
#define VS_ERR_NN_BAD_STATE                      VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_BAD_STATE)
#define VS_ERR_NN_NULL_PTR                       VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NULL_PTR)
#define VS_ERR_NN_EXIST                          VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_EXIST)
#define VS_ERR_NN_UNEXIST                        VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_UNEXIST)
#define VS_ERR_NN_NOT_SUPPORT                    VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NOT_SUPPORT)
#define VS_ERR_NN_NOT_IMPLEMENTED                VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NOT_IMPLEMENTED)
#define VS_ERR_NN_NOT_INITIALIZED                VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NOT_INITIALIZED)
#define VS_ERR_NN_NOT_PERM                       VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NOT_PERM)
#define VS_ERR_NN_NO_MEMORY                      VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_NO_MEMORY)
#define VS_ERR_NN_BUF_EMPTY                      VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_BUF_EMPTY)
#define VS_ERR_NN_BAD_ADDR                       VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_BAD_ADDR)
#define VS_ERR_NN_BUSY                           VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_BUSY)
#define VS_ERR_NN_TIMEOUT                        VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_TIMEOUT)
#define VS_ERR_NN_ABORTED                        VS_ERR_CODE(E_MOD_ID_NPU, E_ERR_ABORTED)

#define VS_ERR_NN_INVALID_FILE                   VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_FILE)
#define VS_ERR_NN_INVALID_MODEL                  VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_MODEL)
#define VS_ERR_NN_INVALID_NODE                   VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_NODE)
#define VS_ERR_NN_INVALID_KERNEL                 VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_KERNEL)
#define VS_ERR_NN_INVALID_INPUT_NUM              VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_INPUT_NUM)
#define VS_ERR_NN_INVALID_OUTPUT_NUM             VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_INVALID_OUTPUT_NUM)
#define VS_ERR_NN_VERIFIED_FAILED                VS_ERR_CODE(E_MOD_ID_NPU, E_NN_ERR_VERIFIED_FAILED)

/**
* @brief Define cluster id of multi core npu hardware device
* @details There support two cores now for npu cluster
*/
typedef enum vs_nn_cluster_id {
    E_NN_CLUSTER_ID_0 = 0,                                 ///< define cluster index 0
    E_NN_CLUSTER_ID_1,                                     ///< define cluster index 1
    E_NN_CLUSTER_ID_MAX
} vs_nn_cluster_id_e;

/**
* @brief Define the memory information that npu used
* @details the Define memory information that npu used
*/
typedef struct vs_nn_mem_info {
    vs_uint32_t         size;                              ///< the size of memory buffer
    vs_void_t           *p_virt_addr;                      ///< the virtual address for the buffer
    vs_uint64_t         phys_addr;                         ///< the physic address for the buffer
} vs_nn_mem_info_s;

typedef vs_nn_mem_info_s vs_nn_src_mem_info_s;
typedef vs_nn_mem_info_s vs_nn_dst_mem_info_s;
typedef vs_nn_mem_info_s vs_nn_model_mem_info_s;

/**
* @brief Define the data format
* @details Define the data format that nn sdk support for input and output
*/
typedef enum vs_nn_data_format{
    E_NN_DATA_FORMAT_FP32 = 0,
    E_NN_DATA_FORMAT_FP16,
    E_NN_DATA_FORMAT_UINT8,
    E_NN_DATA_FORMAT_INT8,
    E_NN_DATA_FORMAT_UINT16,
    E_NN_DATA_FORMAT_INT16,
    E_NN_DATA_FORMAT_CHAR,
    E_NN_DATA_FORMAT_BFP16,
    E_NN_DATA_FORMAT_INT32,
    E_NN_DATA_FORMAT_UINT32,
    E_NN_DATA_FORMAT_INT64,
    E_NN_DATA_FORMAT_UINT64,
    E_NN_DATA_FORMAT_FP64,
    E_NN_DATA_FORMAT_MAX
} vs_nn_data_format_e;

/**
* @brief Define the data type
* @details Define the data type that nn sdk support for input and output
*/
typedef enum vs_nn_data_type {
    E_NN_DATA_TYPE_TENSOR = 0,                             ///< the type is tensor data, be pre-processed
    E_NN_DATA_TYPE_IMAGE,                                  ///< the type is image, such as rgb/bgr/nv12/nv21
    E_NN_DATA_TYPE_ARRAY,                                  ///< array data
    E_NN_DATA_TYPE_SCALAR,                                 ///< scalar data
    E_NN_DATA_TYPE_MAX
} vs_nn_data_type_e;

/**
* @brief Define the data type
* @details Define the data type that nn sdk support for input and output
*/
typedef struct vs_nn_node {
    vs_uint32_t         id;                                ///< input or output node id
    vs_nn_data_format_e format;                            ///< the format of data be store in node
    vs_nn_data_type_e   type;                              ///< the input/output data type
    vs_uint32_t         dim_count;                         ///< the dimension of input/output
    vs_uint32_t         dim_size[NN_MAX_DIM_COUNT];        ///< the size of each dimension, input node in WHCN order
    vs_char_t           bottom_name[NN_MAX_NODE_NAME_LEN]; ///< bottom_name of report layer or data layer
} vs_nn_node_s;

/**
* @brief Define the network model struct
* @details Detail define the model information, include input/output, name
*/
typedef struct vs_nn_model {
    vs_char_t           model_name[NN_MAX_MODEL_NAME_LEN]; ///< The name of NBG model
    vs_uint32_t         buffer_size;                       ///< The size of buffer be used to run model, max sum of buffer sizes required by driver, input and output
    vs_uint32_t         input_num;                         ///< The number of input nodes for this model
    vs_uint32_t         output_num;                        ///< The number ofr output nodes for this model
    vs_nn_node_s        inputs[NN_MAX_INPUT_NODES];        ///< Input nodes
    vs_nn_node_s        outputs[NN_MAX_OUTPUT_NODES];      ///< output nodes
} vs_nn_model_s;

/**
* @brief Define the model parameter
* @details Detail define model parameters which can be set by application
*/
typedef struct vs_nn_model_param {
    vs_uint32_t         priority;                          ///< the priority of network
    vs_nn_cluster_id_e  cluster_id;                        ///< set the cluster index which will run the model later
    vs_bool_t           enable_pre_proc;                   ///< whether to enable npu pre process, read-only, can only be obtained through vs_mal_nn_model_param_get
    vs_bool_t           enable_post_proc;                  ///< whether to enable npu post process, read-only, can only be obtained through vs_mal_nn_model_param_get
} vs_nn_model_param_s;

/**
* @brief Define the blob data
* @details Detail define the model general data structure
*/
typedef struct vs_nn_data {
    vs_nn_data_format_e format;                            ///< input/ouput data format
    vs_uint32_t         stride;                            ///< line with alignment, the width and stride of the input data must be equal, reserved
    vs_nn_mem_info_s    buffer;                            ///< data buffer information
} vs_nn_data_s;

typedef vs_nn_data_s vs_nn_src_data_s;
typedef vs_nn_data_s vs_nn_dst_data_s;

/**
* @brief Define forward attributes for normal CNN model
* @details Define the detail forward information for each influence
*/
typedef struct vs_nn_invoke_attr {
    vs_uint32_t         src_num;                           ///< number of input data
    vs_uint32_t         dst_num;                           ///< number of output datta
    vs_nn_src_data_s    src_data[NN_MAX_INPUT_NODES];      ///< input data array
    vs_nn_dst_data_s    dst_data[NN_MAX_OUTPUT_NODES];     ///< output data array
} vs_nn_invoke_attr_s;

#ifdef __cplusplus
}
#endif
#endif /*!<  __VS_NN_DEFINES_H__ */
