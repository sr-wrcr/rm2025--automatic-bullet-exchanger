/**
 * @file client_ui_plot.h
 * @author EHusky
 * @date 2024-07-27
 *
 * @note 角度值含义为：0°指12点钟方向，顺时针绘制；
 *       屏幕位置：（0,0）为屏幕左下角（1920，1080）为屏幕右上角；
 *       浮点数：整型数均为32位，对于浮点数，实际显示的值为输入的值/1000，
 *       如在details_c、details_d、details_e对应的字节输入1234，选手端实际显示的值将为1.234；
 *       即使发送的数值超过对应数据类型的限制，图形仍有可能显示，但此时不保证显示的效果。
 */

#ifndef _CLIENT_UI_PLOT_H__
#define _CLIENT_UI_PLOT_H__

#include "referee_protocol.h"

/**
 * @brief 绘制直线
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 起始点x坐标
 * @param end_x 终点x坐标
 * @param start_y 起始点y坐标
 * @param end_y 终点y坐标
 */
extern void uiPlotLine(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                       uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t end_x,
                       uint32_t start_y, uint32_t end_y);

/**
 * @brief 绘制矩形
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 起始点x坐标
 * @param end_x 对顶点x坐标
 * @param start_y 起始点y坐标
 * @param end_y 对顶点y坐标
 */
extern void uiPlotRectangle(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                            uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t end_x,
                            uint32_t start_y, uint32_t end_y);

/**
 * @brief 绘制正圆
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 圆心x坐标
 * @param start_y 圆心y坐标
 * @param radius 半径
 */
extern void uiPlotCircle(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                         uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                         uint32_t radius);

/**
 * @brief 绘制椭圆
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 中心点x坐标
 * @param start_y 中心点y坐标
 * @param x_length x半轴长度
 * @param y_length y半轴长度
 */
extern void uiPlotEllipse(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                          uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                          uint32_t x_length, uint32_t y_length);

/**
 * @brief 绘制圆弧
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 中心点x坐标
 * @param start_y 中心点y坐标
 * @param start_degree 起始角度
 * @param end_degree 终止角度
 * @param x_length x半轴长度
 * @param y_length y半轴长度
 */
extern void uiPlotArc(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                      uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                      uint32_t start_degree, uint32_t end_degree, uint32_t x_length, uint32_t y_length);

/**
 * @brief 绘制浮点数
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 起始点x坐标
 * @param start_y 起始点y坐标
 * @param font_size 字体大小
 * @param value 显示的值的千倍放大整数
 */
extern void uiPlotFloatNum(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                           uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x,
                           uint32_t start_y, uint32_t font_size, int32_t value);

/**
 * @brief 绘制整形数
 *
 * @param figure 图形数据结构体
 * @param figure_name 图形名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 起始点x坐标
 * @param start_y 起始点y坐标
 * @param font_size 字体大小
 * @param value 显示的值
 */
extern void uiPlotIntNum(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                         uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                         uint32_t font_size, int32_t value);

/**
 * @brief 绘制字符
 *
 * @param character 字符数据结构体
 * @param character_name 字符名称，仅作为索引
 * @param operate_tpye 图形操作类型
 * @param layer 图层 0-9 9为最上层
 * @param color 图形颜色
 * @param width 图形线宽 1-9
 * @param start_x 起始点x坐标
 * @param start_y 起始点y坐标
 * @param font_size 字体大小
 * @param data 字符内容数据地址
 * @param character_length 字符内容数据长度
 */
extern void uiPlotChar(interaction_character_t *character, char character_name[3], figure_operation_type_e operate_tpye,
                       uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                       uint32_t font_size, char *data, uint32_t character_length);

#endif /* _CLIENT_UI_PLOT_H__ */
