#include "stdio.h"
#include "string.h"

#include "referee.h"

static game_status_t game_status;
static game_robot_HP_t game_robot_HP;
static robot_status_t robot_status;
static power_heat_data_t power_heat_data;
static shoot_data_t shoot_data;
static projectile_allowance_t projectile_allowance;
static uint8_t robot_interaction_message_data[118];
static custom_robot_data_t customer_controller_data;
static vt_link_remote_control_t vt_link_remote_control;

void refereeInitData(void)
{
    memset(&game_status, 0, sizeof(game_status_t));
    memset(&game_robot_HP, 0, sizeof(game_robot_HP_t));
    memset(&robot_status, 0, sizeof(robot_status_t));
    memset(&power_heat_data, 0, sizeof(power_heat_data_t));
    memset(&shoot_data, 0, sizeof(shoot_data_t));
    memset(&projectile_allowance, 0, sizeof(projectile_allowance_t));
    memset(robot_interaction_message_data, 0, 118 * sizeof(uint8_t));
    memset(&customer_controller_data, 0, sizeof(custom_robot_data_t));
    memset(&vt_link_remote_control, 0, sizeof(vt_link_remote_control_t));
}

void referee_data_decode(uint8_t *frame, uint16_t cmd_id)
{
    uint8_t index = REF_HEADER_CMDID_LEN;

    switch (cmd_id)
    {
        /* 常规链路 */
    case GAME_STATUS_CMD_ID: {
        memcpy(&game_status, frame + index, sizeof(game_status_t));
        break;
    }
    case GAME_ROBOT_HP_CMD_ID: {
        memcpy(&game_robot_HP, frame + index, sizeof(game_robot_HP_t));
        break;
    }
    case ROBOT_STATUS_CMD_ID: {
        memcpy(&robot_status, frame + index, sizeof(robot_status_t));
        break;
    }
    case POWER_HEAT_DATA_CMD_ID: {
        memcpy(&power_heat_data, frame + index, sizeof(power_heat_data_t));
        break;
    }
    case SHOOT_DATA_CMD_ID: {
        memcpy(&shoot_data, frame + index, sizeof(shoot_data_t));
        break;
    }
    case PROJECTILE_ALLOWANCE_CMD_ID: {
        memcpy(&projectile_allowance, frame + index, sizeof(projectile_allowance_t));
        break;
    }

    case ROBOT_INTERACTION_DATA_CMD_ID: {
        memcpy(robot_interaction_message_data, frame + index, 8u);
        break;
    }

        /* 图传链路 */

    case CUSTOM_ROBOT_DATA_CMD_ID: {
        uint32_t temp;
        float temp_out;
        for (uint8_t i = 0; i < 6; i++)
        {
            temp = ((frame[index + 3U + sizeof(float) * i] << 24) | (frame[index + 2U + sizeof(float) * i] << 16) |
                    (frame[index + 1U + sizeof(float) * i] << 8) | frame[index + 0U + sizeof(float) * i]);
            temp_out = *(float *)&temp;
            customer_controller_data.pose[i] = temp_out;
        }
        customer_controller_data.key = frame[index + sizeof(float) * 6];
        break;
    }
    case REMOTE_CONTROL_CMD_ID: {
        memcpy(&vt_link_remote_control, frame + index, sizeof(vt_link_remote_control_t));
        break;
    }

    default:
        break;
    }
}

const game_status_t *getGameStatus(void)
{
    return &game_status;
}

const game_robot_HP_t *getRobotHp(void)
{
    return &game_robot_HP;
}

const robot_status_t *getRobotStatus(void)
{
    return &robot_status;
}

uint8_t getRobotStatusId(void)
{
    return robot_status.robot_id;
}

uint16_t getRobotCurrentHp(void)
{
    return robot_status.current_HP;
}

const power_heat_data_t *getPowerHeatData(void)
{
    return &power_heat_data;
}

const shoot_data_t *getShootData(void)
{
    return &shoot_data;
}

const projectile_allowance_t *getProjectileAllowance(void)
{
    return &projectile_allowance;
}

const uint8_t *getRobotInteractionMessageData(void)
{
    return robot_interaction_message_data;
}

const custom_robot_data_t *getCustomerControllerData(void)
{
    return &customer_controller_data;
}

const vt_link_remote_control_t *getVtLinkRemoteControlData(void)
{
    return &vt_link_remote_control;
}
