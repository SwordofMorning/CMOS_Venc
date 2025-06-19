/**
 * @file    sample_common_nn.c
 * @brief   sample nn implementation
 * @details
 * @author  Visinex Software Group
 * @date    2022-06-25
 * @version v1.00
 * @Copyright (c) 2022 Shanghai Visinex Technologies Co., Ltd. All rights reserved.
 *
 */

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

#include "vs_type.h"
#include "vs_vo_defines.h"
#include "vs_mal_vo.h"
#include "vs_mal_vpp.h"
#include "vs_mal_gpe.h"
#include "vs_mal_nn.h"
#include "sample_common.h"
#include "vs_mal_log.h"

/*******************************************yolov5*******************************************/
//number of classes
#define YOLOV5_CLS_NUM			80
//input size of network
#define YOLOV5_IMAGE_SIZE		320
//threshold of score of a class
#define YOLOV5_CONF_Threshold	0.6
//threshold of detect target in a grid
#define YOLOV5_OBJ_THRESHOLD	0.5
//threshold of iou
#define YOLOV5_NMS_THRESHOLD	0.6
//max number of generate boxes
#define YOLOV5_MAX_PROPOSALS	100

/*******************************************centerface*****************************************/
//centerface the width and height of the network input
#define CENTERFACE_IMAGE_H		544
#define CENTERFACE_IMAGE_W		960
//centerface the width and height of the network output
#define CENTERFACE_OUTPUT_H 	136
#define CENTERFACE_OUTPUT_W 	240
//centerface the threshold for score filtering
#define CENTERFACE_SCORE_THRESHOLD 0.35
//centerface the threshold of nms
#define CENTERFACE_NMS_THRESHOLD 0.3

/*******************************************yolov3*********************************************/
//yolov3 image size
#define YOLOV3_IMAGE_SIZE		608
#define YOLOV3_LISTSIZE 		85
#define YOLOV3_OBJ_THRESH		0.25
#define YOLOV3_NMS_THRESH		0.45
#define YOLOV3_NUM_CLS			80
#define YOLOV3_MAX_BOXES		30

/*******************************************crop for pre process***************************/
//pre process hav crop,should match with enable_preproc_crop in acuity _inputmeta.yml
#define NN_PRE_PROC_CROP_SUPPORT
#define NN_PRE_PROC_CROP_ORG_W    512
#define NN_PRE_PROC_CROP_ORG_H    288

/********************************************common define *************************************/
//post process debug flag
// #define NN_POST_RESULT_DEBUG

//mem addr debug flag
// #define NN_MEM_ADDR_DEBUG

//number of classes
#define NN_COCO_CLS_NUM			  80

#define NN_CLASSIFY_NUM_CLASSES   1000


//define max and min function
#define     MAX(a, b)       ((a) > (b) ? (a) : (b))
#define     MIN(a, b)       ((a) < (b) ? (a) : (b))

#define     sigmoid(x)      ((vs_float_t) 1 / (1 + exp(-x)))


/* check the pointer, return the pointer variable name if the pointer is null */
#define VS_NN_CHECK_PTR(ptr) \
    do { \
        if (ptr == VS_NULL) { \
            printf("pointer:"#ptr" is null.\n"); \
            return VS_ERR_NN_NULL_PTR; \
        } \
    } while(0)

/****************************************struct***********************************************/


/****************************************local function export**********************************/
/* yolov5lite post process */
static vs_int32_t nn_yolov5lite_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);
/* centerface post process */
static vs_int32_t nn_centerface_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);
/* yolov3 post process */
static vs_int32_t nn_yolov3_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);
/* yolov5s post process */
static vs_int32_t nn_yolov5s_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);
/* vgg16 post process */
static vs_int32_t nn_vgg16_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);
/* mobilenet post process */
static vs_int32_t nn_classify_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result);


