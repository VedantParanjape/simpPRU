/*
 * Copyright (C) 2015 Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *
 *	* Redistributions in binary form must reproduce the above copyright
 *	  notice, this list of conditions and the following disclaimer in the
 *	  documentation and/or other materials provided with the
 *	  distribution.
 *
 *	* Neither the name of Texas Instruments Incorporated nor the names of
 *	  its contributors may be used to endorse or promote products derived
 *	  from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _PRU_ECAP_H_
#define _PRU_ECAP_H_

/* PRU_ECAP register set */
typedef struct {

	/* PRU_ECAP_TSCTR register bit field */
	union {
		volatile uint32_t TSCTR;

		volatile struct {
			unsigned TSCTR	: 32;		//31:0
		} TSCTR_bit;
	};	// 0x0

	/* PRU_ECAP_CTRPHS register bit field */
	union {
		volatile uint32_t CTRPHS;

		volatile struct {
			unsigned CTRPHS	: 32;		//31:0
		} CTRPHS_bit;
	};	// 0x4

	/* PRU_ECAP_CAP1 register bit field */
	union {
		volatile uint32_t CAP1;

		volatile struct {
			unsigned CAP1	: 32;		//31:0
		} CAP1_bit;
	};	// 0x8

	/* PRU_ECAP_CAP2 register bit field */
	union {
		volatile uint32_t CAP2;

		volatile struct {
			unsigned CAP2	: 32;		//31:0
		} CAP2_bit;
	};	// 0xC

	/* PRU_ECAP_CAP3 register bit field */
	union {
		volatile uint32_t CAP3;

		volatile struct {
			unsigned CAP3	: 32;		//31:0
		} CAP3_bit;
	};	// 0x10

	/* PRU_ECAP_CAP4 register bit field */
	union {
		volatile uint32_t CAP4;

		volatile struct {
			unsigned CAP4	: 32;		//31:0
		} CAP4_bit;
	};	// 0x14

	uint32_t rsvd118[4];		// 0x118 - 0x124

	/* PRU_ECAP_ECCTL1 register bit field */
	union { 
        volatile uint16_t ECCTL1; // 0x28

        volatile struct {
            unsigned CAP1POL  : 1; // 0
            unsigned CTRRST1  : 1; // 1
            unsigned CAP2POL  : 1; // 2
            unsigned CTRRST2  : 1; // 3
            unsigned CAP3POL  : 1; // 4
            unsigned CTRRST3  : 1; // 5
            unsigned CAP4POL  : 1; // 6
            unsigned CTRRST4  : 1; // 7
            unsigned CAPLDEN  : 1; // 8
            unsigned PRESCALE : 5; // 13:9
            unsigned FREE_SOFT : 2; // 15:14 
        } ECCTL1_bit;
    }

	/* PRU_ECAP_ECCTL2 register bit field */
	union {
        volatile uint16_t ECCTL2;	// 0x2A

        volatile struct {
            unsigned CONT_ONESHT : 1; // 0
            unsigned STOP_WRAP   : 2; // 2:1
            unsigned REARM       : 1; // 3
            unsigned TSCTRSTOP   : 1; // 4
            unsigned SYNCI_EN    : 1; // 5
            unsigned SYNCO_SEL   : 2; // 7:6
            unsigned SWSYNC      : 1; // 8
            unsigned CAP_APWM    : 1; // 9
            unsigned APWMPOL     : 1; // 10
            unsigned RESERVED0   : 5; // 15:11
        } EECTL2_bit;
    }

	/* PRU_ECAP_ECEINT register bit field */
	union {
        volatile uint16_t ECEINT;	// 0x2C

        volatile struct {
            unsigned RESERVED0 : 1; // 0
            unsigned CEVT1     : 1; // 1
            unsigned CEVT2     : 1; // 2
            unsigned CEVT3     : 1; // 3
            unsigned CEVT4     : 1; // 4
            unsigned CTROVF    : 1; // 5
            unsigned CTR_PRD   : 1; // 6
            unsigned CTR_CMP   : 1; // 7
            unsigned RESERVED1 : 8; // 15:8
        } ECEINT_bit;
    }
	/* PRU_ECAP_ECFLG register bit field */
	union {
        volatile uint16_t ECFLG;	// 0x2E

        volatile struct {
            unsigned INT       : 1; // 0
            unsigned CEVT1     : 1; // 1
            unsigned CEVT2     : 1; // 2
            unsigned CEVT3     : 1; // 3
            unsigned CEVT4     : 1; // 4
            unsigned CTROVF    : 1; // 5
            unsigned CTR_PRD   : 1; // 6
            unsigned CTR_CMP   : 1; // 7
            unsigned RESERVED0 : 8; // 15:8
        } ECFLG_bit;
    }
	/* PRU_ECAP_ECCLR register bit field */
	union {
        volatile uint16_t ECCLR;	// 0x30
        
        volatile struct {
            unsigned INT       : 1; // 0
            unsigned CEVT1     : 1; // 1
            unsigned CEVT2     : 1; // 2
            unsigned CEVT3     : 1; // 3
            unsigned CEVT4     : 1; // 4
            unsigned CTROVF    : 1; // 5
            unsigned CTR_PRD   : 1; // 6
            unsigned CTR_CMP   : 1; // 7
            unsigned RESERVED0 : 8; // 15:8
        } ECCLR_bit;
    }

	/* PRU_ECAP_ECFRC register bit field */
	union {
        volatile uint16_t ECFRC;	// 0x32

        volatile struct {
            unsigned RESERVED0 : 1; // 0
            unsigned CEVT1     : 1; // 1
            unsigned CEVT2     : 1; // 2
            unsigned CEVT3     : 1; // 3
            unsigned CEVT4     : 1; // 4
            unsigned CTROVF    : 1; // 5
            unsigned CTR_PRD   : 1; // 6
            unsigned CTR_CMP   : 1; // 7
            unsigned RESERVED1 : 8; // 15:8
        } ECFRC_bit;
    }
	uint32_t rsvd34[10];		// 0x34 - 0x58

	/* PRU_ECAP_REVID register bit field */
	union {
		volatile uint32_t REVID;

		volatile struct {
			unsigned REV		: 32;		//31:0
		} REVID_bit;
	};	// 0x5C
} pruEcap;

#ifdef __GNUC__
static volatile pruEcap *__CT_ECAP = (void*)0x00030000;
#define CT_ECAP (*__CT_ECAP)
#else
volatile __far pruEcap CT_ECAP __attribute__((cregister("PRU_ECAP", near), peripheral));
#endif

#endif /* _PRU_ECAP_H_ */
