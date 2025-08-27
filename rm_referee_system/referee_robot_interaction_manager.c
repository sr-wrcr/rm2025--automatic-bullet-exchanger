#include "stdlib.h"
#include "string.h"

#include "referee_robot_interaction_manager.h"

#include "referee_frame_process.h"

static robot_interaction_manager_t manager;

static uint8_t *manage_and_encode_message_data(void);
// static uint8_t *encode_client_ui_delete_data(layer_delete_type_e delete_type, uint8_t layer);
static uint8_t *manage_and_encode_client_ui_plot_data(void);

static robot_interaction_data_header_t *modify_interaction_data_header(uint16_t sub_cmd_id, uint16_t target_id);
static uint8_t get_sub_data_len_by_sub_cmd_id(uint16_t sub_cmd_id);

bool refereeRobotInteractionMessageInit(uint8_t message_num)
{
    manager.message_num = message_num;
    manager.configured_message_num = 0;
    manager.message_encode_index = 0;
    if (manager.message_num > 0 && manager.message_num < 255)
    {
        manager.messages = malloc(manager.message_num * sizeof(interaction_message_factory_t));
        memset(manager.messages, 0, manager.message_num * sizeof(interaction_message_factory_t));
    }
    else
    {
        return false;
    }

    return true;
}

/**
 * @brief 初始化机器人交互数据管理器
 *
 * @param step_clock_source 时钟源
 * @param step_time
 * @param figure_num
 * @param character_num
 */
bool refereeRobotInteractionManagerInit(uint8_t figure_num, uint8_t character_num)
{
    memset(&manager, 0, sizeof(robot_interaction_manager_t));

    manager.init_success = false;

    manager.last_successful_send_time = 0;

    manager.interaction_data_sent = malloc(REF_PROTOCOL_FRAME_DATA_MAX_SIZE);
    memset(manager.interaction_data_sent, 0, REF_PROTOCOL_FRAME_DATA_MAX_SIZE);

    manager.figure_num = figure_num;
    manager.configured_figure_num = 0;
    manager.last_queue_refresh_figure_index = 0;
    if (manager.figure_num > 0 && manager.figure_num < 255)
    {
        manager.figures = malloc(manager.figure_num * sizeof(interaction_figure_factory_t));
        memset(manager.figures, 0, manager.figure_num * sizeof(interaction_figure_factory_t));
    }
    else
    {
        return false;
    }

    manager.character_num = character_num;
    manager.configured_character_num = 0;
    if (manager.character_num > 0 && manager.character_num < 255)
    {
        manager.characters = malloc(manager.character_num * sizeof(interaction_character_factory_t));
        memset(manager.characters, 0, manager.character_num * sizeof(interaction_character_factory_t));
    }
    else
    {
        return false;
    }

    return manager.init_success = true, manager.init_success;
}

/**
 * @brief 编码机器人交互数据
 *
 * @param robot_interaction_type 机器人交互类型
 * @return uint8_t* 一整包可直接发给裁判系统的数据流
 * @note 使用的时候务必检查返回值是否为NULL，仅当不为NULL时用户才可发送
 */
uint8_t *refereeEncodeRobotInteractionData(uint32_t now_time, robot_interaction_type_e robot_interaction_type)
{
    if (!manager.init_success)
        return NULL;

    if ((float)(now_time - manager.last_successful_send_time) > ROBOT_INTERACTION_COMM_TIME_MS)
    {
        uint8_t *sub_data = NULL;

        switch (robot_interaction_type)
        {
        case ROBOT_TO_ROBOT_INTERACTION: {
            if (sub_data = manage_and_encode_message_data(), sub_data == NULL)
                return NULL;
            return referee_pack_data(ROBOT_INTERACTION_DATA_CMD_ID, sub_data,
                                     sizeof(robot_interaction_data_header_t) +
                                         manager.messages[manager.message_encode_index].data_length);
        }
        case CLIENT_UI_PLOT: {
            if (sub_data = manage_and_encode_client_ui_plot_data(), sub_data == NULL)
                return NULL;
            return referee_pack_data(ROBOT_INTERACTION_DATA_CMD_ID, sub_data,
                                     sizeof(robot_interaction_data_header_t) +
                                         get_sub_data_len_by_sub_cmd_id(manager.sub_cmd_id));
        }
        case SENTRY_CMD: {
            return NULL;
        }
        case RADAR_CMD: {
            return NULL;
        }

        default:
            return NULL;
        }
    }

    return NULL;
}

