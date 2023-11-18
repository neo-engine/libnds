// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2020 Gericom

#ifndef LIBTEAK_APBP_H__
#define LIBTEAK_APBP_H__

#include <teak/types.h>

// Host Port Interface (APBP aka HPI)

/// APBP DSP-to-ARM Reply 0 (R/W)
#define REG_APBP_REP0                   (*(vu16 *)0x80C0)
/// APBP ARM-to-DSP Command 0 (R)
#define REG_APBP_CMD0                   (*(vu16 *)0x80C2)

/// APBP DSP-to-ARM Reply 1 (R/W)
#define REG_APBP_REP1                   (*(vu16 *)0x80C4)
/// APBP ARM-to-DSP Command 1 (R)
#define REG_APBP_CMD1                   (*(vu16 *)0x80C6)

/// APBP DSP-to-ARM Reply 2 (R/W)
#define REG_APBP_REP2                   (*(vu16 *)0x80C8)
/// APBP ARM-to-DSP Command 2 (R)
#define REG_APBP_CMD2                   (*(vu16 *)0x80CA)

/// APBP DSP-to-ARM Semaphore Set Flags (R/W)
#define REG_APBP_PSEM                   (*(vu16 *)0x80CC)
/// APBP ARM-to-DSP Semaphore Interrupt Mask (R/W)
#define REG_APBP_PMASK                  (*(vu16 *)0x80CE)
/// APBP ARM-to-DSP Semaphore Ack Flags (W?)
#define REG_APBP_PCLEAR                 (*(vu16 *)0x80D0)
/// APBP ARM-to-DSP Semaphore Get Flags (R)
#define REG_APBP_SEM                    (*(vu16 *)0x80D2)

/// APBP Control (R/W)
#define REG_APBP_CONTROL                (*(vu16 *)0x80D4)

#define APBP_CONTROL_ARM_BIG_ENDIAN     BIT(2)
#define APBP_CONTROL_IRQ_CMD0_DISABLE   BIT(8)
#define APBP_CONTROL_IRQ_CMD1_DISABLE   BIT(12)
#define APBP_CONTROL_IRQ_CMD2_DISABLE   BIT(13)

/// APBP DSP-side Status (R)
#define REG_APBP_STAT                   (*(vu16 *)0x80D6)

#define APBP_STAT_REP0_UNREAD           BIT(5)
#define APBP_STAT_REP1_UNREAD           BIT(6)
#define APBP_STAT_REP2_UNREAD           BIT(7)

#define APBP_STAT_CMD0_NEW              BIT(8)
#define APBP_STAT_CMD1_NEW              BIT(12)
#define APBP_STAT_CMD2_NEW              BIT(13)

#define APBP_STAT_SEM_FLAG              BIT(9)

/// APBP ARM-side Status (mirror of ARM9 Port 400430Ch) (R)
#define REG_APBP_ARM_STAT               (*(vu16 *)0x80D8)

#define APBP_ARM_STAT_RD_XFER_BUSY      BIT(0)

#define APBP_ARM_STAT_WR_XFER_BUSY      BIT(1)

#define APBP_ARM_STAT_PERI_RESET        BIT(2)

#define APBP_ARM_STAT_RD_FIFO_FULL      BIT(5)
#define APBP_ARM_STAT_RD_FIFO_READY     BIT(6)
#define APBP_ARM_STAT_WR_FIFO_FULL      BIT(7)
#define APBP_ARM_STAT_WR_FIFO_EMPTY     BIT(8)

#define APBP_ARM_STAT_REP_NEW_SHIFT     10

#define APBP_ARM_STAT_REP0_NEW          (1 << APBP_ARM_STAT_REP_NEW_SHIFT)
#define APBP_ARM_STAT_REP1_NEW          (1 << (APBP_ARM_STAT_REP_NEW_SHIFT + 1))
#define APBP_ARM_STAT_REP2_NEW          (1 << (APBP_ARM_STAT_REP_NEW_SHIFT + 2))

#define APBP_ARM_STAT_CMD_UNREAD_SHIFT  13

#define APBP_ARM_STAT_CMD0_UNREAD       (1 << APBP_ARM_STAT_CMD_UNREAD_SHIFT)
#define APBP_ARM_STAT_CMD1_UNREAD       (1 << (APBP_ARM_STAT_CMD_UNREAD_SHIFT + 1))
#define APBP_ARM_STAT_CMD2_UNREAD       (1 << (APBP_ARM_STAT_CMD_UNREAD_SHIFT + 2))

static inline void apbpSetSemaphore(uint16_t mask)
{
    REG_APBP_PSEM = mask;
}

static inline void apbpSetSemaphoreMask(uint16_t mask)
{
    REG_APBP_PMASK = mask;
}

static inline void apbpClearSemaphore(uint16_t mask)
{
    REG_APBP_PCLEAR = mask;
}

static inline uint16_t apbpGetSemaphore(void)
{
    return REG_APBP_SEM;
}

void apbpSendData(uint16_t id, uint16_t data);

uint16_t apbpReceiveData(uint16_t id);

#endif // LIBTEAK_APBP_H__
