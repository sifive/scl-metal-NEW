/******************************************************************************
 *
 * SiFive Cryptographic Library (SCL)
 *
 ******************************************************************************
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

/**
 * @file hca_trng.c
 * @brief
 *
 * @copyright Copyright (c) 2020 SiFive, Inc
 * @copyright SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <stdio.h>

#include <scl/scl_retdefs.h>

#include <metal/io.h>
#include <metal/machine/platform.h>

#include <backend/hardware/hca_macro.h>
#include <backend/hardware/scl_hca.h>

#include <backend/hardware/v0.5/random/hca_trng.h>
#include <backend/hardware/v0.5/sifive_hca-0.5.x.h>

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
#ifndef __riscv_xlen
#error __riscv_xlen is not defined
#endif

#if ((__riscv_xlen != 64) && (__riscv_xlen != 32))
#error "Unexpected __riscv_xlen"
#endif

typedef struct _hca_trng_data
{
    const metal_scl_t *scl;
    uint32_t *data;
    void (*callback)(int32_t);
} hca_trng_data_t;

CRYPTO_DATA static hca_trng_data_t hca_trng_isr_data;

CRYPTO_FUNCTION static void hca_trng_isr(int id, void * priv_data)
{
    hca_trng_data_t *hca_trng_isr_data_ptr = (hca_trng_data_t *)priv_data;
    
    /* Remove compiler warning about unused variables */
	( void ) id;

    if (NULL == hca_trng_isr_data_ptr) {
        // we can do nothing, just return
        return;
    }

    if (NULL == hca_trng_isr_data_ptr->scl) {
        // we can do nothing, just return
        return;
    }

    // read TRNG_DATA register
    *hca_trng_isr_data_ptr->data = 
        METAL_REG32(hca_trng_isr_data_ptr->scl->hca_base, 
                    METAL_SIFIVE_HCA_TRNG_DATA);

    // Disable interrupt
    hca_setfield32(hca_trng_isr_data_ptr->scl, METAL_SIFIVE_HCA_TRNG_CR, 0,
                HCA_REGISTER_TRNG_CR_RNDIRQEN_OFFSET,
                HCA_REGISTER_TRNG_CR_RNDIRQEN_MASK);
    
    if (NULL == hca_trng_isr_data_ptr->callback)
    {
        hca_trng_isr_data_ptr->callback(SCL_OK);
    }
}

int32_t hca_trng_init(const metal_scl_t *const scl)
{
    int ret = SCL_OK;

    if (NULL == scl)
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_REV))
    {
        // revision of TRNG is Zero so the TRNG is not present.
        return SCL_ERROR;
    }

    // Lock Trim Value
    hca_setfield32(scl, METAL_SIFIVE_HCA_TRNG_TRIM, 1,
                   HCA_REGISTER_TRNG_TRIM_LOCK_OFFSET,
                   HCA_REGISTER_TRNG_TRIM_LOCK_MASK);

    // start on-demand health test
    hca_setfield32(scl, METAL_SIFIVE_HCA_TRNG_CR, 1,
                   HCA_REGISTER_TRNG_CR_HTSTART_OFFSET,
                   HCA_REGISTER_TRNG_CR_HTSTART_MASK);

    while ((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_SR) >>
            HCA_REGISTER_TRNG_SR_HTR_OFFSET) &
           HCA_REGISTER_TRNG_SR_HTR_MASK)
    {
        // test that all 0's are read back from TRNG_DATA during startup health
        // test
        if (METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_DATA) != 0)
        {
            if ((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_SR) >>
                 HCA_REGISTER_TRNG_SR_HTR_OFFSET) &
                HCA_REGISTER_TRNG_SR_HTR_MASK)
            {
                return SCL_RNG_ERROR;
            }
        }
    }

    // Test Heath test status
    if (((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_SR) >>
          HCA_REGISTER_TRNG_SR_HTS_OFFSET) &
         HCA_REGISTER_TRNG_SR_HTS_MASK) != 0)
    {
        ret = SCL_RNG_ERROR;
    }

    hca_setfield32(scl, METAL_SIFIVE_HCA_TRNG_CR, 0,
                   HCA_REGISTER_TRNG_CR_HTSTART_OFFSET,
                   HCA_REGISTER_TRNG_CR_HTSTART_MASK);
    return ret;
}

int32_t hca_trng_getdata(const metal_scl_t *const scl, uint32_t *data_out)
{
    if (NULL == scl)
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_REV))
    {
        // revision of TRNG is Zero so the TRNG is not present.
        return SCL_ERROR;
    }

    // Poll for RNDRDY bit
    while (((METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_SR) >>
             HCA_REGISTER_TRNG_SR_RNDRDY_OFFSET) &
            HCA_REGISTER_TRNG_SR_RNDRDY_MASK) == 0)
        ;

    // read TRNG_DATA register
    *data_out = METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_DATA);

    return SCL_OK;
}

int32_t hca_trng_getdata_with_irq(const metal_scl_t *const scl, 
                                  uint32_t *data_out, 
                                  void (*callback)(int32_t))
{
    if (NULL == scl)
    {
        return (SCL_INVALID_INPUT);
    }

    if (NULL == scl->system_register_handler)
    {
        return (SCL_INVALID_INPUT);
    }

    if (0 == METAL_REG32(scl->hca_base, METAL_SIFIVE_HCA_TRNG_REV))
    {
        // revision of TRNG is Zero so the TRNG is not present.
        return SCL_ERROR;
    }

    hca_trng_isr_data.scl = scl;
    hca_trng_isr_data.data = data_out;
    hca_trng_isr_data.callback = callback;

    // Register Handler
    scl->system_register_handler(scl, &hca_trng_isr, &hca_trng_isr_data);

    // enable interrupt
    hca_setfield32(scl, METAL_SIFIVE_HCA_TRNG_CR, 1,
                HCA_REGISTER_TRNG_CR_RNDIRQEN_OFFSET,
                HCA_REGISTER_TRNG_CR_RNDIRQEN_MASK);

    return SCL_OK;
}

#endif /* METAL_SIFIVE_HCA_VERSION */