/****************************************global value**********************************/
static sample_nn_nb_info_s g_nb_info[] = {
    {"./nn/st/yolov5lite_320_320_nv12_uint8_fp32_single.nb", "yolov5_lite_single", 1000 / 75, nn_yolov5lite_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/st/yolov5lite_320_320_nv12_uint8_fp32_dual.nb", "yolov5_lite_dual", 1000 / 65, nn_yolov5lite_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/model/centerface_960_544_nv12_uint8_fp32_single.nb", "centerface_single", 1000 / 40, nn_centerface_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/model/centerface_960_544_nv12_uint8_fp32_dual.nb", "centerface_dual",1000 / 22, nn_centerface_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/model/yolov3_640_608_nv12_uint8_fp32_single.nb", "yolov3_single", 1000 / 2, nn_yolov3_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/model/yolov3_640_608_nv12_uint8_fp32_dual.nb", "yolov3_dual", 1000 / 3, nn_yolov3_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/model/mobilenet_244_244_rgb_uint8_fp32_single.nb", "mobilenet_single", 1000 / 2, nn_classify_postproc, E_NN_MODE_CLASSIFFICATION}, \
    {"./nn/model/mobilenet_244_244_rgb_uint8_fp32_dual.nb", "mobilenet_dual", 1000 / 3, nn_classify_postproc, E_NN_MODE_CLASSIFFICATION}, \
    {"./nn/model/resnet50_244_244_rgb_uint8_fp32_single.nb", "resnet50_single", 1000 / 2, nn_classify_postproc, E_NN_MODE_CLASSIFFICATION}, \
    {"./nn/model/resnet50_244_244_rgb_uint8_fp32_dual.nb", "resnet50_dual", 1000 / 3, nn_classify_postproc, E_NN_MODE_CLASSIFFICATION}, \
    {"./nn/st/yolov5s_640_640_nv12_uint8_fp32_single.nb", "yolov5s_single", 1000 / 16, nn_yolov5s_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/st/yolov5s_640_640_nv12_uint8_fp32_dual.nb", "yolov5s_dual", 1000 / 14, nn_yolov5s_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/st/vgg16_256_224_nv12_uint8_fp32_single.nb", "vgg16_single",1000 / 30, nn_vgg16_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/st/vgg16_256_224_nv12_uint8_fp32_dual.nb", "vgg16_dual", 1000 / 25, nn_vgg16_postproc, E_NN_MODE_DETECTION}, \
    {"./nn/st/yolov6s_640_640_nv12_uint8_fp32_single.nb", "yolov6s_single", 1000 / 30, VS_NULL, E_NN_MODE_DETECTION}, \
    {"./nn/st/yolov6s_640_640_nv12_uint8_fp32_dual.nb", "yolov6s_dual", 1000 / 30, VS_NULL, E_NN_MODE_DETECTION}, \
    {"./nn/st/densenet121_256_224_nv12_uint8_fp32_single.nb", "densenet121_single", 1000 / 50, VS_NULL, E_NN_MODE_DETECTION}, \
    {"./nn/st/densenet121_256_224_nv12_uint8_fp32_dual.nb", "densenet121_dual", 1000 / 50, VS_NULL, E_NN_MODE_DETECTION}
};

/* coco class color */
vs_uint32_t coco_color_tab[NN_COCO_CLS_NUM] = {
    0xFFFF0000, 0xFFA85085, 0xFF1C7A72, 0xFF5651AC, 0xFF343868, 0xFF8E5456, 0xFF39597A, 0xFF370741, 0xFFB66619, 0xFF2CB96D,
    0xFF016E84, 0xFF7A9E55, 0xFF46B91A, 0xFF890111, 0xFF789051, 0xFF8107B8, 0xFFB6519B, 0xFF16B145, 0xFF38BB9E, 0xFF022749,
    0xFF9C323C, 0xFF15B321, 0xFF0D45A5, 0xFF8E8B3F, 0xFF2DBF9F, 0xFF69AD20, 0xFF077185, 0xFF2E8722, 0xFF692256, 0xFF1623BE,
    0xFF93AB08, 0xFFB54C70, 0xFF1F3C37, 0xFFA43658, 0xFF604BB5, 0xFF5F9326, 0xFF71343F, 0xFF884195, 0xFF776718, 0xFF1F212D,
    0xFF428887, 0xFF3E5B6C, 0xFF310B4C, 0xFF3106BD, 0xFF9951A8, 0xFF7D1394, 0xFF816559, 0xFF3141B3, 0xFF80211A, 0xFF49A41A,
    0xFF1C3F86, 0xFF1D6A52, 0xFFAF7634, 0xFF514A2A, 0xFFB29370, 0xFF4A9D32, 0xFFFFFFFF, 0xFF3E16B1, 0xFF15646A, 0xFF15232A,
    0xFF0D0879, 0xFF8E5C1C, 0xFFFFFF00, 0xFF69761E, 0xFF07B97C, 0xFF2E2292, 0xFF69B8A9, 0xFF161205, 0xFF934749, 0xFFB5405B,
    0xFF1F27B8, 0xFFA4B321, 0xFF603212, 0xFF5F0F6A, 0xFF714436, 0xFF887470, 0xFF778B82, 0xFF1F8B22, 0xFF42067F, 0xFF3E2702
};

/* coco class name*/
char *coco_class_name[NN_COCO_CLS_NUM] = {
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat"," traffic light",
    "fire hydrant", "stop sign", "parking meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow",
    "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee",
    "skis", "snowboard", "sports ball", "kite", "baseball bat", "baseball glove", "skateboard", "surfboard",
    "tennis racket", "bottle", "wine glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich",
    "orange", "broccoli", "carrot", "hot dog", "pizza", "donut", "cake", "chair", "sofa", "pottedplant", "bed",
    "diningtable", "toilet", "tvmonitor", "laptop", "mouse", "remote", "keyboard", "cell phone", "microwave", "oven",
    "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy bear", "hair drier", "toothbrush"
};

/******************************************** function **************************************/
/* get cur time*/
vs_uint64_t nn_time_get_ms(vs_void_t)
{
    vs_uint64_t time_now_ms = 0;
    struct timeval cur_timeval;

    gettimeofday(&cur_timeval, NULL);
    time_now_ms = cur_timeval.tv_sec * 1000 + cur_timeval.tv_usec / 1000;

    return time_now_ms;
}

// mergesort function, arr is origin boxes, start is left edge, end is right edge, temp is auxiliary boxes
static vs_void_t nn_merge_sort(vs_box_info_s *arr, vs_int32_t start, vs_int32_t end, vs_box_info_s *temp)
{
	if (start >= end) {
		return;
    }
	//divide-and-conquer method
	vs_int32_t mid = (start + end) / 2;

	nn_merge_sort(arr, start, mid, temp);
	nn_merge_sort(arr, mid + 1, end, temp);

	//merge two ordered sequences
	vs_int32_t length = 0; 					// the length of auxiliary boxes
	vs_int32_t i_start = start;
	vs_int32_t i_end = mid;
	vs_int32_t j_start = mid + 1;
	vs_int32_t j_end = end;
	vs_int32_t i = 0;

	//order in scores from largest to smallest
	while (i_start <= i_end && j_start <= j_end) {
		if (arr[i_start].score > arr[j_start].score) {
			temp[length] = arr[i_start];
			length++;
			i_start++;
		} else {
			temp[length] = arr[j_start];
			length++;
			j_start++;
		}
	}

	//merge other boxes
	while (i_start <= i_end) {
		temp[length] = arr[i_start];
		i_start++;
		length++;
	}

	while (j_start <= j_end) {
		temp[length] = arr[j_start];
		length++;
		j_start++;
	}

	//put auxiliary boxes to origin boxes
	for (i = 0; i < length; i++) {
		arr[start + i] = temp[i];
	}
}

//nms process function, generate_boxes is the information of generate proposals, num_box is number of generate proposals, result_boxes will take used boxes
static vs_int32_t nn_nms(vs_box_info_s *generate_boxes, vs_int32_t num_box, const vs_float_t threshold,
	 sample_nn_det_obj_s *dect_obj)
{
	//auxiliary boxes
	vs_box_info_s	temp[num_box];
	//area of boxes
	vs_float_t area[num_box];
	//check of boxes
	vs_bool_t		check[num_box];
	//number of used boxes
	vs_int32_t num_result = 0;
    vs_int32_t i, j;

	//sort boxes
	nn_merge_sort(generate_boxes, 0, num_box - 1, temp);
	//calculate area and set check
	for (i = 0; i < num_box; i++) {
		area[i]  = generate_boxes[i].w *generate_boxes[i].h;
		check[i] = VS_TRUE;
	}

	//nms
	for (i = 0; i < num_box; i++) {
		//use the box with the largest score
		if (check[i] && num_result < NN_MAX_DETECT_OBJ_NUM) {
			dect_obj[num_result].x = (vs_uint32_t) generate_boxes[i].x;
			dect_obj[num_result].y = (vs_uint32_t) generate_boxes[i].y;
			dect_obj[num_result].w = (vs_uint32_t) generate_boxes[i].w;
			dect_obj[num_result].h = (vs_uint32_t) generate_boxes[i].h;
			dect_obj[num_result].cprob = (vs_float_t) generate_boxes[i].score;
			dect_obj[num_result].cid = (vs_uint32_t) generate_boxes[i].label;
			dect_obj[num_result].color = coco_color_tab[dect_obj[num_result].cid];
			num_result++;

			//check other boxes with lower score
			for (j = i + 1; j < num_box; j++) {
				if (check[j] && generate_boxes[i].label == generate_boxes[j].label) {
					//caculate iou
					vs_float_t x1 = MAX(generate_boxes[i].x, generate_boxes[j].x);
					vs_float_t y1 = MAX(generate_boxes[i].y, generate_boxes[j].y);
					vs_float_t x2 = MIN(generate_boxes[i].x + generate_boxes[i].w,
                                        generate_boxes[j].x + generate_boxes[j].w);
					vs_float_t y2 = MIN(generate_boxes[i].y + generate_boxes[i].h,
                                        generate_boxes[j].y + generate_boxes[j].h);
					vs_float_t w = MAX(0., x2 - x1 + 1);
					vs_float_t h = MAX(0., y2 - y1 + 1);
					vs_float_t inter = w * h;
					vs_float_t iou = inter / (area[i] +area[j] -inter);

					//Do not use duplicate boxes
					if (iou >= threshold) {
						check[j] = VS_FALSE;
					}
				}
			}
		}
	}

	return num_result;
}
#ifdef NN_POST_RESULT_DEBUG
static vs_void_t nn_post_result_show(sample_nn_post_result_s *p_nn_post_result)
{
    vs_uint32_t i = 0;

    if (p_nn_post_result->model_type == E_NN_MODE_DETECTION) {
        for (i = 0; i < p_nn_post_result->detection.obj_num; i++) {
            printf("index[%d] object id:%3d, prob:%1.2f, start(%3d,%3d), width[%3d],height[%3d]\n",
            			i,
            			p_nn_post_result->detection.objs[i].cid,
            			p_nn_post_result->detection.objs[i].cprob,
            			p_nn_post_result->detection.objs[i].x,
            			p_nn_post_result->detection.objs[i].y,
            			p_nn_post_result->detection.objs[i].w,
            			p_nn_post_result->detection.objs[i].h);
        }
    } else if (p_nn_post_result->model_type == E_NN_MODE_CLASSIFFICATION) {
        printf("object id:%3d, prob:%1.2f \n",
        			p_nn_post_result->classification.cid,
        			p_nn_post_result->classification.max_prob);
    } else {
        vs_sample_trace("model_type[%d] not support \n", p_nn_post_result->model_type);
    }
}
#endif

static vs_int32_t nn_yolov3_nms_boxes(vs_float_t box_class[][5], vs_int32_t box_count, vs_int32_t *box_use)
{
	vs_int32_t flag = 0, index = 0, total = 0;
	vs_float_t area[box_count], score_max = 0.0f, x1 = 0.0f, y1 = 0.0f, x2 = 0.0f, y2 = 0.0f;
	vs_float_t w = 0.0f, h = 0.0f, iou = 0.0f;

	for (vs_int32_t c = 0; c < box_count; c++) {
		box_use[c] = 1;
		area[c]  = box_class[c][2] *box_class[c][3];
	}

	while (1) {
		flag = 0;
		score_max = 0;

		for (vs_int32_t c = 0; c < box_count; c++) {
			if (box_use[c] == 1 && box_class[c][4] > score_max) {
				score_max = box_class[c][4];
				index = c;
				flag = 1;
			}
		}

		if (flag == 0)
			break;

		box_use[index] = 2;
		total++;

		for (vs_int32_t c = 0; c < box_count; c++) {
			if (box_use[c] == 1) {
				x1 = MAX(box_class[c][0], box_class[index][0]);
				y1 = MAX(box_class[c][1], box_class[index][1]);
				x2 = MIN(box_class[c][0] +box_class[c][2], box_class[index][0] +box_class[index][2]);
				y2 = MIN(box_class[c][1] +box_class[c][3], box_class[index][1] +box_class[index][3]);
				w = MAX(0.0, x2 - x1);
				h = MAX(0.0, y2 - y1);
				iou  = w * h / (area[c] +area[index] -w * h);

				if (iou > YOLOV3_NMS_THRESH)
					box_use[c] = 0;
			}
		}
	}

	return total;
}

static vs_int32_t nn_yolov3_nms(vs_int32_t *boxes_count, vs_float_t boxes_class[][YOLOV3_MAX_BOXES][5],
	 sample_nn_det_obj_s *dect_obj)
{
	vs_int32_t box_num = 0, **boxes_use, boxes_num = 0, c = 0;
	vs_int32_t b = 0;
    int i = 0;

	boxes_use = (vs_int32_t **) malloc(YOLOV3_NUM_CLS * sizeof(vs_int32_t *));
	for (c = 0; c < YOLOV3_NUM_CLS; c++) {
		if (boxes_count[c] > 0) {
			box_num += boxes_count[c];
			boxes_use[c] = (int *) malloc(boxes_count[c] *sizeof(int));
			boxes_num += nn_yolov3_nms_boxes(boxes_class[c], boxes_count[c], boxes_use[c]);
		}
	}

	for (c = 0; c < YOLOV3_NUM_CLS; c++) {
		for (i = 0; i < boxes_count[c]; i++) {
			if (boxes_use[c][i] == 2 && b < YOLOV3_MAX_BOXES) {
				dect_obj[b].x = (vs_uint32_t) boxes_class[c][i][0];
				dect_obj[b].y = (vs_uint32_t) boxes_class[c][i][1];
				dect_obj[b].w = (vs_uint32_t) boxes_class[c][i][2];
				dect_obj[b].h = (vs_uint32_t) boxes_class[c][i][3];
				dect_obj[b].cprob = (vs_float_t) boxes_class[c][i][4];
				dect_obj[b].cid  = c;
				dect_obj[b].color = coco_color_tab[dect_obj[b].cid];
				b++;
			}
		}
	}

	for (c = 0; c < YOLOV3_NUM_CLS; c++) {
		if (boxes_count[c] > 0) {
			free(boxes_use[c]);
		}
	}

	free(boxes_use);

	return boxes_num;
}

static vs_void_t nn_yolov3_get_boxes(const vs_int32_t out_index, vs_int32_t *boxes_count, vs_float_t boxes_class[][YOLOV3_MAX_BOXES][5],
	 vs_float_t *ptr, vs_int32_t sz)
{
	vs_int32_t masks[3][3] = {{0, 1, 2}, { 3, 4, 5},{6, 7, 8}};
	vs_int32_t anchors[9][2] = {{12, 26},{19, 36},{40, 28},{36, 75},{76, 55},{72, 146},{142, 110},{192, 243},{459, 401}};
	vs_int32_t grid = 0, mask = 0;
	vs_float_t box_input[YOLOV3_LISTSIZE] = {0.0f};
	vs_int32_t i = 0, j = 0, num = 0, k = 0;
	vs_float_t class_score = 0;
	vs_int32_t class_ind = 0;

	grid = 0;
	if (out_index == 2 && STRIDE_UP(sz, 64) == 1472896) {
		grid = 76;
		mask = 0;
	} else if (out_index == 1 && STRIDE_UP(sz, 64) == 368256) {
		grid = 38;
		mask = 1;
	} else if (out_index == 0 && STRIDE_UP(sz, 64) == 92096) {
		grid = 19;
		mask = 2;
	}

	if (grid == 0) {
		vs_sample_trace("grid is zero. out_index(%d) sz(%d) STRIDE_UP_64=(%d)\n", out_index, sz, STRIDE_UP(sz, 64));
	} else {
		vs_int32_t stride = grid * grid;

		for (i = 0; i < grid; i++) {
			for (j = 0; j < grid; j++) {
				for (num = 0; num < 3; num++) {
					for (k = 0; k < YOLOV3_LISTSIZE; k++) {
						if (k == 2 || k == 3) {
							box_input[k] = exp(ptr[(num * YOLOV3_LISTSIZE + k) *stride]);
						} else {
							box_input[k] = sigmoid(ptr[(num * YOLOV3_LISTSIZE + k) *stride]);
						}
					}

					class_score = 0;
					class_ind = 0;
					for (k = 0; k < YOLOV3_NUM_CLS; k++) {
						if (box_input[k + 5] > class_score) {
							class_score  = box_input[k + 5];
							class_ind = k;
						}
					}

					if (class_score * box_input[4] >= YOLOV3_OBJ_THRESH &&
						 boxes_count[class_ind] < YOLOV3_MAX_BOXES) {
						box_input[0] += j;
						box_input[1] += i;
						box_input[0] /= grid;
						box_input[1] /= grid;
						box_input[0] *= YOLOV3_IMAGE_SIZE;
						box_input[1] *= YOLOV3_IMAGE_SIZE;
						box_input[2] *= anchors[masks[mask][num]][0];
						box_input[3] *= anchors[masks[mask][num]][1];
						box_input[0] -= box_input[2] / 2;
						box_input[1] -= box_input[3] / 2;

						boxes_class[class_ind][boxes_count[class_ind]][0] = box_input[0];
						boxes_class[class_ind][boxes_count[class_ind]][1] = box_input[1];
						boxes_class[class_ind][boxes_count[class_ind]][2] = box_input[2];
						boxes_class[class_ind][boxes_count[class_ind]][3] = box_input[3];
						boxes_class[class_ind][boxes_count[class_ind]][4] = class_score * box_input[4];
						boxes_count[class_ind] ++;
					}
				}

				ptr++;
			}
		}
	}
}

//yolov5lite post process results function
static vs_int32_t nn_yolov5lite_get_boxes(vs_float_t *preds, vs_box_info_s *generate_boxes)
{
	//anchors pool
	const vs_float_t anchors[3][6] = { {10.0, 13.0, 16.0, 30.0, 33.0, 23.0},
		{30.0, 61.0, 62.0, 45.0, 59.0, 119.0},
		{116.0, 90.0, 156.0, 198.0, 373.0, 326.0}};
	//block spanning step size
	const vs_float_t stride[3] = {8.0, 16.0, 32.0};
	//data length of a single block
	const vs_int32_t nout = YOLOV5_CLS_NUM + 5;
	//other index variables
	vs_int32_t n = 0, q = 0, i = 0, j = 0, k = 0, num_box = 0;
    vs_float_t class_score = 0;
    vs_int32_t class_ind = 0;

	for (n = 0; n < 3; n++) {
		//number of grid
		vs_int32_t num_grid_x = (vs_int32_t) (YOLOV5_IMAGE_SIZE / stride[n]);
		vs_int32_t num_grid_y = (vs_int32_t) (YOLOV5_IMAGE_SIZE / stride[n]);

		for (q = 0; q < 3; q++) {
			//anchor
			const vs_float_t anchor_w = anchors[n][q *2];
			const vs_float_t anchor_h = anchors[n][q * 2 + 1];

			for (i = 0; i < num_grid_y; i++) {
				for (j = 0; j < num_grid_x; j++) {
					//determine if the target has been detected
					vs_float_t box_score = preds[4];

					if (box_score > YOLOV5_OBJ_THRESHOLD) {
						//find the class with the largest score
						class_score = 0;
						class_ind = 0;

						for (k = 0; k < YOLOV5_CLS_NUM; k++) {
							if (preds[k + 5] > class_score) {
								class_score  = preds[k + 5];
								class_ind = k;
							}
						}

						if (class_score > YOLOV5_CONF_Threshold && num_box < YOLOV5_MAX_PROPOSALS) {
							//center point coordinates
							vs_float_t cx = (preds[0] *2.f - 0.5f + j) *stride[n];
							vs_float_t cy = (preds[1] *2.f - 0.5f + i) *stride[n];

							//width and height of box
							generate_boxes[num_box].w = pow(preds[2] *2.f, 2.f) *anchor_w;
							generate_boxes[num_box].h = pow(preds[3] *2.f, 2.f) *anchor_h;
							//upper left coordinates
							generate_boxes[num_box].x = cx - 0.5 * generate_boxes[num_box].w;
							generate_boxes[num_box].y = cy - 0.5 * generate_boxes[num_box].h;
							generate_boxes[num_box].score = class_score;
							generate_boxes[num_box].label = class_ind;
							num_box++;
						}
					}

					//go to next block result
					preds += nout;
				}
			}
		}
	}

	return num_box;
}

//centerface post process results function
static vs_int32_t nn_centerface_get_boxes(vs_float_t *preds[3], vs_box_info_s *generate_boxes)
{
	vs_float_t s0, s1, o0, o1, score, x1, y1, x2, y2, w, h;
	vs_uint32_t offset_ind = CENTERFACE_OUTPUT_H * CENTERFACE_OUTPUT_W;
	vs_uint32_t i, j, index;
	//other index variables
	vs_int32_t num_box = 0;

	for (i = 0; i < CENTERFACE_OUTPUT_H; i++) {
		for (j = 0; j < CENTERFACE_OUTPUT_W; j++) {
			index = i * CENTERFACE_OUTPUT_W + j;

			if (preds[0][index] > CENTERFACE_SCORE_THRESHOLD) {
				s0 = exp(preds[1][index]) * 4;
				s1 = exp(preds[1][index + offset_ind]) * 4;
				o0 = preds[2][index];
				o1 = preds[2][index + offset_ind];
				score = preds[0][index];

				x1 = MAX(0.0, ((vs_float_t) j + o1 + 0.5) * 4 - s1 / 2);
				y1 = MAX(0.0, ((vs_float_t) i + o0 + 0.5) * 4 - s0 / 2);
				x1 = MIN(x1, CENTERFACE_IMAGE_W);
				y1 = MIN(y1, CENTERFACE_IMAGE_H);
				x2 = MIN((x1 + s1), CENTERFACE_IMAGE_W);
				y2 = MIN((y1 + s0), CENTERFACE_IMAGE_H);
				w = x2 - x1 + 1;
				h = y2 - y1 + 1;

				generate_boxes[num_box].x = x1;
				generate_boxes[num_box].y = y1;
				generate_boxes[num_box].w = w;
				generate_boxes[num_box].h = h;
				generate_boxes[num_box].score = score;
				generate_boxes[num_box].label = 0;
				num_box++;
			}
		}
	}

	return num_box;
}

static vs_void_t nn_nbinfo_debug(vs_nn_model_s *p_model)
{
    vs_int32_t i = 0, j = 0;

    for (i = 0; i < p_model->input_num; i++) {
        printf("input: %d, [n,c,h,w]: ", i);
        for (j = p_model->inputs[i].dim_count - 1; j >= 0; j--) {
            printf("%d ",p_model->inputs[i].dim_size[j]);
        }
        printf(", data format: %d", p_model->inputs[i].format);
        printf(", data type: %d", p_model->inputs[i].type);
        printf(", bottom name: %s\n", p_model->inputs[i].bottom_name);
    }

    for (i = 0; i < p_model->output_num; i++) {
        printf("output: %d, dim: ", i);
        for (j = p_model->outputs[i].dim_count -1; j >= 0; j--) {
            printf("%d ",p_model->outputs[i].dim_size[j]);
        }
        printf(", data format: %d", p_model->outputs[i].format);
        printf(", data type: %d", p_model->outputs[i].type);
        printf(", bottom name: %s\n", p_model->outputs[i].bottom_name);
    }
}

/* get file size */
static vs_uint32_t nn_file_size_get(const vs_char_t *name)
{
    FILE *fp = fopen((char*)name,"rb");
    vs_uint32_t size = 0;
    if (fp) {
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fclose(fp);
    } else {
        vs_sample_trace("fopen file %s failed.\n", name);
        return VS_FAILED;
    }
    return size;
}

/* read file */
static vs_int32_t nn_file_read(const vs_char_t *name, vs_void_t *dst)
{
    vs_uint32_t size = 0;

    FILE *fp = fopen((char*)name,"rb");
    if (fp) {
        fseek(fp,0,SEEK_END);
        size = ftell(fp);
        fseek(fp,0,SEEK_SET);
        fread(dst,size,1,fp);
        fclose(fp);
        vs_sample_trace("fread file: %s.\n",name);
    } else {
        vs_sample_trace("fopen file %s failed.\n", name);
        return VS_FAILED;
    }
    return VS_SUCCESS;
}

/* alloc memory from mmz, note: read data should vs_mal_sys_cache_invalidate*/
static vs_int32_t nn_memory_alloc(vs_uint64_t *phys_addr, vs_void_t **virt_addr, vs_uint32_t len)
{
    vs_int32_t ret = VS_FAILED;
    vs_char_t mmb_name[] = "sample_nn_alloc";

    ret = vs_mal_mmz_alloc(VS_NULL, mmb_name, len, phys_addr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_mmz_alloc failed with err 0x%x\n", ret);
        return ret;
    }

    *virt_addr = vs_mal_sys_mmap_cached(*phys_addr, len);
    if (*virt_addr == VS_NULL) {
        vs_sample_trace("vs_mal_sys_mmap failed\n");
        goto error;
    }

#ifdef NN_MEM_ADDR_DEBUG
    vs_sample_trace("vs_mal_mmz_alloc phys_addr[%llu] virt_addr[%p] \n", *phys_addr, *virt_addr);
#endif
    return ret;

error:
    vs_mal_mmz_free(*phys_addr);
    return VS_FAILED;
}

/* free used memory which is allocated from mmz */
static vs_int32_t nn_memory_free(vs_uint64_t phys_addr, vs_void_t *p_virt_addr, vs_uint32_t len)
{
    vs_int32_t ret = VS_SUCCESS;

    ret = vs_mal_sys_unmap(p_virt_addr, len);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_sys_unmap failed with err 0x%x\n", ret);
    }

    ret = vs_mal_mmz_free(phys_addr);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_mmz_free failed with err 0x%x\n", ret);
    }
