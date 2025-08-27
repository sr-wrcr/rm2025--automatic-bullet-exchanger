#include "string.h"

#include "client_ui_plot.h"

void uiPlotLine(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye, uint32_t layer,
                figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t end_x, uint32_t start_y,
                uint32_t end_y)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_LINE;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_d = end_x;
    figure->details_e = end_y;
}

void uiPlotRectangle(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                     uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t end_x,
                     uint32_t start_y, uint32_t end_y)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_RECTANGLE;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_d = end_x;
    figure->details_e = end_y;
}

void uiPlotCircle(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                  uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                  uint32_t radius)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_CIRCLE;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_c = radius;
}

void uiPlotEllipse(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                   uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                   uint32_t x_length, uint32_t y_length)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_ELLIPSE;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_d = x_length;
    figure->details_e = y_length;
}

void uiPlotArc(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye, uint32_t layer,
               figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y, uint32_t start_degree,
               uint32_t end_degree, uint32_t x_length, uint32_t y_length)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_ARC;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_a = start_degree;
    figure->details_b = end_degree;
    figure->details_d = x_length;
    figure->details_e = y_length;
}

void uiPlotFloatNum(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                    uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                    uint32_t font_size, int32_t value)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_FLOAT_NUM;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_a = font_size;

    if (value > 2145387000)
        value = 2145387000;
    else if (value < -2145387000)
        value = -2145387000;
    if (value >= 0)
    {
        figure->details_e = value / 2097152;
        figure->details_d = (value - figure->details_e * 2097152) / 1024;
        figure->details_c = value - figure->details_e * 2097152 - figure->details_d * 1024;
    }
    else
    {
        value = -value;
        figure->details_e = 2047 - value / 2097152;
        figure->details_d = 2047 - (value - figure->details_e * 2097152) / 1024;
        figure->details_c = 1024 - (value - figure->details_e * 2097152 - figure->details_d * 1024);
    }
}

void uiPlotIntNum(interaction_figure_t *figure, char figure_name[3], figure_operation_type_e operate_tpye,
                  uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                  uint32_t font_size, int32_t value)
{
    memset(figure, 0, sizeof(interaction_figure_t));
    for (uint8_t i = 0; i < 3 && figure_name[i] != '\0'; i++)
        figure->figure_name[2 - i] = figure_name[i];
    figure->operate_tpye = operate_tpye;
    figure->figure_tpye = FIGURE_INT_NUM;
    figure->layer = layer;
    figure->layer = figure->layer > 9 ? 9 : figure->layer;
    figure->color = color;
    figure->width = width;
    // figure->width = figure->width > 9 ? 9 : figure->width;
    figure->start_x = start_x;
    figure->start_y = start_y;
    figure->details_a = font_size;

    if (value > 2145387000)
        value = 2145387000;
    else if (value < -2145387000)
        value = -2145387000;
    if (value >= 0)
    {
        figure->details_e = value / 2097152;
        figure->details_d = (value - figure->details_e * 2097152) / 1024;
        figure->details_c = value - figure->details_e * 2097152 - figure->details_d * 1024;
    }
    else
    {
        value = -value;
        figure->details_e = 2047 - value / 2097152;
        figure->details_d = 2047 - (value - figure->details_e * 2097152) / 1024;
        figure->details_c = 1024 - (value - figure->details_e * 2097152 - figure->details_d * 1024);
    }
}

void uiPlotChar(interaction_character_t *character, char character_name[3], figure_operation_type_e operate_tpye,
                uint32_t layer, figure_color_type_e color, uint32_t width, uint32_t start_x, uint32_t start_y,
                uint32_t font_size, char *data, uint32_t character_length)
{
    memset(character, 0, sizeof(interaction_character_t));
    for (uint8_t i = 0; i < 3 && character_name[i] != '\0'; i++)
        character->figure.figure_name[2 - i] = character_name[i];

    character->figure.operate_tpye = operate_tpye;
    character->figure.figure_tpye = FIGURE_CHARACTER;
    character->figure.layer = layer;
    character->figure.layer = character->figure.layer > 9 ? 9 : character->figure.layer;
    character->figure.color = color;
    character->figure.width = width;
    character->figure.width = character->figure.width > 9 ? 9 : character->figure.width;
    character->figure.start_x = start_x;
    character->figure.start_y = start_y;
    character->figure.details_a = font_size;
    character->figure.details_b = character_length;

    for (uint8_t i = 0; i < character->figure.details_b; i++)
    {
        character->data[i] = *data;
        data++;
    }
}