void refereeRobotInteractionManagerSuccessfullySentHook(uint32_t now_time)
{
    if (!manager.init_success)
        return;

    manager.last_successful_send_time = now_time;
}

uint8_t refereeRobotInteractionMessageConfig(uint16_t message_id, uint16_t target_id,
                                             void (*builder)(uint8_t *, uint8_t *))
{
    if (!manager.init_success)
        return 255u;
    if (manager.configured_message_num >= manager.message_num)
        return 255u;
    if (builder == NULL)
        return 255u;

    uint8_t index = manager.configured_message_num;
    manager.configured_message_num++;

    manager.messages[index].message_id = message_id;
    manager.messages[index].target_id = target_id;

    manager.messages[index].data_length = 0;

    manager.messages[index].builder = builder;

    return index;
}
// void refereeSetRobotInteractionMessageBuilder(uint8_t index, void (*builder)(interaction_figure_t *))
// {
//     if (index >= manager.message_target_num)
//         return;
//     if (builder == NULL)
//         return;

//     manager.figures[index].builder = builder;
// }

// void refereeSetRobotInteractionLayerDeleterBuilder(void (*builder)(interaction_layer_delete_t *))
// {
//     if (builder == NULL)
//         return;

//     manager.layer_deleter_builder = builder;
// }

uint8_t refereeRobotInteractionFigureConfig(client_ui_refresh_level_e refresh_level,
                                            void (*builder)(interaction_figure_t *, figure_operation_type_e))
{
    if (!manager.init_success)
        return 255u;
    if (manager.configured_figure_num >= manager.figure_num)
        return 255u;
    if (builder == NULL)
        return 255u;

    uint8_t index = manager.configured_figure_num;
    manager.configured_figure_num++;

    manager.figures[index].builder = builder;

    manager.figures[index].refresh_level = refresh_level;

    return index;
}

uint8_t refereeRobotInteractionCharacterConfig(void (*builder)(interaction_character_t *, figure_operation_type_e))
{
    if (!manager.init_success)
        return 255u;
    if (manager.configured_character_num >= manager.character_num)
        return 255u;
    if (builder == NULL)
        return 255u;

    uint8_t index = manager.configured_character_num;
    manager.configured_character_num++;

    manager.characters[index].builder = builder;

    return index;
}

void refereeClientUiDynamicFigureAdjust(uint8_t index, client_ui_refresh_level_e refresh_level)
{
    if (!manager.init_success)
        return;
    if (index >= manager.configured_figure_num)
        return;

    manager.figures[index].refresh_level = refresh_level;
}

void refereeClientUiOperate(client_ui_operation_type_e operation_type, uint8_t index)
{
    if (!manager.init_success)
        return;

    if (operation_type == UI_RESET_ALL)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_plotted = false;
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_plotted = false;
    }
    else if (operation_type == UI_RESET_ALL_FIGURES)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_plotted = false;
    }
    else if (operation_type == UI_RESET_ALL_CHARACTERS)
    {
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_plotted = false;
    }
    else if (operation_type == UI_RESET_FIGURE)
    {
        if (index >= manager.configured_figure_num)
            return;
        manager.figures[index].is_plotted = false;
    }
    else if (operation_type == UI_RESET_CHARACTER)
    {
        if (index >= manager.configured_character_num)
            return;
        manager.characters[index].is_plotted = false;
    }
    else if (operation_type == UI_HIDE_ALL)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_hidden = true;
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_hidden = true;
    }
    else if (operation_type == UI_HIDE_ALL_FIGURES)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_hidden = true;
    }
    else if (operation_type == UI_HIDE_ALL_CHARACTERS)
    {
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_hidden = true;
    }
    else if (operation_type == UI_HIDE_FIGURE)
    {
        if (index >= manager.configured_figure_num)
            return;
        manager.figures[index].is_hidden = true;
    }
    else if (operation_type == UI_HIDE_CHARACTER)
    {
        if (index >= manager.configured_character_num)
            return;
        manager.characters[index].is_hidden = true;
    }
    else if (operation_type == UI_DISPLAY_ALL)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_hidden = false;
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_hidden = false;
    }
    else if (operation_type == UI_DISPLAY_ALL_FIGURES)
    {
        for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            manager.figures[i].is_hidden = false;
    }
    else if (operation_type == UI_DISPLAY_ALL_CHARACTERS)
    {
        for (uint8_t i = 0; i < manager.configured_character_num; i++)
            manager.characters[i].is_hidden = false;
    }
    else if (operation_type == UI_DISPLAY_FIGURE)
    {
        if (index >= manager.configured_figure_num)
            return;
        manager.figures[index].is_hidden = false;
    }
    else if (operation_type == UI_DISPLAY_CHARACTER)
    {
        if (index >= manager.configured_character_num)
            return;
        manager.characters[index].is_hidden = false;
    }
}