#ifdef NN_MEM_ADDR_DEBUG
    vs_sample_trace("vs_mal_mmz_free phys_addr[%llu] virt_addr[%p] \n", phys_addr, p_virt_addr);
#endif

    return ret;
}

/* get size of data type */
static vs_uint32_t nn_format_bytes(const vs_nn_data_format_e format)
{
    vs_uint32_t bytes_per_data = 1;
    switch(format) {
    case E_NN_DATA_FORMAT_FP32:
        bytes_per_data = 4;
        break;
    case E_NN_DATA_FORMAT_FP16:
        bytes_per_data = 2;
        break;
    case E_NN_DATA_FORMAT_UINT8:
    case E_NN_DATA_FORMAT_INT8:
        bytes_per_data = 1;
        break;
    case E_NN_DATA_FORMAT_UINT16:
    case E_NN_DATA_FORMAT_INT16:
        bytes_per_data = 2;
        break;
    case E_NN_DATA_FORMAT_CHAR:
        bytes_per_data = 1;
        break;
    case E_NN_DATA_FORMAT_BFP16:
        bytes_per_data = 2;
        break;
    case E_NN_DATA_FORMAT_INT32:
    case E_NN_DATA_FORMAT_UINT32:
        bytes_per_data = 4;
        break;
    case E_NN_DATA_FORMAT_INT64:
    case E_NN_DATA_FORMAT_UINT64:
    case E_NN_DATA_FORMAT_FP64:
        bytes_per_data = 8;
        break;
    default:
        vs_sample_trace("data type %d is not correct, return 4 bytes for this error\n", format);
        bytes_per_data = 4;
        break;
    }

    return bytes_per_data;
}

