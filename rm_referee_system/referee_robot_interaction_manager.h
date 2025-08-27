#ifndef _REFEREE_ROBOT_INTERACTION_MANAGER_H__
#define _REFEREE_ROBOT_INTERACTION_MANAGER_H__

#include "stdbool.h"

#include "referee.h"
#include "referee_protocol.h"

#define AUTO_ID_OBTAIN 1 // 是否自动获取ID

#if AUTO_ID_OBTAIN
#define ROBOT_SELF_ID getRobotStatusId()
#else
#define ROBOT_SELF_ID ROBOT_ID_RED_HERO
#endif

#define REFEREE_SYSTEM_SERVER_ID 0x8080

#define MAX_FIGURE_PLOT_NUM_IN_A_SENT (7U)
#define MAX_REAL_TIME_REFRESH_UI_NUM (MAX_FIGURE_PLOT_NUM_IN_A_SENT)
typedef enum RobotInteractionType
{
    ROBOT_TO_ROBOT_INTERACTION = 0,
    CLIENT_UI_PLOT,
    SENTRY_CMD,
    RADAR_CMD,

} robot_interaction_type_e;

typedef enum ClientUiRefreshLevel
{
    UI_REFRESH_ONCE = 0,
    UI_REFRESH_REAL_TIME,
    UI_REFRESH_IN_QUEUE,
} client_ui_refresh_level_e;

typedef enum ClientUiOperationType
{
    UI_RESET_ALL,
    UI_RESET_ALL_FIGURES,
    UI_RESET_ALL_CHARACTERS,
    UI_RESET_FIGURE,
    UI_RESET_CHARACTER,

    UI_HIDE_ALL,
    UI_HIDE_ALL_FIGURES,
    UI_HIDE_ALL_CHARACTERS,
    UI_HIDE_FIGURE,
    UI_HIDE_CHARACTER,

    UI_DISPLAY_ALL,
    UI_DISPLAY_ALL_FIGURES,
    UI_DISPLAY_ALL_CHARACTERS,
    UI_DISPLAY_FIGURE,
    UI_DISPLAY_CHARACTER,

} client_ui_operation_type_e;

typedef struct InteractionMessageFactory
{
    uint16_t message_id;
    uint16_t target_id;
    uint8_t message[112];
    uint8_t data_length;
    void (*builder)(uint8_t *, uint8_t *);
} interaction_message_factory_t;

typedef struct InteractionFigureFactory
{
    interaction_figure_t figure;
    void (*builder)(interaction_figure_t *, figure_operation_type_e);

    bool is_plotted;
    bool is_hidden;
    client_ui_refresh_level_e refresh_level;
} interaction_figure_factory_t;

typedef struct InteractionCharacterFactory
{
    interaction_character_t character;
    void (*builder)(interaction_character_t *, figure_operation_type_e);

    bool is_plotted;
    bool is_hidden;
} interaction_character_factory_t;

typedef struct RobotInteractionManager
{
    bool init_success;

    uint32_t last_successful_send_time; // 上次成功发送的时间

    robot_interaction_data_header_t robot_interaction_data_header; // 子内容数据头
    uint16_t sub_cmd_id;                                           // 子内容ID
    uint8_t *interaction_data_sent;                                // 子内容数据

    // 机器人间通信
    uint8_t message_num;
    uint8_t configured_message_num;
    uint8_t message_encode_index;
    interaction_message_factory_t *messages;

    // UI
    // interaction_layer_delete_t layer_deleter;
    // void (*layer_deleter_builder)(interaction_layer_delete_t *);
    uint8_t figure_num;
    uint8_t configured_figure_num;
    uint8_t last_queue_refresh_figure_index;
    interaction_figure_factory_t *figures;
    uint8_t character_num;
    uint8_t configured_character_num;
    interaction_character_factory_t *characters;

    // // 烧饼自主决策
    // interaction_sentry_cmd_t sentry_cmd;
    // void (*sentry_cmd_builder)(interaction_sentry_cmd_t *);

    // // 雷达自主决策
    // interaction_radar_cmd_t radar_cmd;
    // void (*radar_cmd_builder)(interaction_radar_cmd_t *);

} robot_interaction_manager_t;

extern bool refereeRobotInteractionMessageInit(uint8_t message_num);
extern bool refereeRobotInteractionManagerInit(uint8_t figure_num, uint8_t character_num);

extern uint8_t *refereeEncodeRobotInteractionData(uint32_t now_time, robot_interaction_type_e robot_interaction_type);
extern void refereeRobotInteractionManagerSuccessfullySentHook(uint32_t now_time);

extern uint8_t refereeRobotInteractionMessageConfig(uint16_t message_id, uint16_t target_id,
                                                    void (*builder)(uint8_t *, uint8_t *));

// extern void refereeSetRobotInteractionLayerDeleterBuilder(void (*builder)(interaction_layer_delete_t *));
extern uint8_t refereeRobotInteractionFigureConfig(client_ui_refresh_level_e refresh_level,
                                                   void (*builder)(interaction_figure_t *, figure_operation_type_e));
extern uint8_t refereeRobotInteractionCharacterConfig(void (*builder)(interaction_character_t *,
                                                                      figure_operation_type_e));

extern void refereeClientUiDynamicFigureAdjust(uint8_t index, client_ui_refresh_level_e refresh_level);
extern void refereeClientUiOperate(client_ui_operation_type_e operation_type, uint8_t index);

#endif /* _REFEREE_ROBOT_INTERACTION_MANAGER_H__ */
