/* This file is part of the bladeRF project:
 *   http://www.github.com/nuand/bladeRF
 *
 * Copyright (c) 2015 Nuand LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdint.h>
#include <stdbool.h>
#include "pkt_handler.h"
#include "pkt_retune.h"
#include "nios_pkt_retune.h"    /* Packet format definition */
#include "devices.h"
#include "band_select.h"
#include "debug.h"


void pkt_retune(struct pkt_buf *b)
{
    bladerf_module module;
    uint64_t timestamp;
    struct lms_freq f;

    nios_pkt_retune_unpack(b->req, &module, &timestamp,
                           &f.nint, &f.nfrac, &f.freqsel, &f.low_band);

    if (timestamp == NIOS_PKT_RETUNE_NOW) {
        /* Fire off this retune operation now */

        switch (module) {
            case BLADERF_MODULE_RX:
            case BLADERF_MODULE_TX:
                lms_set_precalculated_frequency(NULL, module, &f);
                band_select(NULL, module, f.low_band);
                break;

            default:
                /* TODO Mark a failure in the response */
                goto out;
        }

    } else {
        /* TODO Schedule the retune operation in our queue */
    }

out:
    /* TODO Have a success/failure in the response */
    memcpy(b->resp, b->req, sizeof(b->req));
}