/* get nn model input/output node valid data size */
vs_int32_t sample_common_nn_io_data_size_get(const vs_nn_node_s *p_node)
{
    vs_int32_t i = 0;
	vs_uint32_t io_data_size = 1;
	for (i = 0; i < p_node->dim_count; i++) {
		io_data_size = io_data_size * p_node->dim_size[i];
	}
	io_data_size *= nn_format_bytes(p_node->format);
	return io_data_size;
}

/* alloc mmz memory for nn input */
static vs_int32_t nn_input_memory_alloc(const vs_nn_model_s *p_model, vs_nn_data_s *p_input)
{
    vs_int32_t i = 0;
    vs_int32_t ret = 0;

    for (i = 0; i < p_model->input_num; i++) {

        p_input[i].format = p_model->inputs[i].format;
        p_input[i].buffer.size  = STRIDE_UP(sample_common_nn_io_data_size_get(&p_model->inputs[i]), NN_MODEL_BUFFER_ALIGNMENT);

        // alloc from mmz
        ret = nn_memory_alloc(&p_input[i].buffer.phys_addr,
                                            &p_input[i].buffer.p_virt_addr,
                                            p_input[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("nn_memory_alloc failed with err 0x%x\n", ret);
            return VS_FAILED;
        }
    }
    return VS_SUCCESS;
}

/* alloc mmz memory for nn output */
static vs_int32_t nn_output_memory_alloc(const vs_nn_model_s *p_model, vs_nn_data_s *p_output)
{
    vs_int32_t i = 0;
    vs_int32_t ret = 0;

    for (i = 0; i < p_model->output_num; i++) {

        p_output[i].format = p_model->outputs[i].format;
        p_output[i].buffer.size  = STRIDE_UP(sample_common_nn_io_data_size_get(&p_model->outputs[i]), NN_MODEL_BUFFER_ALIGNMENT);

        // alloc from mmz
        ret = nn_memory_alloc(&p_output[i].buffer.phys_addr,
                                            &p_output[i].buffer.p_virt_addr,
                                            p_output[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("nn_memory_alloc failed with err 0x%x\n", ret);
            return VS_FAILED;
        }
    }
    return VS_SUCCESS;
}

/* invalidate cached mmz, cache mmz must be invalidate before read */
static vs_int32_t nn_data_memory_invalidate(vs_uint32_t data_num, vs_nn_data_s *p_nn_data)
{
    vs_int32_t i = 0;
    vs_int32_t ret = 0;

    for (i = 0; i < data_num; i++) {
        // invalidate mmz, cache mmz must be invalidate before read
        ret = vs_mal_sys_cache_invalidate(p_nn_data[i].buffer.phys_addr, p_nn_data[i].buffer.p_virt_addr,
                                         p_nn_data[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_sys_cache_invalidate failed with err 0x%x\n", ret);
            return VS_FAILED;
        }
#ifdef NN_MEM_ADDR_DEBUG
        vs_sample_trace("vs_mal_sys_cache_invalidate phys_addr[%llu] virt_addr[%p] \n",
                        p_nn_data[i].buffer.phys_addr, p_nn_data[i].buffer.p_virt_addr);
#endif
    }
    return VS_SUCCESS;
}

/* flush cached mmz, cache mmz must be flush after write */
static vs_int32_t nn_data_memory_flush(vs_uint32_t data_num, vs_nn_data_s *p_nn_data)
{
    vs_int32_t i = 0;
    vs_int32_t ret = 0;

    for (i = 0; i < data_num; i++) {
        // flush mmz, cache mmz must be flush after write
        ret = vs_mal_sys_cache_flush(p_nn_data[i].buffer.phys_addr, p_nn_data[i].buffer.p_virt_addr,
                                     p_nn_data[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_sys_cache_flush. failed, ret 0x%x. \n", ret);
        }
#ifdef NN_MEM_ADDR_DEBUG
        vs_sample_trace("vs_mal_sys_cache_flush phys_addr[%llu] virt_addr[%p] \n",
                    p_nn_data[i].buffer.phys_addr, p_nn_data[i].buffer.p_virt_addr);
#endif

    }
    return VS_SUCCESS;
}

/*********************************************************
 *  mobilenet & resnet50 classify_postproc
**********************************************************/
vs_int32_t nn_classify_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
    vs_uint32_t i = 0, j = 0;
    vs_uint32_t cid = 0;
    vs_float_t *ptr = VS_NULL;
    vs_float_t max_prob = 0.0f;

    for (i = 0; i < out_num; i++) {
        ptr = (vs_float_t*)out_datas[i].buffer.p_virt_addr;
        max_prob = ptr[0];
        for (j = 0; j < NN_CLASSIFY_NUM_CLASSES; j++ )
        {
            if (ptr[j] > max_prob) {
                max_prob = ptr[j];
                cid = j;
            }
        }
    }
    p_nn_post_result->classification.max_prob = max_prob;
    p_nn_post_result->classification.cid = cid;
    p_nn_post_result->is_updated = VS_TRUE;
#ifdef NN_POST_RESULT_DEBUG
    nn_post_result_show(p_nn_post_result);
#endif
    return VS_SUCCESS;
}

/*********************************************************
 * Yolov5lite
**********************************************************/
static vs_int32_t nn_yolov5lite_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
	//generate proposals
	vs_box_info_s	generate_boxes[YOLOV5_MAX_PROPOSALS] = { 0 };
	vs_int32_t num_box = 0;
	vs_uint32_t i = 0;
	vs_nn_dst_data_s data = { 0 };

	for (i = 0; i < out_num; i++) {
		data = out_datas[i];
		num_box  = nn_yolov5lite_get_boxes((vs_float_t *) data.buffer.p_virt_addr, generate_boxes);
	}

	// do update detect result
	p_nn_post_result->detection.obj_num = nn_nms(generate_boxes, num_box, YOLOV5_NMS_THRESHOLD,
		                                                        p_nn_post_result->detection.objs);
	p_nn_post_result->is_updated = VS_TRUE;
#ifdef NN_POST_RESULT_DEBUG
    nn_post_result_show(p_nn_post_result);
#endif
	return VS_SUCCESS;
}

/*********************************************************
 * CenterFace
**********************************************************/
static vs_int32_t nn_centerface_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
	//generate proposals
	vs_box_info_s	generate_boxes[CENTERFACE_OUTPUT_H * CENTERFACE_OUTPUT_W];
	vs_int32_t num_box = 0;
	vs_uint32_t i = 0;
	vs_nn_dst_data_s data = {0};
	vs_float_t *	ptr[3];

	for (i = 0; i < out_num; i++) {
		data = out_datas[i];
		ptr[i] = (vs_float_t *) data.buffer.p_virt_addr;
	}

	num_box  = nn_centerface_get_boxes(ptr, generate_boxes);

	// do update detect result
	p_nn_post_result->detection.obj_num = nn_nms(generate_boxes, num_box, CENTERFACE_NMS_THRESHOLD,
		                                                        p_nn_post_result->detection.objs);
	p_nn_post_result->is_updated = VS_TRUE;
#ifdef NN_POST_RESULT_DEBUG
    nn_post_result_show(p_nn_post_result);
#endif
	return VS_SUCCESS;
}

/*********************************************************
 * Yolov3
**********************************************************/
static vs_int32_t nn_yolov3_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
	vs_int32_t boxes_count[YOLOV3_NUM_CLS] = {0};
	vs_float_t boxes_class[YOLOV3_NUM_CLS][YOLOV3_MAX_BOXES][5];
	vs_uint32_t i = 0;
	vs_nn_dst_data_s data = {0};

	for (i = 0; i < out_num; i++) {
		data = out_datas[i];
		nn_yolov3_get_boxes(i, boxes_count, boxes_class, (vs_float_t *) data.buffer.p_virt_addr,
			 data.buffer.size / sizeof (vs_float_t));
	}

	// do update detect result
	p_nn_post_result->detection.obj_num = nn_yolov3_nms(boxes_count, boxes_class, p_nn_post_result->detection.objs);
	p_nn_post_result->is_updated = VS_TRUE;
#ifdef NN_POST_RESULT_DEBUG
    nn_post_result_show(p_nn_post_result);
#endif
	return VS_SUCCESS;
}