static uint8_t *manage_and_encode_message_data(void)
{
    manager.sub_cmd_id = manager.messages[manager.message_encode_index].message_id;

    manager.messages[manager.message_encode_index].builder(manager.messages[manager.message_encode_index].message,
                                                           &manager.messages[manager.message_encode_index].data_length);

    /* 数据头编码 */
    memcpy(manager.interaction_data_sent,
           modify_interaction_data_header(manager.sub_cmd_id, manager.messages[manager.message_encode_index].target_id),
           sizeof(robot_interaction_data_header_t));
    /* 数据内容编码 */
    memset(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t), 0,
           manager.messages[manager.message_encode_index].data_length);
    memcpy(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t),
           manager.messages[manager.message_encode_index].message,
           manager.messages[manager.message_encode_index].data_length);

    manager.message_encode_index =
        manager.message_encode_index >= (manager.configured_message_num - 1) ? 0 : manager.message_encode_index + 1;

    return manager.interaction_data_sent;
}

// static uint8_t *encode_client_ui_delete_data(layer_delete_type_e delete_type, uint8_t layer)
// {
//     return NULL;
// }

/**
 * @brief 自动决策并编码UI数据
 *
 * @note 编码优先级 静态图形 > 静态字符 > 动态图形
 */
static uint8_t *manage_and_encode_client_ui_plot_data(void)
{
    /* 索引值 图形最多能发7个 所以有7个索引值 */
    uint8_t index[7] = {0};
    uint8_t used_index_num = 0;

    /* 静态图形 */
    for (uint8_t i = 0; i < manager.configured_figure_num; i++)
    {
        if (manager.figures[i].refresh_level == UI_REFRESH_ONCE &&
            (manager.figures[i].is_hidden ? manager.figures[i].is_plotted : !manager.figures[i].is_plotted))
        {
            index[used_index_num] = i;
            used_index_num++;
            if (used_index_num >= MAX_FIGURE_PLOT_NUM_IN_A_SENT)
                break;
        }
    }
    if (used_index_num)
    {
        /* 设定子内容ID */
        manager.sub_cmd_id = PLOT_SEVEN_FIGURE_SUBCMD_ID;
        /* 构建数据内容 */
        for (uint8_t i = 0; i < used_index_num; i++)
        {
            if (manager.figures[index[i]].builder != NULL)
            {
                figure_operation_type_e figure_operation_type = FIGURE_NOP;
                if (manager.figures[index[i]].is_hidden)
                {
                    if (manager.figures[index[i]].is_plotted)
                    {
                        figure_operation_type = FIGURE_DELETE;
                        manager.figures[index[i]].is_plotted = false;
                    }
                    else
                    {
                        figure_operation_type = FIGURE_NOP;
                    }
                }
                else
                {
                    if (manager.figures[index[i]].is_plotted)
                    {
                        figure_operation_type = FIGURE_MODIFY;
                    }
                    else
                    {
                        figure_operation_type = FIGURE_ADD;
                        manager.figures[index[i]].is_plotted = true;
                    }
                }
                manager.figures[index[i]].builder(&manager.figures[index[i]].figure, figure_operation_type);
            }
        }
        /* 数据头编码 */
        memcpy(manager.interaction_data_sent,
               modify_interaction_data_header(manager.sub_cmd_id, getRobotClientId(ROBOT_SELF_ID)),
               sizeof(robot_interaction_data_header_t));
        /* 数据内容编码 */
        memset(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t), 0,
               MAX_FIGURE_PLOT_NUM_IN_A_SENT * sizeof(interaction_figure_t));
        for (uint8_t i = 0; i < used_index_num; i++)
        {
            memcpy(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t) +
                       i * sizeof(interaction_figure_t),
                   &manager.figures[index[i]].figure, sizeof(interaction_figure_t));
        }

        return manager.interaction_data_sent;
    }

    /* 静态字符 */

    for (uint8_t i = 0; i < manager.configured_character_num; i++)
    {
        if (manager.characters[i].is_hidden ? manager.characters[i].is_plotted : !manager.characters[i].is_plotted)
        {
            index[used_index_num] = i;
            used_index_num++;
            break;
        }
    }
    if (used_index_num)
    {
        /* 设定子内容ID */
        manager.sub_cmd_id = PLOT_CHARACTER_FIGURE_SUBCMD_ID;
        /* 构建数据内容 */
        if (manager.characters[index[0]].builder != NULL)
        {
            figure_operation_type_e figure_operation_type = FIGURE_NOP;
            if (manager.characters[index[0]].is_hidden)
            {
                if (manager.characters[index[0]].is_plotted)
                {
                    figure_operation_type = FIGURE_DELETE;
                    manager.characters[index[0]].is_plotted = false;
                }
                else
                {
                    figure_operation_type = FIGURE_NOP;
                }
            }
            else
            {
                if (manager.characters[index[0]].is_plotted)
                {
                    figure_operation_type = FIGURE_MODIFY;
                }
                else
                {
                    figure_operation_type = FIGURE_ADD;
                    manager.characters[index[0]].is_plotted = true;
                }
            }
            manager.characters[index[0]].builder(&manager.characters[index[0]].character, figure_operation_type);
            /* 数据头编码 */
            memcpy(manager.interaction_data_sent,
                   modify_interaction_data_header(manager.sub_cmd_id, getRobotClientId(ROBOT_SELF_ID)),
                   sizeof(robot_interaction_data_header_t));
            /* 数据内容编码 */
            memcpy(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t),
                   &manager.characters[index[0]].character, sizeof(interaction_character_t));

            return manager.interaction_data_sent;
        }
    }

    /* 动态图形 */

    /* 队列刷新的图形数量 */
    uint8_t queue_refresh_figure_num = 0;
    for (uint8_t i = 0; i < manager.configured_figure_num; i++)
    {
        if (manager.figures[i].refresh_level == UI_REFRESH_REAL_TIME &&
            (manager.figures[i].is_hidden ? manager.figures[i].is_plotted : true))
        {
            index[used_index_num] = i;
            used_index_num++;
            if (used_index_num >= MAX_FIGURE_PLOT_NUM_IN_A_SENT)
                break;
        }
        else if (manager.figures[i].refresh_level == UI_REFRESH_IN_QUEUE &&
                 (manager.figures[i].is_hidden ? manager.figures[i].is_plotted : true))
        {
            queue_refresh_figure_num++;
        }
    }
    if (used_index_num < MAX_FIGURE_PLOT_NUM_IN_A_SENT)
    {
        /* 本次编码可以发送的队列刷新图形数量 也即实时刷新图形余下的空位数量 */
        uint8_t figure_plot_vacancy = MAX_FIGURE_PLOT_NUM_IN_A_SENT - used_index_num;
        /* 队列刷新图形数量不多于实时刷新图形余下的空位数量时 其最终也将被实时刷新 */
        if (figure_plot_vacancy >= queue_refresh_figure_num)
        {
            for (uint8_t i = 0; i < manager.configured_figure_num; i++)
            {
                if (manager.figures[i].refresh_level == UI_REFRESH_IN_QUEUE &&
                    (manager.figures[i].is_hidden ? manager.figures[i].is_plotted : true))
                {
                    index[used_index_num] = i;
                    used_index_num++;
                    if (used_index_num >= MAX_FIGURE_PLOT_NUM_IN_A_SENT)
                        break;
                }
            }
        }
        /* 否则就在所有队列刷新图形中按顺序循环刷新 */
        else
        {
            /* 已占用的空位数量 */
            uint8_t processed_queue_figure_num = 0;
            for (uint8_t i = manager.last_queue_refresh_figure_index + 1;
                 processed_queue_figure_num < figure_plot_vacancy;
                 i = i >= manager.configured_figure_num - 1 ? 0 : i + 1)
            {
                if (i >= manager.configured_figure_num)
                    continue;
                if (manager.figures[i].refresh_level == UI_REFRESH_IN_QUEUE &&
                    (manager.figures[i].is_hidden ? manager.figures[i].is_plotted : true))
                {
                    index[used_index_num] = i;
                    used_index_num++;
                    if (used_index_num >= MAX_FIGURE_PLOT_NUM_IN_A_SENT)
                    {
                        manager.last_queue_refresh_figure_index = i;
                        break;
                    }
                }
            }
        }
    }
    if (used_index_num)
    {
        /* 设定子内容ID */
        manager.sub_cmd_id = PLOT_SEVEN_FIGURE_SUBCMD_ID;
        /* 构建数据内容 */
        for (uint8_t i = 0; i < used_index_num; i++)
        {
            if (manager.figures[index[i]].builder != NULL)
            {
                figure_operation_type_e figure_operation_type = FIGURE_NOP;
                if (manager.figures[index[i]].is_hidden)
                {
                    if (manager.figures[index[i]].is_plotted)
                    {
                        figure_operation_type = FIGURE_DELETE;
                        manager.figures[index[i]].is_plotted = false;
                    }
                    else
                    {
                        figure_operation_type = FIGURE_NOP;
                    }
                }
                else
                {
                    if (manager.figures[index[i]].is_plotted)
                    {
                        figure_operation_type = FIGURE_MODIFY;
                    }
                    else
                    {
                        figure_operation_type = FIGURE_ADD;
                        manager.figures[index[i]].is_plotted = true;
                    }
                }
                manager.figures[index[i]].builder(&manager.figures[index[i]].figure, figure_operation_type);
            }
        }
        /* 数据头编码 */
        memcpy(manager.interaction_data_sent,
               modify_interaction_data_header(manager.sub_cmd_id, getRobotClientId(ROBOT_SELF_ID)),
               sizeof(robot_interaction_data_header_t));
        /* 数据内容编码 */
        memset(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t), 0,
               MAX_FIGURE_PLOT_NUM_IN_A_SENT * sizeof(interaction_figure_t));
        for (uint8_t i = 0; i < used_index_num; i++)
        {
            memcpy(manager.interaction_data_sent + sizeof(robot_interaction_data_header_t) +
                       i * sizeof(interaction_figure_t),
                   &manager.figures[index[i]].figure, sizeof(interaction_figure_t));
        }

        return manager.interaction_data_sent;
    }

    return NULL;
}

