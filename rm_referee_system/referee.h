#ifndef REFEREE_H
#define REFEREE_H

#include "stdint.h"

#include "referee_protocol.h"

extern void refereeInitData(void);
extern void referee_data_decode(uint8_t *frame, uint16_t cmd_id);

extern const game_status_t *getGameStatus(void);
extern const game_robot_HP_t *getRobotHp(void);
extern const robot_status_t *getRobotStatus(void);
extern uint8_t getRobotStatusId(void);
extern uint16_t getRobotCurrentHp(void);
extern const power_heat_data_t *getPowerHeatData(void);
extern const shoot_data_t *getShootData(void);
extern const projectile_allowance_t *getProjectileAllowance(void);
extern const uint8_t *getRobotInteractionMessageData(void);

extern const game_robot_HP_t *getRobotHp(void);

extern const custom_robot_data_t *getCustomerControllerData(void);
extern const vt_link_remote_control_t *getVtLinkRemoteControlData(void);

#endif