/*********************************************************
 * Yolov5s
**********************************************************/
static vs_int32_t nn_yolov5s_postproc( const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
	//vs_sample_trace("yolov5s not support post process!!!\n");
	return VS_SUCCESS;
}

/*********************************************************
 * VGG16
**********************************************************/
static vs_int32_t nn_vgg16_postproc(const vs_nn_data_s *out_datas, const vs_int32_t out_num, sample_nn_post_result_s *p_nn_post_result)
{
	//vs_sample_trace("vgg16 not support post process!!!\n");
	return VS_SUCCESS;
}

static vs_int32_t nn_invoke_block(const vs_nn_src_data_s *p_nn_src_data, sample_nn_info_s *p_nn_info, sample_nn_post_result_s *p_temp_result)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_uint32_t task_handle = 0;
    vs_uint32_t i = 0;
    vs_nn_invoke_attr_s attr = {0};
    vs_nn_model_s *p_model = VS_NULL;

    //vs_sample_trace("nn_invoke_block start\n");
    p_model = &p_nn_info->model;
    // prepare input attribute in invoke attr
    attr.src_num = p_model->input_num;
    attr.dst_num = p_model->output_num;
    for (i = 0; i < p_model->input_num; i++){
        attr.src_data[i].format = p_nn_src_data[i].format;
        attr.src_data[i].buffer.size = p_nn_src_data[i].buffer.size;
        attr.src_data[i].buffer.phys_addr = p_nn_src_data[i].buffer.phys_addr;
        attr.src_data[i].buffer.p_virt_addr = p_nn_src_data[i].buffer.p_virt_addr;
    }
    for (i = 0; i < p_model->output_num; i++){
        attr.dst_data[i].format = p_nn_info->output_info.dst_data[i].format;
        attr.dst_data[i].buffer.size = p_nn_info->output_info.dst_data[i].buffer.size;
        attr.dst_data[i].buffer.phys_addr = p_nn_info->output_info.dst_data[i].buffer.phys_addr;
        attr.dst_data[i].buffer.p_virt_addr = p_nn_info->output_info.dst_data[i].buffer.p_virt_addr;
    }
    // do invoke

    // When using cache MMZ as an input/output buffer, ensure to flush the cache if any writes, including memset operations, have been performed before.
    ret = vs_mal_nn_invoke(&task_handle, p_model, &attr, VS_TRUE);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_invoke failure with err 0x%x\n", ret);
        return VS_FAILED;
    }

    // do post process
    if (p_nn_info->nb_info.post_proc) {
        if (p_nn_info->nb_info.model_type == E_NN_MODE_SEGMENTATION) {
            p_temp_result->model_type = E_NN_MODE_SEGMENTATION;
            p_temp_result->segmentation.disp_rgn_handle = p_nn_info->disp_rgn_handle;
        } else if (p_nn_info->nb_info.model_type == E_NN_MODE_STYLE_TRANSFER) {
            p_temp_result->model_type = E_NN_MODE_STYLE_TRANSFER;
            p_temp_result->style_transfer.disp_rgn_handle = p_nn_info->disp_rgn_handle;
        } else if (p_nn_info->nb_info.model_type == E_NN_MODE_CLASSIFFICATION) {
            p_temp_result->model_type = E_NN_MODE_CLASSIFFICATION;
        } else if (p_nn_info->nb_info.model_type == E_NN_MODE_DETECTION) {
            p_temp_result->model_type = E_NN_MODE_DETECTION;
        } else {
            vs_sample_trace("model_type[%d] not support! \n", p_nn_info->nb_info.model_type);
            return VS_FAILED;
        }

        /* When using cache MMZ as an output buffer,and want to read the result,ensure to invalidate it firstly */
        ret = nn_data_memory_invalidate(attr.dst_num, attr.dst_data);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_nn_invoke failure with err 0x%x\n", ret);
            return VS_FAILED;
        }
        ret = p_nn_info->nb_info.post_proc(attr.dst_data, attr.dst_num, p_temp_result);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("nn_post_process failure with err 0x%x\n", ret);
            return VS_FAILED;
        }
    }

    return ret;
}