// 烧饼和雷达还需要提供修改内容的API和获取编码后数据流的API

static robot_interaction_data_header_t *modify_interaction_data_header(uint16_t sub_cmd_id, uint16_t target_id)
{
    manager.robot_interaction_data_header.data_cmd_id = sub_cmd_id;
    manager.robot_interaction_data_header.sender_id = ROBOT_SELF_ID;
    manager.robot_interaction_data_header.receiver_id = target_id;

    return &manager.robot_interaction_data_header;
}

static uint8_t get_sub_data_len_by_sub_cmd_id(uint16_t sub_cmd_id)
{
    if (sub_cmd_id == LAYER_DELETE_SUBCMD_ID)
        return ROBOT_INTERACTION_LAYER_DELETE_SUB_DATA_SIZE;
    else if (sub_cmd_id == PLOT_ONE_FIGURE_SUBCMD_ID)
        return ROBOT_INTERACTION_PLOT_ONE_FIGURE_SUB_DATA_SIZE;
    else if (sub_cmd_id == PLOT_TWO_FIGURE_SUBCMD_ID)
        return ROBOT_INTERACTION_PLOT_TWO_FIGURE_SUB_DATA_SIZE;
    else if (sub_cmd_id == PLOT_FIVE_FIGURE_SUBCMD_ID)
        return ROBOT_INTERACTION_PLOT_FIVE_FIGURE_SUB_DATA_SIZE;
    else if (sub_cmd_id == PLOT_SEVEN_FIGURE_SUBCMD_ID)
        return ROBOT_INTERACTION_PLOT_SEVEN_FIGURE_SUB_DATA_SIZE;
    else if (sub_cmd_id == PLOT_CHARACTER_FIGURE_SUBCMD_ID)
        return ROBOT_INTERACTION_PLOT_CHARACTER_FIGURE_SUB_DATA_SIZE;
    else if (sub_cmd_id == SENTRY_CMD_SUBCMD_ID)
        return ROBOT_INTERACTION_SENTRY_CMD_SUB_DATA_SIZE;
    else if (sub_cmd_id == RADAR_CMD_SUBCMD_ID)
        return ROBOT_INTERACTION_RADAR_CMD_SUB_DATA_SIZE;

    return 113;
}
