/**
* @file vs_comm_video.h
* @brief comm video info
* @details comm video info
* @author VS codec group
* @date 2021-06-30
* @version v0.10
* @Copyright (C) 2021, Shanghai Visinex Technologies Co., Ltd. All rights reserved.
*/

#ifndef __VS_COMM_VIDEO_H__
#define __VS_COMM_VIDEO_H__

#include "vs_type.h"
#include "vs_comm.h"

#ifdef __cplusplus
extern "C" {
#endif

/** max gdc region num */
#define MAX_FISHEYE_REGION_NUM  4
/** Size of lmf config data  */
#define GDC_LMF_LENGTH          128

#define MAX_MMZ_NAME_LEN	32

/**
* @brief Defines the border attributes.
* @details
*/
typedef struct vs_border {
    vs_uint32_t top_width;        ///< top border width, in pixel, value range [0, 8192]
    vs_uint32_t bottom_width;     ///< bottom border width, in pixel, value range [0, 8192]
    vs_uint32_t left_width;       ///< right border width, in pixel, value range [0, 8192]
    vs_uint32_t right_width;      ///< right border width, in pixel, value range [0, 8192]
    vs_uint32_t color;            ///< Color Border color, the color format is ARGB
} vs_border_s;

/**
* @brief Defines the coordinate information.
* @details
*/
typedef struct vs_point {
    vs_int32_t x;      ///< horizontal coordinate, value range [0, 8192]
    vs_int32_t y;      ///< vertical coordinate, value range [0, 8192]
} vs_point_s;

/**
* @brief Defines the rectangle information.
* @details
*/
typedef struct vs_rect {
    vs_int32_t x;           ///< rectangle top left horizontal coordinate, value range [0, 8192]
    vs_int32_t y;           ///< rectangle top left vertical coordinate, value range [0, 8192]
    vs_uint32_t width;      ///< rectangle width, value range [0, 8192]
    vs_uint32_t height;     ///< rectangle height, value range [0, 8192]
} vs_rect_s;

/**
* @brief Defines video region information.
* @details
*/
typedef struct vs_region_info {
    vs_uint32_t          region_num;    ///< count of the rectangle
    vs_rect_s            *p_region;      ///< region rectangle information
} vs_region_info_s;

/**
* @brief Defines the dimension information.
* @details
*/
typedef struct vs_size {
    vs_uint32_t width;      ///< video Width, value range [0, 8192]
    vs_uint32_t height;     ///< video Height, value range [0, 8192]
} vs_size_s;

/**
* @brief Defines crop attributes.
* @details
*/
typedef struct vs_crop {
    vs_bool_t  enable;      ///< crop enable
    vs_rect_s  rect;        ///< crop source rectangle
} vs_crop_s;

/**
* @brief Defines frame rate control information.
* @details
*/
typedef struct vs_video_frc {
    vs_int32_t  src_framerate;        ///< source frame rate. value range: [1, 240] or -1, default is 30
    vs_int32_t  dst_framerate;        ///< dest frame rate.   value range: [1, 240] or -1, default is 30
} vs_video_frc_s;

/**
* @brief Defines the aspect ratio mode.
* @details
*/
typedef enum vs_aspect_ratio_mode {
    E_ASPECT_RATIO_MODE_NONE   = 0,      ///< aspect ratio full screen
    E_ASPECT_RATIO_MODE_AUTO,            ///< aspect ratio no change, 1:1
    E_ASPECT_RATIO_MODE_MANUAL,          ///< aspect ratio manual set
    E_ASPECT_RATIO_MODE_MAX
} vs_aspect_ratio_mode_e;

/**
* @brief Defines the aspect ratio information.
* @details
*/
typedef struct vs_aspect_ratio {
    vs_aspect_ratio_mode_e mode;       ///< aspect ratio mode,value range, none/auto/manual, default is none
    vs_uint32_t            bg_color;   ///< aspect ratio background color, the color format is ARGB
    vs_rect_s              rect;       ///< aspect ratio video rectangle,only valid in manual mode
} vs_aspect_ratio_s;

/**
* @brief Defines the pixel format.
* @details
*/
typedef enum vs_pixel_format {
    E_PIXEL_FORMAT_RGB444 = 0,     ///< rgb
    E_PIXEL_FORMAT_RGB555,
    E_PIXEL_FORMAT_RGB565,
    E_PIXEL_FORMAT_RGB888,
    E_PIXEL_FORMAT_BGR444,         ///< bgr
    E_PIXEL_FORMAT_BGR555,
    E_PIXEL_FORMAT_BGR565,
    E_PIXEL_FORMAT_BGR888,
    E_PIXEL_FORMAT_ARGB1555,       ///< argb
    E_PIXEL_FORMAT_ARGB4444,
    E_PIXEL_FORMAT_ARGB8565,
    E_PIXEL_FORMAT_ARGB8888,
    E_PIXEL_FORMAT_ARGB2BPP,
    E_PIXEL_FORMAT_ABGR1555,      ///< abgr
    E_PIXEL_FORMAT_ABGR4444,
    E_PIXEL_FORMAT_ABGR8565,
    E_PIXEL_FORMAT_ABGR8888,
    E_PIXEL_FORMAT_BAYER_8BPP,    ///< bayer
    E_PIXEL_FORMAT_BAYER_10BPP,
    E_PIXEL_FORMAT_BAYER_12BPP,
    E_PIXEL_FORMAT_BAYER_14BPP,
    E_PIXEL_FORMAT_BAYER_16BPP,
    E_PIXEL_FORMAT_YVU_422P,      ///<  planar
    E_PIXEL_FORMAT_YVU_420P,
    E_PIXEL_FORMAT_YVU_422SP,     ///<  semi-planar
    E_PIXEL_FORMAT_YVU_420SP,
    E_PIXEL_FORMAT_YVU_444SP,
    E_PIXEL_FORMAT_YUV_422SP,
    E_PIXEL_FORMAT_YUV_420SP,
    E_PIXEL_FORMAT_YUYV_422,      ///<
    E_PIXEL_FORMAT_YVYU_422,
    E_PIXEL_FORMAT_UYVY_422,
    E_PIXEL_FORMAT_VYUY_422,
    E_PIXEL_FORMAT_YYUV_422,
    E_PIXEL_FORMAT_YYVU_422,
    E_PIXEL_FORMAT_UVYY_422,
    E_PIXEL_FORMAT_VUYY_422,
    E_PIXEL_FORMAT_VY1UY0_422,
    E_PIXEL_FORMAT_YUV_400,
    E_PIXEL_FORMAT_UV_420,
    E_PIXEL_FORMAT_MAX
} vs_pixel_format_e;

/**
* @brief Defines the video_format.
* @details
*/
typedef enum vs_video_format {
    E_VIDEO_FORMAT_LINEAR = 0,       ///<  nature video line */
    E_VIDEO_FORMAT_TILE_4x4,         ///<  tile cell: 4pixel x 4line */
    E_VIDEO_FORMAT_MAX
} vs_video_format_e;

/**
* @brief Defines the video compression format.
* @details
*/
typedef enum vs_compress_mode {
    E_COMPRESS_MODE_NONE = 0,    ///<  no compress
    E_COMPRESS_MODE_RASTER,      ///<  compress
    E_COMPRESS_MODE_MAX
} vs_compress_mode_e;

/**
* @brief Defines the luminance information.
* @details Defines the luminance information.
*/
typedef struct vs_frame_luma_info {
    vs_uint64_t luma_sum;           ///<  Luma sum of current frame
    vs_uint32_t luma_average;       ///<  Luma average of current frame
    vs_uint32_t luma_max;           ///<  Luma max
    vs_uint32_t luma_min;           ///<  Luma min
} vs_frame_luma_info_s;



/**
* @brief Define flags of the frame.
* @details
*/
typedef enum vs_frame_flag {
    E_FRAME_FLAG_NONE        = 0,
    E_FRAME_FLAG_SNAP        = (1 << 0),
    E_FRAME_FLAG_REPEAT      = (1 << 1),
    E_FRAME_FLAG_EXP_FLASH_0 = (1 << 2),
    E_FRAME_FLAG_EXP_FLASH_1 = (1 << 3),
    E_FRAME_FLAG_EXP_FLASH_2 = (1 << 4),
    E_FRAME_FLAG_EXP_FLASH_3 = (1 << 5),
    E_FRAME_FLAG_EXP_LONG    = (1 << 6),
    E_FRAME_FLAG_EXP_SHORT   = (1 << 7),
    E_FRAME_FLAG_STRB_FLASH_0 = (1 << 8),
    E_FRAME_FLAG_STRB_FLASH_1 = (1 << 9),
    E_FRAME_FLAG_STRB_FLASH_2 = (1 << 10),
    E_FRAME_FLAG_STRB_FLASH_3 = (1 << 11),
    E_FRAME_FLAG_STRB_FLASH_4 = (1 << 12),
    E_FRAME_FLAG_STRB_FLASH_5 = (1 << 13),
    E_FRAME_FLAG_STRB_FLASH_6 = (1 << 14),
    E_FRAME_FLAG_STRB_FLASH_7 = (1 << 15),

    E_FRAME_FLAG_MAX         = (1 << 31)
} vs_frame_flag_e;

/**
* @brief Define supplement info of the frame.
* @details  reserved by isp mod
*/
typedef struct vs_frame_supplement {
    vs_void_t     *p_isp_info;
    vs_void_t     *p_lowlatency_info;
} vs_frame_supplement_s;

/**
* @brief Defines the color gamut range enumeration.
* @details
*/
typedef enum vs_color_gamut {
    E_COLOR_GAMUT_BT601 = 0,      ///<  gamut BT.601
    E_COLOR_GAMUT_BT709,          ///<  gamut BT.709
    E_COLOR_GAMUT_USER,           ///<  user-defined and non-standard color gamut
    E_COLOR_GAMUT_MAX
} vs_color_gamut_e;

/**
* @brief Defines the dynamic range of the frame.
* @details  reserved by vo mod
*/
typedef enum vs_dynamic_range {
    E_DYNAMIC_RANGE_SDR8 = 0,
    E_DYNAMIC_RANGE_MAX
} vs_dynamic_range_e;

/**
* @brief Defines raw/yuv data bit_depth.
* @details
*/
typedef enum vs_bit_depth {
    E_BIT_DEPTH_8 = 0,         ///< 8 bit depth
    E_BIT_DEPTH_10,            ///< 10 bit depth
    E_BIT_DEPTH_12,            ///< 12 bit depth
    E_BIT_DEPTH_14,            ///< 14 bit depth
    E_BIT_DEPTH_16,            ///< 16 bit depth
    E_BIT_DEPTH_24,            ///< 24 bit depth
    E_BIT_DEPTH_32,            ///< 32 bit depth
    E_BIT_DEPTH_MAX
} vs_bit_depth_e;

/**
* @brief Define yuv format FCB(Frame control block) data structure.
* @details
*/
typedef struct vs_video_frame {
    vs_uint32_t             width;           ///<  frame width, value range (0, 8192]
    vs_uint32_t             height;          ///<  frame height, value range (0, 8192]
    vs_pixel_format_e       pixel_format;    ///<  pixel format
    vs_video_format_e       video_format;    ///<  video format
    vs_compress_mode_e      compress_mode;   ///<  compress mode
    vs_dynamic_range_e      dynamic_range;   ///<  dynamic range
    vs_color_gamut_e        color_gamut;     ///<
    vs_uint32_t             compress_header_stride[3];      ///< frame compress header's data stride for Y/U/V, value range [0, 8192]
    vs_uint32_t             stride[3];                      ///< frame data's stride for Y/U/V, value range [0, 8192]
    vs_uint64_t             compress_header_phys_addr[3];   ///< frame compress header's data phy addr
    vs_uint64_t             compress_header_virt_addr[3];   ///< frame compress header's data vir addr
    vs_uint64_t             phys_addr[3];     ///< frame data's phy addr
    vs_uint64_t             virt_addr[3];     ///< frame data's vir addr
    vs_uint32_t             id;               ///< frame serial number, value range >= 0
    vs_uint64_t             pts;              ///< time stamp, value range >= 0
    vs_uint64_t             private_data;     ///< user data for sei
    vs_frame_flag_e         frame_flag;       ///< frame flag for snap
    vs_frame_supplement_s   supplement;       ///< reserved by isp mod
} vs_video_frame_s;

/**
* @brief Define video frame information.
* @details
*/
typedef struct vs_video_frame_info {
    vs_video_frame_s  frame;       ///< Frame control block
    vs_uint32_t       poolid;      ///< vb pool id
    vs_mod_id_e       modid;       ///< frame producer module id
} vs_video_frame_info_s;

/**
* @brief Defines the rgb format FCB(Frame control block) data structure.
* @details
*/
typedef struct vs_bitmap {
    vs_pixel_format_e      pixel_format;  ///< bitmap's pixel format
    vs_uint32_t            width;         ///< bitmap width
    vs_uint32_t            height;        ///< bitmap height
    vs_void_t              *p_data;       ///< address of bitmap's data
} vs_bitmap_s;

/**
 * @brief Define the rotation mode
 * @details there define two modes of rotation, fixed and anyview.
 */
typedef enum vs_rotation_mode {
    E_ROTATION_MODE_FIXED = 0,      ///< just support 4 fixed angle rotation, 0/90/180/270 degree, 0 means not rotation.
    E_ROTATION_MODE_ANYVIEW,        ///< support any angle rotation, such as 15 or 120 degree.
    E_ROTATION_MODE_MAX
} vs_rotation_mode_e;

/**
* @brief Defines the fixed rotation degree mode.
* @details
*/
typedef enum vs_fixed_rotation {
    E_FIXED_ROTATION_0 = 0,          ///< video fixed rotation mode clockwise 0 degree
    E_FIXED_ROTATION_90,             ///< video fixed rotation mode clockwise 90 degree
    E_FIXED_ROTATION_180,            ///< video fixed rotation mode clockwise 180 degree
    E_FIXED_ROTATION_270,            ///< video fixed rotation mode clockwise 270 degree
    E_FIXED_ROTATION_MAX
} vs_fixed_rotation_e;

/**
* @brief Defines the config data userspace attributes.
* @details Defines the config data userspace attributes.
*/
typedef struct vs_correction_algo_cfg {
    vs_void_t       *cfg_virt_addr;     ///<  config virtual buffer address
    vs_uint32_t     cfg_virt_size;      ///<  config virtual buffer size
}vs_correction_algo_cfg_s;

/**
* @brief Define the anyview rotation parameter
* @details  Define the anyview rotation parameter, include angle, offset, dest_size and zoom.
*/
typedef struct vs_anyview_rotation {
    vs_uint32_t               angle;            ///< indicate the rotation angle 0 - 360 degree
    vs_int32_t                x_center_offset;  ///< change x position of rotation center
    vs_int32_t                y_center_offset;  ///< change y position of rotation center
    vs_size_s                 dest_size;        ///< define the output image size after anyview rotated, becuase there is zoom in or out.
    vs_int32_t                zoom;             ///< scale coefficients for the original and catalog images
    vs_correction_algo_cfg_s  algo_cfg;       ///< algorithm cfg
} vs_anyview_rotation_s;

/**
* @brief Define the rotation control, including the rotation type and attribute
* @details  enable rotation ,then choose rotation type and set detail attribute according to the rotation mode.
*/
typedef struct vs_rotation_attr {
    vs_bool_t                 enable;           ///< enable rotation or not
    vs_rotation_mode_e        mode;             ///< choose the rotation mode, fixed or anyview
    union {
        vs_fixed_rotation_e   fixed_rotation;	///< fixed rotation type setting
        vs_anyview_rotation_s anyview_rotation; ///< anyview rotation struct information
    };
} vs_rotation_attr_s;


/**
* @brief Defines the wdr_mode.
* @details  reserved by isp mod
*/
typedef enum vs_wdr_mode {
    E_WDR_MODE_NONE = 0,
    E_WDR_MODE_BUILT_IN,
    E_WDR_MODE_2To1_LINE,
    E_WDR_MODE_3To1_LINE,
    E_WDR_MODE_4To1_LINE,
    E_WDR_MODE_MAX
} vs_wdr_mode_e;

/**
* @brief Defines mirror mode.
* @details
*/
typedef enum vs_mirror_mode {
    E_MIRROR_MODE_NONE = 0x0,          ///< no mirror
    E_MIRROR_MODE_HORIZONTAL,          ///< horizontal mirror
    E_MIRROR_MODE_VERTICAL,            ///< vertical mirror
    E_MIRROR_MODE_BOTH,                ///< horizontal & vertical mirror
    E_MIRROR_MODE_MAX
} vs_mirror_mode_e;

/**
* @brief Defines the quadrangle information.
* @details
*/
typedef struct vs_quadrangle {
    vs_bool_t     solid;             ///< the quadrangle is solid or hollow
    vs_uint32_t   thick;             ///< the thick of four sides
    vs_point_s    point[4];          ///< four coordinates for a quadrilateral
} vs_quadrangle_s;

/**
* @brief Defines coordinate  mode.
* @details
*/
typedef enum vs_coordinate_mode {
    E_COORDINATE_MODE_RATIO       = 0,    ///< Ratio coordinate
    E_COORDINATE_MODE_ABSOLUTE,           ///< Absolute coordinate
    E_COORDINATE_MODE_MAX
} vs_coordinate_mode_e;

/**
* @brief Defines zoom in mode.
* @details
*/
typedef enum vs_zoom_mode {
    E_ZOOM_MODE_RECT = 0,       ///< zoom in by rect
    E_ZOOM_MODE_RATIO,          ///< zoom in by ratio
    E_ZOOM_MODE_MAX
} vs_zoom_mode_e;

/**
* @brief Defines zoom in/out ratio attributes.
* @details Defines crop attributes.
*/
typedef  struct vs_zoom_ratio {
    vs_uint32_t x_ratio;     ///< the start point's to be zoomed, Range: [0, 1000]; XRatio = x * 1000 / W
    vs_uint32_t y_ratio;     ///< the start point's to be zoomed, Range: [0, 1000]; YRatio = y * 1000 / H
    vs_uint32_t w_ratio;     ///< the width to be zoomed, Range: [0, 1000]; WRatio = w * 1000 / W
    vs_uint32_t h_ratio;     ///< the height to be zoomed, Range: [0, 1000]; HRatio = h * 1000 / H
} vs_zoom_ratio_s;

/**
* @brief Defines zoom in attributes.
* @details
*/
typedef struct vs_zoom_in_attr {
    vs_zoom_mode_e      zoom_in_type;     ///< choose the mode of zoom in
    union {
        vs_rect_s       zoom_in_rect;     ///< zoom in by rect
        vs_zoom_ratio_s zoom_in_ratio;    ///< zoom in by ratio
    };
} vs_zoom_in_attr_s;

/**
* @brief Defines the fisheye view mode enumeration.
* @details Defines the fisheye view mode enumeration.
*/
typedef enum vs_fisheye_view_mode {
    E_FISHEYE_VIEW_MODE_360 = 0,            ///< set gdc correction mode : 360 panorama
    E_FISHEYE_VIEW_MODE_180,                ///< set gdc correction mode : 180 panorama
    E_FISHEYE_VIEW_MODE_NORMAL,             ///< set gdc correction mode : ldc
    E_FISHEYE_VIEW_MODE_NO_TRANSFORM,       ///< set gdc correction mode : no transform
    E_FISHEYE_VIEW_MODE_MAX
} vs_fisheye_view_mode_e;

/**
* @brief Defines the fisheye mount mode enumeration.
* @details Defines the fisheye mount mode enumeration.
*/
typedef enum vs_fisheye_install_mode {
    E_FISHEYE_INSTALL_MODE_DESKTOP = 0,     ///< set gdc install mode : desktop
    E_FISHEYE_INSTALL_MODE_CEILING,         ///< set gdc install mode : ceiling
    E_FISHEYE_INSTALL_MODE_WALL,            ///< set gdc install mode : wall
    E_FISHEYE_INSTALL_MODE_MAX
} vs_fisheye_install_mode_e;

/**
* @brief Defines the fisheye region attributes only used in mode 180.
* @details Defines the fisheye region attributes only used in mode 180.
*/
typedef struct vs_fisheye_mode_180_attr {
	vs_int32_t              trapezoid_left_angle;                   ///< the strength of trapezoid left
	vs_int32_t              trapezoid_right_angle;                  ///< the strength of trapezoid right
	vs_int32_t              azimuth;                                ///< rotation of the projection asix in degrees, effect when elevation is not 0
	vs_int32_t              elevation;                              ///< tilt of the projection axis in degrees
	vs_uint32_t             rotation_angle;                         ///< rotation angle
	vs_uint32_t             fov_width;                              ///< horizontal fov of ROI
	vs_uint32_t             fov_height;                             ///< vertical fov of ROI
	vs_bool_t               ratio_keep;                             ///< Whether aspect fov ration is keep
	vs_int32_t              cylindricity_x;                         ///< spherical along the X axi
	vs_int32_t              cylindricity_y;                         ///< spherical along the Y axis
} vs_fisheye_mode_180_attr_s;

/**
* @brief Defines the fisheye region attributes.
* @details Defines the fisheye region attributes.
*/
typedef struct vs_fisheye_region_attr {
    vs_fisheye_view_mode_e         view_mode;                              ///< view mode: 180/360/normal
    vs_uint32_t                    pan;                                    ///< pan value,
    vs_uint32_t                    tilt;                                   ///< tilt value
    vs_int32_t                     zoom;                                   ///< zoom value,In mode 360, it effect fov of roi, In mode 180, it effect scale ratio
    vs_rect_s                      src_rect;                               ///< Input ROI start position, width, and height
    vs_rect_s                      dst_rect;                               ///< Output window start position, width, and height
    union {
        vs_int32_t                 mode_360_strength;                      ///< strength effect only in mode 360
        vs_fisheye_mode_180_attr_s mode_180_attr;                          ///< parameter effect only in mode 180
    };
} vs_fisheye_region_attr_s;

/**
* @brief Defines the fisheye attributes.
* @details Defines the fisheye attributes.
*/
typedef struct vs_fisheye_attr {
    vs_bool_t                   enable;                                 ///< whether enable fisheye correction or not
    vs_bool_t                   lmf_enable;                             ///< whether LMF coefficient is from user or not
    vs_bool_t                   bgcolor_enable;                         ///< whether use background color or not
    vs_int32_t                  bgcolor_value;                          ///< background color value 0 ~ 0xFFFFFF RGB
    vs_int32_t                  h_offset;                               ///< the horizontal offset
    vs_int32_t                  v_offset;                               ///< the vertical offset
    vs_int32_t                  diameter;                               ///< diameter of fov
    vs_uint32_t                 fov;                                    ///< fov value
    vs_fisheye_install_mode_e   mode;                                   ///< the fisheye install mode
    vs_uint32_t                 region_num;                             ///< the fisheye region num
    vs_fisheye_region_attr_s    region_attr[MAX_FISHEYE_REGION_NUM];    ///< the attribution of fisheye region
    vs_correction_algo_cfg_s    algo_cfg;                               ///< algorithm cfg
} vs_fisheye_attr_s;

/**
* @brief Defines the ldc correction mode enumeration.
* @details Defines the ldc correction mode enumeration.
*/
typedef enum vs_ldc_mode {
    E_LDC_MODE_PANORAMIC = 0,     ///< set ldc correct mode : panoramic
    E_LDC_MODE_UNIVERSAL,         ///< set ldc correct mode : universal
    E_LDC_MODE_MAX
} vs_ldc_mode_e;

/**
* @brief Defines the ldc attributes.
* @details Defines the ldc attributes.
*/
typedef struct vs_ldc_attr {
    vs_bool_t               enable;             ///< whether enable or not
    vs_ldc_mode_e           mode;               ///< correct mode panoramic or universal
    vs_bool_t               ratio_keep;         ///< Whether aspect ration is keep
    vs_int32_t              h_ratio;            ///< field angle ration of horizontal,valid when (mode=E_LDC_MODE_UNIVERSAL) || (mode=E_LDC_MODE_PANORAMIC && ratio_keep=0)
    vs_int32_t              v_ratio;            ///< field angle ration of vertical,valid when (mode=E_LDC_MODE_UNIVERSAL) || (mode=E_LDC_MODE_PANORAMIC && ratio_keep=0)
    vs_int32_t              ratio;              ///< field angle ration of all,valid when (mode=E_LDC_MODE_PANORAMIC && ratio_keep=1)
    vs_int32_t              center_x_offset;    ///< horizontal offset of the image distortion center relative to image center
    vs_int32_t              center_y_offset;    ///< vertical offset of the image distortion center relative to image center
    vs_int32_t              zoom;               ///< Scale coefficients for the original and catalog images
    vs_int32_t              input_fov;			///< fov of source frame

    vs_int32_t              diameter;           ///< diameter of fov,it is invalid when panoramic mode
    vs_int32_t              fov_width;          ///< horizontal fov of dest frame, it is invalid when panoramic mode
    vs_int32_t              fov_height;         ///< vertical fov of dest frame, it is invalid when panoramic mode
    vs_int32_t              azimuth;            ///< rotation of the projection asix in degrees, effect when elevation is not 0
    vs_int32_t              elevation;          ///< tilt of the projection axis in degrees
    vs_uint32_t             rotation_angle;     ///< rotation angle
    vs_correction_algo_cfg_s algo_cfg;          ///< algorithm cfg
} vs_ldc_attr_s;

/**
* @brief Defines the lmf cfg attributes.
* @details Defines the lmf cfg attributes.
*/
typedef struct vs_fisheye_lmf_cfg {
    vs_uint16_t     lmf_coef[GDC_LMF_LENGTH];   ///< user lmf data
} vs_fisheye_lmf_cfg_s;

/**
* @brief Defines the gdc task attributes.
* @details Defines the gdc task attributes.
*/
typedef struct vs_task_attr {
    vs_video_frame_info_s src_frame;           ///< the source image info
    vs_video_frame_info_s dst_frame;           ///< the destination image info
    vs_bool_t             check_only;          ///< Whether check only mode or not
    vs_uint64_t           reserved[4];         ///< private data
    vs_uint64_t           debug;               ///< using for debug setting
} vs_task_attr_s;

typedef enum vs_lowlatency_mode {
    E_LOWLATENCY_MODE_NORMAL,
    E_LOWLATENCY_MODE_VENC_ONLY,
    E_LOWLATENCY_MODE_MAX
} vs_lowlatency_mode_e;

typedef enum vs_lowlatency_linebuf_mode {
    E_LOWLATENCY_LINEBUF_MODE_16_LINE = 0,
    E_LOWLATENCY_LINEBUF_MODE_64_LINE,
    E_LOWLATENCY_LINEBUF_MODE_MAX
} vs_lowlatency_linebuf_mode_e;

typedef struct vs_lowlatency_attr {
    vs_bool_t        enable;
    vs_lowlatency_mode_e mode;
    union {
        vs_int32_t line_cnt;
        vs_lowlatency_linebuf_mode_e linebuf_mode;
    };
} vs_lowlatency_attr_s;

#ifdef __cplusplus
}
#endif

#endif /*!<  __VS_COMM_VIDEO_H__ */