vs_void_t* nn_producer_proc(vs_void_t *args)
{
    vs_int32_t ret = 0;
    vs_video_frame_info_s nn_frame = {0};
    vs_nn_src_data_s p_nn_src_data[4];
    sample_nn_post_result_s *p_temp_result = VS_NULL;
    sample_nn_info_s *p_nn_info = (sample_nn_info_s *)args;
    vs_uint64_t w_phys_addr;
    vs_void_t *p_w_virt_addr;
    vs_uint64_t h_phys_addr;
    vs_void_t *p_h_virt_addr;

    vs_sample_trace("nn_producer_proc start.\n");
    p_temp_result = (sample_nn_post_result_s *)malloc(sizeof(sample_nn_post_result_s));
    if (p_temp_result == VS_NULL) {
        vs_sample_trace("malloc p_temp_result sizeof %ld\n", sizeof(sample_nn_post_result_s));
        return VS_NULL;
    }

    do{
        // get frame
        ret = vs_mal_vpp_chn_frame_acquire(p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid, &nn_frame, 5 * NN_VPP_ACQUIRE_TIMEOUT);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vpp_chn_frame_acquire failed with err 0x%x, group_id %d, chn_id %d\n", \
                            ret, p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid);
            continue;
        }

        // for NV12/NV21,have 2 input nodes.

        // Constraint check:input data stride should same as width
        if ((nn_frame.frame.width != nn_frame.frame.stride[0]) || (nn_frame.frame.width != nn_frame.frame.stride[1])) {
            vs_sample_trace("nn Constraint check:vpp frame stride[0] %d or stride[1] %d not equal to width %d\n", nn_frame.frame.stride[0], nn_frame.frame.stride[1], nn_frame.frame.width);
            vs_mal_vpp_chn_frame_release(p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid, &nn_frame);
            continue;
        }

        // inputs[0] for y
        p_nn_src_data[0].format = p_nn_info->model.inputs[0].format;
        p_nn_src_data[0].buffer.size = STRIDE_UP(sample_common_nn_io_data_size_get(&p_nn_info->model.inputs[0]), NN_MODEL_BUFFER_ALIGNMENT);
        p_nn_src_data[0].buffer.phys_addr = nn_frame.frame.phys_addr[0];

        // inputs[1] for uv
        p_nn_src_data[1].format = p_nn_info->model.inputs[1].format;
        p_nn_src_data[1].buffer.size = STRIDE_UP(sample_common_nn_io_data_size_get(&p_nn_info->model.inputs[1]), NN_MODEL_BUFFER_ALIGNMENT);
        p_nn_src_data[1].buffer.phys_addr = nn_frame.frame.phys_addr[1];

        //if enable pre-process cropping, the number of input nodes will increase to 4,
        //it needs to fill in the width and height of the image before cropping
#ifdef NN_PRE_PROC_CROP_SUPPORT
        if (p_nn_info->model.input_num == 4) {
            // inputs[2] for width of the image before cropping
            p_nn_src_data[2].format = p_nn_info->model.inputs[2].format;
            p_nn_src_data[2].buffer.size = STRIDE_UP(sample_common_nn_io_data_size_get(&p_nn_info->model.inputs[2]), NN_MODEL_BUFFER_ALIGNMENT);
            ret = nn_memory_alloc(&w_phys_addr, &p_w_virt_addr, p_nn_src_data[2].buffer.size);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("nn_memory_alloc failed with err 0x%x\n", ret);
                vs_mal_vpp_chn_frame_release(p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid, &nn_frame);
                continue;
            }
            *(vs_int32_t *)p_w_virt_addr = NN_PRE_PROC_CROP_ORG_W;//width of the image before cropping
            p_nn_src_data[2].buffer.phys_addr = w_phys_addr;
            p_nn_src_data[2].buffer.p_virt_addr = p_w_virt_addr;

            // inputs[2] for height of the image before cropping
            p_nn_src_data[3].format = p_nn_info->model.inputs[3].format;
            p_nn_src_data[3].buffer.size = STRIDE_UP(sample_common_nn_io_data_size_get(&p_nn_info->model.inputs[3]), NN_MODEL_BUFFER_ALIGNMENT);
            ret = nn_memory_alloc(&h_phys_addr, &p_h_virt_addr, p_nn_src_data[3].buffer.size);
            if (ret != VS_SUCCESS) {
                vs_sample_trace("nn_memory_alloc failed with err 0x%x\n", ret);
                vs_mal_vpp_chn_frame_release(p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid, &nn_frame);
                nn_memory_free(w_phys_addr, p_w_virt_addr, p_nn_src_data[2].buffer.size);
                continue;
            }
            *(vs_int32_t *)p_h_virt_addr = NN_PRE_PROC_CROP_ORG_H;//height of the image before cropping
            p_nn_src_data[3].buffer.phys_addr = h_phys_addr;
            p_nn_src_data[3].buffer.p_virt_addr = p_h_virt_addr;

            nn_data_memory_flush(4, p_nn_src_data);
        }
#endif
        // invoke and post process
        memset(p_temp_result, 0, sizeof(sample_nn_post_result_s));
        ret = nn_invoke_block(p_nn_src_data, p_nn_info, p_temp_result);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("nn_invoke_block failed with err 0x%x\n", ret);
        }

#ifdef NN_PRE_PROC_CROP_SUPPORT
        if (p_nn_info->model.input_num == 4) {//input_num == 4 means open crop function
            nn_memory_free(w_phys_addr, p_w_virt_addr, p_nn_src_data[2].buffer.size);
            nn_memory_free(h_phys_addr, p_h_virt_addr, p_nn_src_data[3].buffer.size);
        }
#endif

        // do update detect result
        pthread_mutex_lock(&p_nn_info->thead_info.mutex);
        p_nn_info->thead_info.result = *p_temp_result;
        p_nn_info->thead_info.result.is_updated = VS_TRUE;
        pthread_mutex_unlock(&p_nn_info->thead_info.mutex);

        //release frame
        ret = vs_mal_vpp_chn_frame_release(p_nn_info->input_info.producer_chn.devid, p_nn_info->input_info.producer_chn.chnid, &nn_frame);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("vs_mal_vpp_chn_frame_release failed with err 0x%x\n", ret);
            break;
        }
    } while (p_nn_info->thead_info.stop_flag != VS_TRUE);

    if (p_temp_result != VS_NULL) {
        free(p_temp_result);
        p_temp_result = VS_NULL;
    }
    vs_sample_trace("nn_producer_proc exit.\n");
    return VS_NULL;
}

/**********************************************************************************************************************
                                Sample NN function
 *********************************************************************************************************************/
vs_int32_t sample_common_nn_init()
{
    vs_int32_t ret = 0;
    // nn init
    ret = vs_mal_nn_init();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_init failed with err 0x%x\n", ret);
        return ret;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_nn_nb_info_get(sample_nn_model_id_e model_id, vs_bool_t is_single_model, sample_nn_nb_info_s *p_nb_info)
{
    VS_NN_CHECK_PTR(p_nb_info);

    if (model_id >= E_NN_MODEL_ID_MAX || model_id < 0) {
        vs_sample_trace("model id should be[%d,%d).\n", 0, E_NN_MODEL_ID_MAX);
        return VS_FAILED;
    }

    if (p_nb_info == VS_NULL) {
        vs_sample_trace("p_nb_info should not be NULL.\n");
        return VS_FAILED;
    }

    if (is_single_model == VS_TRUE ) {
        memcpy(p_nb_info, &g_nb_info[2 * model_id], sizeof(sample_nn_nb_info_s));
    } else if (is_single_model == VS_FALSE){
        memcpy(p_nb_info, &g_nb_info[2 * model_id + 1], sizeof(sample_nn_nb_info_s));
    } else {
        vs_sample_trace("is_single_model should not be %d.\n", is_single_model);
        return VS_FAILED;
    }

    return VS_SUCCESS;
}

vs_int32_t sample_common_nn_create(sample_nn_info_s *p_nn_info)
{
    vs_uint32_t file_size = 0;
    vs_void_t *nb_file_ptr = VS_NULL;
    vs_nn_mem_info_s model_buffer = {0};
    vs_nn_model_s *p_model = VS_NULL;
    vs_nn_model_param_s model_param = {0};
    vs_int32_t ret = VS_SUCCESS;

    VS_NN_CHECK_PTR(p_nn_info);
    p_model = &p_nn_info->model;

    // read nb model to memory
    file_size = nn_file_size_get(p_nn_info->nb_info.nb_file);
    if(file_size == VS_FAILED) {
        goto exit0;
    }
    nb_file_ptr = (vs_void_t *)malloc(sizeof(vs_char_t)*file_size);
    VS_NN_CHECK_PTR(nb_file_ptr);
    memset(nb_file_ptr, 0, sizeof(vs_char_t)*file_size);
    ret = nn_file_read(p_nn_info->nb_info.nb_file, nb_file_ptr);
    if (ret != VS_SUCCESS) {
        goto exit0;
    }

    // create model from memory
    model_buffer.size = file_size;
    model_buffer.p_virt_addr = nb_file_ptr;
    ret = vs_mal_nn_model_create(&model_buffer, p_nn_info->nb_info.model_name, p_model);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_model_create failed with err 0x%x, file name: %s\n",ret, p_nn_info->nb_info.nb_file);
        goto exit0;
    }
    nn_nbinfo_debug(p_model);

    model_param.cluster_id = p_nn_info->cluster_id;
    ret = vs_mal_nn_model_param_set(p_model, &model_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_model_param_set failed with err 0x%x, priority = %d, cluster_id = %d\n",
                        ret, model_param.priority, model_param.cluster_id);
        goto exit1;
    }

    // verify model
    ret = vs_mal_nn_model_verify(p_model);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_model_verify failed with err 0x%x\n", ret);
        goto exit1;
    }

    // query whether the model enabled pre & post processing
    ret = vs_mal_nn_model_param_get(p_model, &model_param);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_model_param_get failed with err 0x%x\n", ret);
        goto exit1;
    }
    vs_sample_trace("model_name[%s] create success, enable_pre_proc=%u, enable_post_proc=%u.\n",
        p_nn_info->model.model_name, model_param.enable_pre_proc, model_param.enable_post_proc);

    /*get cache mmz*/
    ret = nn_output_memory_alloc(p_model, p_nn_info->output_info.dst_data);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("nn_output_memory_alloc failure with err 0x%x\n", ret);
        goto exit1;
    }

    if(nb_file_ptr){
        free(nb_file_ptr);
        nb_file_ptr = VS_NULL;
    }
    return VS_SUCCESS;

exit1:
    vs_mal_nn_model_destroy(p_model);
exit0:
    if(nb_file_ptr){
        free(nb_file_ptr);
        nb_file_ptr = VS_NULL;
    }

    return VS_FAILED;
}

vs_int32_t sample_common_nn_run(sample_nn_info_s *p_nn_info)
{
    vs_int32_t ret = VS_SUCCESS;
    VS_NN_CHECK_PTR(p_nn_info);

    pthread_mutex_init(&p_nn_info->thead_info.mutex, VS_NULL);
    // invoke and post process threads
    if (p_nn_info->input_info.mode == E_NN_INPUT_MODE_PRODUCER) {
        ret = pthread_create(&p_nn_info->thead_info.thread_id, 0, nn_producer_proc, (vs_void_t *)p_nn_info);
    } else if (p_nn_info->input_info.mode == E_NN_INPUT_MODE_TENSOR_DATA) {
        ret = nn_invoke_block(p_nn_info->input_info.src_data, p_nn_info, &p_nn_info->thead_info.result);
    } else {
        ret = VS_FAILED;
        vs_sample_trace("nn mode type[%d] not support!\n", p_nn_info->input_info.mode);
    }

    return ret;
}

vs_int32_t sample_common_nn_result_get(sample_nn_info_s *p_nn_info, sample_nn_post_result_s *p_results)
{
    VS_NN_CHECK_PTR(p_nn_info);
    VS_NN_CHECK_PTR(p_results);

    if (p_nn_info->thead_info.result.is_updated == VS_TRUE) {
        pthread_mutex_lock(&p_nn_info->thead_info.mutex);
        if (memcpy(p_results, &p_nn_info->thead_info.result, sizeof(sample_nn_post_result_s))) {
            p_nn_info->thead_info.result.is_updated = VS_FALSE;
        }
        pthread_mutex_unlock(&p_nn_info->thead_info.mutex);
    }

    return VS_SUCCESS;
}

/*just call when E_NN_INPUT_MODE_PRODUCER*/
vs_int32_t sample_common_nn_stop(sample_nn_info_s *p_nn_info)
{
    vs_int32_t ret = VS_SUCCESS;
    void* result = VS_NULL;

    vs_sample_trace("sample_common_nn_stop  call \n");
    VS_NN_CHECK_PTR(p_nn_info);
    if (p_nn_info->input_info.mode == E_NN_INPUT_MODE_PRODUCER) {
        p_nn_info->thead_info.stop_flag = VS_TRUE;
        if (p_nn_info->thead_info.thread_id != 0) {
            pthread_join(p_nn_info->thead_info.thread_id, &result);
            p_nn_info->thead_info.thread_id = 0;
        }
    }

    return ret;
}

vs_int32_t sample_common_nn_destroy(sample_nn_info_s *p_nn_info)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_uint32_t i = 0;

    VS_NN_CHECK_PTR(p_nn_info);
    if (p_nn_info->thead_info.stop_flag != VS_TRUE) {
        sample_common_nn_stop(p_nn_info);
    }

    for (i = 0; i < p_nn_info->model.output_num; i++) {
        ret = nn_memory_free(p_nn_info->output_info.dst_data[i].buffer.phys_addr,
                             p_nn_info->output_info.dst_data[i].buffer.p_virt_addr,
                             p_nn_info->output_info.dst_data[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_common_nn_destroy output buffer failed, ret 0x%x. \n", ret);
        }
    }

    ret = vs_mal_nn_model_destroy(&p_nn_info->model);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_model_destroy failed with err 0x%x model_name[%s]\n", ret, p_nn_info->model.model_name);
        return VS_FAILED;
    }

    vs_sample_trace("model_name[%s] destroy success.\n", p_nn_info->model.model_name);
    return VS_SUCCESS;
}

vs_int32_t sample_common_nn_exit()
{
    vs_int32_t ret = 0;
    // nn exit
    ret = vs_mal_nn_exit();
    if (ret != VS_SUCCESS) {
        vs_sample_trace("vs_mal_nn_exit failed with err 0x%x\n", ret);
        return ret;
    }
    return VS_SUCCESS;
}

vs_int32_t sample_common_nn_input_buffer_create_by_file(const vs_nn_model_s *p_model, vs_nn_src_data_s *p_src_data, const vs_char_t *input_file[])
{
    vs_uint32_t file_size = 0;
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t i = 0;

    ret = nn_input_memory_alloc(p_model, p_src_data);
    if (ret != VS_SUCCESS) {
        vs_sample_trace("alloc input(%d) mmz failed with err 0x%x\n", i, ret);
        return VS_FAILED;
    }

    // get input data
    for (i = 0; i < p_model->input_num; i++) {
        file_size = nn_file_size_get(input_file[i]);
        if (p_src_data[i].buffer.size != file_size) {
            vs_sample_trace("input_%d size doesn't matched, model input size: %d, file size: %d\n",
                            i, p_src_data[i].buffer.size, file_size);
        }
        ret = nn_file_read(input_file[i], p_src_data[i].buffer.p_virt_addr);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("nn_file_read. failed, ret 0x%x. \n", ret);
        }
    }

    ret |= nn_data_memory_flush(p_model->input_num, p_src_data);

    return ret;
}

vs_int32_t sample_common_nn_input_buffer_destroy(const vs_nn_model_s *p_model, vs_nn_src_data_s *p_src_data)
{
    vs_int32_t ret = VS_SUCCESS;
    vs_int32_t i = 0;

    for (i = 0; i < p_model->input_num; i++) {
        ret = nn_memory_free(p_src_data[i].buffer.phys_addr,
                                p_src_data[i].buffer.p_virt_addr,
                                p_src_data[i].buffer.size);
        if (ret != VS_SUCCESS) {
            vs_sample_trace("sample_nn_destroy_input_buffer input buffer failed, ret 0x%x. \n", ret);
        }
    }

    return ret;
}


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */
